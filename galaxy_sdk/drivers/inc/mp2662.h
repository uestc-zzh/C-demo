/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file mp2662.h
 * @brief header of battery charger MP2662
 * @author Xie Mengdong <Mengdong.Xie@verisilicon.com>
 */

#ifndef __DRV_MP2662_H
#define __DRV_MP2662_H

#include "hal_charger.h"

/** @addtogroup MP2662
 *  @brief Charger MP2662 Driver
 *  @ingroup DRIVER
 *  @{
 */

/**
 * @brief device initialization of MP2662
 * @param[in] device charger device of MP2662
 * @return execution of this function, VSD_SUCCESS for success, others for
 * failure.
 */
int mp2662_device_init(ChargerDevice *device);

/** @} */

#endif /* __DRV_MP2662_H */
