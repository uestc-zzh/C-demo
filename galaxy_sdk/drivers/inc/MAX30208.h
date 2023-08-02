/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file MAX30208.h
 * @brief public head file of temperature sensor MAX30208
 */

#ifndef _DRV_MAX30208_H
#define _DRV_MAX30208_H

#include "hal_tmp.h"

/** @addtogroup MAX30208
 *  MAX30208 temperature sensor driver
 *  @ingroup DRIVER
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 *@brief Used to control MAX30208.
 *@param tmp_device temp device of MAX30208
 */
int max30208_device_init(struct TmpDevice *tmp_device);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _DRV_MAX30208_H */
