/**
 * Copyright (C) 2021-2023 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_time_api.h
 * @brief OSAL time APIs
 */

#ifndef __OSAL_TIME_H__
#define __OSAL_TIME_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "osal_adapter.h"

/** @addtogroup TIME
 *  OSAL Time API and definitions
 *  @ingroup OSAL
 *  @{
 */

/**
 * @brief Get the number of milliseconds since the operating system(or
 * scheduler) started
 *
 * @note Invocation is not recommended when the SysTimer interrupt is masked.
 * For example, after entering the critical section through
 * osal_enter_critical(). This causes the RTOS to stop tick counting, which
 * affects the accuracy of the results returned by the function.
 */
uint64_t osal_get_uptime(void);

/**
 * @brief Get the number of microseconds since the operating system(or
 * scheduler) started
 *
 * @note Invocation is not recommended when the SysTimer interrupt is masked.
 * For example, after entering the critical section through
 * osal_enter_critical(). This causes the RTOS to stop tick counting, which
 * affects the accuracy of the results returned by the function.
 */
uint64_t osal_get_uptime_us(void);

/**
 * @brief Setup the systick timer to generate the tick interrupts at the
 * required frequency
 */
void osal_setup_timer(void);

/**
 * @brief Update the systick timer configurations to ensure accurate timing when
 * the operating system's base clock changes
 */
void osal_update_systick(void);

/**
 * @brief Convert time in milliseconds to ticks
 *
 * @note A fraction less then one tick will be counted as one tick
 * @param duration A period of time in milliseconds
 * @return uint32_t Number of ticks
 */
uint32_t osal_ms_to_tick(uint32_t duration);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_TIME_H__ */
