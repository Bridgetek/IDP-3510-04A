/*
 * bsp_rgb_led.c
 * 
 * Created on: 03 Mar 2025
 *     Author: Vo Van Buong (BRT-SG)
 */

#include <stdint.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "bsp_board.h"
#include "bsp_rgb_led.h"

static void spi_tx(uint8_t n);
static void set_single_color(led_color_t p, uint8_t num_of_led);
static void set_array_color(led_color_t *p, uint8_t num_of_led);

static uint8_t toggle;


void bsp_led_init(void)
{
    gpio_config_t led_data_cfg = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1 << GPIO_LED_DATA),
        .pull_up_en = true,
    };
    gpio_config(&led_data_cfg);
    gpio_set_level(GPIO_LED_DATA, 0);

    gpio_config_t led_clk_cfg = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1 << GPIO_LED_CLK),
        .pull_up_en = true,
    };
    gpio_config(&led_clk_cfg);
    gpio_set_level(GPIO_LED_CLK, 0);

    gpio_config_t led_en_cfg = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1 << GPIO_LED_EN),
    };
    gpio_config(&led_en_cfg);
    gpio_set_level(GPIO_LED_EN, 1);
}

void bsp_led_turn_off(void)
{
    gpio_set_level(GPIO_LED_CLK, 1);
    gpio_set_level(GPIO_LED_EN, 0);
    gpio_set_level(GPIO_LED_DATA, 1);
}

void bsp_led_set_single_color(uint32_t color, uint8_t num_of_led)
{
    led_color_t c = {.u32 = color};
    set_single_color(c, num_of_led);
}

void bsp_led_set_array_color(led_color_t *color, uint8_t num_of_led)
{
    set_array_color(color, num_of_led);
}

void bsp_led_toggle_single_color(uint32_t color, uint8_t num_of_led)
{
    led_color_t c = {.u32 = color};

    if (toggle == 0)
    {
        toggle = 1;
    }
    else
    {
        toggle = 0;
        c.i = 0;
    }
    set_single_color(c, num_of_led);
}

void bsp_led_toggle_array_color(led_color_t *color, uint8_t num_of_led)
{
    led_color_t c[LED_ARRAY_NUM];
    for (int i = 0; i < LED_ARRAY_NUM; i++)
    {
        c[i].u32 = color[i].u32;
    }
    if (toggle == 0)
    {
        toggle = 1;
    }
    else
    {
        toggle = 0;
        for (int i = 0; i < LED_ARRAY_NUM; i++)
        {
            c[i].i = 0;
        }
    }
    set_array_color(c, num_of_led);
}

static void set_single_color(led_color_t color, uint8_t num_of_led)
{
    for (int i = 0; i < 4; i++)
    {
        spi_tx(0x00);  // 4 byte start-frame marker
    }
  
    for (int j = 0; j < num_of_led; j++)
    {
        spi_tx(color.i | 0xC0);
        spi_tx(color.b);
        spi_tx(color.g);
        spi_tx(color.r);
    }
  
    for (int i = 0; i < 4; i++)
    {
        spi_tx(0xFF);
    }
}

static void set_array_color(led_color_t *color, uint8_t num_of_led)
{
    for (int i = 0; i < 4; i++)
    {
        spi_tx(0x00);  // 4 byte start-frame marker
    }
  
    for (int j = 0; j < num_of_led; j++)
    {
        spi_tx(color[j].i | 0xC0);
        spi_tx(color[j].b);
        spi_tx(color[j].g);
        spi_tx(color[j].r);
    }
  
    for (int i = 0; i < 4; i++)
    {
        spi_tx(0xFF);
    }
}

static void spi_tx(uint8_t n)
{
    uint8_t led_data_pin = GPIO_LED_DATA;
    uint8_t led_clk_pin = GPIO_LED_CLK;
  
    for (uint8_t i = 8; i--; n <<= 1) {
      if (n & 0x80) {
        gpio_set_level(led_data_pin, 1);
      } else {
        gpio_set_level(led_data_pin, 0);
      }
  
      gpio_set_level(led_clk_pin, 1);
      gpio_set_level(led_clk_pin, 0);
    }
}
