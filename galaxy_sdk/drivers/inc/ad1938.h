/**
 * Header for Analog codec AD1938 IP driver
 *
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 *
 * @file ad1938.h
 * @brief header of codec AD1938 IP driver
 * @author Haiyong Liu<haiyong.liu@verisilicon.com>
 */

#ifndef __AD1938_H__
#define __AD1938_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup AD1938
 *  @brief AD1938 Driver
 *  @ingroup DRIVER
 *  @{
 */

#include <stdint.h>
#include "vsd_common.h"
#include "hal_codec.h"

#define AD1938_ADDR (0x04)

/**
 * @fn ad1938_device_init
 * @brief create the codec ad1938 instance
 *
 * @param config The hardware configuration for ad1938 instance
 * @return int int {@link VSD_SUCCESS} for success, otherwise is error
 */
int ad1938_device_init(const CodecHWConfig *config);

/**
 * @fn ad1938_device_release
 * @brief Remove the ad1938 device instance and release the resource
 *
 * @param codec the codec instance
 * @return int {@link VSD_SUCCESS} for success, otherwise is error
 */
int ad1938_device_release(CodecDevice *codec);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AD1938_H__ */
