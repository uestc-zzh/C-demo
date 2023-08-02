/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file soc_init.h
 * @brief Header of SoC init
 */
#ifndef _SOC_INIT_H_
#define _SOC_INIT_H_

/** @addtogroup SOC
 *  @brief SoC init APIs
 *  @ingroup BSP
 *  @{
 */

/**
 * @brief SoC init function, should be called before SoC accessing
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int soc_init(void);

/** @} */

#endif // _SOC_INIT_H_
