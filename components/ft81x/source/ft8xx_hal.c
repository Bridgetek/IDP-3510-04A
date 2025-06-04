/*******************************************************************************
 * Includes
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "ft8xx_gpu.h"

/*******************************************************************************
 * Defines
 *******************************************************************************/

#define POWER_DOWN_CYCLE_DELAY	50
#define FT8XX_CHIP_BOOTUP_DELAY 200

/*******************************************************************************
 * Local Types and Typedefs
 *******************************************************************************/

/*Nothing to declare*/

/*******************************************************************************
 * Global Variables
 *******************************************************************************/

/*******************************************************************************
 * Static Variables
 *******************************************************************************/
static spi_device_handle_t spi_dev;
static uint8_t NO_OF_DUMMIES = 1;
/*******************************************************************************
 * Static Function Prototypes
 *******************************************************************************/

/*******************************************************************************
 * Functions
 *******************************************************************************/

static inline void ft8xx_cs_select(void)
{
    gpio_set_level(GPIO_LCD_CS, 0);
}

static inline void ft8xx_cs_deselect(void)
{
    gpio_set_level(GPIO_LCD_CS, 1);
}

esp_err_t ft8xx_hal_init(void)
{
    esp_err_t ret = ESP_OK;

    // Initialize FT8xx Chip select pin
    gpio_config_t cs_config = {
        .pin_bit_mask = (1ULL << GPIO_LCD_CS),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = true,
    };
    ret = gpio_config(&cs_config);
    ESP_ERROR_CHECK(ret);
    gpio_set_level(GPIO_LCD_CS, 1);

    // Initialize FT8xx Power down pin
    gpio_config_t pdn_config = {
        .pin_bit_mask = (1ULL << GPIO_LCD_PDN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = true,
    };
    ret = gpio_config(&pdn_config);
    ESP_ERROR_CHECK(ret);
    gpio_set_level(GPIO_LCD_PDN, 1);
    
    spi_bus_config_t bus_config = {
        .miso_io_num = GPIO_LCD_MISO,
        .mosi_io_num = GPIO_LCD_MOSI,
        .sclk_io_num = GPIO_LCD_SCLK,
        .quadhd_io_num = -1,
        .quadwp_io_num = -1,
    };
    spi_device_interface_config_t dev_config = {
        .clock_speed_hz = LCD_SPI_CLK_HZ * 1000 * 1000,
        .clock_source = SPI_CLK_SRC_DEFAULT,
        .mode = 0,
        .spics_io_num = -1,
        .queue_size = 3,
        .duty_cycle_pos = 128,
    };

    ret = spi_bus_initialize(LCD_SPI_HOST, &bus_config, 1); // DMA Channel 0 is limited to 32 bytes. So use DMA Channel 1
    ESP_ERROR_CHECK(ret);
    ret = spi_bus_add_device(LCD_SPI_HOST, &dev_config, &spi_dev);
    ESP_ERROR_CHECK(ret);

    return ret;
}

void ft8xx_hal_set_dummies(uint8_t d)
{
    NO_OF_DUMMIES = d;
}

void ft8xx_power()
{
    gpio_set_level(GPIO_LCD_PDN, 0);
    vTaskDelay(POWER_DOWN_CYCLE_DELAY / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_LCD_PDN, 1);
    vTaskDelay(POWER_DOWN_CYCLE_DELAY / portTICK_PERIOD_MS);
}

void ft8xx_hal_host_command(uint32_t cmd)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 3 * 8,
    t.tx_buffer = (uint8_t*)&cmd;
    ft8xx_cs_select();
    ret = spi_device_polling_transmit(spi_dev, &t);
    ft8xx_cs_deselect();
    ESP_ERROR_CHECK(ret);
}

uint8_t ft8xx_hal_rd8(uint32_t address)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    uint8_t wr_buffer[6] = {0};
    uint8_t read_data = 0;

    /* Write address and dummy bytes */
    wr_buffer[0] = (uint8_t)(address >> 16);
    wr_buffer[1] = (uint8_t)(address >> 8);
    wr_buffer[2] = (address & 0xff);
    t.length = (3 + NO_OF_DUMMIES) * 8;
    t.tx_buffer = wr_buffer;
    ft8xx_cs_select();
    ret = spi_device_polling_transmit(spi_dev, &t);
    ESP_ERROR_CHECK(ret);

    /* Read back data */
    memset(&t, 0, sizeof(t));
    t.length = 1 * 8;
    t.tx_buffer = NULL;
    t.rx_buffer = &read_data;
    ret = spi_device_polling_transmit(spi_dev, &t);
    ft8xx_cs_deselect();
    ESP_ERROR_CHECK(ret);

    return read_data;
}

uint16_t ft8xx_hal_rd16(uint32_t address)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    uint8_t wr_buffer[6] = {0};
    uint8_t read_data[2] = {0};

    /* Write address and dummy bytes */
    wr_buffer[0] = (uint8_t)(address >> 16);
    wr_buffer[1] = (uint8_t)(address >> 8);
    wr_buffer[2] = (address & 0xff);
    t.length = (3 + NO_OF_DUMMIES) * 8;
    t.tx_buffer = wr_buffer;
    ft8xx_cs_select();
    ret = spi_device_polling_transmit(spi_dev, &t);
    ESP_ERROR_CHECK(ret);

    /* Read back data */
    memset(&t, 0, sizeof(t));
    t.length = 2 * 8;
    t.tx_buffer = NULL;
    t.rx_buffer = read_data;
    ret = spi_device_polling_transmit(spi_dev, &t);
    ft8xx_cs_deselect();
    ESP_ERROR_CHECK(ret);

    return (uint16_t)(read_data[1] << 8) | read_data[0];
}

uint32_t ft8xx_hal_rd32(uint32_t address)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    uint8_t wr_buffer[6] = {0};
    uint8_t read_data[4] = {0};

    /* Write address and dummy bytes */
    wr_buffer[0] = (uint8_t)(address >> 16);
    wr_buffer[1] = (uint8_t)(address >> 8);
    wr_buffer[2] = (address & 0xff);
    t.length = (3 + NO_OF_DUMMIES) * 8;
    t.tx_buffer = wr_buffer;
    ft8xx_cs_select();
    ret = spi_device_polling_transmit(spi_dev, &t);
    ESP_ERROR_CHECK(ret);

    /* Read back data */
    memset(&t, 0, sizeof(t));
    t.length = 4 * 8;
    t.tx_buffer = NULL;
    t.rx_buffer = read_data;
    ret = spi_device_polling_transmit(spi_dev, &t);
    ft8xx_cs_deselect();
    ESP_ERROR_CHECK(ret);
    return ((read_data[3] << 24) | (read_data[2] << 16) | (read_data[1] << 8) | read_data[0]);
}

void ft8xx_hal_rd8s(uint32_t address, uint8_t *buffer, uint16_t size)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    uint8_t wr_buffer[6] = {0};

    /* Write address and dummy bytes */
    wr_buffer[0] = (uint8_t)(address >> 16);
    wr_buffer[1] = (uint8_t)(address >> 8);
    wr_buffer[2] = (address & 0xff);
    t.length = (3 + NO_OF_DUMMIES) * 8;
    t.tx_buffer = wr_buffer;
    ft8xx_cs_select();
    ret = spi_device_polling_transmit(spi_dev, &t);
    ESP_ERROR_CHECK(ret);

    /* Read back data */
    memset(&t, 0, sizeof(t));
    t.length = size * 8;
    t.tx_buffer = NULL;
    t.rx_buffer = buffer;
    ret = spi_device_polling_transmit(spi_dev, &t);
    ft8xx_cs_deselect();
    ESP_ERROR_CHECK(ret);
}

void ft8xx_hal_wr8(uint32_t address, uint8_t buffer)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    uint8_t wr_buffer[4];

    wr_buffer[0] = (uint8_t)(0x80 | (address >> 16));
    wr_buffer[1] = (uint8_t)(address >> 8);
    wr_buffer[2] = (address & 0xff);
    wr_buffer[3] = buffer;

    t.length = 4 * 8;
    t.tx_buffer = wr_buffer;
    ft8xx_cs_select();
    ret = spi_device_polling_transmit(spi_dev, &t);
    ft8xx_cs_deselect();
    ESP_ERROR_CHECK(ret);
}

void ft8xx_hal_wr8s(uint32_t address, const uint8_t *buffer, const uint32_t len)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    uint8_t wr_buffer[4];

    wr_buffer[0] = (uint8_t)(0x80 | (address >> 16));
    wr_buffer[1] = (uint8_t)(address >> 8);
    wr_buffer[2] = (address & 0xff);

    t.length = 3 * 8;
    t.tx_buffer = wr_buffer;
    ft8xx_cs_select();
    ret = spi_device_polling_transmit(spi_dev, &t);
    ESP_ERROR_CHECK(ret);

    memset(&t, 0, sizeof(t));
    t.length = len * 8;
    t.tx_buffer = buffer;
    ret = spi_device_polling_transmit(spi_dev, &t);
    ft8xx_cs_deselect();
    ESP_ERROR_CHECK(ret);
}

void ft8xx_hal_wr16(uint32_t address, uint16_t buffer)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    uint8_t wr_buffer[5];

    wr_buffer[0] = (uint8_t)(0x80 | (address >> 16));
    wr_buffer[1] = (uint8_t)(address >> 8);
    wr_buffer[2] = (address & 0xff);
    wr_buffer[3] = buffer & 0xff;
    wr_buffer[4] = ((buffer >> 8) & 0xff);

    t.length = 5 * 8;
    t.tx_buffer = wr_buffer;
    ft8xx_cs_select();
    ret = spi_device_polling_transmit(spi_dev, &t);
    ft8xx_cs_deselect();
    ESP_ERROR_CHECK(ret);
}

void ft8xx_hal_wr32(uint32_t address, uint32_t buffer)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    uint8_t wr_buffer[7];

    wr_buffer[0] = (uint8_t)(0x80 | (address >> 16));
    wr_buffer[1] = (uint8_t)(address >> 8);
    wr_buffer[2] = (address & 0xff);
    wr_buffer[3] = buffer & 0xff;
    wr_buffer[4] = ((buffer >> 8) & 0xff);
    wr_buffer[5] = ((buffer >> 16) & 0xff);
    wr_buffer[6] = ((buffer >> 24) & 0xff);

    t.length = 7 * 8;
    t.tx_buffer = wr_buffer;
    ft8xx_cs_select();
    ret = spi_device_polling_transmit(spi_dev, &t);
    ft8xx_cs_deselect();
    ESP_ERROR_CHECK(ret);
}

void ft8xx_hal_write_string(const char *string)
{
    uint16_t length = (uint16_t)strnlen(string, FT_SONARQ_STR_MAXLEN);
    uint8_t	 _char = 0;
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    t.length = length * 8;
    t.tx_buffer = string;
    ft8xx_cs_select();
    ret = spi_device_polling_transmit(spi_dev, &t);
    ESP_ERROR_CHECK(ret);

    memset(&t, 0, sizeof(t));
    t.length = 1 * 8;
    t.tx_buffer = &_char;
    ret = spi_device_polling_transmit(spi_dev, &t);
    ft8xx_cs_deselect();
    ESP_ERROR_CHECK(ret);
}

void ft8xx_hal_coproc_start_transfer(uint32_t address)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    uint8_t wr_buffer[3];

    wr_buffer[0] = (uint8_t)(0x80 | (address >> 16));
    wr_buffer[1] = (uint8_t)(address >> 8);
    wr_buffer[2] = (address & 0xff);

    t.length = 3 * 8;
    t.tx_buffer = wr_buffer;
    ft8xx_cs_select();
    ret = spi_device_polling_transmit(spi_dev, &t);
    ESP_ERROR_CHECK(ret);
}

void ft8xx_hal_coproc_transfer(uint8_t *buffer, uint16_t len)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    t.length = len * 8;
    t.tx_buffer = buffer;
    ret = spi_device_polling_transmit(spi_dev, &t);
    ESP_ERROR_CHECK(ret);
}

void ft8xx_hal_coproc_command(uint32_t cmd)
{
    uint8_t buffer[4] = {
        (cmd & 0xff),
        ((cmd >> 8) & 0xff),
        ((cmd >> 16) & 0xff),
        ((cmd >> 24) & 0xff)};
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    t.length = 4 * 8;
    t.tx_buffer = buffer;
    ret = spi_device_polling_transmit(spi_dev, &t);
    ESP_ERROR_CHECK(ret);
}

void ft8xx_hal_coproc_end_transfer(void)
{
    ft8xx_cs_deselect();
}

void ft8xx_hal_coproc_transfer_string(const char *string)
{
    uint16_t length = (uint16_t)strnlen(string, FT_SONARQ_STR_MAXLEN);
    uint8_t	 _char = 0;
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    
    t.length = length * 8;
    t.tx_buffer = string;
    ret = spi_device_polling_transmit(spi_dev, &t);
    ESP_ERROR_CHECK(ret);

    memset(&t, 0, sizeof(t));
    t.length = 1 * 8;
    t.tx_buffer = &_char;
    ret = spi_device_polling_transmit(spi_dev, &t);
    ESP_ERROR_CHECK(ret);
}
