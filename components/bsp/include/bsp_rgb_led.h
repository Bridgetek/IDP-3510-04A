/*
 * bsp_rgb_led.h
 * 
 * Created on: 03 Mar 2025
 *     Author: Vo Van Buong (BRT-SG)
 */

#ifndef BSP_RGB_LED_HDR_H_
#define BSP_RGB_LED_HDR_H_

#include <stdint.h>
#include "bsp_board.h"
#include "colornames.h"

#define BSP_LED_INTENSITY_MAX       31U
#define BSP_LED_INTENSITY_HIGH      BSP_LED_INTENSITY_MAX
#define BSP_LED_INTENSITY_MEDIUM    16U
#define BSP_LED_INTENSITY_LOW       8U
#define BSP_LED_INTENSITY_OFF       0U

#define LED_COLOR(color, intensity) (uint32_t)(color | (intensity << 24))

typedef union
{
    struct
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t i;  /* Intensity [0 - 31] */
    };
    uint32_t u32;
} led_color_t;

void bsp_led_init(void);
void bsp_led_turn_off(void);
void bsp_led_set_single_color(uint32_t color, uint8_t num_of_led);
void bsp_led_set_array_color(led_color_t *color, uint8_t num_of_led);
void bsp_led_toggle_single_color(uint32_t color, uint8_t num_of_led);
void bsp_led_toggle_array_color(led_color_t *color, uint8_t num_of_led);

#endif /* BSP_RBB_LED_HDR_H_ */
