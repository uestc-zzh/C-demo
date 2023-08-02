/**
 * Copyright (C) 2023 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_i3c.c
 * @brief HAL interface for I3C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hal_i3c.h"
#include "vsd_error.h"
#include "vsd_common.h"

static I3cDevice *g_i3c_dev[I3C_PORT_MAX] = { NULL };

static inline I3cOperations *get_ops(const I3cDevice *device)
{
    return (I3cOperations *)device->ops;
}

int hal_i3c_add_dev(I3cDevice *device)
{
    int ret   = VSD_ERR_FULL;
    uint8_t i = 0;

    if (!device)
        return VSD_ERR_INVALID_PARAM;

    for (i = 0; i < sizeof(g_i3c_dev) / sizeof(g_i3c_dev[0]); i++) {
        if (g_i3c_dev[i] == NULL) {
            g_i3c_dev[i] = device;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

int hal_i3c_remove_dev(I3cDevice *device)
{
    int ret   = VSD_ERR_NON_EXIST;
    uint8_t i = 0;

    if (!device)
        return VSD_ERR_INVALID_PARAM;

    for (i = 0; i < sizeof(g_i3c_dev) / sizeof(g_i3c_dev[0]); i++) {
        if (g_i3c_dev[i] == device) {
            g_i3c_dev[i] = NULL;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

I3cDevice *hal_i3c_get_device(uint8_t port_id)
{
    uint8_t i;

    for (i = 0; i < sizeof(g_i3c_dev) / sizeof(g_i3c_dev[0]); i++) {
        if (g_i3c_dev[i]->port_id == port_id) {
            return g_i3c_dev[i];
        }
    }
    return NULL;
}

int hal_i3c_init(const I3cDevice *device)
{
    int32_t ret = VSD_ERR_GENERIC;

    if (!device)
        return VSD_ERR_INVALID_PARAM;

    if (get_ops(device)->init)
        ret = get_ops(device)->init(device);

    return ret;
}

int hal_i3c_reg_write(const I3cDevice *device, const I3cConfig *config,
                      uint32_t reg_addr, const uint8_t *data, uint32_t size)
{
    int32_t ret = VSD_ERR_GENERIC;

    if (!device || !config)
        return VSD_ERR_INVALID_PARAM;

    if (get_ops(device)->master_reg_write)
        ret = get_ops(device)->master_reg_write(device, config, reg_addr, data,
                                                size);

    return ret;
}

int hal_i3c_reg_read(const I3cDevice *device, const I3cConfig *config,
                     uint32_t reg_addr, uint8_t *data, uint32_t size)
{
    int32_t ret = VSD_ERR_GENERIC;

    if (!device || !config)
        return VSD_ERR_INVALID_PARAM;

    if (get_ops(device)->master_reg_read)
        ret = get_ops(device)->master_reg_read(device, config, reg_addr, data,
                                               size);

    return ret;
}

int hal_i3c_ccc_set(const I3cDevice *device, const I3cConfig *config,
                    uint8_t ccc_cmd_type, const uint8_t *payload,
                    uint32_t payload_len)
{
    int32_t ret = VSD_ERR_GENERIC;

    if (!device || !config)
        return VSD_ERR_INVALID_PARAM;

    if ((ccc_cmd_type & I3C_CCC_DIRECT) == 0)
        return VSD_ERR_INVALID_PARAM;

    if (get_ops(device)->master_reg_write)
        ret = get_ops(device)->ccc_set(device, config, ccc_cmd_type, payload,
                                       payload_len);

    return ret;
}

int hal_i3c_ccc_get(const I3cDevice *device, const I3cConfig *config,
                    uint8_t ccc_cmd_type, uint8_t *payload,
                    uint32_t payload_len)
{
    int32_t ret = VSD_ERR_GENERIC;

    if (!device || !config)
        return VSD_ERR_INVALID_PARAM;

    if ((ccc_cmd_type & I3C_CCC_DIRECT) == 0)
        return VSD_ERR_INVALID_PARAM;

    if (get_ops(device)->master_reg_read)
        ret = get_ops(device)->ccc_get(device, config, ccc_cmd_type, payload,
                                       payload_len);

    return ret;
}

int hal_i3c_set_addr(const I3cDevice *device, const I3cConfig *config)
{
    int32_t ret = VSD_ERR_GENERIC;

    if (!device || !config)
        return VSD_ERR_INVALID_PARAM;

    if (get_ops(device)->set_addr)
        ret = get_ops(device)->set_addr(device, config);

    return ret;
}

int hal_i3c_finalize(const I3cDevice *device)
{
    if (!device)
        return VSD_ERR_INVALID_PARAM;

    if (get_ops(device)->deinit)
        get_ops(device)->deinit(device);

    return VSD_SUCCESS;
}

DRV_ISR_SECTION
void hal_i3c_irq_handler(const I3cDevice *device)
{
    if (!device || !get_ops(device)->irq_handler)
        return;

    return get_ops(device)->irq_handler(device);
}
