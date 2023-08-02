/**
 * Copyright (C) 2020-2023 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_task_api.h
 * @brief OSAL task APIs
 */

#ifndef __OSAL_TASK_H__
#define __OSAL_TASK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include "vs_conf.h"
#include "osal_adapter.h"

/** @addtogroup TASK
 *  OSAL Task API and definitions
 *  @ingroup OSAL
 *  @{
 */

/**
 * @brief Create a task
 *
 * @param func A pointer to the function that implements the task
 * @param name A descriptive name for the task
 * @param stack_size The number of words the stack can hold, not the number of
 * bytes
 * @param task_priority The priority at which the task will execute
 * @param param A parameter for task function
 * @return void* Task handle for success, NULL for failure
 */
void *osal_create_task(void *func, char *name, uint32_t stack_size,
                       uint32_t task_priority, void *param);

/**
 * @brief Delete the specific task
 *
 * @param task Task handle
 */
void osal_delete_task(void *task);

/**
 * @brief Suspend the scheduler
 *
 * @note Suspending the scheduler prevents a context switch from occurring, but
 * leaves interrupts enabled
 * @note For FreeRTOS, API functions must not be called while the scheduler is
 * suspended
 */
void osal_suspend_all(void);

/**
 * @brief Resume the scheduler
 */
void osal_resume_all(void);

/**
 * @brief Place the calling task into the sleep(aka blocked) state until the
 * sleep period has expired
 *
 * @param ms Sleep period, in miniseconds
 */
void osal_sleep(int32_t ms);

/**
 * @brief Start the scheduler
 */
void osal_start_scheduler(void);

/**
 * @brief End the scheduler
 */
void osal_end_scheduler(void);

/**
 * @brief Get the state of scheduler
 *
 * @return true Started
 * @return false Stopped
 */
bool osal_started(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_TASK_H__ */
