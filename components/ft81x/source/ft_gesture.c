/*
 * ft_gesture.c
 *
 *  Created on: 15 Sep 2020
 *      Author: prabhakaran.d
 *  Modified on: 2022
 *      Author: Thinh Do
 */
#include "ft_gesture.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define max(a, b)	  (((a) > (b)) ? (a) : (b))
#define min(a, b)	  (((a) < (b)) ? (a) : (b))
#define VELOCITY_CTRL (24)

/* @Thinh:
 * touch_ptr->scroller.base.touch.y
 * touch_ptr->scroller.base.touch.x
 * This value is got from experiment
 */
#define SCROLLER_VERTICAL_BASE_MAX	 (7680)
#define SCROLLER_HORIZONTAL_BASE_MAX (7680)

#define LONGTAP_CONFIRM_MSEC (150)
#define TAP_CONFIRM_MSEC	 (100)

FTGLSettings_t gl_settings;
FTGLTouch_t	   touch = {0};
FTGLTouch_t	  *touch_ptr;

int16_t		vertical_scroller_base_max = 0;
static bool FTGL_GetSwipeDetectByTouchType(FTGLTouchType_e touch_type);

uint32_t gesture_get_time(void)
{
    return pdTICKS_TO_MS(xTaskGetTickCount());
}

bool gesture_timer_is_expired(uint32_t start, uint32_t period)
{
    return (((gesture_get_time() - start) & UINT_MAX) > period);
}

FTGLStatus_e FTGL_Init(FTGLSettings_t s)
{
    int i = 0;
    gl_settings.number_of_touch_points = s.number_of_touch_points;
    gl_settings.scroller.start.xy = s.scroller.start.xy;
    gl_settings.scroller.end.xy = s.scroller.end.xy;
    gl_settings.drag.enable_horizontal = s.drag.enable_horizontal;
    gl_settings.drag.enable_vertical = s.drag.enable_vertical;

    touch_ptr = &touch;
    for (i = 0; i < 5; i++) {
        touch_ptr->current_touch[i].xy = 0x80008000;
        touch_ptr->previous_touch[i].xy = 0x80008000;
    }
    touch_ptr->touch_type = ftgl_touch_type_none;
    touch_ptr->pendown = 0;
    touch_ptr->penup = 0;
    return ftgl_ok;
}

static void update_previous_touch_coordinates()
{
    int i = 0;
    for (i = 0; i < gl_settings.number_of_touch_points; i++) {
        if (touch_ptr->current_touch[i].xy != 0x80008000)
            touch_ptr->previous_touch[i].xy = touch_ptr->current_touch[i].xy;
    }
    touch_ptr->number_of_pervious_touch = touch_ptr->number_of_current_touch;
}

static void update_current_touch_coordinates()
{
    int i;
    touch_ptr->number_of_current_touch = 0;
    for (i = 0; i < gl_settings.number_of_touch_points; i++) {
        touch_ptr->current_touch[i].xy = (uint32_t)gesture_touch(i).value;
        touch_ptr->number_of_current_touch += (touch_ptr->current_touch[i].xy != 0x80008000);
    }
    touch_ptr->penup = ((touch_ptr->number_of_pervious_touch != 0) && (touch_ptr->number_of_current_touch == 0)) ||
                       ((touch_ptr->number_of_pervious_touch == 0) && (touch_ptr->number_of_current_touch == 0));

    if (!(touch_ptr->penup | touch_ptr->pendown)) {
        touch_ptr->pendown = 1;
        touch_ptr->pendown_time = gesture_get_time();
        touch_ptr->penup_down_interval = touch_ptr->penup_time;
        touch_ptr->penup_time = 0;
    }
    else if (touch_ptr->penup) {
        if (touch_ptr->pendown) {
            touch_ptr->pendown = 0;
            touch_ptr->penup_time = gesture_get_time();
        }
    }
}

static void get_touch_type()
{

    if (touch_ptr->touch_type == ftgl_touch_type_none) {
        switch (touch_ptr->tap_counts) {
        case 1:
            touch_ptr->touch_type = ftgl_single_tap;
            break;

        case 2:
            touch_ptr->touch_type = ftgl_double_tap;
            break;
        }
    }
    touch_ptr->tap_counts = 0;
    touch_ptr->delta_x = 0;
    touch_ptr->delta_y = 0;
}

static void detect_tap()
{
    if (touch_ptr->touch_type != ftgl_touch_type_none)
        return;
    if (touch_ptr->pendown) {
        if (gesture_timer_is_expired(touch_ptr->pendown_time, LONGTAP_CONFIRM_MSEC)) {
            touch_ptr->touch_type = ftgl_long_tap;
            touch_ptr->ignore_touch = true;
            return;
        }
        if (touch_ptr->penup_down_interval != 0) {
            touch_ptr->penup_down_interval = 0;
            touch_ptr->tap_counts++;
        }
    }
    else {
        if (touch_ptr->penup && gesture_timer_is_expired(touch_ptr->penup_time, TAP_CONFIRM_MSEC)) {
            get_touch_type();
            return;
        }
    }
}

static void detect_swipe()
{
    static FTGLTouchCoordinates_t start = {.xy = 0x80008000};
    if (touch_ptr->pendown) {
        if (start.xy == 0x80008000) {
            start.xy = touch_ptr->current_touch[0].xy;
            touch_ptr->delta_x = 0;
            touch_ptr->delta_y = 0;
        }
    }
    else {
        if (touch_ptr->penup) {
            if (gesture_timer_is_expired(touch_ptr->penup_time, 20)) {
                if (start.xy != 0x80008000) {
                    touch_ptr->delta_x = touch_ptr->previous_touch[0].touch.x - start.touch.x;
                    touch_ptr->delta_y = touch_ptr->previous_touch[0].touch.y - start.touch.y;
                    if (touch_ptr->delta_y < -10) {
                        touch_ptr->touch_type = ftgl_swipe_left;
                    }
                    else if (touch_ptr->delta_y > 10) {
                        touch_ptr->touch_type = ftgl_swipe_right;
                    }
                    start.xy = 0x80008000;
                }
                return;
            }
        }
    }
}

void scroller(uint8_t touch_type)
{
    int16_t change = 0;
    if (touch_ptr->scroller.mode == horizontal_mode) {
        touch_ptr->touch_type = touch_type;
        change = max(1, abs(touch_ptr->scroller.vel.touch.x) >> 6);
        if (touch_ptr->scroller.vel.touch.x < 0)
            touch_ptr->scroller.vel.touch.x += change;
        else
            touch_ptr->scroller.vel.touch.x -= change;

        if (touch_ptr->scroller.vel.touch.x > 0)
            touch_ptr->scroller.base.touch.x += min(VELOCITY_CTRL, touch_ptr->scroller.vel.touch.x);
        if (touch_ptr->scroller.vel.touch.x < 0)
            touch_ptr->scroller.base.touch.x += max(-VELOCITY_CTRL, touch_ptr->scroller.vel.touch.x);
        touch_ptr->scroller.base.touch.x = max(gl_settings.scroller.start.touch.x, min(touch_ptr->scroller.base.touch.x, gl_settings.scroller.end.touch.x));

        if (touch_ptr->scroller.vel.touch.x == 0 || touch_ptr->scroller.base.touch.x <= gl_settings.scroller.start.touch.x || touch_ptr->scroller.base.touch.x >= gl_settings.scroller.end.touch.x) {
            touch_ptr->scroller.mode = idle_mode;
            touch_type = ftgl_touch_type_none;
        }
        else {
            touch_ptr->tap_counts = 0;
        }
    }
    else if (touch_ptr->scroller.mode == vertical_mode) {
        touch_ptr->touch_type = touch_type;
        change = max(1, abs(touch_ptr->scroller.vel.touch.y) >> 6);
        if (touch_ptr->scroller.vel.touch.y < 0)
            touch_ptr->scroller.vel.touch.y += change;
        else
            touch_ptr->scroller.vel.touch.y -= change;

        if (touch_ptr->scroller.vel.touch.y > 0)
            touch_ptr->scroller.base.touch.y += min(VELOCITY_CTRL, touch_ptr->scroller.vel.touch.y);
        if (touch_ptr->scroller.vel.touch.y < 0)
            touch_ptr->scroller.base.touch.y += max(-VELOCITY_CTRL, touch_ptr->scroller.vel.touch.y);

        touch_ptr->scroller.base.touch.y = max(gl_settings.scroller.start.touch.y, min(touch_ptr->scroller.base.touch.y, gl_settings.scroller.end.touch.y));

        if (touch_ptr->scroller.vel.touch.y == 0 || touch_ptr->scroller.base.touch.y <= gl_settings.scroller.start.touch.y || touch_ptr->scroller.base.touch.y >= gl_settings.scroller.end.touch.y) {
            touch_ptr->scroller.mode = idle_mode;
            touch_type = ftgl_touch_type_none;
        }
        else {
            touch_ptr->tap_counts = 0;
        }
    }
}

int16_t detect_swipe_scroller()
{
    static FTGLTouchCoordinates_t sPoint = {.xy = 0};
    static FTGLTouchType_e		  touch_type = ftgl_touch_type_none;
    if (touch_ptr->number_of_current_touch) {
        if (sPoint.xy) {
            int16_t	 xDiff = sPoint.touch.x - touch_ptr->current_touch[0].touch.x;
            int16_t	 yDiff = sPoint.touch.y - touch_ptr->current_touch[0].touch.y;
            uint16_t absoulteXDiff = abs((int16_t)xDiff);
            uint16_t absoulteYDiff = abs((int16_t)yDiff);
            if (absoulteYDiff > absoulteXDiff) {
                if (yDiff > 0) {
                    touch_type = ftgl_scroll_up;
                }
                else if (yDiff < 0) {
                    touch_type = ftgl_scroll_down;
                }
                touch_ptr->scroller.vel.touch.y = yDiff;
                touch_ptr->scroller.mode = vertical_mode;
                if (gl_settings.drag.enable_vertical) {
                    touch_ptr->scroller.base.touch.y += yDiff;
                    touch_ptr->scroller.base.touch.y = max(gl_settings.scroller.start.touch.y, min(touch_ptr->scroller.base.touch.y, gl_settings.scroller.end.touch.y));
                    touch_ptr->touch_type = touch_type;
                }
            }
            else if (absoulteYDiff < absoulteXDiff) {

                if (xDiff > 0) {
                    touch_type = ftgl_scroll_left;
                }
                else if (xDiff < 0) {
                    touch_type = ftgl_scroll_right;
                }
                touch_ptr->scroller.vel.touch.x = xDiff;
                touch_ptr->scroller.mode = horizontal_mode;
                if (gl_settings.drag.enable_horizontal) {
                    touch_ptr->scroller.base.touch.x += xDiff;
                    touch_ptr->scroller.base.touch.x = max(gl_settings.scroller.start.touch.x, min(touch_ptr->scroller.base.touch.x, gl_settings.scroller.end.touch.x));
                    touch_ptr->touch_type = touch_type;
                }
            }
        }
        sPoint.xy = touch_ptr->current_touch[0].xy;
    }
    else {
        sPoint.xy = 0;
        scroller(touch_type);
    }
    return 0;
}

void FTGL_AnimateScroller(FTGLScrollerMode_e mode, uint16_t src, uint16_t dst)
{
    if (mode == horizontal_mode)
        touch_ptr->scroller.vel.touch.x = src - dst;
    else if (mode == vertical_mode)
        touch_ptr->scroller.vel.touch.y = src - dst;
    touch_ptr->scroller.mode = mode;
}

FTGLTouchType_e FTGL_Run()
{
    touch_ptr->touch_type = ftgl_touch_type_none;
    update_previous_touch_coordinates();
    update_current_touch_coordinates();
    detect_tap();
    detect_swipe_scroller();
    if (touch_ptr->penup && touch_ptr->ignore_touch) {
        if (touch_ptr->touch_type != ftgl_touch_type_none) {
            touch_ptr->ignore_touch = false;
            touch_ptr->touch_type = ftgl_touch_type_none;
        }
    }
    return touch_ptr->touch_type;
}

FTGLScrollerInfo_t FTGL_GetScrollerInfo()
{
    if (vertical_scroller_base_max == 0)
        vertical_scroller_base_max = SCROLLER_VERTICAL_BASE_MAX;

    if (touch_ptr->scroller.base.touch.y >= vertical_scroller_base_max)
        touch_ptr->scroller.base.touch.y = vertical_scroller_base_max;

    return touch_ptr->scroller;
}

/**
 * @brief Set the maximum of y offset scroller.
 *        Caller need to set y_offset_max = 0
 *        to update y offset to be default.
 *        Basically, every drawing scroller
 *        will need to set this max value for
 *        scroller works properly.
 *
 * @param val value to be updated
 */

void FTGL_ScrollerSetVerticalBaseMax(int16_t val)
{
    if (val == 0)
        vertical_scroller_base_max = SCROLLER_VERTICAL_BASE_MAX;

    vertical_scroller_base_max = val;
}

void FTGL_ScrollerSetCurrVerticalBaseOffset(int16_t val)
{
    touch_ptr->scroller.base.touch.y = val;
}

static bool FTGL_GetSwipeDetectByTouchType(FTGLTouchType_e touch_type)
{
    static uint8_t prev_touch_type = ftgl_touch_type_none;
    uint8_t		   curr_mode = (uint8_t)touch_ptr->scroller.mode;
    if ((touch_type != ftgl_scroll_left) &&
        (touch_type != ftgl_scroll_right) &&
        (touch_type != ftgl_scroll_up) &&
        (touch_type != ftgl_scroll_down)) {
        return false;
    }

    if (curr_mode != idle_mode && touch_ptr->touch_type == touch_type) {
        prev_touch_type = (uint8_t)touch_ptr->touch_type;
        return false;
    }

    if (curr_mode == idle_mode && prev_touch_type == touch_type) {
        prev_touch_type = ftgl_touch_type_none;
        return true;
    }

    return false;
}

bool FTGL_LeftSwipeDetected()
{
    return FTGL_GetSwipeDetectByTouchType(ftgl_scroll_left);
}

bool FTGL_RightSwipeDetected()
{
    return FTGL_GetSwipeDetectByTouchType(ftgl_scroll_right);
}
