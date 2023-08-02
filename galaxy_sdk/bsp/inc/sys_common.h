/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file sys_common.h
 * @brief Common definition for system common
 */

#ifndef __SYS_COMMON_H__
#define __SYS_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup PEGASUS
 *  @brief System common definition for Pegasus
 *  @ingroup SOC
 *  @{
 */

/** application functions section */
#define APP_SECTION __attribute__((section(".stext")))
/** factory functions section */
#define FACTORY_SECTION __attribute__((section(".stext")))
/** noncritical functions section */
#define NONCRITICAL_SECTION __attribute__((section(".stext")))
/** critical functions section */
#define CRITICAL_SECTION __attribute__((section(".itext")))
/** driver ISR functions section */
#define DRV_ISR_SECTION __attribute__((section(".itext")))
/** driver critical functions section */
#define DRV_CRITICAL_SECTION __attribute__((section(".itext")))

/** noncritical data section */
#define NONCRITICAL_DATA __attribute__((section(".sdram_data")))
/** sensor data section */
#define SENSOR_DATA_SECTION __attribute__((section(".sensor_data")))
/** noncache data section */
#define NONCACHE_DATA_SECTION __attribute__((section(".noncache_data")))

/** 2KB Bootloader reserved data section, can be reused for data */
#define RSV_DATA_SECTION __attribute__((section(".rsv_data")))

#define NON_XIP_TEXT __attribute__((section(".nonxip_text")))
#define NON_XIP_DATA __attribute__((section(".sdram_data")))

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __SYS_COMMON_H__ */
