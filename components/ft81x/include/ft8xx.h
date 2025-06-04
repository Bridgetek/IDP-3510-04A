/*
 * ft8xx.h
 *
 *  Created on: 16 Sep 2020
 *      Author: prabhakaran.d
 */

#ifndef _FT8XX_H_
#define _FT8XX_H_

#include <stdint.h>
#include <stdbool.h>

typedef union {
    uint32_t value;
    struct {
        uint16_t x;
        uint16_t y;
    } axis;
} FTTouch_t;

typedef union {
    uint32_t value;
    struct {
        uint8_t tag;
        uint8_t reserved;
        uint16_t range;
    } tracker;
} FTTracker_t;

typedef union {
    uint32_t value;
    struct {
        uint8_t alpha;
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    } rgba;
} FTColor_t;

typedef union {
    int32_t xy;
    struct {
        int16_t x;
        int16_t y;
    } axis;
} FTOffset_t;

typedef union {
    uint32_t size;
    struct {
        uint16_t width;
        uint16_t height;
    } prp;
} FTSize_t;

// Specially loaded bitmap formats
#define JPEG 0x91
#define PNG 0x92

typedef struct ft8xx_bitmap_info
{
    int32_t width;
    int32_t height;
    int32_t stride;
    int32_t format;
    int32_t sub_format;
    int32_t size;
    const char *file_path;
    bool loaded_to_ram;
    bool compressed;
    uint32_t addr;
} ft8xx_bitmap_info;

typedef struct {
    uint16_t Width;
    uint16_t Height;
    uint16_t HCycle;
    uint16_t HOffset;
    uint16_t HSync0;
    uint16_t HSync1;
    uint16_t VCycle;
    uint16_t VOffset;
    uint16_t VSync0;
    uint16_t VSync1;
    uint16_t PCLK;
    uint16_t Swizzle;
    uint16_t PCLKPol;
    uint16_t CSpread;
    uint16_t Dither;
} FTDisplayPrp_t;

int ft8xx_init();
void ft8xx_active_mode();
void ft8xx_sleep_mode();
void ft8xx_audio_mute();
void ft8xx_stop_sound();
void ft8xx_play_synth_sound(uint8_t voice, uint8_t midi_note, uint8_t volume);
void ft8xx_set_synth_sound_volume(uint8_t volume);
void ft8xx_set_playback_volume(uint8_t volume);
void ft8xx_display_off(void);
void ft8xx_set_brightness(uint8_t brightness);
void ft8xx_set_pwm(uint8_t pwm_duty);
void ft8xx_set_pwmhz(uint16_t pwm_hz);
void ft8xx_write_gram(uint32_t address, uint8_t *data, uint32_t len);
void ft8xx_read_gram(uint32_t address, uint8_t *buffer, uint16_t len);
uint8_t ft8xx_get_tag();
uint8_t ft8xx_read_tag();
uint32_t ft8xx_read_tracker();
FTTouch_t ft8xx_get_touch(uint8_t touchno);
FTTracker_t ft8xx_get_tracker(uint8_t tracker);

#endif /* INCLUDES_EVE_FT8XX_H_ */
