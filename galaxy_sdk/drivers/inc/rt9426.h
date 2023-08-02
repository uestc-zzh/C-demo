/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file rt9426.h
 * @brief public head file of sensor RT9426
 * @author Jianyu Ke <Jianyu.Ke@verisilicon.com>
 */
#ifndef _RT9426_H_
#define _RT9426_H_

#pragma once

#include <stdint.h>
#include "vsd_common.h"
#include "hal_gpio.h"
#include "hal_sensor_common.h"
#include "hal_battery.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RT9426_ADDRESS 0x55

/**
 * @brief Init the device for RT9426
 *
 * @param bat_device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int rt9426_device_init(BatteryDev *bat_device);

/**
 * @brief Deinit the device for RT9426
 *
 * @param bat_device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int rt9426_device_deinit(BatteryDev *bat_device);

#ifdef __cplusplus
}
#endif

#endif /* _RT9426_H_ */
