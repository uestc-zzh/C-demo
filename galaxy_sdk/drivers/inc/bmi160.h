/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file bmi160.h
 * @brief public head file of imu sensor BMI160
 */

/**
 * @addtogroup BMI160
 * @brief BMI160 Driver
 * @ingroup DRIVER
 * @{
 */

#ifndef BMI160_H_
#define BMI160_H_

/*************************** C++ guard macro *****************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "hal_gpio.h"
#include "hal_spi.h"
#include "hal_imu.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

/**
 * @brief init the device for bmi160/bmx160
 *
 * @param imu_device handle of imu device
 * @param imu_config imu sensor configuration
 * @return 0 for succeed, others for failure
 */
int bmi160_device_init(ImuDevice *imu_device, const ImuConfig *imu_config);

/**
 * @brief deinit the device for bmi160/bmx160
 *
 * @param imu_device handle of imu device
 * @return 0 for succeed, others for failure
 */
int bmi160_device_deinit(ImuDevice *imu_device);

/*************************** C++ guard macro *****************************/
#ifdef __cplusplus
}
#endif

#endif /* BMI160_H_ */
/** @}*/
