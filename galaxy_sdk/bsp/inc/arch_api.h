/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file arch_api.h
 * @brief public head file of arch api
 * @author Shaobo Tu <Shaobo.Tu@verisilicon.com>
 */

#ifndef __ARCH_API_H__
#define __ARCH_API_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup ARCH
 *  ARCH API.
 *  @ingroup BSP
 *  @{
 */

/**
 * @enum CpuSleepMode
 * @brief Sleep mode of CPU
 */
typedef enum CpuSleepMode {
    CPU_SLEEP_GENERAL, /**< Default option, or CPU only has one sleep mode */
    CPU_SLEEP_SHALLOW, /**< Shallow sleep mode */
    CPU_SLEEP_DEEP, /**< Deep sleep mode */
} CpuSleepMode;

/**
 * @brief Cpu idle
 */
void cpu_idle(void);

/**
 * @brief Cpu sleep
 * @param sleep_mode @see CpuSleepMode
 */
void cpu_sleep(CpuSleepMode sleep_mode);

/**
 * @brief Cpu halt
 */
void cpu_halt(void);

/**
 * @brief set priority of interrupt
 */
void cpu_set_isr_priority(int irqn, int prio);

/**
 * @brief disable interrupts
 */
void cpu_disable_interrupts(void);

/**
 * @brief memory barrier
 */
void cpu_mem_barrier(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __ARCH_API_H__ */
