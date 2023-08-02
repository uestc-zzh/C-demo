/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_vdt.c
 * @brief HAL for vdt driver
 */
#include <stdlib.h>
#include "hal_vdt.h"
#include "vsd_error.h"
#include "sys_common.h"

VdtDevice *g_vdt_device[VDT_MAX_DEVICE] = { NULL };

static inline VdtOperations *get_ops(uint8_t hw_id)
{
    return (VdtOperations *)g_vdt_device[hw_id]->ops;
}

int hal_vdt_add_device(VdtDevice *device)
{
    uint32_t i;
    for (i = 0; i < VDT_MAX_DEVICE; i++) {
        if (g_vdt_device[i] == NULL) {
            g_vdt_device[i] = device;
            return VSD_SUCCESS;
        }
    }
    return VSD_ERR_FULL;
}

int hal_vdt_remove_device(VdtDevice *device)
{
    uint32_t i;
    for (i = 0; i < VDT_MAX_DEVICE; i++) {
        if (g_vdt_device[i] == device) {
            g_vdt_device[i] = NULL;
            break;
        }
    }
    return VSD_SUCCESS;
}

VdtDevice *hal_vdt_get_device(uint8_t hw_id)
{
    uint32_t i;
    for (i = 0; i < VDT_MAX_DEVICE; i++) {
        if (g_vdt_device[i]->hw_id == hw_id) {
            return g_vdt_device[i];
        }
    }
    return NULL;
}

int hal_vdt_trigger_work_mode(VdtDevice *device)
{
    if (!get_ops(device->hw_id)->trigger_work_mode) {
        return VSD_ERR_GENERIC;
    }
    return get_ops(device->hw_id)->trigger_work_mode(device);
}

int hal_vdt_set_mode(VdtDevice *device, uint8_t mode, uint8_t param)
{
    if (!get_ops(device->hw_id)->set_mode) {
        return VSD_ERR_GENERIC;
    }
    return get_ops(device->hw_id)->set_mode(device, mode, param);
}

int hal_vdt_set_callback(VdtDevice *device, VdtCallback hook,
                         void *callback_arg)
{
    if (!get_ops(device->hw_id)->set_callback) {
        return VSD_ERR_GENERIC;
    }
    return get_ops(device->hw_id)->set_callback(device, hook, callback_arg);
}

int hal_vdt_power_ctrl(VdtDevice *device, uint8_t channel, uint8_t enable)
{
    if (!get_ops(device->hw_id)->power_ctrl) {
        return VSD_ERR_GENERIC;
    }
    return get_ops(device->hw_id)->power_ctrl(device, channel, enable);
}

DRV_ISR_SECTION
void hal_vdt_irq_handler(VdtDevice *device)
{
    if (!get_ops(device->hw_id)->irq_handler) {
        return;
    }
    return get_ops(device->hw_id)->irq_handler(device);
}
