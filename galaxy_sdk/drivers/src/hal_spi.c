/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_spi.c
 * @brief HAL for SPI driver
 * @author Jinghua Yu<jinghua.yu@verisilicon.com>
 */

#include <stddef.h>
#include <string.h>
#include "hal_spi.h"
#include "vsd_error.h"

static SpiDevice *g_spi_dev[HAL_SPI_DEV_MAX];

static inline SpiOperations *get_ops(const SpiDevice *device)
{
    return (SpiOperations *)device->ops;
}

int hal_spi_add_dev(SpiDevice *device)
{
    uint32_t i;

    for (i = 0; i < HAL_SPI_DEV_MAX; ++i) {
        if (g_spi_dev[i] == NULL) {
            g_spi_dev[i] = device;
            return VSD_SUCCESS;
        }
    }
    return VSD_ERR_FULL;
}

int hal_spi_remove_dev(SpiDevice *device)
{
    uint32_t i;

    for (i = 0; i < HAL_SPI_DEV_MAX; ++i) {
        if (g_spi_dev[i] == device) {
            g_spi_dev[i] = NULL;
            return VSD_SUCCESS;
        }
    }
    return VSD_ERR_NON_EXIST;
}

SpiDevice *hal_spi_get_dev_by_id(uint8_t bus_id)
{
    uint32_t i;

    for (i = 0; i < HAL_SPI_DEV_MAX; ++i) {
        if (g_spi_dev[i] && (g_spi_dev[i]->bus_id == bus_id)) {
            return g_spi_dev[i];
        }
    }
    return NULL;
}

int hal_spi_drv_init(const SpiDevice *device)
{
    if (!device)
        return VSD_ERR_INVALID_POINTER;

    if (!get_ops(device)->spi_drv_init)
        return VSD_ERR_UNSUPPORTED;
    return get_ops(device)->spi_drv_init(device);
}

int hal_spi_fill_msg(const SpiDevice *device, SpiMessage *spi_msg,
                     uint8_t *tx_data, uint32_t tx_size, uint8_t *rx_data,
                     uint32_t rx_size, const XferInfo *xfer_info)
{
    if (!tx_data && !rx_data)
        return VSD_ERR_INVALID_POINTER;

    if (((device->dmac_dev->hw_cfg->width_capability) & WIDTH_CAP_8_BIT) == 0) {
        if (rx_size > SPI_DMA_BUFF_SIZE) {
            // TODO: unsupported now
            return VSD_ERR_UNSUPPORTED;
        }
    }

    memset(spi_msg, 0, sizeof(SpiMessage));
    spi_msg->tx_buf         = tx_data;
    spi_msg->tx_buf_len     = tx_size;
    spi_msg->rx_buf         = rx_data;
    spi_msg->rx_buf_len     = rx_size;
    spi_msg->spec_xfer_mode = XFER_MODE_DEFAULT;
    spi_msg->is_async       = xfer_info->is_async;
    // the spi_msg->rx_cont will be updated for necessary after fill_msg
    spi_msg->callback = xfer_info->callback;
    return VSD_SUCCESS;
}

int hal_spi_transfer(const SpiDevice *device, SpiMessage *spi_msg,
                     const SpiTransConfig *trans_config)
{
    if (!device)
        return VSD_ERR_INVALID_POINTER;

    if (!spi_msg->tx_buf && !spi_msg->rx_buf)
        return VSD_ERR_INVALID_PARAM;

    if (!get_ops(device)->transfer_one_msg)
        return VSD_ERR_UNSUPPORTED;

    return get_ops(device)->transfer_one_msg(device, spi_msg, trans_config);
}

int hal_spiflash_transfer(const SpiDevice *device, SpiMessage *spi_msg,
                          const SpiTransConfig *config, SpiFlashTransInfo *info)
{
    if (!device || !spi_msg || !config || !info)
        return VSD_ERR_INVALID_POINTER;

    if (!get_ops(device)->spi_flash_transfer)
        return VSD_ERR_UNSUPPORTED;

    return get_ops(device)->spi_flash_transfer(device, spi_msg, config, info);
}

int hal_spi_stop(const SpiDevice *device)
{
    if (!device)
        return VSD_ERR_INVALID_POINTER;

    if (!get_ops(device)->stop)
        return VSD_ERR_UNSUPPORTED;

    return get_ops(device)->stop(device);
}

DRV_ISR_SECTION
void hal_spi_irq_handler(const SpiDevice *device)
{
    if (!device)
        return;

    if (!get_ops(device)->irq_ins_handler)
        return;

    return get_ops(device)->irq_ins_handler(device);
}
