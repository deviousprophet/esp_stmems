#ifndef ESP_STMEMS_H
#define ESP_STMEMS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stddef.h>
#include <stdint.h>

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"
#include "esp_err.h"

/** @defgroup  Endianness definitions
 * @{
 *
 */

#ifndef DRV_BYTE_ORDER
#ifndef __BYTE_ORDER__

#define DRV_LITTLE_ENDIAN 1234
#define DRV_BIG_ENDIAN    4321

/** if _BYTE_ORDER is not defined, choose the endianness of your architecture
 * by uncommenting the define which fits your platform endianness
 */
// #define DRV_BYTE_ORDER    DRV_BIG_ENDIAN
#define DRV_BYTE_ORDER    DRV_LITTLE_ENDIAN

#else /* defined __BYTE_ORDER__ */

#define DRV_LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
#define DRV_BIG_ENDIAN    __ORDER_BIG_ENDIAN__
#define DRV_BYTE_ORDER    __BYTE_ORDER__

#endif /* __BYTE_ORDER__*/
#endif /* DRV_BYTE_ORDER */

/**
 * @}
 *
 */

/** @defgroup STMicroelectronics sensors common types
 * @{
 *
 */

#ifndef MEMS_SHARED_TYPES
#define MEMS_SHARED_TYPES

typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
    uint8_t bit0 : 1;
    uint8_t bit1 : 1;
    uint8_t bit2 : 1;
    uint8_t bit3 : 1;
    uint8_t bit4 : 1;
    uint8_t bit5 : 1;
    uint8_t bit6 : 1;
    uint8_t bit7 : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
    uint8_t bit7 : 1;
    uint8_t bit6 : 1;
    uint8_t bit5 : 1;
    uint8_t bit4 : 1;
    uint8_t bit3 : 1;
    uint8_t bit2 : 1;
    uint8_t bit1 : 1;
    uint8_t bit0 : 1;
#endif /* DRV_BYTE_ORDER */
} bitwise_t;

#define PROPERTY_DISABLE (0U)
#define PROPERTY_ENABLE  (1U)

/** @addtogroup  Interfaces_Functions
 * @brief       This section provide a set of functions used to read and
 *              write a generic register of the device.
 *              MANDATORY: return 0 -> no Error.
 * @{
 *
 */

typedef int32_t (*stmdev_write_ptr)(void*, uint8_t, const uint8_t*, uint16_t);
typedef int32_t (*stmdev_read_ptr)(void*, uint8_t, uint8_t*, uint16_t);
typedef void (*stmdev_mdelay_ptr)(uint32_t millisec);

typedef struct {
    /** Component mandatory fields **/
    stmdev_write_ptr write_reg;
    stmdev_read_ptr read_reg;
    /** Component optional fields **/
    stmdev_mdelay_ptr mdelay;
    /** Customizable optional pointer **/
    void* handle;
} stmdev_ctx_t;

/**
 * @}
 *
 */

#endif /* MEMS_SHARED_TYPES */

#ifndef MEMS_UCF_SHARED_TYPES
#define MEMS_UCF_SHARED_TYPES

/** @defgroup    Generic address-data structure definition
 * @brief       This structure is useful to load a predefined configuration
 *              of a sensor.
 *              You can create a sensor configuration by your own or using
 *              Unico / Unicleo tools available on STMicroelectronics
 *              web site.
 *
 * @{
 *
 */

typedef struct {
    uint8_t address;
    uint8_t data;
} ucf_line_t;

/**
 * @}
 *
 */

#endif /* MEMS_UCF_SHARED_TYPES */

typedef stmdev_ctx_t esp_stmems_handle_t;

esp_err_t esp_stmems_new_i2c_device(i2c_port_t i2c_num, uint8_t i2c_address, esp_stmems_handle_t* handle);

esp_err_t esp_stmems_new_spi_device(spi_host_device_t host_id, const spi_device_interface_config_t* dev_config, esp_stmems_handle_t* handle);

#ifdef __cplusplus
}
#endif

#endif /* ESP_STMEMS_H */