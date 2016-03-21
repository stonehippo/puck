# puck
An accelerometer-driven, physical interface for interacting with digital systems via MQTT. In other words, it's an IoT switch.

## Firmware

Puck's firmware has the following dependencies:

* [ESP8266 Arduino library](https://github.com/esp8266/Arduino)
* [Adafruit MMA8451 accelerometer library](https://github.com/adafruit/Adafruit_MMA8451_Library)
* [Adafruit unified sensor library](https://github.com/adafruit/Adafruit_Sensor)
* [Adafruit MQTT library](https://github.com/adafruit/Adafruit_MQTT_Library)

I install these via the Arduino Library manager, but you might prefer direct installation. See the various libraries for instruction on how to install.

I am currently building the Puck firmware using [Arduino 1.6.8](http://arduino.cc) on OS X 10.11. It should build fine with any version of Arduino >= 1.6.

### WiFi Configuration

To keep secure information out of this repository, WiFi SSID and password information is stored in a separate header file, `wifi_config.h`. You need to create this file in the Puck folder, and define two values.

```
#define MY_WIFI_SSID        "MyNetwork"
#define MY_WIFI_PASSWORD    "MyNetworkPassword"
```

Once created, the config header will be included into the main firmware build.

Take a look at (and dupe) `example_wifi_config.h` if you want to see what `wifi_config.h` must contain.

## Hardware

* [ESP8266 ESP-01 Module](https://github.com/esp8266/esp8266-wiki/wiki/Hardware_versions)
* [MMA8451 accelerometer breakout board](https://www.adafruit.com/products/2019), from [Adafruit](http://adafruit.com)

## License

Puck is copyright (c) 2016 George White <stonehippo@gmail.com>.
This code is licensed under GPL v3. License is available [here](/LICENSE).
