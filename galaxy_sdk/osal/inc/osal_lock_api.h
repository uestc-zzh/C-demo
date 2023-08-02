/**
 * Copyright (C) 2020-2023 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_lock_api.h
 * @brief OSAL lock APIs
 */

#ifndef __OSAL_LOCK_H__
#define __OSAL_LOCK_H__

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

/** @addtogroup LOCK
 *  OSAL Lock API and definitions
 *  @ingroup OSAL
 *  @{
 */

/**
 * @brief Initialize a mutex
 *
 * @param mu The mutex to be initialized
 * @return int OSAL_TRUE for success, others for failure
 */
int osal_init_mutex(OsalMutex *mu);

/**
 * @brief Delete a mutex
 *
 * @note Do not delete a mutex that has tasks blocked on it (tasks that are
 * in the blocked state waiting for the mutex to become available)
 * @param mu The mutex being deleted
 * @return int OSAL_TRUE for success, others for failure
 */
int osal_delete_mutex(OsalMutex *mu);

/**
 * @brief Unlock or release a mutex
 *
 * @param mu The mutex to be unlocked
 * @return int OSAL_TRUE for success, others for failure
 */
int osal_unlock_mutex(OsalMutex *mu);

/**
 * @brief Lock or take a mutex
 *
 * @param mu The mutex to be locked
 * @param timeout The maximum amount of time the task should remain in the
 * blocked state to wait for the mutex if it is not already available, in
 * millisecond.
 * 1) If timeout is zero, then osal_lock_mutex will return immediately if the
 * mutex is not available.
 * 2) Setting timeout to OSAL_WAIT_FOREVER will cause the task to wait
 * indefinitely (without a timeout)
 * @return int OSAL_TRUE for success, others for failure
 */
int osal_lock_mutex(OsalMutex *mu, uint32_t timeout);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_LOCK_H__ */
