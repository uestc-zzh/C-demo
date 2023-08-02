/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_pmu.c
 * @brief Power management uint HAL
 */

#include "vsd_error.h"
#include "hal_pmu.h"
#include "sys_common.h"

int hal_pmu_init(const PmuHw *pmu)
{
    if (!pmu || !pmu->config || !pmu->funcs ||
        (pmu->config->type >= PMU_TYPE_COUNT))
        return VSD_ERR_INVALID_PARAM;

    if (pmu->funcs->init)
        return pmu->funcs->init(pmu->config);
    return VSD_SUCCESS;
}

int hal_pmu_en_wakeup_src(const PmuHw *pmu, WakeUpSourceInfo wake_info,
                          bool enable, PmuIrqHandler handler)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->wakeup_src_en || !pmu->config)
        return VSD_ERR_INVALID_POINTER;
    if (wake_info.wakeup_src >= WAKEUP_SOURCE_COUNT)
        return VSD_ERR_INVALID_PARAM;
    return pmu->funcs->wakeup_src_en(pmu->config, wake_info, enable, handler);
}

int hal_pmu_enter_deep_sleep(const PmuHw *pmu, uint32_t sleep_period,
                             bool en_fast_boot)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->enter_deep_sleep || !pmu->config)
        return VSD_ERR_INVALID_POINTER;
    return pmu->funcs->enter_deep_sleep(pmu->config, sleep_period,
                                        en_fast_boot);
}

int hal_pmu_exit_deep_sleep(const PmuHw *pmu, uint8_t *const wakeup_src)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->exit_deep_sleep || !pmu->config)
        return VSD_ERR_INVALID_POINTER;
    return pmu->funcs->exit_deep_sleep(pmu->config, wakeup_src);
}

uint32_t hal_pmu_latency_ms(const PmuHw *pmu)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->latency || !pmu->config)
        return 0;
    return pmu->funcs->latency(pmu->config);
}

int hal_pmu_enter_low_power(const PmuHw *pmu)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->enter_low_power || !pmu->config)
        return VSD_ERR_INVALID_POINTER;
    return pmu->funcs->enter_low_power(pmu->config);
}

int hal_pmu_exit_low_power(const PmuHw *pmu, uint8_t *const wakeup_src)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->exit_low_power || !pmu->config)
        return VSD_ERR_INVALID_POINTER;
    return pmu->funcs->exit_low_power(pmu->config, wakeup_src);
}

int hal_pmu_enter_sleep(const PmuHw *pmu)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->enter_sleep || !pmu->config)
        return VSD_ERR_INVALID_POINTER;
    return pmu->funcs->enter_sleep(pmu->config);
}

int hal_pmu_exit_sleep(const PmuHw *pmu, uint8_t *const wakeup_src)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->exit_sleep || !pmu->config)
        return VSD_ERR_INVALID_POINTER;
    return pmu->funcs->exit_sleep(pmu->config, wakeup_src);
}

int hal_pmu_enter_snooze(const PmuHw *pmu)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->enter_snooze || !pmu->config)
        return VSD_ERR_INVALID_POINTER;
    return pmu->funcs->enter_snooze(pmu->config);
}

int hal_pmu_exit_snooze(const PmuHw *pmu, uint8_t *const wakeup_src)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->exit_snooze || !pmu->config)
        return VSD_ERR_INVALID_POINTER;
    return pmu->funcs->exit_snooze(pmu->config, wakeup_src);
}

int hal_pmu_lp_state_switch_cfg(const PmuHw *pmu, uint8_t state_switch_item,
                                uint64_t cfg_mask)
{
    if (!pmu || !pmu->funcs || !pmu->funcs->lp_state_switch_cfg ||
        !pmu->config) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (state_switch_item >= LPSS_ITEM_COUNT) {
        return VSD_ERR_INVALID_PARAM;
    }
    return pmu->funcs->lp_state_switch_cfg(pmu->config, state_switch_item,
                                           cfg_mask);
}

int hal_pmu_ds_init(const PmuHw *pmu, WakeUpSourceInfo wake_info)
{
    if (!pmu || !pmu->funcs || !pmu->config)
        return VSD_ERR_INVALID_PARAM;
    if (wake_info.wakeup_src >= WAKEUP_SOURCE_COUNT)
        return VSD_ERR_INVALID_PARAM;
    if (pmu->funcs->ds_init) {
        return pmu->funcs->ds_init(pmu->config, wake_info);
    }
    /* Returns success by default when ds init is not required */
    return VSD_SUCCESS;
}

DRV_ISR_SECTION
void hal_pmu_ds_irq_handler(const PmuHw *pmu, int irq_id)
{
    if (!pmu || !pmu->funcs || !pmu->config)
        return;
    if (pmu->funcs->ds_irq_handler) {
        return pmu->funcs->ds_irq_handler(pmu->config, irq_id);
    }
}
