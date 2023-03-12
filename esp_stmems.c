#include <string.h>

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"
#include "esp_check.h"
#include "esp_log.h"

#include "esp_stmems.h"

#define I2C_TIMEOUT_MS (100)

static const char* TAG = "esp_stmems";

typedef struct stmems_i2c_device_handle_s* stmems_i2c_device_handle_t;
typedef spi_device_handle_t stmems_spi_device_handle_t;

struct stmems_i2c_device_handle_s {
    i2c_port_t i2c_num;
    uint8_t i2c_address;
};

static esp_err_t i2c_write_reg(void* handle, uint8_t reg, const uint8_t* bufp, uint16_t len) {
    stmems_i2c_device_handle_t stmems = (stmems_i2c_device_handle_t)handle;
    uint8_t data[len + 1];
    data[0] = reg;
    memcpy(data + 1, bufp, len);
    ESP_RETURN_ON_ERROR(i2c_master_write_to_device(stmems->i2c_num, stmems->i2c_address, data, len + 1, pdMS_TO_TICKS(I2C_TIMEOUT_MS)), TAG,
                        "Write reg failed");
    return ESP_OK;
}

static esp_err_t i2c_read_reg(void* handle, uint8_t reg, uint8_t* bufp, uint16_t len) {
    stmems_i2c_device_handle_t stmems = (stmems_i2c_device_handle_t)handle;
    ESP_RETURN_ON_ERROR(i2c_master_write_read_device(stmems->i2c_num, stmems->i2c_address, &reg, 1, bufp, len, pdMS_TO_TICKS(I2C_TIMEOUT_MS)), TAG,
                        "Read reg failed");
    return ESP_OK;
}

static esp_err_t spi_write_reg(void* handle, uint8_t reg, const uint8_t* bufp, uint16_t len) {
    return ESP_OK;
}

static esp_err_t spi_read_reg(void* handle, uint8_t reg, uint8_t* bufp, uint16_t len) {
    return ESP_OK;
}

static void mdelay(uint32_t millisec) {
    vTaskDelay(pdMS_TO_TICKS(millisec));
}

esp_err_t esp_stmems_new_i2c_device(i2c_port_t i2c_num, uint8_t i2c_address, esp_stmems_handle_t* handle) {
    ESP_RETURN_ON_FALSE(i2c_num < I2C_NUM_MAX, ESP_ERR_INVALID_ARG, TAG, "Invalid i2c num");
    ESP_RETURN_ON_FALSE(handle, ESP_ERR_INVALID_ARG, TAG, "Invalid handle");

    stmems_i2c_device_handle_t stmems_i2c = (stmems_i2c_device_handle_t)malloc(sizeof(stmems_i2c_device_handle_t));
    ESP_RETURN_ON_FALSE(stmems_i2c, ESP_ERR_NO_MEM, TAG, "Malloc failed");

    stmems_i2c->i2c_num     = i2c_num;
    stmems_i2c->i2c_address = i2c_address;

    handle->handle          = stmems_i2c;
    handle->write_reg       = i2c_write_reg;
    handle->read_reg        = i2c_read_reg;
    handle->mdelay          = mdelay;

    return ESP_OK;
}

esp_err_t esp_stmems_new_spi_device(spi_host_device_t host_id, const spi_device_interface_config_t* dev_config, esp_stmems_handle_t* handle) {
    ESP_RETURN_ON_FALSE(handle, ESP_ERR_INVALID_ARG, TAG, "Invalid handle");

    stmems_spi_device_handle_t stmems_spi = (stmems_spi_device_handle_t)malloc(sizeof(stmems_spi_device_handle_t));
    ESP_RETURN_ON_FALSE(stmems_spi, ESP_ERR_NO_MEM, TAG, "Malloc failed");

    esp_err_t ret = spi_bus_add_device(host_id, dev_config, &stmems_spi);
    ESP_GOTO_ON_ERROR(ret, err, TAG, "SPI bus add device failed");

    handle->handle    = stmems_spi;
    handle->write_reg = spi_write_reg;
    handle->read_reg  = spi_read_reg;
    handle->mdelay    = mdelay;

    return ret;

err:
    free(stmems_spi);
    return ret;
}