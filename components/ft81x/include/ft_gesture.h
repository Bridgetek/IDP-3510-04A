/*
 * ft_gesture.h
 *
 *  Created on: 15 Sep 2020
 *      Author: prabhakaran.d
 */

#ifndef INCLUDES_EVE_FT_GESTURE_H_
#define INCLUDES_EVE_FT_GESTURE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ft8xx.h"

#define gesture_touch(t) ft8xx_get_touch(t)

typedef enum {
    ftgl_ok,
    ftgl_setparam_error,
    ftgl_mode_error,
    ftgl_touch_point_change,
    ftgl_tap_change,
    ftgl_swipe_change,
    ftgl_scroll_change,
    ftgl_transform_change,
    ftgl_warning = 0xfe,
    ftgl_error = 0xff,
} FTGLStatus_e;

typedef enum {
    ftgl_tpoints_none,
    ftgl_tpoints_one,
    ftgl_tpoints_two,
    ftgl_tpoints_three,
    ftgl_tpoints_FOUR,
    ftgl_tpoints_five,
    ftgl_tpoints_max = 5,
} FTGLTouchPoints_e;

typedef enum {
    ftgl_touch_type_none,
    ftgl_swipe_up,
    ftgl_swipe_down,
    ftgl_swipe_left,
    ftgl_swipe_right,
    ftgl_scroll_up,
    ftgl_scroll_down,
    ftgl_scroll_left,
    ftgl_scroll_right,
    ftgl_single_tap,
    ftgl_double_tap,
    ftgl_long_tap,
} FTGLTouchType_e;

typedef enum {
    ftgl_scroller_none,
    ftgl_scroller_horizontal,
    ftgl_scroller_vertical,
} FTGLScrollMode_e;

typedef union {
    int32_t xy;
    struct {
        int16_t x;
        int16_t y;
    } touch;
} FTGLTouchCoordinates_t;

typedef struct {
    FTGLTouchCoordinates_t start;
    FTGLTouchCoordinates_t end;
} FTGLScrollerSettings_t;

typedef struct {
    uint8_t enable_vertical;
    uint8_t enable_horizontal;
} FTGLDrag_t;

typedef struct {
    FTGLTouchPoints_e	   number_of_touch_points;
    FTGLScrollerSettings_t scroller;
    FTGLDrag_t			   drag;
} FTGLSettings_t;

typedef enum {
    idle_mode,
    horizontal_mode,
    vertical_mode,
    auto_mode
} FTGLScrollerMode_e;

typedef struct {
    FTGLTouchCoordinates_t vel;
    FTGLTouchCoordinates_t base;
    FTGLScrollerMode_e	   mode;
} FTGLScrollerInfo_t;

typedef struct {
    FTGLTouchCoordinates_t current_touch[5];
    FTGLTouchCoordinates_t previous_touch[5];
    uint8_t				   penup;
    uint8_t				   pendown;
    uint8_t				   number_of_pervious_touch;
    uint8_t				   number_of_current_touch;
    uint32_t			   pendown_time;
    uint32_t			   penup_time;
    uint32_t			   penup_down_interval;
    uint8_t				   tap_counts;
    int16_t				   delta_x;
    int16_t				   delta_y;
    uint8_t				   ignore_touch;
    FTGLScrollerInfo_t	   scroller;
    FTGLTouchType_e		   touch_type;

} FTGLTouch_t;
uint32_t			gesture_get_time(void);
bool				gesture_timer_is_expired(uint32_t start, uint32_t period);
FTGLStatus_e		FTGL_Init(FTGLSettings_t s);
FTGLTouchType_e		FTGL_Run();
FTGLScrollerInfo_t	FTGL_GetScrollerInfo();
void				FTGL_AnimateScroller(FTGLScrollerMode_e mode, uint16_t src, uint16_t dst);
void				FTGL_ScrollerSetVerticalBaseMax(int16_t val);
void				FTGL_ScrollerSetCurrVerticalBaseOffset(int16_t val);
bool 				FTGL_LeftSwipeDetected();
bool 				FTGL_RightSwipeDetected();

#endif /* INCLUDES_EVE_FT_GESTURE_H_ */
