/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_adc.c
 * @brief HAL for ADC
 */

#include <stdint.h>
#include <stdlib.h>
#include "vsd_common.h"
#include "hal_adc.h"
#include "vsd_error.h"
#include "osal_heap_api.h"

static AdcDevice *g_adc_dev[ADC_DEV_MAX] = { NULL };

static inline AdcOperations *get_ops(const AdcDevice *device)
{
    return (AdcOperations *)device->ops;
}

int hal_adc_add_dev(AdcDevice *device)
{
    int ret = VSD_ERR_FULL;
    uint8_t i;

    for (i = 0; i < sizeof(g_adc_dev) / sizeof(g_adc_dev[0]); i++) {
        if (g_adc_dev[i] == NULL) {
            g_adc_dev[i] = device;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

int hal_adc_remove_dev(AdcDevice *device)
{
    int ret = VSD_ERR_NON_EXIST;
    uint8_t i;

    for (i = 0; i < sizeof(g_adc_dev) / sizeof(g_adc_dev[0]); i++) {
        if (g_adc_dev[i] == device) {
            g_adc_dev[i] = NULL;
            ret          = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

AdcDevice *hal_adc_get_device(uint8_t id)
{
    uint8_t i;

    for (i = 0; i < sizeof(g_adc_dev) / sizeof(g_adc_dev[0]); i++) {
        if (g_adc_dev[i] && (g_adc_dev[i]->hw_config->id == id)) {
            return g_adc_dev[i];
        }
    }
    return NULL;
}

int hal_adc_power(const AdcDevice *device, bool pwr_ctrl)
{
    if (!device || !get_ops(device))
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->power_ctrl)
        return VSD_ERR_UNSUPPORTED;
    return get_ops(device)->power_ctrl(device, pwr_ctrl);
}

int hal_adc_calibrate(const AdcDevice *device)
{
    if (!device || !get_ops(device))
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->calibrate)
        return VSD_ERR_UNSUPPORTED;
    return get_ops(device)->calibrate(device);
}

int hal_adc_self_diagnosis(const AdcDevice *device, uint8_t vol_sel,
                           uint32_t *value)
{
    if (!device || !get_ops(device) || !value)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->self_diagnosis)
        return VSD_ERR_UNSUPPORTED;
    return get_ops(device)->self_diagnosis(device, vol_sel, value);
}

int hal_adc_config_sampling(const AdcDevice *device,
                            const AdcSamplingConfig *samp_cfg)
{
    if (!device || !get_ops(device) || !samp_cfg)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->config_sampling)
        return VSD_ERR_UNSUPPORTED;
    return get_ops(device)->config_sampling(device, samp_cfg);
}

int hal_adc_read(const AdcDevice *device, const AdcSamplingConfig *samp_cfg,
                 const AdcChannelsCfg *channels_cfg)
{
    if (!device || !get_ops(device) || !samp_cfg || !channels_cfg)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->read)
        return VSD_ERR_UNSUPPORTED;
    return get_ops(device)->read(device, samp_cfg, channels_cfg);
}

int hal_adc_start(const AdcDevice *device, const AdcSamplingConfig *samp_cfg,
                  const AdcChannelsCfg *channels_cfg)
{
    if (!device || !get_ops(device) || !samp_cfg || !channels_cfg)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->stop)
        return VSD_ERR_UNSUPPORTED;
    return get_ops(device)->start(device, samp_cfg, channels_cfg);
}

int hal_adc_stop(const AdcDevice *device, const AdcSamplingConfig *samp_cfg,
                 const AdcChannelsCfg *channels_cfg)
{
    if (!device || !get_ops(device) || !samp_cfg || !channels_cfg)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->stop)
        return VSD_ERR_UNSUPPORTED;
    return get_ops(device)->stop(device, samp_cfg, channels_cfg);
}

DRV_ISR_SECTION
void hal_adc_irq_handler(const AdcDevice *device)
{
    if (!device || !get_ops(device) || !get_ops(device)->irq_handler)
        return;
    return get_ops(device)->irq_handler(device);
}
