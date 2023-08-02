/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file vs_spiflash.h
 * @brief public header of spi-norflash
 */

#ifndef __DRV_VS_SPIFLASH_H
#define __DRV_VS_SPIFLASH_H

#include "hal_flash.h"

/** @addtogroup VS_FLASH
 *  VS spiflash Definition.
 *  @ingroup DRIVER
 *  @{
 */

/**
 * @brief VeriSilicon QSPI flash device init API
 * @param info The flash info table, NULL for internal supported flash,
 * User can set info to NULL for GD25VQ32C, GD25LQ32E, GD25WQ32E & GD25Q16E
 * @param qspi_dev The QSPI device connecting with flash
 * @param trans_cfg The SPI transfer configuration
 * @param dev_id The give device ID for this flash device
 * @return Return VSD_SUCCESS for success, others for failure
 */
int vs_spiflash_device_init(const NorFlashInfo *info, SpiDevice *qspi_dev,
                            const SpiTransConfig *trans_cfg, uint8_t dev_id);

/**
 * @brief VeriSilicon flash device deinit API
 * @param device The flash device which will be deinited
 */
int vs_spiflash_device_deinit(FlashDevice *device);

/** @} */

#endif // __DRV_VS_SPIFLASH_H
