/**
 * Copyright (C) 2021-2023 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file osal_sys_state_api.h
 * @brief OSAL system state APIs
 */

#ifndef __OSAL_SYS_STATE_H__
#define __OSAL_SYS_STATE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "vs_conf.h"
#include "osal_adapter.h"

/** @addtogroup STATE
 *  OSAL System State API and definitions
 *  @ingroup OSAL
 *  @{
 */

/* Macros used to indicate state of task */
#define TASK_RUNNING_CHAR ('X')
#define TASK_BLOCKED_CHAR ('B')
#define TASK_READY_CHAR ('R')
#define TASK_DELETED_CHAR ('D')
#define TASK_SUSPENDED_CHAR ('S')
/* Macros used to indicate indicate whether the task is alive */
#define TASK_ALIVE_CHAR ('Y')
#define TASK_DEAD_CHAR ('N')

#define EACH_TASK_INFO_SIZE 64

/**
 * @brief Dump current status of all tasks
 */
void osal_dump_os_state(void);

/**
 * @brief Dump current status of system heap
 */
void osal_dump_heap_size(void);

/**
 * @brief Get the total amount of heap space that remains unallocated
 *
 * @return uint32_t Size of the free heap
 */
uint32_t osal_get_free_heap(void);

/**
 * @brief Get current task name
 *
 * @return task name
 */
char *osal_get_task_name(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_SYS_STATE_H__ */
