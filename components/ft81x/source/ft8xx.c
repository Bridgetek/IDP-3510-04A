/*
 * ft8xx.c
 *
 *  Created on: 16 Sep 2020
 *      Author: prabhakaran.d
 */
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "bsp_board.h"
#include "ft8xx_gpu.h"
#include "ft8xx_hal.h"
#include "ft8xx_cmd.h"

#define FT8xx_ACTIVE	0x00
#define FT8xx_INTOSC	0x48
#define FT8xx_EXOSC		0x44
#define FT8xx_SLEEP		0x41
#define FT8xx_CLOCKSEL	0x62

int ft8xx_init()
{
    int		retries = 0;
    uint8_t reg_id = 0;
    uint8_t bootupscreen[] = {
        0, 0, 0, 2,	 // GPU instruction CLEAR_COLOR_RGB
        7, 0, 0, 38, // GPU instruction CLEAR
        0, 0, 0, 0,	 // GPU instruction DISPLAY
    };

    ft8xx_power();
    ft8xx_hal_set_dummies(1);
    // Select external 12MHz clock, the system clock will be 60MHz as default
    ft8xx_hal_host_command(FT8xx_EXOSC);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    // Second byte of Clock Select command will keep system clock at 60MHz
    ft8xx_hal_host_command(FT8xx_CLOCKSEL);
    ft8xx_hal_host_command(FT8xx_ACTIVE);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    while ((reg_id = ft8xx_hal_rd8(REG_ID)) != 0x7c && (retries < 20)) {
        retries++;
        printf("%x.", reg_id);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    if (retries >= 20) {
        printf("\n%s(): ft8xx_rd(REG_ID) failed with %d retries\n", __func__,
               retries);
        return -1;
    }

    ft8xx_hal_wr16(REG_SOUND, 0x60); // configure silence as sound to be played
    ft8xx_hal_wr8(REG_PLAY, 1);		 // play sound
    ft8xx_hal_wr16(REG_HCYCLE, LCD_HCYCLE);
    ft8xx_hal_wr16(REG_HOFFSET, LCD_HOFFSET);
    ft8xx_hal_wr16(REG_HSYNC0, LCD_HSYNC0);
    ft8xx_hal_wr16(REG_HSYNC1, LCD_HSYNC1);
    ft8xx_hal_wr16(REG_VCYCLE, LCD_VCYCLE);
    ft8xx_hal_wr16(REG_VOFFSET, LCD_VOFFSET);
    ft8xx_hal_wr16(REG_VSYNC0, LCD_VSYNC0);
    ft8xx_hal_wr16(REG_VSYNC1, LCD_VSYNC1);
    ft8xx_hal_wr16(REG_HSIZE, LCD_WIDTH);
    ft8xx_hal_wr16(REG_VSIZE, LCD_HEIGHT);
    ft8xx_hal_wr8(REG_SWIZZLE, LCD_SWIZZLE);
    ft8xx_hal_wr8(REG_PCLK_POL, LCD_PCLK_POL);    
    ft8xx_hal_wr16(REG_CSPREAD, LCD_CSPREAD);
    ft8xx_hal_wr16(REG_DITHER, LCD_DITHER);

    // all pins are configured as output
    ft8xx_hal_wr16(REG_GPIOX_DIR, 0xffff);
    // Bit 15 for display control pin, is set to 1 - enable
    // Bit 12 for output drive current of RGB signal pin. 0 - 5mA, 1 - 10mA
    // Other pins are set to maximum current, 20mA
    ft8xx_hal_wr16(REG_GPIOX, 0xefff);

    // Testing for screen-tearing
    // ft8xx_hal_wr8(REG_ADAPTIVE_FRAMERATE, 0);
    ft8xx_hal_wr8s(RAM_DL, bootupscreen, sizeof(bootupscreen));
    ft8xx_hal_wr8(REG_DLSWAP, DLSWAP_FRAME);
    // Set PCLK for LCD is 10MHz
    ft8xx_hal_wr8(REG_PCLK, 6); // after this display is visible on the LCD

    // Set LCD brightness to 20, maximum is 128
    ft8xx_set_pwm(20);

    return 0;
}

void ft8xx_set_playback_volume(uint8_t volume)
{
    ft8xx_hal_wr8(REG_VOL_PB, volume);
}

void ft8xx_set_synth_sound_volume(uint8_t volume)
{
    ft8xx_hal_wr8(REG_VOL_SOUND, volume);
}

void ft8xx_play_synth_sound(uint8_t voice, uint8_t midi_note, uint8_t volume)
{
    ft8xx_hal_wr8(REG_VOL_SOUND, volume);
    ft8xx_hal_wr16(REG_SOUND, (midi_note << 8) | voice);
    ft8xx_hal_wr8(REG_PLAY, 1);
}

void ft8xx_audio_mute()
{
    // BRT_AN_AN033 BT81X Prog Guide pg.20 "Code snippet 4 - Avoid Pop Sound"
    // Stops the AUDIO_L PWM and puts the signal pin to zero.
    ft8xx_play_synth_sound(0x60, 00, 00);
}

void ft8xx_stop_sound()
{
    ft8xx_hal_wr16(REG_SOUND, 0); // C8 MIDI note on xylophone
    ft8xx_hal_wr8(REG_PLAY, 1);	  // play the sound
}

void ft8xx_sleep_mode()
{
    ft8xx_hal_host_command(FT8xx_SLEEP);
}
void ft8xx_active_mode()
{
    ft8xx_hal_host_command(FT8xx_ACTIVE);
}

void ft8xx_display_off(void)
{
    FTColor_t color = {.value = 0};
    ft8xx_command_dl_start(color);
    ft8xx_swap(1);
    ft8xx_hal_wr8(REG_PWM_DUTY, 0);
}

void ft8xx_set_brightness(uint8_t brightness)
{
    // DEPRECATING, because of many confusion of FP factor
    printf("%s(): THIS FUNCTION IS DEPRECATED, use ft8xx_set_pwm() instead.\n",
           __func__);
    ft8xx_hal_wr8(REG_PWM_DUTY, brightness * 1.28);
}

void ft8xx_set_pwmhz(uint16_t pwm_hz)
{
    ft8xx_hal_wr16(REG_PWM_HZ, pwm_hz);
}

void ft8xx_set_pwm(uint8_t pwm_duty)
{
    ft8xx_hal_wr8(REG_PWM_DUTY, pwm_duty);
}

void ft8xx_write_gram(uint32_t address, uint8_t *data, uint32_t len)
{
    ft8xx_hal_wr8s(address, data, len);
}

void ft8xx_read_gram(uint32_t address, uint8_t *buffer, uint16_t len)
{
    ft8xx_hal_rd8s(address, buffer, len);
}

uint8_t ft8xx_get_tag()
{
    return ft8xx_hal_rd8(REG_TOUCH_TAG);
}

uint8_t ft8xx_read_tag()
{
    return ft8xx_hal_rd8(REG_TOUCH_TAG);
}

uint32_t ft8xx_read_tracker()
{
    return ft8xx_hal_rd32(REG_TRACKER);
}

FTTouch_t ft8xx_get_touch(uint8_t touchno)
{
    FTTouch_t t;
    uint32_t  tVal = 0;
    t.value = 0x80008000;
    switch (touchno) {
    case 0:
        tVal = ft8xx_hal_rd32(REG_CTOUCH_TOUCH0_XY);
        break;

    case 1:
        tVal = ft8xx_hal_rd32(REG_CTOUCH_TOUCH1_XY);
        break;

    case 2:
        tVal = ft8xx_hal_rd32(REG_CTOUCH_TOUCH2_XY);
        break;

    case 3:
        tVal = ft8xx_hal_rd32(REG_CTOUCH_TOUCH3_XY);
        break;

    case 4:
        tVal = (ft8xx_hal_rd16(REG_CTOUCH_TOUCH4_X) << 16) | ft8xx_hal_rd16(REG_CTOUCH_TOUCH4_Y);
        break;
    }
    t.axis.x = tVal >> 16;
    t.axis.y = tVal & 0xffff;
    return t;
}

FTTracker_t ft8xx_get_tracker(uint8_t tracker)
{

    FTTracker_t t;
    t.value = 0;
    switch (tracker) {
    case 0:
        t.value = ft8xx_hal_rd32(REG_TRACKER);
        break;
#ifdef FT81x_ENABLE
    case 1:
        t.value = ft8xx_hal_rd32(REG_TRACKER_1);
        break;

    case 2:
        t.value = ft8xx_hal_rd32(REG_TRACKER_2);
        break;

    case 3:
        t.value = ft8xx_hal_rd32(REG_TRACKER_3);
        break;

    case 4:
        t.value = ft8xx_hal_rd32(REG_TRACKER_4);
        break;
#endif
    }
    return t;
}
