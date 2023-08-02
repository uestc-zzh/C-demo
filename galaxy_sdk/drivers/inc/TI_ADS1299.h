/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file TI_ADS1299.h
 * @brief Head file for TI_ADS1299
 * @author Jianyu Ke <Jianyu.Ke@verisilicon.com>
 */
#ifndef TI_ADS1299_H
#define TI_ADS1299_H

#include "hal_gpio.h"
#include "hal_spi.h"
#include "hal_ecg.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Init the device for TI_ADS1299
 *
 * @param ecg_device Ecg device pointer
 * @param ecg_sensor_config Ecg device configuration
 * @return The error code
 * @retval 0 for succeed, others for failure
 */
int ti_ads1299_device_init(EcgDevice *ecg_device,
                           const SensorHwConfig *ecg_sensor_config);

/**
 * @brief Deinit the device for TI_ADS1299
 *
 * @param ecg_device Ecg device pointer
 * @return The error code
 * @retval 0 for succeed, others for failure
 */
int ti_ads1299_device_deinit(EcgDevice *ecg_device);

#ifdef __cplusplus
}
#endif

#endif /*TI_ADS1299_H*/
