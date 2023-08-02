/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_dac.c
 * @brief HAL for DAC
 */

#include "hal_dac.h"

static DacDevice *g_dac_dev[DAC_PORT_MAX] = { NULL };

static inline DacOperations *get_ops(const DacDevice *device)
{
    return (DacOperations *)device->ops;
}

int hal_dac_add_dev(DacDevice *device)
{
    int ret = VSD_ERR_FULL;
    uint8_t i;

    for (i = 0; i < sizeof(g_dac_dev) / sizeof(g_dac_dev[0]); i++) {
        if (g_dac_dev[i] == NULL) {
            g_dac_dev[i] = device;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

int hal_dac_remove_dev(DacDevice *device)
{
    int ret = VSD_ERR_NON_EXIST;
    uint8_t i;

    for (i = 0; i < sizeof(g_dac_dev) / sizeof(g_dac_dev[0]); i++) {
        if (g_dac_dev[i] == device) {
            g_dac_dev[i] = NULL;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

DacDevice *hal_dac_get_device(uint8_t id)
{
    DacDevice *dac = NULL;
    uint8_t i;

    for (i = 0; i < sizeof(g_dac_dev) / sizeof(g_dac_dev[0]); i++) {
        if (g_dac_dev[i] && (g_dac_dev[i]->hw_config->id == id)) {
            dac = g_dac_dev[i];
            break;
        }
    }
    return dac;
}

int hal_dac_init(const DacDevice *device, DacConfig dac_config)
{
    if (!get_ops(device))
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->init)
        return VSD_ERR_UNSUPPORTED;
    return get_ops(device)->init(device, dac_config);
}

int hal_dac_enable(const DacDevice *device, bool enable, DacCompleteCallback cb)
{
    if (!get_ops(device))
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->enable)
        return VSD_ERR_UNSUPPORTED;
    return get_ops(device)->enable(device, enable, cb);
}

int hal_dac_write(const DacDevice *device, DacInput *dac_input)
{
    if (!get_ops(device) || !dac_input->p_buffer)
        return VSD_ERR_INVALID_POINTER;
    if (!dac_input->length)
        return VSD_ERR_INVALID_PARAM;
    if (!get_ops(device)->write)
        return VSD_ERR_UNSUPPORTED;
    return get_ops(device)->write(device, dac_input);
}

DRV_ISR_SECTION
void hal_dac_irq_handler(const DacDevice *device)
{
    if (!get_ops(device) || !get_ops(device)->irq_handler)
        return;
    return get_ops(device)->irq_handler(device);
}
