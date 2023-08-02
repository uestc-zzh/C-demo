/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file eeprom.h
 * @brief public head file of EEPROM driver
 */

#ifndef _DRV_EEPROM_H_
#define _DRV_EEPROM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_eeprom.h"

/** @addtogroup DRV_EEPROM
 *  EEPROM API
 *  @ingroup DRIVER
 *  @{
 */

/**
 * @brief EEPROM device init API
 *
 * @param device The EEPROM device
 * @return Return VSD_SUCCESS for success, others for failure
 */
int eeprom_device_init(EepromDevice *device);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _DRV_EEPROM_H_ */
