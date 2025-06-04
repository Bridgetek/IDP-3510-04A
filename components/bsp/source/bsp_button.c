/*
 * bsp_button.c
 * 
 * Created on: 03 Mar 2025
 *     Author: Vo Van Buong (BRT-SG)
 */

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_timer.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "bsp_button.h"

#define GPIO_QUEUE_SIZE     10

static const char *TAG = "bsp_button";

static QueueHandle_t gpio_event_queue = NULL;
static TaskHandle_t gpio_task_handle = NULL;
static uint32_t g_long_press_time = 1000;
static uint32_t g_press_time = 100;
static uint8_t g_short_press_detected = 0;
static uint8_t g_long_press_detected = 0;

static void gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(gpio_event_queue, &gpio_num, &pxHigherPriorityTaskWoken);
}

static uint32_t millis(void)
{
    return esp_timer_get_time() / 1000;
}

static void gpio_task(void *arg)
{
    uint32_t gpio_num;
    int intr_neg_edge = 0;
    int last_press_time = 0;
    int last_long_press_time = 0;

    while (1)
    {
        if (xQueueReceive(gpio_event_queue, &gpio_num, 10 / portTICK_PERIOD_MS))
        {
            if (gpio_get_level(gpio_num) == 0)
            {
                last_press_time = millis();
                last_long_press_time = millis();
                intr_neg_edge = 1;
            }
            else
            {
                uint32_t down_time = millis() - last_press_time;
                intr_neg_edge = 0;
                if (down_time >= g_press_time && down_time < g_long_press_time)
                {
                    if (g_short_press_detected == 0)
                    {
                        g_short_press_detected = 1;
                        g_long_press_detected = 0;
                    }
                }
                last_press_time = millis();
            }
        }
        else if (intr_neg_edge)
        {
            uint32_t down_time = millis() - last_long_press_time;

            if (down_time >= g_long_press_time)
            {
                last_long_press_time = millis();
                if (g_long_press_detected == 0)
                {
                    g_long_press_detected = 1;
                    g_short_press_detected = 0;
                }
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void bsp_button_init(void)
{
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_ANYEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1 << GPIO_BUTTON),
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    };
    gpio_config(&io_conf);

    gpio_event_queue = xQueueCreate(GPIO_QUEUE_SIZE, sizeof(uint32_t));
    if (gpio_event_queue == NULL)
    {
        ESP_LOGE(TAG, "Init event queue failed");
        return;
    }

    xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 10, &gpio_task_handle);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_BUTTON, gpio_isr_handler, (void*)GPIO_BUTTON);
}

void bsp_button_task_set_priority(UBaseType_t priority)
{
    vTaskPrioritySet(gpio_task_handle, priority);
}

void bsp_button_set_trigger_time(uint32_t press_time, uint32_t long_press_time)
{
    g_press_time = press_time;
    g_long_press_time = long_press_time;
}

void bsp_button_clear_short_press_flag(void)
{
    g_short_press_detected = 0;
}

void bsp_button_clear_long_press_flag(void)
{
    g_long_press_detected = 0;
}

uint8_t bsp_button_read_short_press_flag(void)
{
    return g_short_press_detected;
}

uint8_t bsp_button_read_long_press_flag(void)
{
    return g_long_press_detected;
}
