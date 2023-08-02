/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file device.h
 * @brief Definition for device info
 */
#ifndef _DEVICE_H_
#define _DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "hal_gpio.h"

/** @addtogroup DEVICE
 *  - Device definition
 *  @ingroup BOARD
 *  @{
 */

/**
 * @brief Device ID definitions for board
 */
typedef enum DeviceID {
    PWR_KEY_DEV_ID = 0, /**< Power key */
    PPG_SENSOR_ID, /**< PPG sensor */
    ECG_SENSOR_ID, /**< ECG sensor */
    TMP_SENSOR_ID, /**< Temperature sensor */
    IMU_SENSOR_ID, /**< IMU sensor */
    OLED_ID, /**< OLED display */
    PPG_ECG_SENSOR_ID, /**< PPG & ECG combo sensor */
    CHARGER_ID, /**< Charger */
    BATTERY_ID, /**< Battery sensor */
    EXT_FLASH_ID, /**< External flash */
    EEPROM_I2C_ID, /**< EEPROM with I2C bus connection */
    EEPROM_I3C_ID, /**< EEPROM with I3C bus connection */
    LED_PWM_DEV_ID, /**< PWM for LED */
    LED_GPIO_DEV0_ID, /**< GPIO for LED0 */
    LED_GPIO_DEV1_ID, /**< GPIO for LED1 */
    MAX_DEVICE_ID,
} DeviceID;

/** @} */

#ifdef __cplusplus
}
#endif
#endif // _DEVICE_H_
