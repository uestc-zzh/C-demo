/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_gpd.c
 * @brief Generic power domain HAL
 */

#include "hal_gpd.h"
#include "vsd_error.h"
#include "gpd_drv.h"

int hal_gpd_init(GpdHw *gpd_hw, const GpdHwCfg *config)
{
    if (!gpd_hw || !config || config->type >= GPD_TYPE_COUNT)
        return VSD_ERR_INVALID_PARAM;
    switch (config->type) {
    case GPD_TYPE_BINARY:
        gpd_hw->funcs = &vs_gpd_bin_ops;
        break;
    case GPD_TYPE_PCU_CTRL:
        gpd_hw->funcs = &vs_gpd_pcu_ctrl_ops;
        break;
    case GPD_TYPE_RAM_RETEN:
        gpd_hw->funcs = &vs_gpd_ram_reten_ops;
        break;
    default:
        return VSD_ERR_INVALID_PARAM;
    }
    gpd_hw->config = config;
    if (gpd_hw->funcs->init)
        return gpd_hw->funcs->init(config);
    return VSD_SUCCESS;
}

int hal_gpd_set_state(const GpdHw *gpd_hw, uint8_t state)
{
    if (!gpd_hw)
        return VSD_ERR_INVALID_POINTER;
    if (gpd_hw->funcs->set_state) {
        return gpd_hw->funcs->set_state(gpd_hw->config, state);
    }
    return VSD_ERR_UNSUPPORTED;
}

int hal_gpd_pd_enable(const GpdHw *gpd_hw, bool enable)
{
    if (!gpd_hw)
        return VSD_ERR_INVALID_POINTER;
    if (gpd_hw->funcs->pd_enable) {
        return gpd_hw->funcs->pd_enable(gpd_hw->config, enable);
    }
    return VSD_ERR_UNSUPPORTED;
}

uint8_t hal_gpd_get_parent(const GpdHw *gpd_hw)
{
    if (!gpd_hw)
        return GPD_ID_INVALID;
    if (gpd_hw->funcs->get_parent)
        return gpd_hw->funcs->get_parent(gpd_hw->config);
    return GPD_ID_INVALID;
}

int hal_gpd_get_state(const GpdHw *gpd_hw, uint8_t *state)
{
    if (!gpd_hw)
        return VSD_ERR_INVALID_POINTER;
    if (gpd_hw->funcs->get_state)
        return gpd_hw->funcs->get_state(gpd_hw->config, state);
    return VSD_ERR_UNSUPPORTED;
}
