/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file gpd_drv.h
 * @brief Operations of various power management unit drivers
 */

#ifndef __GPD_DRV_H__
#define __GPD_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup DRV_POWER
 *  @brief POWER Driver
 *  @ingroup DRIVER
 *  @{
 */

#include <stdint.h>
#include "hal_gpd.h"

extern const GpdFuncs vs_gpd_bin_ops;
extern const GpdFuncs vs_gpd_pcu_ctrl_ops;
extern const GpdFuncs vs_gpd_ram_reten_ops;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __GPD_DRV_H__ */
