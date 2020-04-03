#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "nvs_flash.h"

#define WIFI_SSID ""
#define WIFI_PSK ""
#define WIFI_MAX_RETRIES 5

static const char *TAG = "wifi";
static int s_retry_num = 0;
static EventGroupHandle_t s_wifi_event_group;

static void wifi_event_handler(void *arg, esp_event_base_t event_base, 
                               int32_t event_id, void *event_data) {
  switch (event_id) {
    case WIFI_EVENT_STA_START:
      /* Station started; begin the scan/connection process */
      esp_wifi_connect();
      break;
    case WIFI_EVENT_STA_DISCONNECTED:
      /* Station disconnected from the AP; try to reconnect */
      if (s_retry_num < WIFI_MAX_RETRIES) {
        esp_wifi_connect();
        s_retry_num++;
      }
      else {
        /* Set BIT1 to indicate that the connection failed */
        xEventGroupSetBits(s_wifi_event_group, BIT1);
      }
      break;
  }
}

static void ip_event_handler(void *arg, esp_event_base_t event_base, 
                             int32_t event_id, void *event_data) {
  ip_event_got_ip_t *event;

  switch (event_id) {
    case IP_EVENT_STA_GOT_IP:
      s_retry_num = 0;

      /* IP address succesfully received from the DHCP server; print it out */
      event = (ip_event_got_ip_t *) event_data;
      ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));

      /* Set BIT0 to indicate that the connection is established */
      xEventGroupSetBits(s_wifi_event_group, BIT0);

      break;
  }
}

esp_err_t wifi_initialize_station(void) {
  /* Create an event group to indicate if we are connected or not */
  s_wifi_event_group = xEventGroupCreate();
  
  /* Initialize the TCP/IP stack */
  ESP_ERROR_CHECK(esp_netif_init());

  /* Create a default event loop for WiFi events */
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  
  /* Create a default WiFi station */
  esp_netif_create_default_wifi_sta();

  /* Initialize the WiFi driver with default values */
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  /* Register an event handler for all WIFI_EVENTs */
  esp_event_handler_instance_t instance_any_id;
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
    WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_any_id
  ));
  
  /* Register an event handler for IP_EVENT_STA_GOT_IP */
  esp_event_handler_instance_t instance_got_ip;
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
    IP_EVENT, IP_EVENT_STA_GOT_IP, &ip_event_handler, NULL, &instance_got_ip
  ));
  
  /* Set the WiFi operating mode to station (STA) */
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

  /* Set the WiFi configuration */
  wifi_config_t wifi_config = {
    .sta = {
      .ssid = WIFI_SSID,
      .password = WIFI_PSK
    }
  };
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));

  /* Start WiFi, create a station control block and start the station */
  ESP_ERROR_CHECK(esp_wifi_start());

  /* Wait until connection is established (BIT0) or failed (BIT1) */
  EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
    BIT0 | BIT1, pdFALSE, pdFALSE, portMAX_DELAY);

  /* Check the event bits */
  if (bits & BIT0) {
    ESP_LOGI(TAG, "Connected to AP");
  }
  else if (bits & BIT1) {
    ESP_LOGI(TAG, "Failed to connect to AP");
  }

  /* Unregister event handlers */
  ESP_ERROR_CHECK(esp_event_handler_instance_unregister(
    IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip
  ));
  ESP_ERROR_CHECK(esp_event_handler_instance_unregister(
    WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id
  ));

  /* Delete the event group */
  vEventGroupDelete(s_wifi_event_group);
  
  return ESP_OK;
}

void app_main(void) {
  esp_err_t ret = 0;

  ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || 
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  wifi_initialize_station();
}
