/*
 * bsp_display.c
 * 
 * Created on: 26 Feb 2025
 *     Author: Vo Van Buong (BRT-SG)
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "bsp_display.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_littlefs.h"

#define BLOCK_LEN   512

static const char *TAG = "BSP_LCD";

static uint8_t g_littlefs_initialized = 0;

esp_err_t bsp_display_init_fs(void)
{
    ESP_LOGI(TAG, "Initializing LittleFS");

    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/littlefs",
        .partition_label = "storage",
        .format_if_mount_failed = true,
        .dont_mount = false,
    };

    esp_err_t ret = esp_vfs_littlefs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE(TAG, "Failed to find LittleFS partition");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize LittleFS (%s)", esp_err_to_name(ret));
        }
        return ret;
    }
    g_littlefs_initialized = 1;

    size_t total = 0;
    size_t used = 0;
    ret = esp_littlefs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get LittleFS partition information (%s)", esp_err_to_name(ret));
        esp_littlefs_format(conf.partition_label);
    }
    else
    {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }
    return ret;
}

esp_err_t bsp_display_init(void)
{
    esp_err_t ret = ESP_OK;

    ret = ft8xx_hal_init();
    
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Initialize HAL driver failed");
    }

    ret = ft8xx_init();

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Initialize LCD FT8xx driver failed");
    }

    ret = bsp_display_init_fs();

    return ret;
}

void bsp_display_ui_start(uint32_t rgba)
{
    FTColor_t bgcolor = {.value = rgba};
	ft8xx_command_dl_start(bgcolor);
}

void bsp_display_ui_end(bool wait)
{
	ft8xx_swap(wait);
}

void bsp_display_ui_store_context(void)
{
	ft8xx_command(SAVE_CONTEXT());
}

void bsp_display_ui_restore_context(void)
{
	ft8xx_command(RESTORE_CONTEXT());
}

void bsp_display_ui_draw_rect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t rgba)
{
	ft8xx_command(SAVE_CONTEXT());
	ft8xx_command(TAG_MASK(0xff));
	ft8xx_command(BEGIN(RECTS));
	ft8xx_command(COLOR_A(rgba & 0xff));
	ft8xx_command(COLOR_RGB(rgba >> 24, rgba >> 16, rgba >> 8));
	ft8xx_command(VERTEX2F(x * 16, y * 16));
	ft8xx_command(VERTEX2F((x + width) * 16, (y + height) * 16));
	ft8xx_command(END());
	ft8xx_command(TAG_MASK(0));
	ft8xx_command(RESTORE_CONTEXT());
}

void bsp_display_ui_draw_text(int16_t x, int16_t y, uint32_t rgba, int16_t font, uint16_t options, const char *s)
{
	ft8xx_command(SAVE_CONTEXT());
	ft8xx_command(TAG_MASK(0xff));
	ft8xx_command(COLOR_A(rgba & 0xff));
	ft8xx_command(COLOR_RGB(rgba >> 24, rgba >> 16, rgba >> 8));
    ft8xx_cmd_text_c(x, y, font, options, s);
    ft8xx_command(TAG_MASK(0));
    ft8xx_command(RESTORE_CONTEXT());
}

esp_err_t bsp_display_load_draw_file(ft8xx_bitmap_info *bitmap)
{
    FILE *file;
    esp_err_t ret = ESP_OK;
    uint32_t file_size = 0;
    uint8_t *buffer = NULL;
    uint16_t block_len;
    uint32_t addr = bitmap->addr;

    file = fopen(bitmap->file_path, "rb");
    if (file == NULL)
    {
        ESP_LOGE(TAG, "%s() Unable to open: %s", __func__, bitmap->file_path);
        return ESP_FAIL;
    }
    fseek(file, 0 , SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = malloc(BLOCK_LEN);
    if (buffer == NULL)
    {
        ESP_LOGE(TAG, "%s() Unable allocate memory", __func__);
        ret = ESP_ERR_NO_MEM;
        goto err;
    }
    
    while (file_size)
    {
        block_len = file_size > BLOCK_LEN ? BLOCK_LEN : file_size;
        fread(buffer, 1, block_len, file);
        file_size -= block_len;
        block_len += 3;
        block_len &= ~3U;
        gpu_start_transfer(block_len);
        gpu_transfer((uint8_t *)buffer, block_len);
        gpu_end_transfer(block_len);
        addr += block_len;
    }
    bitmap->loaded_to_ram = true;
err:
    if (buffer)
    {
        free(buffer);
    }
    fclose(file);
    return ret;
}

esp_err_t bsp_display_load_inflate_file(ft8xx_bitmap_info *bitmap)
{
    FILE *file;
    esp_err_t ret = ESP_OK;
    uint32_t file_size = 0;
    uint8_t *buffer = NULL;
    uint16_t block_len;
    uint32_t addr = bitmap->addr;

    file = fopen(bitmap->file_path, "rb");
    if (file == NULL)
    {
        ESP_LOGE(TAG, "%s() Unable to open: %s", __func__, bitmap->file_path);
        return ESP_FAIL;
    }
    fseek(file, 0 , SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = malloc(BLOCK_LEN);
    if (buffer == NULL)
    {
        ESP_LOGE(TAG, "%s() Unable allocate memory", __func__);
        ret = ESP_ERR_NO_MEM;
        goto err;
    }
    ft8xx_command_inflate(addr);
    while (file_size)
    {
        block_len = file_size > BLOCK_LEN ? BLOCK_LEN : (uint16_t)file_size;
        fread(buffer, 1, block_len, file);
        file_size -= block_len;
        block_len += 3;
        block_len &= ~3U;
        gpu_start_transfer(block_len);
        gpu_transfer((uint8_t *)buffer, block_len);
        gpu_end_transfer(block_len);
        addr += block_len;
    }
    gpu_wait_for_idle();
    bitmap->loaded_to_ram = true;
err:
    if (buffer)
    {
        free(buffer);
    }
    fclose(file);
    return ret;    
}

esp_err_t bsp_display_load_image_file(ft8xx_bitmap_info *bitmap)
{
    FILE *file;
    esp_err_t ret = ESP_OK;
    uint32_t file_size = 0;
    uint8_t *buffer = NULL;
    uint16_t block_len;
    uint32_t addr = bitmap->addr;

    file = fopen(bitmap->file_path, "rb");
    if (file == NULL)
    {
        ESP_LOGE(TAG, "%s() Unable to open: %s", __func__, bitmap->file_path);
        return ESP_FAIL;
    }
    fseek(file, 0 , SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = malloc(BLOCK_LEN);
    if (buffer == NULL)
    {
        ESP_LOGE(TAG, "%s() Unable allocate memory", __func__);
        ret = ESP_ERR_NO_MEM;
        goto err;
    }
    ft8xx_command_load_image(addr, OPT_NODL);
    while (file_size)
    {
        block_len = file_size > BLOCK_LEN ? BLOCK_LEN : (uint16_t)file_size;
        fread(buffer, 1, block_len, file);
        file_size -= block_len;
        block_len += 3;
        block_len &= ~3U;
        gpu_start_transfer(block_len);
        gpu_transfer((uint8_t *)buffer, block_len);
        gpu_end_transfer(block_len);
        addr += block_len;
    }
    gpu_wait_for_idle();
    bitmap->loaded_to_ram = true;
err:
    if (buffer)
    {
        free(buffer);
    }
    fclose(file);
    return ret;
}

esp_err_t bsp_display_ui_draw_bitmap(int16_t x, int16_t y, uint32_t rgba, ft8xx_bitmap_info *bitmap)
{
    if (!bitmap)
    {
        ESP_LOGE(TAG, "%s() Bitmap NULL", __func__);
        return ESP_ERR_INVALID_ARG;
    }

    if (!bitmap->loaded_to_ram)
    {
        if (bitmap->format == PNG || bitmap->format == JPEG)
        {
            if (bsp_display_load_image_file(bitmap) != ESP_OK)
            {
                ESP_LOGE(TAG, "%s() Load image file failed", __func__);
                return ESP_FAIL;
            }
        }
        else if (bitmap->compressed)
        {
            if (bsp_display_load_inflate_file(bitmap) != ESP_OK)
            {
                ESP_LOGE(TAG, "%s() Load inflate file failed", __func__);
                return ESP_FAIL;
            }
        }
        else if (bsp_display_load_draw_file(bitmap) != ESP_OK)
        {
            ESP_LOGE(TAG, "%s() Load raw file failed", __func__);
            return ESP_FAIL;
        }
    }
    FTColor_t color;
    color.value = rgba;
    ft8xx_command(SAVE_CONTEXT());
    ft8xx_command(COLOR_A(color.rgba.alpha));
    ft8xx_command(COLOR_RGB(color.rgba.red, color.rgba.green, color.rgba.blue));
    ft8xx_command(BEGIN(BITMAPS));
    ft8xx_command(BITMAP_SOURCE(bitmap->addr));
    if (bitmap->format == PNG || bitmap->format == JPEG)
    {
        ft8xx_command(BITMAP_LAYOUT(bitmap->sub_format, bitmap->stride, bitmap->height));
    }
    else
    {
        ft8xx_command(BITMAP_LAYOUT(bitmap->format, bitmap->stride, bitmap->height));
    }
    ft8xx_command(BITMAP_SIZE(NEAREST, BORDER, BORDER, bitmap->width, bitmap->height));
    ft8xx_command(VERTEX2F(x * 16, y * 16));
    ft8xx_command(RESTORE_CONTEXT());
    return ESP_OK;
}
