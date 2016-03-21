#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

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
