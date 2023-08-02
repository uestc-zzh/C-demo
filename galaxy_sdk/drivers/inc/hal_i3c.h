/**
 * Copyright (C) 2023 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_i3c.h
 * @brief public head file of I3C HAL
 *
 */

#ifndef _HAL_I3C_H
#define _HAL_I3C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "hal_common.h"
#include "hal_dmac.h"

/**< Max supported I3C ports */
#define I3C_PORT_MAX (3)

/* I3C CCC (Common Command Codes) related definitions */
#define I3C_CCC_DIRECT BIT(7)

#define I3C_CCC_ID(id, broadcast) ((id) | ((broadcast) ? 0 : I3C_CCC_DIRECT))

/* CCC Commands valid in both broadcast and unicast modes */
#define I3C_CCC_ENEC(broadcast) I3C_CCC_ID(0x0, broadcast)
#define I3C_CCC_DISEC(broadcast) I3C_CCC_ID(0x1, broadcast)
#define I3C_CCC_ENTAS(as, broadcast) I3C_CCC_ID(0x2 + (as), broadcast)
#define I3C_CCC_RSTDAA(broadcast) I3C_CCC_ID(0x6, broadcast)
#define I3C_CCC_SETMWL(broadcast) I3C_CCC_ID(0x9, broadcast)
#define I3C_CCC_SETMRL(broadcast) I3C_CCC_ID(0xa, broadcast)
#define I3C_CCC_SETXTIME(broadcast) ((broadcast) ? 0x28 : 0x98)
#define I3C_CCC_VENDOR(id, broadcast) ((id) + ((broadcast) ? 0x61 : 0xe0))

/* Broadcast-only CCC commands */
#define I3C_CCC_ENTDAA I3C_CCC_ID(0x7, true)
#define I3C_CCC_DEFSLVS I3C_CCC_ID(0x8, true)
#define I3C_CCC_ENTTM I3C_CCC_ID(0xb, true)
#define I3C_CCC_ENTHDR(x) I3C_CCC_ID(0x20 + (x), true)

/* Unicast-only CCC commands */
#define I3C_CCC_SETDASA I3C_CCC_ID(0x7, false)
#define I3C_CCC_SETNEWDA I3C_CCC_ID(0x8, false)
#define I3C_CCC_GETMWL I3C_CCC_ID(0xb, false)
#define I3C_CCC_GETMRL I3C_CCC_ID(0xc, false)
#define I3C_CCC_GETPID I3C_CCC_ID(0xd, false)
#define I3C_CCC_GETBCR I3C_CCC_ID(0xe, false)
#define I3C_CCC_GETDCR I3C_CCC_ID(0xf, false)
#define I3C_CCC_GETSTATUS I3C_CCC_ID(0x10, false)
#define I3C_CCC_GETACCMST I3C_CCC_ID(0x11, false)
#define I3C_CCC_SETBRGTGT I3C_CCC_ID(0x13, false)
#define I3C_CCC_GETMXDS I3C_CCC_ID(0x14, false)
#define I3C_CCC_GETHDRCAP I3C_CCC_ID(0x15, false)
#define I3C_CCC_GETXTIME I3C_CCC_ID(0x19, false)

/* Events for ENEC and DISEC commands */
#define I3C_CCC_EVENT_SIR BIT(0)
#define I3C_CCC_EVENT_MR BIT(1)
#define I3C_CCC_EVENT_HJ BIT(3)

typedef enum I3cIdDef {
    I3C_ID_0,
    I3C_ID_MAX,
} I3cIdDef;

#define HAL_I3C_DEV_MAX (I3C_ID_MAX)

// #define I3C_DEBUG
#ifdef I3C_DEBUG
#define I3C_DEBUG(fmt, args...) uart_printf(fmt, ##args)
#else
#define I3C_DEBUG(fmt, args...)
#endif

/**
 * @enum I3cWorkMode
 * @brief Define the I3C work mode
 */
enum I3cWorkMode {
    I3C_MODE_MAIN_MASTER      = 1, /**< i3c works as main master */
    I3C_MODE_SECONDARY_MASTER = 2, /**< i3c works as secondary master */
    I3C_MODE_I3C_SLAVE        = 3, /**< i3c works as slave */
    I3C_MODE_I2C_LEGACY_SLAVE = 4, /**< i3c works as i2c legacy slave */
};

/**
 * @enum I3cBusRate
 * @brief Specifies one of the standard I3C bus bit rates for I3C communication
 * 0: SDR0 1: SDR1 2: SDR2 3: SDR3 4: SDR4 5: HDR-TS 6: HDR-DDR 7: I2C FM
 */
enum I3cBusRate {
    I3C_BUS_RATE_SDR0    = 0, /**< I3C bus SCL RATE: SDR0 */
    I3C_BUS_RATE_SDR1    = 1, /**< I3C bus SCL RATE: SDR1 */
    I3C_BUS_RATE_SDR2    = 2, /**< I3C bus SCL RATE: SDR2 */
    I3C_BUS_RATE_SDR3    = 3, /**< I3C bus SCL RATE: SDR3 */
    I3C_BUS_RATE_SDR4    = 4, /**< I3C bus SCL RATE: SDR4 */
    I3C_BUS_RATE_HDR_TS  = 5, /**< I3C bus SCL RATE: HDR-TS */
    I3C_BUS_RATE_HDR_DDR = 6, /**< I3C bus SCL RATE: HDR-DDR */
    I3C_BUS_RATE_I2C_FM  = 7, /**< I3C bus SCL RATE: I2C FM */
};

/**
 * @enum I3cI2cModeBusRate
 * @brief Specifies one of the I2C mode I3C bus bit rates for I3C communication
 * 0: I2C FM 1: I2C FM+
 */
enum I3cI2cModeBusRate {
    I3C_I2C_MODE_BUS_RATE_I2C_FM  = 0, /**< I3C bus SCL RATE: I2C FM */
    I3C_I2C_MODE_BUS_RATE_I2C_FMP = 1, /**< I3C bus SCL RATE: I2C FMP */
};

/**
 * @brief This struct define i3c config args
 */
typedef struct I3cConfig {
    uint8_t speed; /**< I3C bus speed, @see I3cBusRate */
    uint8_t work_mode; /**< master or slave mode, @see I3cWorkMode */
    uint8_t xfer_mode; /**< polling, intr or dma mode, @see I3cXferMode */
    uint8_t dev_static_addr; /**< slave device static addr */
    uint8_t dev_dynamic_addr; /**< slave device dynamic addr */
    uint8_t dev_idx; /**< slave device addr index */
    uint8_t reg_width; /**< width of reg addr in byte */
} I3cConfig;

/**
 * @brief Structure of I3C hardware configuration
 */
typedef struct I3cHwConfig {
    uint32_t base; /**< Base address of I3C device */
    uint8_t id; /**< Device ID of this I3C device */
    uint8_t xfer_capability; /**< I3C capability */
    uint8_t master_dyn_addr; /**< I3C master dynamic address */
    uint8_t tx_mux_id; /**< Hardware tx handshake id */
    uint8_t rx_mux_id; /**< Hardware rx handshake id */
    uint16_t clk_id; /**< Clock ID of this I3C device */
    uint16_t irq_id; /**< IRQ ID of this I3C device */
} I3cHwConfig;

/**
 * @struct I3cDevice
 * @brief This struct define i3c main handle
 */
typedef struct I3cDevice {
    uint8_t port_id; /**< I3C port id */
    DmacDevice *dmac_dev; /**<  Instance of DMAC device */
    const I3cHwConfig *hw_cfg; /**< I3C hardware configuration */
    const void *ops; /**< I3C operations */
    void *ctx; /**< Private context */
} I3cDevice;

/**
 * @brief Structure of operations for HAL of I3C
 */
typedef struct I3cOperations {
    /**
     * I3C operation for init
     */
    int (*init)(const I3cDevice *device);

    /**
     * I3C operation for single read
     */
    int (*master_reg_read)(const I3cDevice *device, const I3cConfig *config,
                           uint32_t reg_addr, uint8_t *rev_buf,
                           uint32_t rev_size);
    /**
     * I3C operation for single write
     */
    int (*master_reg_write)(const I3cDevice *device, const I3cConfig *config,
                            uint32_t reg_addr, const uint8_t *wr_addr,
                            uint32_t wr_size);
    /**
     * I3C operation for CCC get Command
     */
    int (*ccc_get)(const I3cDevice *device, const I3cConfig *config,
                   uint8_t ccc_cmd_type, uint8_t *payload,
                   uint32_t payload_len);
    /**
     * I3C operation for CCC set Command
     */
    int (*ccc_set)(const I3cDevice *device, const I3cConfig *config,
                   uint8_t ccc_cmd_type, const uint8_t *payload,
                   uint32_t payload_len);
    /**
     * I3C operation for device address assignment
     */
    int (*set_addr)(const I3cDevice *device, const I3cConfig *config);
    /**
     * I3C operation for deinit
     */
    void (*deinit)(const I3cDevice *device);
    /**
     * I3C operation for irq handler
     */
    void (*irq_handler)(const I3cDevice *device);
} I3cOperations;

/**
 * @brief Add I3C device instance
 * @note This API should be called by I3C IP driver to add initialized instance
 * to HAL, not for application usage
 * @param device The device of I3C will be added
 * @return Return VSD_SUCCESS for succeed, others for failure
 */
int hal_i3c_add_dev(I3cDevice *device);

/**
 * @brief Get I3C device instance
 * @param port_id The port id of I3C
 * @return Return the I3C instance
 */
I3cDevice *hal_i3c_get_device(uint8_t port_id);

/**
 * @brief Initialises an I3C interface
 * Prepares an I3C hardware interface for communication as a master or slave
 * @note This API shall be called once before calling any I3C read/write APIs
 * @param[in]  device  the device of i3c port which will be initialized
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_i3c_init(const I3cDevice *device);

/**
 * @brief I3c reg write
 *
 * @param[in]  device         the i3c device
 * @param[in]  config         the i3c config of device
 * @param[in]  reg_addr       reg address
 * @param[in]  data           i3c master send data
 * @param[in]  size           i3c master send data size in byte
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_i3c_reg_write(const I3cDevice *device, const I3cConfig *config,
                      uint32_t reg_addr, const uint8_t *data, uint32_t size);

/**
 * @brief I3c master reg read
 *
 * @param[in]   device         the i3c device
 * @param[in]   config         the i3c config of device
 * @param[in]   reg_addr       reg address
 * @param[out]  data           buffer to store read data from I3C device
 * @param[in]   size           i3c master send data size in byte
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_i3c_reg_read(const I3cDevice *device, const I3cConfig *config,
                     uint32_t reg_addr, uint8_t *data, uint32_t size);

/**
 * @brief I3c ccc set command
 *
 * @param[in]  device         the i3c device
 * @param[in]  config         the i3c config of device
 * @param[in]  ccc_cmd_type   the type of ccc command
 * @param[in]  payload        the payload address of i3c ccc command
 * @param[in]  payload_len    the payload length of i3c ccc command
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_i3c_ccc_set(const I3cDevice *device, const I3cConfig *config,
                    uint8_t ccc_cmd_type, const uint8_t *payload,
                    uint32_t payload_len);

/**
 * @brief I3c ccc get command
 *
 * @param[in]  device         the i3c device
 * @param[in]  config         the i3c config of device
 * @param[in]  ccc_cmd_type   the type of ccc command
 * @param[in]  payload        the payload address of i3c ccc command
 * @param[in]  payload_len    the payload length of i3c ccc command
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_i3c_ccc_get(const I3cDevice *device, const I3cConfig *config,
                    uint8_t ccc_cmd_type, uint8_t *payload,
                    uint32_t payload_len);

/**
 * @brief I3c device address assignment
 *
 * @param[in]   device         the i3c device
 * @param[in]   config         the i3c config of device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_i3c_set_addr(const I3cDevice *device, const I3cConfig *config);

/**
 * @brief Deinitialises an I3C device
 * @param[in]  device  the i3c device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_i3c_finalize(const I3cDevice *device);

/**
 * @brief Interrupt handler for an External Interrupt
 * @param[in] device The i3c device
 */
void hal_i3c_irq_handler(const I3cDevice *device);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_I3C_H */
