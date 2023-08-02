/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file TI_TMP117.h
 * @brief public head file of temperature sensor TI_TMP117
 */

#pragma once

#include <stdint.h>
#include "hal_tmp.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup TMP117
 *  TMP117 temperature sensor driver
 *  @ingroup DRIVER
 *  @{
 */

#define TMP117_ADDRESS_ID0 (0x48)
#define TMP117_ADDRESS_ID1 (0x49)
#define TMP117_ADDRESS_ID2 (0x4a)
#define TMP117_ADDRESS_ID3 (0x4b)

/**
 * @brief Init the device for TMP117
 * link the hal_tmp with TMP117
 *
 * @param tmp_device
 * @return The error code
 * @retval 0 for succeed, others for failure
 */
int tmp117_device_init(TmpDevice *tmp_device);

/** @} */

#ifdef __cplusplus
}
#endif
