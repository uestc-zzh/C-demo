/**
 * Header for Analog codec WM8960 IP driver
 *
 * Copyright (C) 2023 VeriSilicon Holdings Co., Ltd.
 *
 * @file wm8960.h
 * @brief header of codec WM8960 IP driver
 * @author Haiyong Liu<haiyong.liu@verisilicon.com>
 */

#include <stdint.h>
#include "vsd_common.h"
#include "hal_codec.h"

#define WM8960_I2C_ADDR (0x1a)

/**
 * @fn wm8960_device_init
 * @brief create the codec wm8960 instance
 *
 * @param config The hardware configuration for wm8960 instance
 * @return int int {@link VSD_SUCCESS} for success, otherwise is error
 */
int wm8960_device_init(const CodecHWConfig *config);

/**
 * @fn wm8960_device_release
 * @brief Remove the wm8960 device instance and release the resource
 *
 * @param codec the codec instance
 * @return int {@link VSD_SUCCESS} for success, otherwise is error
 */
int wm8960_device_release(CodecDevice *codec);
