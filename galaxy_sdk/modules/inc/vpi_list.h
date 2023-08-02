/**
 * Copyright (C) 2021. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file vpi_list.h
 * @brief public header for list define
 */

#ifndef __VPI_LIST_H__
#define __VPI_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup LIST
 *  - LIST API.
 *  @ingroup VPI
 *  @{
 */

struct list_head {
    struct list_head *next, *pre;
};

/** @} */

#ifdef __cplusplus
}
#endif

#endif
