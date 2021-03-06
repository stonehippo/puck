# Puck
An accelerometer-driven, physical interface for interacting with digital systems via MQTT. In other words, it's an IoT switch.

## What The Heck is Puck?

Puck is a very simple connected device. You can think of it as a physical switch for digital products. Flip it on one side, and Puck will signal that it's in the _On_ state. Flip it to the other side, and Puck signals _Off_. That's it.

Ok, that's not _quite_ all of it. First off, let's describe Puck: it's a disc, about 10 cm across and 3 cm thick. One side is painted white, and the other, black. And inside is an ESP8266 WiFi board, an accelerometer, and a battery, plus a little USB charging board.

Puck talks to other devices via the Internet, using a protocol called [MQTT](https://en.wikipedia.org/wiki/MQTT). MQTT is a lightweight protocol, ideal for Internet of Things devices that don't need to send or receive a lot of data. And all Puck sends is pretty much a binary on/off. It's a 1-bit appliance!

The default firmware uses the [Adafruit IO](http://io.adafruit.com) service to send and receive data. But with a little alternation, Puck can speak to almost any service that uses MQTT to communicate with devices.

Using MQTT, Pucks sends it's on/off message to the broker (a service that routes information for MQTT devices), and the broker can then use that information to trigger something else. For example, I have set up my Adafruit IO feed for Puck to be monitoried by [IFTTT](http://iftt.com), which in turn sends a command to some connected lighting in my house. Just by flipping Puck over, I can turn those lights on or off.

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
* [arduino-fsm](https://github.com/jonblack/arduino-fsm)

I install these via the PlatformIO library manager, but you might prefer direct installation. See the various libraries for instruction on how to install.

I am currently building the Puck firmware using [Platform.IO](http://platformio.org) on OS X 10.11.

### WiFi Configuration

To keep secure information out of this repository, WiFi SSID and password information is stored in a separate header file, `include/wifi_config.h`. You need to create this file in the Puck folder, and define two values.

```
#define MY_WIFI_SSID        "MyNetwork"
#define MY_WIFI_PASSWORD    "MyNetworkPassword"
```

Once created, the config header will be included into the main firmware build.

Take a look at (and dupe) [include/example_wifi_config.h](include/example_wifi_config.h) if you want to see what `include/wifi_config.h` must contain.

### MQTT Configuration

Puck is currently set up to use [AdafruitIO](https://io.adafruit.com) as an MQTT provider. You will need to set up your own ID and access toke info in the file `include/mqtt_config.h`. It defines the following values:

```
#define MY_AIO_USERNAME    "...your AIO username (see https://accounts.adafruit.com)..."
#define MY_AIO_KEY         "...your AIO key..."
```

Take a look at [include/example_mqtt_config.h](include/example_mqtt_config.h) for a template file you can copy and fill in.

*Note:* It's entirely possible to use Puck with other MQTT brokers. Right now, the service is set up in `arc/puck.cpp`. However, the values could be migrated to macros defined in `include/mqtt_config.h`, similar to the setup for the MQTT authentication. I'll probably make exactly that modification at some point.

## Hardware

* [ESP8266 ESP-01 Module](https://github.com/esp8266/esp8266-wiki/wiki/Hardware_versions)
* [MMA8451 accelerometer breakout board](https://www.adafruit.com/products/2019), from [Adafruit](http://adafruit.com)

## Contributing

Puck is meant to be open source, in both the hardware and the software. If you've got updates of fixes for the firmware, yay! Fork for this repo, send me a PR and I'll get it merged it. You'll want the [PlatformIO IDE](http://platformio.org), and you'll need to want to clone the code somewhere local.

 If you've refined the hardware in some way, I'd love to hear about it. Even better, consider adding your hardware design to what's already here. Then we can have lots of Pucks to give. ;-)

## License

Puck is copyright (c) 2016 George White <stonehippo@gmail.com>.

This code is licensed under GPL v3. License is available [here](/LICENSE).

Portions of this code written by Tony DiCola for Adafruit Industries,
covered by MIT license.

Adafruit invests time and resources providing this open source code,
please support [Adafruit](https://adafruit.com) and open-source hardware by purchasing
products from Adafruit!
