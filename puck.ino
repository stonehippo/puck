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

// Arduino 1.6.6 bug workaround
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  // clear the first couple of lines, to avoid startup junk
  Serial.println();
  Serial.println();
  wifiSetup();
  accelerometerSetup();
}

void loop() {
  MQTT_connect();

  boolean flipped = isFlipped();
  if(puckonoff.publish(flipped)) {
    Serial.print("Puck published status with value ");
    Serial.println(flipped);
  } else {
    Serial.println("Puck could not publish status");
  }
  Serial.println();
  delay(1000);
}

void accelerometerSetup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!mma.begin()) {
    Serial.println("Could not start MMA8451 accelerometer");
    while(1); // the main component isn't working, so let's freeze
  }
  Serial.println("MMA8451 accelerometer started");
  mma.setRange(MMA8451_RANGE_2_G);
  Serial.print("Range = ");
  Serial.print(2 << mma.getRange());
  Serial.println("G");
}

void wifiSetup() {
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

boolean isFlipped() {
  uint8_t orientation = mma.getOrientation();
  boolean flipped;
  switch(orientation) {
    case MMA8451_PL_PUB:
      flipped = true;
      break;
    case MMA8451_PL_PDB:
      flipped = true;
      break;
    case MMA8451_PL_LRB:
      flipped = true;
      break;
    case MMA8451_PL_LLB:
      flipped = true;
      break;
    default:
      // handle all of the front-oriented cases
      flipped = false;
      break;
  }
  return flipped;
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
