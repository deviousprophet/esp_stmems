#include <string.h>

#include "esp_check.h"
#include "esp_log.h"
#include "i2c_bus.h"

#include "esp_stmems.h"

static const char* TAG = "esp_stmems";

static esp_err_t i2c_write_reg(void* handle, uint8_t reg, const uint8_t* bufp, uint16_t len) {
    i2c_bus_device_handle_t bus_handle = (i2c_bus_device_handle_t)handle;
    return i2c_bus_write_byte(bus_handle, reg, *bufp);
}

static esp_err_t i2c_read_reg(void* handle, uint8_t reg, uint8_t* bufp, uint16_t len) {
    i2c_bus_device_handle_t bus_handle = (i2c_bus_device_handle_t)handle;
    return i2c_bus_read_byte(bus_handle, reg, bufp);
}

static void mdelay(uint32_t millisec) {
    vTaskDelay(pdMS_TO_TICKS(millisec));
}

esp_stmems_handle_t esp_stmems_new_i2c_device(i2c_bus_handle_t bus_handle, uint8_t dev_addr, uint32_t clk_speed) {
    stmdev_ctx_t* stmems_cxt = calloc(1, sizeof(stmdev_ctx_t));
    ESP_RETURN_ON_FALSE(stmems_cxt, NULL, TAG, "calloc memory failed");
    stmems_cxt->handle    = i2c_bus_device_create(bus_handle, dev_addr, clk_speed);
    stmems_cxt->write_reg = i2c_write_reg;
    stmems_cxt->read_reg  = i2c_read_reg;
    stmems_cxt->mdelay    = mdelay;
    return (esp_stmems_handle_t)stmems_cxt;
}
