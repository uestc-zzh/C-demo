/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file bsp.h
 * @brief Header of BSP
 */

#ifndef _BSP_H_
#define _BSP_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 *  Board Software Package
 *  @{
 */

/**
 * @brief Initializes BSP; registers flash_map with the system.
 *
 */
void bsp_init(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
