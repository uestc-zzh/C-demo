/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file pmu_drv.h
 * @brief Header of common definitions for various power management unit(PMU)
 * drivers
 */

#ifndef __PMU_DRV_H__
#define __PMU_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup DRV_POWER
 *  @brief POWER Driver
 *  @ingroup DRIVER
 *  @{
 */

#include <stdint.h>
#include <stdbool.h>
#include "vs_conf.h"
#include "hal_pmu.h"

#if CONFIG_SOC_VMP
extern const PmuFuncs vs_ble_sysctrl_ops;
#else
extern const PmuFuncs vs_pmu_ops;
#endif

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __PMU_DRV_H__ */
