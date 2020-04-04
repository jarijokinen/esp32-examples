# ESP32 RuuviTag Scanner

RuuviTag scanner for ESP32.

The scanner currently supports only data format 3 (0x03).

## Usage

    idf.py build
    idf.py -p /dev/ttyUSB0 flash monitor

The log should look like this:

    I (1335) ruuvitag: ** RuuviTag: FE:5A:35:C2:05:DD
    I (1345) ruuvitag: Temperature: 24.93 C
    I (1345) ruuvitag: Humidity:    15.00 %
    I (3985) ruuvitag: 
    I (3985) ruuvitag: ** RuuviTag: E8:2B:AE:08:AA:F9
    I (3985) ruuvitag: Temperature: 22.34 C
    I (3995) ruuvitag: Humidity:    14.00 %

## License

MIT License. Copyright (c) 2020 [Jari Jokinen](https://jarijokinen.com).  See
[LICENSE](https://github.com/jarijokinen/esp32-examples/blob/master/LICENSE.txt)
for further details.
