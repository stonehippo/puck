#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
// You need to create an include file, wifi_config.h, and set your
// Wifi SSID and password there. Refer to example_wifi_config.h to
// see how this is done.
#include "wifi_config.h"

#define WLAN_SSID       MY_WIFI_SSID
#define WLAN_PASS       MY_WIFI_PASSWORD

// Always prefer HTTPS...security is key for IoT UX!
WiFiClientSecure client;

// I2C pin configuration for ESP8266 (for Wire.begin())
// For example, the default here is SDA on pin 2 and SCL on pin 1.
// This config works well with the ESP-01 ESP8266 module; however,
// you can select whatever pins work for you.
#define SDA_PIN         2
#define SCL_PIN         0

Adafruit_MMA8451 mma = Adafruit_MMA8451();

void setup() {
  Serial.begin(115200);
  // clear the first couple of lines, to avoid startup junk
  Serial.println();
  Serial.println();
  wifiSetup();
  accelerometerSetup();
}

void loop() {}

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
