# ESP32 WiFi Station

An example for setting up ESP32 WiFi in Station Mode (STA mode).

## Usage

Set WIFI_SSID and WIFI_PSK in wifi_station.c, then:

    idf.py build
    idf.py -p /dev/ttyUSB0 flash monitor

Log should look like this:

```
I (618) wifi: wifi driver task: 3ffc0d08, prio:23, stack:3584, core=0
I (618) system_api: Base MAC address is not set
I (618) system_api: read default base MAC address from EFUSE
I (638) wifi: wifi firmware version: 3f7ead7
I (638) wifi: config NVS flash: enabled
I (638) wifi: config nano formating: disabled
I (638) wifi: Init dynamic tx buffer num: 32
I (648) wifi: Init data frame dynamic rx buffer num: 32
I (648) wifi: Init management frame dynamic rx buffer num: 32
I (658) wifi: Init management short buffer num: 32
I (658) wifi: Init static rx buffer size: 1600
I (668) wifi: Init static rx buffer num: 10
I (668) wifi: Init dynamic rx buffer num: 32
W (728) phy_init: failed to load RF calibration data (0x1102), falling back to full calibration
I (868) phy: phy_version: 4180, cb3948e, Sep 12 2019, 16:39:13, 0, 2
I (918) wifi: mode : sta ([removed])
I (2608) wifi: new:<13,0>, old:<1,0>, ap:<255,255>, sta:<13,0>, prof:1
I (3398) wifi: state: init -> auth (b0)
I (3398) wifi: state: auth -> assoc (0)
I (3408) wifi: state: assoc -> run (10)
I (4078) wifi: connected with [removed], aid = 7, channel 13, BW20, bssid = [removed]
I (4088) wifi: security type: 3, phy: bgn, rssi: -62
I (4108) wifi: pm start, type: 1

I (4108) wifi: AP's beacon interval = 102400 us, DTIM period = 1
I (8108) esp_netif_handlers: sta ip: 192.168.1.38, mask: 255.255.255.0, gw: 192.168.1.1
I (8108) wifi: Got IP: 192.168.1.38
I (8108) wifi: Connected to AP
```

## License

MIT License. Copyright (c) 2020 [Jari Jokinen](https://jarijokinen.com).  See
[LICENSE](https://github.com/jarijokinen/esp32-examples/blob/master/LICENSE.txt)
for further details.
