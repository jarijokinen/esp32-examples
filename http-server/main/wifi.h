#ifndef HTTPD_WIFI_H
#define HTTPD_WIFI_H

#include "esp_netif.h"
#include "esp_system.h"

esp_err_t wifi_connect(void);
esp_err_t wifi_disconnect(void);
esp_netif_t *get_wifi_netif(void);

#endif
