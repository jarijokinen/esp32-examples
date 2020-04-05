#include <esp_event.h>
#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <nvs_flash.h>

#include "wifi.h"

static const char *TAG = "http-server";

static esp_err_t index_page_get_handler(httpd_req_t *req)
{
  const char *resp_str = (const char *)req->user_ctx;
  httpd_resp_send(req, resp_str, strlen(resp_str));

  return ESP_OK;
}

static const httpd_uri_t index_page = {
  .uri = "/",
  .method = HTTP_GET,
  .handler = index_page_get_handler,
  .user_ctx = "<html><head><title>Hello</title><body>Hello World</body></html>"
};

static httpd_handle_t start_server(void)
{
  httpd_handle_t server = NULL;
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();

  ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
  if (httpd_start(&server, &config) == ESP_OK) {
    httpd_register_uri_handler(server, &index_page);
    return server;
  }
  
  return NULL;
}

static void stop_server(httpd_handle_t server)
{
  httpd_stop(server);
}

static void connect_handler(void *arg, esp_event_base_t event_base, 
                            int32_t event_id, void *event_data) 
{
  httpd_handle_t *server = (httpd_handle_t *)arg;
  
  if (*server == NULL) {
    *server = start_server();
  }
}

static void disconnect_handler(void *arg, esp_event_base_t event_base, 
                               int32_t event_id, void *event_data) 
{
  httpd_handle_t *server = (httpd_handle_t *)arg;

  if (*server) {
    stop_server(*server);
    *server = NULL;
  }
}

void app_main(void)
{
  static httpd_handle_t server = NULL;

  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  ESP_ERROR_CHECK(wifi_connect());
  
  ESP_ERROR_CHECK(esp_event_handler_register(
    IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server
  ));
  ESP_ERROR_CHECK(esp_event_handler_register(
    WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server
  ));

  server = start_server();
}
