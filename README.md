# Puck
An accelerometer-driven, physical interface for interacting with digital systems via MQTT. In other words, it's an IoT switch.

## What The Heck is Puck?

Puck is a very simple connected device. You can think of it as a physical switch for digital products. Flip it on one side, and Puck will signal that it's in the _On_ state. Flip it to the other side, and Puck signals _Off_. That's it.

Ok, that's not _quite_ all of it. First off, let's describe Puck: it's a disc, about 10 cm across and 3 cm thick. One side is painted white, and the other, black. And inside is an ESP8266 WiFi board, an accelerometer, and a battery, plus a little USB charging board.

Puck talks to other devices via the Internet, using a protocol called [MQTT](https://en.wikipedia.org/wiki/MQTT). MQTT is a lightweight protocol, ideal for Internet of Things devices that don't need to send or receive a lot of data. And all Puck sends is pretty much a binary on/off. It's a 1-bit appliance!

The default firmware uses the [Adafruit IO](http://io.adafruit.com) service to send and receive data. But with a little alternation, Puck can speak to almost any service that uses MQTT to communicate with devices.

Using MQTT, Pucks sends it's on/off message to the broker (a service that routes information for MQTT devices), and the broker can then use that information to trigger something else. For example, I have set up my Adafruit IO feed for Puck to call  [IFTTT](http://iftt.com), which in turn sends a command to some connected lighting in my house. Just by flipping Puck over, I can turn those lights on or off.

### So, it's a switch, then? Why not just use an actual _switch_? Or what about an app?

Good questions.

There are a few reasons why I created and use Puck:

* Controlling something with a physical interface is quicker and simpler than hauling out an app. Any one who walks into the house can use Puck.
* Puck is portable, unlike the switches on my wall. I can move it around where I want to.
* Puck is a bit of an experiment. My job is to work on technology and user experience, and Puck is a testbed for some of that work.
* It's just fun controlling stuff this way,OK?

## Firmware

Puck's firmware has the following dependencies:

* [ESP8266 Arduino library](https://github.com/esp8266/Arduino)
* [Adafruit MMA8451 accelerometer library](https://github.com/adafruit/Adafruit_MMA8451_Library)
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
