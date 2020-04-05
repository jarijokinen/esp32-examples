#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_gap_ble_api.h>
#include <esp_log.h>
#include <nvs_flash.h>

static const char *TAG = "ruuvitag";

static esp_ble_scan_params_t ble_scan_params = {
  .scan_type          = BLE_SCAN_TYPE_ACTIVE,
  .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
  .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
  .scan_interval      = 0x50,
  .scan_window        = 0x30
};

static void esp_gap_cb(esp_gap_ble_cb_event_t event,
                       esp_ble_gap_cb_param_t *param)
{
  switch (event) {
    case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT: {
      uint32_t duration = 0;
      esp_ble_gap_start_scanning(duration);
      break;
    }
    case ESP_GAP_BLE_SCAN_RESULT_EVT: {
      esp_ble_gap_cb_param_t *res = (esp_ble_gap_cb_param_t *)param;

      if (res->scan_rst.adv_data_len > 0) {
        /* Manufacturer: Ruuvi Innovations (0x9904) */
        if (res->scan_rst.ble_adv[5] == 0x99 && 
            res->scan_rst.ble_adv[6] == 0x04) {

          /* Data format: 0x03 */
          if (res->scan_rst.ble_adv[7]== 0x03) {
            ESP_LOGI(TAG, "");

            /* Device name */
            ESP_LOGI(TAG, "** RuuviTag: %02X:%02X:%02X:%02X:%02X:%02X",
              res->scan_rst.bda[0], res->scan_rst.bda[1], res->scan_rst.bda[2],
              res->scan_rst.bda[3], res->scan_rst.bda[4], res->scan_rst.bda[5]);

            /* Temperature */
            ESP_LOGI(TAG, "Temperature: %d.%02d C", 
              res->scan_rst.ble_adv[9], 
              res->scan_rst.ble_adv[10]);
            
            /* Humidity */
            ESP_LOGI(TAG, "Humidity:    %.2f %%", 
              (double)(res->scan_rst.ble_adv[8] / 2));
          }
        }
      }
      break;
    }
    default:
      break;
  }
}

void app_main(void)
{
  ESP_ERROR_CHECK(nvs_flash_init());

  ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
  esp_bt_controller_config_t cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  esp_bt_controller_init(&cfg);
  esp_bt_controller_enable(ESP_BT_MODE_BLE);

  esp_bluedroid_init();
  esp_bluedroid_enable();

  esp_ble_gap_register_callback(esp_gap_cb);
  esp_ble_gap_set_scan_params(&ble_scan_params);
}
