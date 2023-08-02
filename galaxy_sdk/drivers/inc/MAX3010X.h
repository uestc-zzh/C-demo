/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file MAX3010X.h
 * @brief public head file of ppg sensor MAX3010X
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "hal_ppg.h"

#define MAX3010X_ADDRESS 0x57

/**
 * @brief Init the device for MAX3010X
 *
 * @param ppg_device PPG device pointer
 * @param ppg_config PPG device configuration
 * @return The error code
 * @retval 0 for succeed, others for failure
 */
int max_3010x_device_init(PpgDevice *ppg_device,
                          const SensorHwConfig *ppg_config);

/**
 * @brief Deinit the device for MAX3010X
 *
 * @param ppg_device PPG device pointer
 * @return The error code
 * @retval 0 for succeed, others for failure
 */
int max_3010x_device_deinit(PpgDevice *ppg_device);
