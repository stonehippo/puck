#include <Arduino.h>

/*
Puck firmware version 1.0.0

Copyright (c) 2016 George White
This code is licensed under GPL v3. See the LICENSE files for details

Portions of this code written by Tony DiCola for Adafruit Industries,
covered by MIT license.

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!
*/

#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Fsm.h>
#include <puck.h>
// You need to create an include file, wifi_config.h, and set your
// Wifi SSID and password there. Refer to example_wifi_config.h to
// see how this is done.
#include "wifi_config.h"
// You'll also need to create mqtt_config.h, and set your
// Adafruit IO username and key there. Refer to example_mqtt_config.h
// for details.
#include "mqtt_config.h"

#define WLAN_SSID       MY_WIFI_SSID
#define WLAN_PASS       MY_WIFI_PASSWORD

// Always prefer HTTPS...security is key for IoT UX!
WiFiClient client;

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // always prefer SSL!
#define AIO_USERNAME    MY_AIO_USERNAME
#define AIO_KEY         MY_AIO_KEY

// Store the MQTT server, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

// Set up the Adafruit IO feed for the Puck
const char PUCK_ONOFF_FEED[] PROGMEM = AIO_USERNAME "/feeds/puckonoff";
Adafruit_MQTT_Publish puckonoff = Adafruit_MQTT_Publish(&mqtt, PUCK_ONOFF_FEED);

// I2C pin configuration for ESP8266 (for Wire.begin())
// For example, the default here is SDA on pin 2 and SCL on pin 1.
// This config works well with the ESP-01 ESP8266 module; however,
// you can select whatever pins work for you.
#define SDA_PIN         2
#define SCL_PIN         0

Adafruit_MMA8451 mma = Adafruit_MMA8451();

// FSM events
enum Events {
  FLIPPED
};

// Set up the state machine
State state_not_flipped(on_not_flipped_enter, NULL, &on_not_flipped_exit);
State state_flipped(on_flipped_enter, NULL, &on_flipped_exit);
Fsm modes(&state_not_flipped);
    
boolean boot_flag = true;
boolean flipped = false;

void accelerometerSetup();
void wifiSetup();
boolean isFlipped();
void startup();

// Arduino 1.6.6 bug workaround
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  // clear the first couple of lines, to avoid startup junk
  Serial.println();
  Serial.println();
  wifiSetup();
  accelerometerSetup();
  
  modes.add_transition(&state_not_flipped, &state_flipped, FLIPPED, NULL);
  modes.add_transition(&state_flipped, &state_not_flipped, FLIPPED, NULL);
  
  if (isFlipped()) {
    modes.trigger(FLIPPED);
  }
}

void loop() {
  // we don't call modes.run_machine, since we're not using timed or on_state functions
  MQTT_connect();
  if (boot_flag) {
    startup();
  }
  
  if (flipped != isFlipped()) {
    modes.trigger(FLIPPED);
  }
  
  if(! mqtt.ping()) {
   mqtt.disconnect();
  }
}

void on_not_flipped_enter() {
  flipped = false;
  puckonoff.publish(false);
  Serial.print("not flipped (on)");
}

void on_not_flipped_exit() {}

void on_flipped_enter() {
  flipped = true;
  puckonoff.publish(true);
  Serial.print("flipped (off)");
}

void on_flipped_exit() {}

void accelerometerSetup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!mma.begin()) {
    Serial.println("Could not start MMA8451 accelerometer");
    while(1); // the main component isn't working, so let's freeze
  }
  Serial.println("MMA8451 accelerometer started");
  mma.setRange(MMA8451_RANGE_2_G);
}

void wifiSetup() {
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi Connected");
}

boolean isFlipped() {
  uint8_t orientation = mma.getOrientation();
  boolean state;
  switch(orientation) {
    case MMA8451_PL_PUB:
      state = true;
      break;
    case MMA8451_PL_PDB:
      state = true;
      break;
    case MMA8451_PL_LRB:
      state = true;
      break;
    case MMA8451_PL_LLB:
      state = true;
      break;
    default:
      // handle all of the front-oriented cases
      state = false;
      break;
  }
  return state;
}

// Always publish the current status of the puck exactly once on startup
void startup() {
  Serial.println("Booting up communications");
  puckonoff.publish(flipped);
  boot_flag = false;
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
