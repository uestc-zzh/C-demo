/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_flash.c
 * @brief HAL for FLASH
 * @author Ma.Shaohui <Ma.Shaohui@verisilicon.com>
 */

#include <stddef.h>
#include "hal_flash.h"
#include "vsd_error.h"

static FlashDevice *g_flash_dev[FLASH_DEVICE_MAX];

static inline FlashOperations *get_ops(const FlashDevice *device)
{
    return (FlashOperations *)device->ops;
}

int hal_flash_add_dev(FlashDevice *device)
{
    uint32_t i;

    for (i = 0; i < FLASH_DEVICE_MAX; ++i) {
        if (g_flash_dev[i] == NULL) {
            g_flash_dev[i] = device;
            return VSD_SUCCESS;
        }
    }
    return VSD_ERR_FULL;
}

int hal_flash_remove_dev(FlashDevice *device)
{
    uint32_t i;

    for (i = 0; i < FLASH_DEVICE_MAX; ++i) {
        if (g_flash_dev[i] == device) {
            g_flash_dev[i] = NULL;
            return VSD_SUCCESS;
        }
    }
    return VSD_ERR_NON_EXIST;
}

const FlashDevice *hal_flash_get_device(uint8_t dev_id)
{
    uint32_t i;

    for (i = 0; i < FLASH_DEVICE_MAX; ++i) {
        if (g_flash_dev[i] && (g_flash_dev[i]->dev_id == dev_id)) {
            return g_flash_dev[i];
        }
    }
    return NULL;
}

int hal_flash_info_get(const FlashDevice *device, PartitionId in_partition,
                       FlashPartition **partition)
{
    uint32_t i;

    if (!device || !partition) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (in_partition > PARTITION_ID_MAX) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (!device->partitions) {
        return VSD_ERR_UNSUPPORTED;
    }

    for (i = 0; i < device->partition_num; ++i) {
        if (device->partitions[i].id == in_partition) {
            *partition = (FlashPartition *)&device->partitions[i];
            return VSD_SUCCESS;
        }
    }
    return VSD_ERR_NON_EXIST;
}

int hal_flash_init(FlashDevice *device)
{
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device)->init) {
        return VSD_ERR_UNSUPPORTED;
    }

    return get_ops(device)->init(device);
}

int hal_flash_init_partition(FlashDevice *device, const FlashPartInfo *info,
                             uint32_t items)
{
    uint32_t i;

    if (!device || !info) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (items == 0) {
        return VSD_ERR_INVALID_PARAM;
    }
    for (i = 0; i < items; ++i) {
        if (device->flash_info->devicesize == info[i].total_size) {
            device->partitions    = info[i].partinfo;
            device->partition_num = info[i].partnums;
            return VSD_SUCCESS;
        }
    }
    return VSD_ERR_NON_EXIST;
}

int hal_flash_partition_write(const FlashDevice *device, PartitionId pno,
                              uint32_t off, const uint8_t *buf, uint32_t size)
{
    int ret;
    FlashPartition *info;

    if (!buf) {
        return VSD_ERR_INVALID_POINTER;
    }
    ret = hal_flash_info_get(device, pno, &info);
    if (ret != VSD_SUCCESS) {
        return ret;
    }
    if ((off + size) > info->length) {
        return VSD_ERR_INVALID_PARAM;
    }
    if ((info->options & PAR_OPT_WRITE_EN) == 0) {
        return VSD_ERR_UNSUPPORTED; // the partition is read only
    }
    if (!get_ops(device)->program) {
        return VSD_ERR_UNSUPPORTED;
    }
    ret = get_ops(device)->program(device, info->start_addr + off, size, buf);
    return ret;
}

int hal_flash_write(const FlashDevice *device, uint32_t addr, uint32_t size,
                    const uint8_t *buf)
{
    if (!buf || !device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device)->program) {
        return VSD_ERR_UNSUPPORTED;
    }

    return get_ops(device)->program(device, addr, size, buf);
}

int hal_flash_partition_read(const FlashDevice *device, PartitionId pno,
                             uint32_t off, uint8_t *buf, uint32_t size)
{
    int ret;
    FlashPartition *info;

    if (!buf) {
        return VSD_ERR_INVALID_POINTER;
    }
    ret = hal_flash_info_get(device, pno, &info);
    if (ret != VSD_SUCCESS) {
        return ret;
    }
    if ((off + size) > info->length) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (!get_ops(device)->read) {
        return VSD_ERR_UNSUPPORTED;
    }
    ret = get_ops(device)->read(device, info->start_addr + off, size, buf);
    return ret;
}

int hal_flash_read(const FlashDevice *device, uint32_t addr, uint32_t size,
                   uint8_t *buf)
{
    if (!buf || !device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device)->read) {
        return VSD_ERR_UNSUPPORTED;
    }

    return get_ops(device)->read(device, addr, size, buf);
}

int hal_flash_partition_erase(const FlashDevice *device, PartitionId pno,
                              uint32_t off, uint32_t size)
{
    int ret;
    FlashPartition *info;

    ret = hal_flash_info_get(device, pno, &info);
    if (ret != VSD_SUCCESS) {
        return ret;
    }
    if ((info->options & PAR_OPT_WRITE_EN) == 0) {
        return VSD_ERR_UNSUPPORTED;
    }
    if ((size + off) > info->length) {
        return VSD_ERR_INVALID_PARAM;
    }
    if (!get_ops(device)->erase) {
        return VSD_ERR_UNSUPPORTED;
    }
    ret = get_ops(device)->erase(device, info->start_addr + off, size);
    return ret;
}

int hal_flash_erase(const FlashDevice *device, uint32_t addr, uint32_t size)
{
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device)->erase) {
        return VSD_ERR_UNSUPPORTED;
    }

    return get_ops(device)->erase(device, addr, size);
}

int hal_flash_power_mode(const FlashDevice *device, FlashPowerMode mode)
{
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device)->power_mode) {
        return VSD_ERR_UNSUPPORTED;
    }

    return get_ops(device)->power_mode(device, mode);
}

int hal_flash_get_size(const FlashDevice *device, NorFlashDev *nor)
{
    if (!device || !device->flash_info || !nor) {
        return VSD_ERR_INVALID_POINTER;
    }
    nor->chip = (const NorFlashInfo *)device->flash_info;
    return VSD_SUCCESS;
}

int hal_flash_get_factory_info(const FlashDevice *device, NorFactoryDev *nor)
{
    if (!device || !nor || !nor->chip) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device)->get_factory_info) {
        return VSD_ERR_UNSUPPORTED;
    }

    return get_ops(device)->get_factory_info(device, nor->chip->uid,
                                             nor->chip->csid);
}

int hal_flash_reset(const FlashDevice *device)
{
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device)->reset) {
        return VSD_ERR_UNSUPPORTED;
    }

    return get_ops(device)->reset(device);
}

int hal_flash_erase_chip(const FlashDevice *device)
{
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device)->erase_chip) {
        return VSD_ERR_UNSUPPORTED;
    }

    return get_ops(device)->erase_chip(device);
}
