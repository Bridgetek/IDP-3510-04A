/*
 * bsp_board.h
 * 
 * Created on: 26 Feb 2025
 *     Author: Vo Van Buong (BRT-SG)
 */

#ifndef BSP_BOARD_HDR_H_
#define BSP_BOARD_HDR_H_

#include "driver/gpio.h"

/**
 * @brief PD35L LCD GPIO definition and config
 * 
 */
#define FUNC_LCD_EN     (1)
#define FT81x_ENABLE    (1)

#define LCD_SPI_HOST    (SPI2_HOST)
#define LCD_SPI_CLK_HZ  (18) // MHz
#define GPIO_LCD_MOSI   (GPIO_NUM_23)
#define GPIO_LCD_MISO   (GPIO_NUM_25)
#define GPIO_LCD_SCLK   (GPIO_NUM_19)
#define GPIO_LCD_CS     (GPIO_NUM_22)
#define GPIO_LCD_PDN    (GPIO_NUM_18)
#define GPIO_LCD_INT    (GPIO_NUM_21)

#define LCD_WIDTH       (320)
#define LCD_HEIGHT      (240)
#define LCD_HCYCLE      (408)
#define LCD_HOFFSET     (70)
#define LCD_HSYNC0      (0)
#define LCD_HSYNC1      (10)
#define LCD_VCYCLE      (263)
#define LCD_VOFFSET     (13)
#define LCD_VSYNC0      (0)
#define LCD_VSYNC1      (2)
#define LCD_SWIZZLE     (2)
#define LCD_PCLK_POL    (0)
#define LCD_CSPREAD     (1)
#define LCD_DITHER      (1)

/**
 * @brief PD35L RGB LED strip GPIO definition
 * 
 */
#define FUNC_LED_EN     (1)

#define LED_ARRAY_NUM   (7)
#define GPIO_LED_EN     (GPIO_NUM_4)
#define GPIO_LED_CLK    (GPIO_NUM_26)
#define GPIO_LED_DATA   (GPIO_NUM_27)

/**
 * @brief PD35L Button GPIO definition
 * 
 */
#define FUNC_BUTTON_EN  (1)

#define GPIO_BUTTON     (GPIO_NUM_0)

#endif /* BSP_BOARD_HDR_H_ */
