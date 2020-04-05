#include <esp_event.h>
#include <esp_log.h>
#include <esp_netif.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <esp_wifi_default.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>
#include <lwip/err.h>
#include <lwip/sys.h>
#include <string.h>

#include "wifi.h"

#define WIFI_SSID ""
#define WIFI_PSK ""

static EventGroupHandle_t s_wifi_event_group;
static esp_ip4_addr_t s_ip_addr;
static esp_netif_t *s_wifi_esp_netif = NULL;

static void on_got_ip(void *arg, esp_event_base_t event_base, int32_t event_id,
                      void *event_data)
{
  ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;

  memcpy(&s_ip_addr, &event->ip_info.ip, sizeof(s_ip_addr));
  xEventGroupSetBits(s_wifi_event_group, BIT0);
}

esp_err_t wifi_connect(void)
{
  s_wifi_event_group = xEventGroupCreate();
  
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  esp_netif_config_t netif_config = ESP_NETIF_DEFAULT_WIFI_STA();
  esp_netif_t *netif = esp_netif_new(&netif_config);
  assert(netif);

  esp_netif_attach_wifi_station(netif);
  esp_wifi_set_default_wifi_sta_handlers();

  s_wifi_esp_netif = netif;
 
  ESP_ERROR_CHECK(esp_event_handler_register(
    IP_EVENT, IP_EVENT_STA_GOT_IP, &on_got_ip, NULL
  ));

  wifi_config_t wifi_config = {
    .sta = {
      .ssid = WIFI_SSID,
      .password = WIFI_PSK
    }
  };
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_connect());

  xEventGroupWaitBits(s_wifi_event_group, BIT0, true, true, portMAX_DELAY);
  
  return ESP_OK;
}

esp_err_t wifi_disconnect(void)
{
  vEventGroupDelete(s_wifi_event_group);
  s_wifi_event_group = NULL;

  return ESP_OK;
}

esp_netif_t *get_wifi_netif(void)
{
  return s_wifi_esp_netif;
}
