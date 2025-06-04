/*
 * bsp_button.h
 * 
 * Created on: 03 Mar 2025
 *     Author: Vo Van Buong (BRT-SG)
 */

#ifndef BSP_BUTTON_HDR_H_
#define BSP_BUTTON_HDR_H_

#include <stdint.h>
#include "bsp_board.h"

void bsp_button_init(void);
void bsp_button_set_trigger_time(uint32_t press_time, uint32_t long_press_time);

void bsp_button_clear_short_press_flag(void);
void bsp_button_clear_long_press_flag(void);
uint8_t bsp_button_read_short_press_flag(void);
uint8_t bsp_button_read_long_press_flag(void);


#endif /* BSP_BUTTON_HDR_H_ */
