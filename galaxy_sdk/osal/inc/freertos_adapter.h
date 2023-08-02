/**
 * Copyright (C) 2022-2023 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file freertos_adapter.h
 * @brief Definitions for compatibility with FreeRTOS
 */

#ifndef __FREERTOS_ADAPTER__
#define __FREERTOS_ADAPTER__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "portmacro.h"
#include "vs_conf.h"
#if CONFIG_RISCV_ARCH
#include "platform.h"
#endif
#if CONFIG_SOC_CACHE_SUPPORT
/** Non cacheable memory is required and is managed by FreeRTOS */
#include "heap_4_noncache.h"
#endif

/** @addtogroup TIME
 *  @ingroup OSAL
 *  @{
 */

/** The actual time a tick represents, in milliseconds */
#define OSAL_TICK_PERIOD_MS portTICK_PERIOD_MS

/** @} */

/** @addtogroup TASK
 *  @ingroup OSAL
 *  @{
 */

/** Call osal_enter_critical to enter critical sections */
#define osal_enter_critical() taskENTER_CRITICAL()
/** Call osal_exit_critical to exit critical sections */
#define osal_exit_critical() taskEXIT_CRITICAL()
/** The maximum priority available to the application tasks */
#define OSAL_TASK_PRI_HIGHEST (configMAX_PRIORITIES - 1)

/** @} */

/** @addtogroup NOTIFY
 *  @ingroup OSAL
 *  @{
 */

/**
 * @struct OsalNotify
 * @brief Struct of parameters for notification sending
 */
typedef struct OsalNotify {
    void *task_to_notify; /**< Handle of the task being notified */
    unsigned long index_to_notify; /**< The index within the target task's array
                                      of notification values to which the
                                      notification is to be sent */
    uint32_t notify_value; /**< Notification value */
    eNotifyAction action; /**< The way to update the task's notification value
                             @see eNotifyAction */
    uint32_t *pre_ntfy_val; /**< Previous notification value before modified */
    long *higher_task_woken; /**< Indicate whether sending the notification
                              * caused the task to which the notification was
                              * sent to leave the blocked state, and the
                              * unblocked task has a priority higher than the
                              * currently running task */
} OsalNotify;

/** @} */

/** @addtogroup SEMAPHORE
 *  @ingroup OSAL
 *  @{
 */

typedef struct OsalSemaphore {
    SemaphoreHandle_t semaphore;
} OsalSemaphore;

/** @} */

/** @addtogroup LOCK
 *  @ingroup OSAL
 *  @{
 */

typedef struct OsalMutex {
    SemaphoreHandle_t mutex;
} OsalMutex;

/** @} */

/** @addtogroup COMMON
 *  OSAL common definitions
 *  @ingroup OSAL
 *  @{
 */

/** Definition of true of OSAL */
#define OSAL_TRUE pdTRUE
/** Definition of false of OSAL */
#define OSAL_FALSE pdFALSE
/** Setting the wait time to OSAL_WAIT_FOREVER will cause the task to wait
 * indefinitely (without a timeout) */
#define OSAL_WAIT_FOREVER portMAX_DELAY

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __FREERTOS_ADAPTER__ */
