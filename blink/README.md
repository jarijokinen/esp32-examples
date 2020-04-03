# ESP32 Blink

A LED blinking example for ESP32-WROOM-32 (DevKitC V3) using esp-idf.

## Connection

- GPIO pin 4 to LED anode
- GND pin to resistor
- Resistor to LED cathode

## Usage

    idf.py build
    idf.py -p /dev/ttyUSB0 flash monitor

## License

MIT License. Copyright (c) 2020 [Jari Jokinen](https://jarijokinen.com).  See
[LICENSE](https://github.com/jarijokinen/esp32-examples/blob/master/LICENSE.txt)
for further details.
