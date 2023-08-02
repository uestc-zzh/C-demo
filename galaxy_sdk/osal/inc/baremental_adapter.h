/**
 * Copyright (C) 2022-2023 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file baremental_adapter.h
 * @brief Definitions for compatibility with bare mental
 */

#ifndef __BAREMENTAL_ADAPTER__
#define __BAREMENTAL_ADAPTER__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Task */
#define osal_enter_critical()
#define osal_exit_critical()

/* Semaphore */
typedef struct OsalSemaphore {
    void *semaphore;
} OsalSemaphore;

/* Notification */
typedef struct OsalNotify {
    void *task_to_notify;
    unsigned long index_to_notify;
    uint32_t notify_value;
    uint32_t *pre_ntfy_val;
    long *higher_task_woken;
} OsalNotify;

/* Lock */
typedef struct OsalMutex {
    void *mutex;
} OsalMutex;

/* Common */
#define OSAL_TRUE 1
#define OSAL_FALSE 0
#define OSAL_WAIT_FOREVER (0xFFFFFFFF)

#ifdef __cplusplus
}
#endif

#endif /* __BAREMENTAL_ADAPTER__ */
