#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_random.h"
#include "nvs_flash.h"
#include "bsp_rgb_led.h"
#include "bsp_display.h"
#include "ui.h"

#define DEFAULT_SCAN_LIST_SIZE 10
#define EXAMPLE_ESP_WIFI_SSID "BRTCHIP_Guest"
#define EXAMPLE_ESP_WIFI_PASS "chip@solaris"

/* FreeRTOS event group to signal when we are connected*/
EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
#define WIFI_SCAN_DONE_BIT BIT2
#define WIFI_LED_TASK_DONE BIT3

static const char *TAG = "wifi station";

static int s_retry_num = 0;
static esp_netif_ip_info_t ip_info;

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < 5) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "Retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"Connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        memcpy(&ip_info, &event->ip_info, sizeof(esp_netif_ip_info_t));
        ESP_LOGI(TAG, "Got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

esp_err_t get_wifi_signal_strength(int *wifi_signal)
{
    int rssi = 0;
    if (esp_wifi_sta_get_rssi(&rssi) != ESP_OK)
    {
        return ESP_FAIL;
    }
    if (rssi <= -100)
    {
        *wifi_signal = 0;
    }
    else if (rssi >= -50)
    {
        *wifi_signal = 100;
    }
    else
    {
        *wifi_signal = 2 * (rssi + 100);
    }
    return ESP_OK;
}

void wifi_scanning_led_blink_task(void *param)
{
    led_color_t color = {.u32 = LED_COLOR(YELLOW_GREEN, BSP_LED_INTENSITY_MEDIUM)};
    while (1)
    {
        EventBits_t bits = xEventGroupGetBits(s_wifi_event_group);
        if (bits & WIFI_SCAN_DONE_BIT)
        {
            break;
        }
        bsp_led_toggle_single_color(color.u32, LED_ARRAY_NUM);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    xEventGroupSetBits(s_wifi_event_group, WIFI_LED_TASK_DONE);
    vTaskDelete(NULL);
}

void wifi_connected_done_led_task(void *param)
{
    led_color_t led[LED_ARRAY_NUM] = {
        {.u32 = LED_COLOR(GREEN1, BSP_LED_INTENSITY_MEDIUM)},
        {.u32 = LED_COLOR(GREEN4, BSP_LED_INTENSITY_MEDIUM)},
        {.u32 = LED_COLOR(YELLOW4, BSP_LED_INTENSITY_MEDIUM)},
        {.u32 = LED_COLOR(YELLOW1, BSP_LED_INTENSITY_MEDIUM)},
        {.u32 = LED_COLOR(ORANGE1, BSP_LED_INTENSITY_MEDIUM)},
        {.u32 = LED_COLOR(ORANGE4, BSP_LED_INTENSITY_MEDIUM)},
        {.u32 = LED_COLOR(RED4, BSP_LED_INTENSITY_MEDIUM)}
    };
    uint8_t num_of_led_on = 0;
    uint8_t is_up = 1;
    while (1)
    {
        num_of_led_on = is_up ? num_of_led_on + 1 : num_of_led_on - 1;
        if (num_of_led_on == LED_ARRAY_NUM)
        {
            is_up = 0;
        }
        if (num_of_led_on == 0)
        {
            is_up = 1;
        }
        for (int i = 0; i < LED_ARRAY_NUM; i++)
        {
            if (i < num_of_led_on)
            {
                led[i].i = BSP_LED_INTENSITY_MEDIUM;
            }
            else
            {
                led[i].i = BSP_LED_INTENSITY_OFF;
            }
        }
        bsp_led_set_array_color(led, LED_ARRAY_NUM);
        vTaskDelay(70 / portTICK_PERIOD_MS);
    }
}

/* Initialize Wi-Fi as sta and set scan method */
void wifi_scan(void)
{
    s_wifi_event_group = xEventGroupCreate();
    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    // Draw scanning screen
    xTaskCreate(wifi_scanning_led_blink_task, "WIFI_LED", 2048, NULL, 5, NULL);
    ui_wifi_scan_screen(NULL, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    // Start scanning nearby APs
    esp_wifi_scan_start(NULL, true);
    xEventGroupSetBits(s_wifi_event_group, WIFI_SCAN_DONE_BIT);
    xEventGroupWaitBits(s_wifi_event_group, WIFI_LED_TASK_DONE, true, true, portMAX_DELAY);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
    ESP_ERROR_CHECK(esp_wifi_stop());
    // Draw list of scanned APs
    bsp_led_set_single_color(LED_COLOR(GREEN4, BSP_LED_INTENSITY_MEDIUM), LED_ARRAY_NUM);
    ui_wifi_scan_screen(ap_info, ap_count);
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    xEventGroupClearBits(s_wifi_event_group, WIFI_SCAN_DONE_BIT);
    xTaskCreate(wifi_scanning_led_blink_task, "WIFI_LED", 2048, NULL, 5, NULL);
    // Draw connecting screen
    ui_wifi_connecting_screen(EXAMPLE_ESP_WIFI_SSID);

    // Connecting to defined AP
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
            .sae_h2e_identifier = "",
        },
    };
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start());

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    xEventGroupSetBits(s_wifi_event_group, WIFI_SCAN_DONE_BIT);
    xEventGroupWaitBits(s_wifi_event_group, WIFI_LED_TASK_DONE, true, true, portMAX_DELAY);
    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
    * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s", EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
        int wifi_signal = 0;
        xTaskCreate(wifi_connected_done_led_task, "WIFI_LED", 2048, NULL, 5, NULL);
        ft8xx_play_synth_sound(0x49, 96, 0x77);
        while (1)
        {
            get_wifi_signal_strength(&wifi_signal);
            ui_wifi_connected_screen(&ip_info, wifi_signal);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        } 
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s", EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }
    bsp_led_set_single_color(LED_COLOR(RED4, BSP_LED_INTENSITY_MEDIUM), LED_ARRAY_NUM);
    ft8xx_play_synth_sound(0x1A, 0, 0xff);
    ui_wifi_disconnected_screen();
}

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    bsp_led_init();
    ESP_ERROR_CHECK(bsp_display_init());
    bsp_led_set_single_color(LED_COLOR(GREEN4, BSP_LED_INTENSITY_MEDIUM), LED_ARRAY_NUM);
    ui_init();
    ui_welcome_screen();
    // Play bell sound
    ft8xx_play_synth_sound(0x49, 96, 0x77);
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    wifi_scan();
}
