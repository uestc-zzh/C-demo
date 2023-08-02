/*-
 * Copyright (C) 2020. VeriSilicon Holdings Co., Ltd. All rights reserved.
 * Author: chun.xie@verisilicon.com, SPSD
 * Date: 2020-12-1
 * Brief: Application Timer
 */
/* Standard includes. */
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include "vs_conf.h"
#include "vs_list.h"
#include "osal_lock_api.h"
#ifndef __VS_LIST_POOL_H__
#define __VS_LIST_POOL_H__
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup LIST
 *  List poll API.
 *  @ingroup VPI
 *  @{
 */

/**
 * @brief Structure of buffer pool which manage buffer by list
 */
typedef struct ListPool {
    struct list_head pool_head; /*list head*/
    uint32_t length; /*list length*/
    uint32_t item_size; /*buffer size*/
    OsalMutex lock; /*lock of list*/
} ListPool;

/**
 * @brief - Create new pool, pool buffer is managed by list
 *
 * @param length how many buffer in this pool
 * @param item_size buffer size
 * @return pool pointer or NULL
 */
ListPool *vpi_pool_init(uint32_t length, uint32_t item_size);

/**
 * @brief - Put buffer back to pool
 *
 * @param pool buffer pool
 * @param buffer buffer back to pool
 * @return 0 when fail, buffer number when success
 */
int vpi_pool_put_buf_back(ListPool *pool, uint8_t *buffer);

/**
 * @brief - Get buffer from pool
 *
 * @param pool buffer pool
 * @return NULL when fail, buffer when success
 */
uint8_t *vpi_pool_get_buf(ListPool *pool);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
