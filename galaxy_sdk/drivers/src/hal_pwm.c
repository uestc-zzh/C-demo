/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_pwm.c
 * @brief HAL interface for PWM driver
 */

#include "stddef.h"
#include "hal_pwm.h"
#include "vsd_error.h"
#include "sys_common.h"

static PwmDevice *g_pwm_dev[HAL_PWM_DEV_MAX] = { NULL };

static inline PwmOperation *get_ops(const PwmDevice *device)
{
    return (PwmOperation *)device->ops;
}

int hal_pwm_add_dev(PwmDevice *device)
{
    int ret   = VSD_ERR_FULL;
    uint8_t i = 0;

    for (i = 0; i < sizeof(g_pwm_dev) / sizeof(g_pwm_dev[0]); i++) {
        if (g_pwm_dev[i] == NULL) {
            g_pwm_dev[i] = device;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

int hal_pwm_remove_dev(PwmDevice *device)
{
    int ret   = VSD_ERR_NON_EXIST;
    uint8_t i = 0;

    for (i = 0; i < sizeof(g_pwm_dev) / sizeof(g_pwm_dev[0]); i++) {
        if (g_pwm_dev[i] == device) {
            g_pwm_dev[i] = NULL;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

PwmDevice *hal_pwm_get_device(uint8_t device_id)
{
    uint8_t i;

    for (i = 0; i < sizeof(g_pwm_dev) / sizeof(g_pwm_dev[0]); i++) {
        if (g_pwm_dev[i] && (g_pwm_dev[i]->device_id == device_id)) {
            return g_pwm_dev[i];
        }
    }
    return NULL;
}

int hal_pwm_init(const PwmDevice *device, const PwmSettings *param)
{
    if (!device || !device->hw_config || !device->ops ||
        !get_ops(device)->init || !param) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (param->channel >= device->hw_config->ch_num) {
        return VSD_ERR_INVALID_CHANNEL;
    }
    return get_ops(device)->init(device, param);
}

int hal_pwm_start(const PwmDevice *device, const PwmSettings *param)
{
    if (!device || !device->hw_config || !device->ops ||
        !get_ops(device)->start || !param) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (param->channel >= device->hw_config->ch_num) {
        return VSD_ERR_INVALID_CHANNEL;
    }
    return get_ops(device)->start(device, param);
}

int hal_pwm_stop(const PwmDevice *device, uint8_t channel_id)
{
    if (!device || !device->hw_config || !device->ops ||
        !get_ops(device)->stop) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (channel_id >= device->hw_config->ch_num) {
        return VSD_ERR_INVALID_CHANNEL;
    }
    return get_ops(device)->stop(device, channel_id);
}

DRV_ISR_SECTION
void hal_pwm_irq_handler(const PwmDevice *device)
{
    if (!device || !device->hw_config || !device->ops ||
        !get_ops(device)->irq_handler)
        return;
    return get_ops(device)->irq_handler(device);
}
