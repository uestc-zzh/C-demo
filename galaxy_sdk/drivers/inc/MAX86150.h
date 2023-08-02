/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file MAX86150.h
 * @brief public head file of ppg&ecg combo sensor MAX86150
 */

#include <stdint.h>
#include <stdbool.h>
#include "hal_spi.h"
#include "hal_i2c.h"
#include "hal_gpio.h"
#include "device.h"
#include "hal_ppg_ecg_combo.h"

#define MAX86150_ADDRESS 0x5E

/**
 * @brief Init the device for MAX86150
 *
 * @param combo_device Combo device pointer
 * @param combo_config Combo device configuration
 * @return The error code
 * @retval 0 for succeed, others for failure
 */
int max_86150_device_init(PpgEcgComboDevice *combo_device,
                          const SensorHwConfig *combo_config);

/**
 * @brief Deinit the device for MAX86150
 *
 * @param combo_device Combo device pointer
 * @return The error code
 * @retval 0 for succeed, others for failure
 */
int max_86150_device_deinit(PpgEcgComboDevice *combo_device);
