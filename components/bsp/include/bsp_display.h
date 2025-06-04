/*
 * bsp_display.h
 * 
 * Created on: 26 Feb 2025
 *     Author: Vo Van Buong (BRT-SG)
 */

#ifndef BSP_DISPLAY_HDR_H_
#define BSP_DISPLAY_HDR_H_

#include "esp_err.h"
#include "ft8xx.h"
#include "ft8xx_gpu.h"
#include "ft8xx_hal.h"
#include "ft8xx_cmd.h"

esp_err_t bsp_display_init(void);
esp_err_t bsp_display_init_fs(void);
void bsp_display_ui_start(uint32_t rgba);
void bsp_display_ui_end(bool wait);
void bsp_display_ui_store_context(void);
void bsp_display_ui_restore_context(void);
void bsp_display_ui_draw_rect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t rgba);
void bsp_display_ui_draw_text(int16_t x, int16_t y, uint32_t rgba, int16_t font, uint16_t options, const char *s);
esp_err_t bsp_display_load_draw_file(ft8xx_bitmap_info *bitmap);
esp_err_t bsp_display_load_inflate_file(ft8xx_bitmap_info *bitmap);
esp_err_t bsp_display_load_image_file(ft8xx_bitmap_info *bitmap);
esp_err_t bsp_display_ui_draw_bitmap(int16_t x, int16_t y, uint32_t rgba, ft8xx_bitmap_info *bitmap);


#endif /* BSP_DISPLAY_HDR_H_ */
