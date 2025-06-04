/*
 * FT8xxHal.h
 *
 *  Created on: 10 Mar 2017
 *      Author: prabhakaran.d
 */

#ifndef FT8XXHAL_H_
#define FT8XXHAL_H_

#include <stdint.h>
#include <string.h>
#include "esp_err.h"

typedef enum {
    WRITE,
    READ,
} TRANSFER;

esp_err_t ft8xx_hal_init(void);
void ft8xx_hal_write_string(const char *str);
void ft8xx_hal_wr32(uint32_t address, uint32_t buffer);
void ft8xx_hal_wr16(uint32_t address, uint16_t buffer);
void ft8xx_hal_wr8s(uint32_t address, const uint8_t *buffer, const uint32_t len);
void ft8xx_hal_wr8(uint32_t address, uint8_t buffer);
void ft8xx_hal_rd8s(uint32_t address, uint8_t *buffer, uint16_t size);
uint32_t ft8xx_hal_rd32(uint32_t address);
uint16_t ft8xx_hal_rd16(uint32_t address);
uint8_t ft8xx_hal_rd8(uint32_t address);
void ft8xx_spi_channel(uint8_t cs);
void ft8xx_hal_host_command(uint32_t cmd);
void ft8xx_power();
void ft8xx_hal_set_dummies(uint8_t d);
void ft8xx_hal_coproc_start_transfer(uint32_t address);
void ft8xx_hal_coproc_transfer(uint8_t *buffer, uint16_t len);
void ft8xx_hal_coproc_command(uint32_t cmd);
void ft8xx_hal_coproc_end_transfer(void);
void ft8xx_hal_coproc_transfer_string(const char *string);
#endif /* FT8XXHAL_H_ */
