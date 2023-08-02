/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_i2c.c
 * @brief HAL interface for I2C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hal_i2c.h"
#include "vsd_error.h"
#include "vsd_common.h"

static I2cDevice *g_i2c_dev[I2C_PORT_MAX] = { NULL };

static inline I2cOperations *get_ops(const I2cDevice *device)
{
    return (I2cOperations *)device->ops;
}

int hal_i2c_add_dev(I2cDevice *device)
{
    int ret   = VSD_ERR_FULL;
    uint8_t i = 0;

    if (!device)
        return VSD_ERR_INVALID_POINTER;

    for (i = 0; i < sizeof(g_i2c_dev) / sizeof(g_i2c_dev[0]); i++) {
        if (g_i2c_dev[i] == NULL) {
            g_i2c_dev[i] = device;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

int hal_i2c_remove_dev(I2cDevice *device)
{
    int ret   = VSD_ERR_NON_EXIST;
    uint8_t i = 0;

    if (!device)
        return VSD_ERR_INVALID_POINTER;

    for (i = 0; i < sizeof(g_i2c_dev) / sizeof(g_i2c_dev[0]); i++) {
        if (g_i2c_dev[i] == device) {
            g_i2c_dev[i] = NULL;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

I2cDevice *hal_i2c_get_device(uint8_t port_id)
{
    uint8_t i;

    for (i = 0; i < sizeof(g_i2c_dev) / sizeof(g_i2c_dev[0]); i++) {
        if (g_i2c_dev[i] && (g_i2c_dev[i]->port_id == port_id)) {
            return g_i2c_dev[i];
        }
    }
    return NULL;
}

int hal_i2c_init(const I2cDevice *device)
{
    int32_t ret = VSD_ERR_GENERIC;

    if (!device)
        return VSD_ERR_INVALID_POINTER;

    if (get_ops(device)->init)
        ret = get_ops(device)->init(device);

    return ret;
}

int hal_i2c_reg_write(const I2cDevice *device, const I2cConfig *config,
                      uint32_t reg_addr, const uint8_t *data, uint32_t size,
                      const XferInfo *xfer_info)
{
    int32_t ret = VSD_ERR_GENERIC;

    if (!device || !config)
        return VSD_ERR_INVALID_POINTER;

    if (get_ops(device)->master_reg_write)
        ret = get_ops(device)->master_reg_write(device, config, reg_addr, data,
                                                size, xfer_info);

    return ret;
}

int hal_i2c_reg_read(const I2cDevice *device, const I2cConfig *config,
                     uint32_t reg_addr, uint8_t *data, uint32_t size,
                     const XferInfo *xfer_info)
{
    int32_t ret = VSD_ERR_GENERIC;

    if (!device || !config)
        return VSD_ERR_INVALID_POINTER;

    if (get_ops(device)->master_reg_read)
        ret = get_ops(device)->master_reg_read(device, config, reg_addr, data,
                                               size, xfer_info);

    return ret;
}

int hal_i2c_stop(const I2cDevice *device)
{
    if (!device)
        return VSD_ERR_INVALID_POINTER;

    if (!get_ops(device)->stop)
        return VSD_ERR_UNSUPPORTED;

    return get_ops(device)->stop(device);
}

int hal_i2c_finalize(const I2cDevice *device)
{
    if (!device)
        return VSD_ERR_INVALID_POINTER;

    if (!get_ops(device)->deinit)
        return VSD_ERR_UNSUPPORTED;
    else
        get_ops(device)->deinit(device);

    return VSD_SUCCESS;
}

DRV_ISR_SECTION
void hal_i2c_irq_handler(const I2cDevice *device)
{
    if (!device || !get_ops(device)->irq_handler)
        return;

    return get_ops(device)->irq_handler(device);
}
