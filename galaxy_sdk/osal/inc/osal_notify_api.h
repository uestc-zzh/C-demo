/**
 * Copyright (C) 2020-2023 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_notify_api.h
 * @brief OSAL notify APIs
 */

#ifndef __OSAL_NOTIFY_H__
#define __OSAL_NOTIFY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include "vs_conf.h"
#include "osal_adapter.h"

/** @addtogroup NOTIFY
 *  OSAL Notify API and definitions
 *  @ingroup OSAL
 *  @{
 */

/**
 * @struct OsalNotifyWait
 * @brief Struct of parameters for notification waiting
 */
typedef struct OsalNotifyWait {
    unsigned long index_to_wait; /**< The index within the calling task's array
                                    of notification values on which the calling
                                    task will wait for a notification to be
                                    received */
    uint32_t bits_to_clr_on_in; /**< Bit mask to clear on entrying */
    uint32_t bits_to_clr_on_out; /**< Bit mask to clear before exiting */
    uint32_t notify_value; /**< Notification value to be passed out */
    uint32_t ticks_to_wait; /**< The maximum time to wait in the blocked state
                               for a notification to be received */
} OsalNotifyWait;

/**
 * @brief Send a notification to the specific task from task, potentially
 * removing the task from the blocked state, and update updating the task's
 * notification value
 *
 * @param notify Notification sending parameters @see OsalNotify
 * @return int OSAL_TRUE for success, others for failure
 */
int osal_task_notify(OsalNotify *notify);

/**
 * @brief Send a notification to the specific task from ISR
 *
 * @note osal_task_notify_from_isr is the interrupt safe version of
 * osal_task_notify
 * @param notify Notification sending parameters @see OsalNotify
 * @return int OSAL_TRUE for success, others for failure
 */
int osal_task_notify_from_isr(OsalNotify *notify);

/**
 * @brief Wait for the calling task to receive a notification with an optional
 * timeout
 *
 * @param notify_wait Notification waiting parameters @see OsalNotifyWait
 * @return int OSAL_TRUE for success, others for failure
 */
int osal_task_notify_wait(OsalNotifyWait *notify_wait);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_NOTIFY_H__ */
