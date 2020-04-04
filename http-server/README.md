# ESP32 HTTP Server

An HTTP server example for ESP32, using WiFi connection.

## Usage

Set WIFI_SSID and WIFI_PSK in wifi.c, then:

    idf.py build
    idf.py -p /dev/ttyUSB0 flash monitor    # and open http://ip-address:80/

## License

MIT License. Copyright (c) 2020 [Jari Jokinen](https://jarijokinen.com).  See
[LICENSE](https://github.com/jarijokinen/esp32-examples/blob/master/LICENSE.txt)
for further details.
