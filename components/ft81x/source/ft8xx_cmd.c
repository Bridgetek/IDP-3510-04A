#include <stdio.h>
#include <stdlib.h>

#include "ft8xx_gpu.h"
#include "ft8xx_hal.h"
#include "ft8xx_cmd.h"

#define COPROC_ADDRESS(a) (RAM_CMD + a)
#define MAKE_U32(m, l)	  ((((uint32_t)(m)) << 16) | l)

static uint16_t gpu_writepointer = 0;

uint16_t ft8xx_get_writepointer()
{
    return gpu_writepointer;
}

void ft8xx_update_write_pointer()
{
    gpu_writepointer = ft8xx_hal_rd16(REG_CMD_WRITE);
}

bool ft8xx_is_idle(void)
{
    return (gpu_writepointer == ft8xx_hal_rd16(REG_CMD_READ));
}

void ft8xx_swap(bool wait)
{
    gpu_start_transfer(8);
    ft8xx_hal_coproc_command(DISPLAY());
    ft8xx_hal_coproc_command(CMD_SWAP);
    gpu_end_transfer(8);
    if (wait)
        gpu_wait_for_idle();
}

static uint16_t gpu_get_freespace()
{
    uint16_t fullness = (gpu_writepointer - ft8xx_hal_rd16(REG_CMD_READ));
    fullness &= (FT_CMD_FIFO_SIZE - 1);
    return (FT_CMD_FIFO_SIZE - FT_CMD_SIZE) - fullness;
}

static void gpu_check_freespace(uint8_t no_of_bytes)
{
    do {
    } while (gpu_get_freespace() < no_of_bytes);
}

void gpu_start_transfer(uint16_t len)
{
    gpu_check_freespace(len);
    ft8xx_hal_coproc_start_transfer(COPROC_ADDRESS(gpu_writepointer));
}

void gpu_end_transfer(uint16_t size)
{
    ft8xx_hal_coproc_end_transfer();
    gpu_writepointer = (gpu_writepointer + size) & (FT_CMD_FIFO_SIZE - 1);
    gpu_writepointer = (gpu_writepointer + 3) & (0xffc);
    ft8xx_hal_wr16(REG_CMD_WRITE, gpu_writepointer);
}

void gpu_wait_for_idle(void)
{
    // int retries = 0;
    uint16_t readback;
    do {
        readback = ft8xx_hal_rd16(REG_CMD_READ);
        // if (retries++ % 100000 == 0) {
        // 	printf("%s(): gpu_writepointer = %d, actual = %d\n", __func__, gpu_writepointer, readback);
        // }
    } while (readback != gpu_writepointer);
}

void ft8xx_command(uint32_t cmd)
{
    gpu_start_transfer(FT_CMD_SIZE);
    ft8xx_hal_coproc_command(cmd);
    gpu_end_transfer(FT_CMD_SIZE);
}

void ft8xx_command_buffer(const uint8_t *buff, int len)
{
    gpu_check_freespace(len);
    ft8xx_hal_wr8s(RAM_CMD + gpu_writepointer, (uint8_t *)buff, len);
    gpu_end_transfer(len);
    gpu_wait_for_idle();
}

void ft8xx_command_calibrate(uint32_t result)
{
    uint32_t commandbuffer[] = {
        CMD_CALIBRATE,
        result,
    };
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
    gpu_wait_for_idle();
}

void ft8xx_command_dl_start(FTColor_t color)
{
    gpu_start_transfer(12);
    ft8xx_hal_coproc_command(CMD_DLSTART);
    ft8xx_hal_coproc_command(CLEAR_COLOR_RGB(color.rgba.red, color.rgba.green, color.rgba.blue));
    if (color.rgba.alpha == 0 || color.rgba.alpha == 0xFF) {
        ft8xx_hal_coproc_command(CLEAR(1, 1, 1));
    }
    else {
        ft8xx_hal_coproc_command(CLEAR(0, 1, 1));
    }
    gpu_end_transfer(12);
}

void ft8xx_color(FTColor_t color)
{
    ft8xx_command(COLOR_A(color.rgba.alpha));
    ft8xx_command(COLOR_RGB(color.rgba.red, color.rgba.green, color.rgba.blue));
}

void ft8xx_command_fg_color(FTColor_t c)
{
    uint16_t cslen = 8;
    gpu_start_transfer(cslen);
    ft8xx_hal_coproc_command(CMD_FGCOLOR);
    ft8xx_hal_coproc_command(c.value >> 8);
    gpu_end_transfer(cslen);
}

void ft8xx_command_bg_color(FTColor_t c)
{
    uint16_t cslen = 8;
    gpu_start_transfer(cslen);
    ft8xx_hal_coproc_command(CMD_BGCOLOR);
    ft8xx_hal_coproc_command(c.value >> 8);
    gpu_end_transfer(cslen);
}

void ft8xx_color_c(uint32_t rgba)
{
    ft8xx_command(COLOR_A(rgba));
    ft8xx_command(COLOR_RGB(rgba >> 24, rgba >> 16, rgba >> 8));
}

void ft8xx_cmd_fgcolor_c(uint32_t rgba)
{
    uint16_t cslen = 8;
    gpu_start_transfer(cslen);
    ft8xx_hal_coproc_command(CMD_FGCOLOR);
    ft8xx_hal_coproc_command(rgba >> 8);
    gpu_end_transfer(cslen);
}

void ft8xx_cmd_bgcolor_c(uint32_t rgba)
{
    uint16_t cslen = 8;
    gpu_start_transfer(cslen);
    ft8xx_hal_coproc_command(CMD_BGCOLOR);
    ft8xx_hal_coproc_command(rgba >> 8);
    gpu_end_transfer(cslen);
}

void ft8xx_cmd_gradcolor_c(uint32_t rgba)
{
    uint16_t cslen = 8;
    gpu_start_transfer(cslen);
    ft8xx_hal_coproc_command(CMD_GRADCOLOR);
    ft8xx_hal_coproc_command(rgba >> 8);
    gpu_end_transfer(cslen);
}

#if 0
void ft8xx_cmd_fillwidth(uint32_t numpixels)
{
    uint16_t cslen = 8;
    gpu_start_transfer(cslen);
    ft8xx_hal_coproc_command(CMD_FILLWIDTH);
    ft8xx_hal_coproc_command(numpixels);
    gpu_end_transfer(cslen);
}
#endif

void ft8xx_command_text(FTOffset_t offset, int16_t font, uint16_t options, const char *s)
{
    uint16_t cslen = (uint16_t)(12 + strnlen(s, FT_SONARQ_STR_MAXLEN) + 1);
    gpu_start_transfer(cslen);
    ft8xx_hal_coproc_command(CMD_TEXT);
    ft8xx_hal_coproc_command(MAKE_U32(offset.axis.y, offset.axis.x));
    ft8xx_hal_coproc_command(MAKE_U32(options, font));
    gpu_transfer_string(s);
    gpu_end_transfer(cslen);
}

void ft8xx_cmd_text_c(int16_t x, int16_t y, int16_t font, uint16_t options, const char *s)
{
    uint16_t cslen = (uint16_t)(12 + strnlen(s, FT_SONARQ_STR_MAXLEN) + 1);
    gpu_start_transfer(cslen);
    ft8xx_hal_coproc_command(CMD_TEXT);
    ft8xx_hal_coproc_command(MAKE_U32(y, x));
    ft8xx_hal_coproc_command(MAKE_U32(options, font));
    gpu_transfer_string(s);
    gpu_end_transfer(cslen);
}

void ft8xx_cmd_button_c(int16_t x, int16_t y, int16_t w, int16_t h, int16_t font, uint16_t options, const char *s)
{
    uint16_t cslen = (uint16_t)(16 + strnlen(s, FT_SONARQ_STR_MAXLEN) + 1);
    gpu_start_transfer(cslen);
    ft8xx_hal_coproc_command(CMD_BUTTON);
    ft8xx_hal_coproc_command(MAKE_U32(y, x));
    ft8xx_hal_coproc_command(MAKE_U32(h, w));
    ft8xx_hal_coproc_command(MAKE_U32(options, font));
    gpu_transfer_string(s);
    gpu_end_transfer(cslen);
}

void ft8xx_cmd_slider_c(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range)
{
    uint32_t commandbuffer[5];
    commandbuffer[0] = CMD_SLIDER;
    commandbuffer[1] = MAKE_U32(y, x);
    commandbuffer[2] = MAKE_U32(h, w);
    commandbuffer[3] = MAKE_U32(val, options);
    commandbuffer[4] = range;

    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_cmd_track_c(uint16_t x, uint16_t y, uint16_t w, uint16_t h, int16_t tag)
{
    uint32_t commandbuffer[4];

    commandbuffer[0] = CMD_TRACK;
    commandbuffer[1] = MAKE_U32(y, x);
    commandbuffer[2] = MAKE_U32(h, w);
    commandbuffer[3] = tag;

    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_cmd_progress_c(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range)
{
    uint32_t commandbuffer[5];
    commandbuffer[0] = CMD_PROGRESS;
    commandbuffer[1] = MAKE_U32(y, x);
    commandbuffer[2] = MAKE_U32(h, w);
    commandbuffer[3] = MAKE_U32(val, options);
    commandbuffer[4] = range;

    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_cmd_toggle_c(int16_t x, int16_t y, int16_t w, int16_t font, uint16_t options, bool en, const char *s)
{
    if (s == NULL)
        return;

    uint16_t state = en ? 65535 : 0;
    uint16_t cslen = (uint16_t)(16 + strnlen(s, FT_SONARQ_STR_MAXLEN) + 1);
    gpu_start_transfer(cslen);
    ft8xx_hal_coproc_command(CMD_TOGGLE);
    ft8xx_hal_coproc_command(MAKE_U32(y, x));
    ft8xx_hal_coproc_command(MAKE_U32(font, w));
    ft8xx_hal_coproc_command(MAKE_U32(state, options));
    gpu_transfer_string(s);
    gpu_end_transfer(cslen);
}

void ft8xx_command_slider(FTOffset_t offset, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range)
{
    uint32_t commandbuffer[5];
    commandbuffer[0] = CMD_SLIDER;
    commandbuffer[1] = MAKE_U32(offset.axis.y, offset.axis.x);
    commandbuffer[2] = MAKE_U32(h, w);
    commandbuffer[3] = MAKE_U32(val, options);
    commandbuffer[4] = range;

    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_track(FTOffset_t offset, FTSize_t s, int16_t tag)
{
    uint32_t commandbuffer[4];
    ft8xx_command(TAG_MASK(1));
    ft8xx_command(TAG(tag));

    commandbuffer[0] = CMD_TRACK;
    commandbuffer[1] = MAKE_U32(offset.axis.y, offset.axis.x);
    commandbuffer[2] = MAKE_U32(s.prp.height, s.prp.width);
    commandbuffer[3] = tag;

    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);

    ft8xx_command(TAG_MASK(0));
}

void ft8xx_command_button(FTOffset_t offset, FTSize_t size, int16_t font, uint16_t options, const char *s)
{
    uint16_t cslen = (uint16_t)(16 + strnlen(s, FT_SONARQ_STR_MAXLEN) + 1);
    gpu_start_transfer(cslen);
    ft8xx_hal_coproc_command(CMD_BUTTON);
    ft8xx_hal_coproc_command(MAKE_U32(offset.axis.y, offset.axis.x));
    ft8xx_hal_coproc_command(MAKE_U32(size.prp.height, size.prp.width));
    ft8xx_hal_coproc_command(MAKE_U32(options, font));
    gpu_transfer_string(s);
    gpu_end_transfer(cslen);
}

void ft8xx_command_gradient(FTOffset_t offset1, FTColor_t color1, FTOffset_t offset2, FTColor_t color2)
{
    uint32_t commandbuffer[5];
    commandbuffer[0] = CMD_GRADIENT;
    commandbuffer[1] = MAKE_U32(offset1.axis.y, offset1.axis.x);
    commandbuffer[2] = color1.value >> 8;
    commandbuffer[3] = MAKE_U32(offset2.axis.y, offset2.axis.x);
    commandbuffer[4] = color2.value >> 8;

    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_toggle(FTOffset_t offset, int16_t width, int16_t font, uint16_t options, uint16_t state, const char *s)
{
    uint16_t cslen = (uint16_t)(16 + strnlen(s, FT_SONARQ_STR_MAXLEN) + 1);
    gpu_start_transfer(cslen);
    ft8xx_hal_coproc_command(CMD_TOGGLE);
    ft8xx_hal_coproc_command(MAKE_U32(offset.axis.y, offset.axis.x));
    ft8xx_hal_coproc_command(MAKE_U32(font, width));
    ft8xx_hal_coproc_command(MAKE_U32(state, options));
    gpu_transfer_string(s);
    gpu_end_transfer(cslen);
}


void ft8xx_command_number(FTOffset_t offset, int16_t font, uint16_t options, int32_t n)
{
    gpu_start_transfer(16);
    ft8xx_hal_coproc_command(CMD_NUMBER);
    ft8xx_hal_coproc_command(MAKE_U32(offset.axis.y, offset.axis.x));
    ft8xx_hal_coproc_command(MAKE_U32(options, font));
    ft8xx_hal_coproc_command(n);
    gpu_end_transfer(16);
}

void ft8xx_command_load_identity()
{
    uint32_t commandbuffer[4];
    commandbuffer[0] = CMD_LOADIDENTITY;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}



/* Error handling for val is not done, so better to always use range of 65535 in order that needle is drawn within display region */

void ft8xx_command_gauge(FTOffset_t offset, int16_t r, uint16_t options, uint16_t major, uint16_t minor, uint16_t val, uint16_t range)
{
    uint32_t commandbuffer[5];
    commandbuffer[0] = CMD_GAUGE;
    commandbuffer[1] = MAKE_U32(offset.axis.y, offset.axis.x);
    commandbuffer[2] = MAKE_U32(options, r);
    commandbuffer[3] = MAKE_U32(minor, major);
    commandbuffer[4] = MAKE_U32(range, val);
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_regread(uint32_t ptr, uint32_t result)
{
    uint32_t commandbuffer[3];
    commandbuffer[0] = CMD_REGREAD;
    commandbuffer[1] = ptr;
    commandbuffer[2] = 0;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_get_props(uint32_t ptr, uint32_t w, uint32_t h)
{
    uint32_t commandbuffer[4];
    commandbuffer[0] = CMD_GETPROPS;
    commandbuffer[1] = ptr;
    commandbuffer[2] = w;
    commandbuffer[3] = h;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_mem_cpy(uint32_t dest, uint32_t src, uint32_t num)
{
    uint32_t commandbuffer[4];
    commandbuffer[0] = CMD_MEMCPY;
    commandbuffer[1] = dest;
    commandbuffer[2] = src;
    commandbuffer[3] = num;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_spinner(FTOffset_t offset, uint16_t style, uint16_t scale)
{
    uint32_t commandbuffer[3];
    commandbuffer[0] = CMD_SPINNER;
    commandbuffer[1] = MAKE_U32(offset.axis.y, offset.axis.x);
    commandbuffer[2] = MAKE_U32(scale, style);
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_inflate(uint32_t ptr)
{
    uint32_t commandbuffer[2];
    commandbuffer[0] = CMD_INFLATE;
    commandbuffer[1] = ptr;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_translate(int32_t tx, int32_t ty)
{
    uint32_t commandbuffer[3];
    commandbuffer[0] = CMD_TRANSLATE;
    commandbuffer[1] = tx;
    commandbuffer[2] = ty;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_stop()
{
    uint32_t commandbuffer[1];
    commandbuffer[0] = CMD_STOP;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_touch_transform(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t tx0, int32_t ty0, int32_t tx1, int32_t ty1, int32_t tx2, int32_t ty2, uint16_t result)
{
    uint32_t commandbuffer[14];
    commandbuffer[0] = CMD_TOUCH_TRANSFORM;
    commandbuffer[1] = x0;
    commandbuffer[2] = y0;
    commandbuffer[3] = x1;
    commandbuffer[4] = y1;
    commandbuffer[5] = x2;
    commandbuffer[6] = y2;
    commandbuffer[7] = tx0;
    commandbuffer[8] = ty0;
    commandbuffer[9] = tx1;
    commandbuffer[10] = ty1;
    commandbuffer[11] = tx2;
    commandbuffer[12] = ty2;
    commandbuffer[13] = result;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_interrupt(uint32_t ms)
{
    uint32_t commandbuffer[2];
    commandbuffer[0] = CMD_INTERRUPT;
    commandbuffer[1] = ms;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_rotate(int32_t a)
{
    uint32_t commandbuffer[2];
    commandbuffer[0] = CMD_ROTATE;
    commandbuffer[1] = a;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_mem_write(uint32_t ptr, uint32_t num)
{
    uint32_t commandbuffer[3];
    commandbuffer[0] = CMD_MEMWRITE;
    commandbuffer[1] = ptr;
    commandbuffer[2] = num;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}
void ft8xx_command_scroll_bar(FTOffset_t offset, FTSize_t s, uint16_t options, uint16_t val, uint16_t size, uint16_t range)
{
    uint32_t commandbuffer[5];
    commandbuffer[0] = CMD_SCROLLBAR;
    commandbuffer[1] = MAKE_U32(offset.axis.y, offset.axis.x);
    commandbuffer[2] = MAKE_U32(s.prp.height, s.prp.width);
    commandbuffer[3] = MAKE_U32(val, options);
    commandbuffer[4] = MAKE_U32(range, size);
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_get_matrix(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f)
{
    uint32_t commandbuffer[7];
    commandbuffer[0] = CMD_GETMATRIX;
    commandbuffer[1] = a;
    commandbuffer[2] = b;
    commandbuffer[3] = c;
    commandbuffer[4] = d;
    commandbuffer[5] = e;
    commandbuffer[6] = f;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_sketch(FTOffset_t offset, FTSize_t s, uint32_t ptr, uint16_t format)
{
    uint32_t commandbuffer[5];
    commandbuffer[0] = CMD_SKETCH;
    commandbuffer[1] = MAKE_U32(offset.axis.y, offset.axis.x);
    commandbuffer[2] = MAKE_U32(s.prp.height, s.prp.width);
    commandbuffer[3] = ptr;
    commandbuffer[4] = format;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_bitmap_transform(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t tx0, int32_t ty0, int32_t tx1, int32_t ty1, int32_t tx2, int32_t ty2, uint16_t result)
{
    uint32_t commandbuffer[14];
    commandbuffer[0] = CMD_BITMAP_TRANSFORM;
    commandbuffer[1] = x0;
    commandbuffer[2] = y0;
    commandbuffer[3] = x1;
    commandbuffer[4] = y1;
    commandbuffer[5] = x2;
    commandbuffer[6] = y2;
    commandbuffer[7] = tx0;
    commandbuffer[8] = ty0;
    commandbuffer[9] = tx1;
    commandbuffer[10] = ty1;
    commandbuffer[11] = tx2;
    commandbuffer[12] = ty2;
    commandbuffer[13] = result;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}
void ft8xx_command_set_font(uint32_t font, uint32_t ptr)
{
    uint32_t commandbuffer[3];
    commandbuffer[0] = CMD_SETFONT;
    commandbuffer[1] = font;
    commandbuffer[2] = ptr;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_logo()
{
    uint32_t commandbuffer[1];
    commandbuffer[0] = CMD_LOGO;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_append(uint32_t ptr, uint32_t num)
{
    uint32_t commandbuffer[3];
    commandbuffer[0] = CMD_APPEND;
    commandbuffer[1] = ptr;
    commandbuffer[2] = num;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_mem_zero(uint32_t ptr, uint32_t num)
{
    uint32_t commandbuffer[3];
    commandbuffer[0] = CMD_MEMZERO;
    commandbuffer[1] = ptr;
    commandbuffer[2] = num;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_scale(int32_t sx, int32_t sy)
{
    uint32_t commandbuffer[] = {
        CMD_SCALE,
        sx,
        sy};
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_set_matrix()
{
    uint32_t commandbuffer[1];
    commandbuffer[0] = CMD_SETMATRIX;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_get_ptr(uint32_t result)
{
    uint32_t commandbuffer[2];
    commandbuffer[0] = CMD_GETPTR;
    commandbuffer[1] = result;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_cold_start()
{
    uint32_t commandbuffer[1];
    commandbuffer[0] = CMD_COLDSTART;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_keys(FTOffset_t offset, FTSize_t size, int16_t font, uint16_t options, const char *s)
{
    uint32_t commandbuffer[4];
    commandbuffer[0] = CMD_KEYS;
    commandbuffer[1] = MAKE_U32(offset.axis.y, offset.axis.x);
    commandbuffer[2] = MAKE_U32(size.prp.height, size.prp.width);
    commandbuffer[3] = MAKE_U32(options, font);
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    ft8xx_hal_write_string(s);
    gpu_end_transfer(len);
}

void ft8xx_command_dial(FTOffset_t offset, int16_t r, uint16_t options, uint16_t val)
{
    uint32_t commandbuffer[4];
    commandbuffer[0] = CMD_DIAL;
    commandbuffer[1] = MAKE_U32(offset.axis.y, offset.axis.x);
    commandbuffer[2] = MAKE_U32(options, r);
    commandbuffer[3] = val;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_progress(FTOffset_t offset, FTSize_t s, uint16_t options, uint16_t val, uint16_t range)
{
    uint32_t commandbuffer[5];
    uint16_t len = sizeof(commandbuffer);
    commandbuffer[0] = CMD_PROGRESS;
    commandbuffer[1] = MAKE_U32(offset.axis.y, offset.axis.x);
    commandbuffer[2] = MAKE_U32(s.prp.height, s.prp.width);
    commandbuffer[3] = MAKE_U32(val, options);
    commandbuffer[4] = range;
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_load_image(uint32_t ptr, uint32_t options)
{
    uint32_t commandbuffer[3];
    uint16_t len = sizeof(commandbuffer);
    commandbuffer[0] = CMD_LOADIMAGE;
    commandbuffer[1] = ptr;
    commandbuffer[2] = options;
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_snapshot(uint32_t ptr)
{
    uint32_t commandbuffer[2];
    commandbuffer[0] = CMD_SNAPSHOT;
    commandbuffer[1] = ptr;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_mem_crc(uint32_t ptr, uint32_t num, uint32_t result)
{
    uint32_t commandbuffer[4];
    commandbuffer[0] = CMD_MEMCRC;
    commandbuffer[1] = ptr;
    commandbuffer[2] = num;
    commandbuffer[3] = result;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_clock(FTOffset_t offset, int16_t r, uint16_t options, uint16_t h, uint16_t m, uint16_t s, uint16_t ms)
{
    uint32_t commandbuffer[5];
    commandbuffer[0] = CMD_CLOCK;
    commandbuffer[1] = MAKE_U32(offset.axis.y, offset.axis.x);
    commandbuffer[2] = MAKE_U32(options, r);
    commandbuffer[3] = MAKE_U32(m, h);
    commandbuffer[4] = MAKE_U32(ms, s);
    uint8_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_screen_saver()
{
    uint32_t commandbuffer[1];
    commandbuffer[0] = CMD_SCREENSAVER;
    uint8_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

int ft8xx_load_from_mcu_flash_to_gram_coproc(bool init, const uint8_t *buffer, const int address, uint16_t len)
{
    if (init) {
        ft8xx_command(CMD_LOADIMAGE);
        ft8xx_command(address);
        ft8xx_command(OPT_NODL);
    }
    ft8xx_command_buffer(buffer, len);
    return 1;
}

void ft8xx_coproc_update_touch_firmware(const uint8_t *touch_firmware, uint16_t len)
{
    ft8xx_command_buffer(touch_firmware, len);
    gpu_wait_for_idle();
}

#if defined(FT81x_ENABLE) || defined(BT817_ENABLE) || defined(BT815_ENABLE)
void ft8xx_command_set_rotate(uint32_t r)
{
    uint32_t commandbuffer[2];
    commandbuffer[0] = CMD_SETROTATE;
    commandbuffer[1] = r;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}
void ft8xx_command_setbitmap(uint32_t source, uint16_t fmt, FTSize_t size)
{
    uint32_t commandbuffer[] = {
        CMD_SETBITMAP,
        source,
        MAKE_U32(size.prp.width, fmt),
        size.prp.height,
    };
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_set_scratch(uint32_t handle)
{
    uint32_t commandbuffer[2];
    commandbuffer[0] = CMD_SETSCRATCH;
    commandbuffer[1] = handle;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}
void ft8xx_command_set_base(uint32_t base)
{
    uint32_t commandbuffer[2];
    commandbuffer[0] = CMD_SETBASE;
    commandbuffer[1] = base;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}
void ft8xx_command_play_video(uint32_t options)
{
    uint32_t commandbuffer[2];
    commandbuffer[0] = CMD_PLAYVIDEO;
    commandbuffer[1] = options;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}
void ft8xx_command_sync()
{
    uint32_t commandbuffer[1];
    commandbuffer[0] = CMD_SYNC;
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_snapshot2(FTOffset_t offset, FTSize_t s, uint32_t fmt, uint32_t ptr)
{
    uint32_t commandbuffer[5];
    commandbuffer[0] = CMD_SNAPSHOT2;
    commandbuffer[1] = fmt;
    commandbuffer[2] = ptr;
    commandbuffer[3] = MAKE_U32(offset.axis.y, offset.axis.x);
    commandbuffer[4] = MAKE_U32(s.prp.height, s.prp.width);
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_set_font2(uint32_t font, uint32_t ptr, uint32_t firstchar)
{
    uint32_t commandbuffer[] = {
        CMD_SETFONT2,
        font,
        ptr,
        firstchar,
    };
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_rom_font(uint32_t font, uint32_t romslot)
{
    uint32_t commandbuffer[] = {
        CMD_ROMFONT,
        font,
        romslot,
    };
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_video_start()
{
    uint32_t commandbuffer[] = {
        CMD_VIDEOSTART,
    };
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_video_frame(uint32_t dst, uint32_t ptr)
{
    uint32_t commandbuffer[] = {
        CMD_VIDEOFRAME,
        dst,
        ptr,
    };
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_mem_set(uint32_t destaddr, uint32_t value, uint32_t numbytes)
{
    uint32_t commandbuffer[] = {CMD_MEMSET, destaddr, value, numbytes};
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
    gpu_wait_for_idle();
}

#if defined(BT817_ENABLE) || defined(BT815_ENABLE)

void ft8xx_flash_detach()
{
    ft8xx_command(CMD_FLASHDETACH);
}

void ft8xx_flash_attach()
{
    ft8xx_command(CMD_FLASHATTACH);
}

void ft8xx_command_flash_fast(uint32_t result)
{
    // https://www.brtcommunity.com/index.php?topic=288.msg1365#msg1365
    // https://github.com/MatrixOrbital/EVE3-BT81x-Flash/blob/master/process.c#148
    uint32_t commandbuffer[] = {CMD_FLASHFAST, result};
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_flash_read(uint32_t dest, uint32_t src, uint32_t numbytes)
{
    uint32_t commandbuffer[] = {CMD_FLASHREAD, dest, src, numbytes /* & ~4 */};
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
    gpu_wait_for_idle();
}

void ft8xx_command_flash_update(uint32_t dest, uint32_t src, uint32_t numbytes)
{
    uint32_t commandbuffer[] = {CMD_FLASHUPDATE, dest, src, numbytes /* & ~4 */};
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
    gpu_wait_for_idle();
}

void ft8xx_command_flash_eraseall()
{
    ft8xx_command(CMD_FLASHERASE);
    gpu_wait_for_idle();
}

void ft8xx_command_media_fifo(uint32_t ptr, uint32_t size)
{
    uint32_t commandbuffer[] = {
        CMD_MEDIAFIFO,
        ptr,
        size,
    };
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_animframe(FTOffset_t offset, uint32_t aoptr, uint32_t frame)
{
    uint32_t commandbuffer[] = {
        CMD_ANIMFRAME,
        MAKE_U32(offset.axis.y, offset.axis.x),
        aoptr,
        frame,
    };
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

void ft8xx_command_flashsource(uint32_t ptr)
{
    uint32_t commandbuffer[] = {
        CMD_FLASHSOURCE,
        ptr,
    };
    uint16_t len = sizeof(commandbuffer);
    gpu_start_transfer(len);
    gpu_transfer((uint8_t *)commandbuffer, len);
    gpu_end_transfer(len);
}

int ft8xx_flash_switch_to_full_mode()
{
    uint8_t val;
    /* Try detaching and attaching the flash followed by fast mdoe */
    // printf("%s(): sending detach_flash...\n", __func__);
    ft8xx_flash_detach();
    gpu_wait_for_idle();
    // sleep_ms(100);
    val = ft8xx_hal_rd8(REG_FLASH_STATUS);
    if (FLASH_STATUS_DETACHED != val) {
        printf("Error, Flash is unable to detach %d\n", val);
        return 0;
    }

    // printf("%s(): sending attach_flash...\n", __func__);
    ft8xx_flash_attach();
    gpu_wait_for_idle();
    // sleep_ms(100);
    val = ft8xx_hal_rd8(REG_FLASH_STATUS);
    if (FLASH_STATUS_BASIC != val) {
        printf("Error, Flash is unable to re-attach %d\n", val);
        return 0;
    }

    // printf("%s(): setting fast_flash...\n", __func__);
    ft8xx_command_flash_fast(0);
    gpu_wait_for_idle();
    // sleep_ms(100);/*  */
    val = ft8xx_hal_rd8(REG_FLASH_STATUS);
    if (FLASH_STATUS_FULL != val) {
        printf("Error, Flash is not able to get into full mode %d\n", val);
        return 0;
    }
    return 1;
}

#endif /* #if defined(BT817_ENABLE) || defined(BT815_ENABLE) */

#endif /*  #if defined(FT81x_ENABLE) || defined(BT817_ENABLE) || defined(BT815_ENABLE) */
