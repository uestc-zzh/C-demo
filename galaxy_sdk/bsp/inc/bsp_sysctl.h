/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file bsp_sysctl.h
 * @brief public head file of bsp sysctl
 * @author Shaobo Tu <Shaobo.Tu@verisilicon.com>
 */

#ifndef _BSP_SYS_CTL_H
#define _BSP_SYS_CTL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/** @addtogroup PEGASUS
 *  @brief SoC sysctl API for Pegasus
 *  @ingroup SOC
 *  @{
 */

/**
 * @brief Init BSP
 */
void bsp_init(void);

/**
 * @brief Software stop ZSP
 * @return VSD_SUCCESS on success, others on error
 */
int bsp_sw_stop_zsp(void);

/**
 * @brief Software load ZSP
 * @return VSD_SUCCESS on success, others on error
 */
int bsp_sw_load_zsp(void);

/**
 * @brief Software start ZSP
 * @return VSD_SUCCESS on success, others on error
 */
int bsp_sw_start_zsp(void);

/**
 * @brief Get zsp boot status
 * @return The status of zsp boot
 */
int bsp_get_zsp_boot_status(void);

/**
 * @brief Enable IRQ Interrupts
 */
void bsp_enable_irq(void);

/**
 * @brief Disable IRQ Interrupts
 */
void bsp_disable_irq(void);

/**
 * @brief Delay us
 * @param us Delay time.
 */
void delay_us(uint32_t us);

/**
 * @brief Delay ms
 * @param ms Delay time.
 */
void delay_ms(uint32_t ms);

/**
 * @brief Get CPU clock frequency
 * @return The clock frequency of CPU
 */
uint32_t bsp_cpu_clock_get_freq(void);

/**
 * @brief Get RTC clock frequency
 * @return The clock frequency of RTC
 */
uint32_t bsp_rtc_clock_get_freq(void);

/**
 * @brief Get AHB clock frequency
 * @return The clock frequency of AHB
 */
uint32_t bsp_ahb_clock_get_freq(void);

/**
 * @brief Get APB clock frequency
 * @return The clock frequency of APB
 */
uint32_t bsp_apb_clock_get_freq(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _BSP_SYS_CTL_H */
