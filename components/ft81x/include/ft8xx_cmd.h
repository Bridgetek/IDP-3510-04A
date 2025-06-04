/*
 * ft8xx_cmd.h
 *
 *  Created on: 16 Sep 2020
 *      Author: prabhakaran.d
 */

#ifndef INCLUDES_EVE_FT8XX_CMD_H_
#define INCLUDES_EVE_FT8XX_CMD_H_

#include <stdint.h>
#include <stdbool.h>
#include "ft8xx.h"

#define gpu_transfer(buffer, len) ft8xx_hal_coproc_transfer(buffer, len)
#define gpu_transfer_string(s)	  ft8xx_hal_coproc_transfer_string(s)
uint16_t ft8xx_get_writepointer();
void	 ft8xx_update_write_pointer();
void	 ft8xx_swap(bool wait);
bool	 ft8xx_is_idle(void);
void	 gpu_start_transfer(uint16_t len);
void	 gpu_end_transfer(uint16_t size);
void	 gpu_wait_for_idle(void);
void	 ft8xx_command(uint32_t cmd);
void	 ft8xx_command_buffer(const uint8_t *buff, int len);
void	 ft8xx_command_dl_start(FTColor_t color);

void ft8xx_flash_attach();
void ft8xx_flash_detach();
void ft8xx_command_flash_fast(uint32_t result);
void ft8xx_command_flash_update(uint32_t dest, uint32_t src, uint32_t numbytes);
void ft8xx_command_flash_eraseall();
void ft8xx_command_flash_read(uint32_t dest, uint32_t src, uint32_t num);
int	 ft8xx_load_from_mcu_flash_to_gram_coproc(bool init, const uint8_t *buffer, const int address, uint16_t len);
int	 ft8xx_flash_switch_to_full_mode();
void ft8xx_coproc_update_touch_firmware(const uint8_t *touch_firmware, uint16_t len);
void ft8xx_command_mem_set(uint32_t ptr, uint32_t value, uint32_t num);
void ft8xx_command_append(uint32_t ptr, uint32_t num);
void ft8xx_command_memzero(uint32_t ptr, uint32_t num);
void ft8xx_command_memwrite(uint32_t ptr, uint32_t num);
void ft8xx_command_memcpy(uint32_t dest, uint32_t src, uint32_t num);
void ft8xx_command_memcrc(uint32_t ptr, uint32_t num, uint32_t result);

double ft8xx_frame_time();
void   ft8xx_command_sync();
void   ft8xx_command_regread(uint32_t ptr, uint32_t result);
void   ft8xx_command_cold_start();
void   ft8xx_command_calibrate(uint32_t result);
void   ft8xx_command_set_rotate(uint32_t r);
void   ft8xx_command_interrupt(uint32_t ms);

void ft8xx_color_c(uint32_t rgba);
void ft8xx_cmd_fgcolor_c(uint32_t rgba);
void ft8xx_cmd_bgcolor_c(uint32_t rgba);
void ft8xx_cmd_gradcolor_c(uint32_t rgba);
void ft8xx_cmd_fillwidth(uint32_t numpixels);
void ft8xx_cmd_text_c(int16_t x, int16_t y, int16_t font, uint16_t options, const char *s);
void ft8xx_cmd_button_c(int16_t x, int16_t y, int16_t w, int16_t h, int16_t font, uint16_t options, const char *s);
void ft8xx_cmd_slider_c(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range);
void ft8xx_cmd_track_c(uint16_t x, uint16_t y, uint16_t w, uint16_t h, int16_t tag);
void ft8xx_cmd_progress_c(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range);
void ft8xx_cmd_toggle_c(int16_t x, int16_t y, int16_t w, int16_t font, uint16_t options, bool en, const char *s);

void ft8xx_command_set_scratch(uint32_t handle);
void ft8xx_command_load_image(uint32_t ptr, uint32_t options);
void ft8xx_command_get_props(uint32_t ptr, uint32_t w, uint32_t h);
void ft8xx_command_setbitmap(uint32_t source, uint16_t fmt, FTSize_t size);
void ft8xx_command_set_font2(uint32_t font, uint32_t ptr, uint32_t firstchar);
void ft8xx_command_inflate(uint32_t ptr);
void ft8xx_command_get_ptr(uint32_t result);
void ft8xx_command_set_font(uint32_t font, uint32_t ptr);
void ft8xx_command_rom_font(uint32_t font, uint32_t romslot);

void ft8xx_color(FTColor_t color);
void ft8xx_clear_color(FTColor_t color);
void ft8xx_command_fg_color(FTColor_t c);
void ft8xx_command_bg_color(FTColor_t c);
void ft8xx_command_gradcolor(uint32_t c);
void ft8xx_draw_line(FTOffset_t from, FTOffset_t to, FTColor_t color, int16_t width);
void ft8xx_draw_point(FTOffset_t offset, FTColor_t color, uint16_t pointsize, uint8_t tag);
void ft8xx_draw_rectangle(FTOffset_t offset, FTSize_t size, uint16_t width, FTColor_t tcolor, uint8_t tag);
void ft8xx_command_gradient(FTOffset_t offset1, FTColor_t color1, FTOffset_t offset2, FTColor_t color2);
void ft8xx_command_toggle(FTOffset_t offset, int16_t width, int16_t font, uint16_t options, uint16_t state, const char *s);
void ft8xx_command_number(FTOffset_t offset, int16_t font, uint16_t options, int32_t n);
void ft8xx_command_text(FTOffset_t offset, int16_t font, uint16_t options, const char *s);
void ft8xx_command_button(FTOffset_t offset, FTSize_t size, int16_t font, uint16_t options, const char *s);
void ft8xx_command_slider(FTOffset_t offset, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range);
void ft8xx_command_track(FTOffset_t offset, FTSize_t s, int16_t tag);
void ft8xx_command_progress(FTOffset_t offset, FTSize_t s, uint16_t options, uint16_t val, uint16_t range);
void ft8xx_command_stop();
void ft8xx_command_logo();
void ft8xx_command_screen_saver();
void ft8xx_command_spinner(FTOffset_t offset, uint16_t style, uint16_t scale);
void ft8xx_command_toggle(FTOffset_t offset, int16_t width, int16_t font, uint16_t options, uint16_t state, const char *s);
void ft8xx_command_keys(FTOffset_t offset, FTSize_t size, int16_t font, uint16_t options, const char *s);
void ft8xx_command_dial(FTOffset_t offset, int16_t r, uint16_t options, uint16_t val);
void ft8xx_command_clock(FTOffset_t offset, int16_t r, uint16_t options, uint16_t h, uint16_t m, uint16_t s, uint16_t ms);
void ft8xx_command_gauge(FTOffset_t offset, int16_t r, uint16_t options, uint16_t major, uint16_t minor, uint16_t val, uint16_t range);
void ft8xx_command_scroll_bar(FTOffset_t offset, FTSize_t s, uint16_t options, uint16_t val, uint16_t size, uint16_t range);

void ft8xx_command_snapshot(uint32_t ptr);
void ft8xx_command_snapshot2(FTOffset_t offset, FTSize_t s, uint32_t fmt, uint32_t ptr);
void ft8xx_command_sketch(FTOffset_t offset, FTSize_t s, uint32_t ptr, uint16_t format);
void ft8xx_command_touch_transform(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t tx0, int32_t ty0, int32_t tx1, int32_t ty1, int32_t tx2, int32_t ty2, uint16_t result);

void ft8xx_command_set_base(uint32_t base);
void ft8xx_command_load_identity();
void ft8xx_command_set_matrix();
void ft8xx_command_rotate(int32_t a);
void ft8xx_command_scale(int32_t sx, int32_t sy);
void ft8xx_command_translate(int32_t tx, int32_t ty);
void ft8xx_command_get_matrix(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
void ft8xx_command_bitmap_transform(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t tx0, int32_t ty0, int32_t tx1, int32_t ty1, int32_t tx2, int32_t ty2, uint16_t result);

int32_t ft8xx_anim_is_active();
void	ft8xx_command_animxy(uint32_t ch, FTOffset_t offset);
void	ft8xx_command_animframe(FTOffset_t offset, uint32_t aoptr, uint32_t frame);
void	ft8xx_command_animstart(int32_t ch, uint32_t aoptr, uint32_t loop);
void	ft8xx_command_animdraw(int32_t ch);
void	ft8xx_command_animstop(int32_t ch);
void	ft8xx_command_flashsource(uint32_t ptr);
void	ft8xx_command_media_fifo(uint32_t ptr, uint32_t size);
void	ft8xx_command_play_video(uint32_t options);
void	ft8xx_command_video_start();
void	ft8xx_command_video_frame(uint32_t dst, uint32_t ptr);

#endif /* INCLUDES_EVE_FT8XX_CMD_H_ */
