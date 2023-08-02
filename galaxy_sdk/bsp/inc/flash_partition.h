/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file flash_partition.h
 * @brief Head file for flash partitions
 */
#ifndef _FLASH_PARTITION_H_
#define _FLASH_PARTITION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_flash.h"
#include "vs_conf.h"

/* Logic partition on flash devices */
static const FlashPartition flash_partitions_2MB[] = {
    {
        .id          = PARTITION_ID_BOOTLOADER,
        .description = "Bootloader",
        .start_addr  = 0x00000000,
        .length      = 0x00010000, /* 64K bytes */
#if CONFIG_BUILD_BOOTLOADER
        .options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
#else
        .options = PAR_OPT_READ_EN,
#endif
    },
    {
        .id          = PARTITION_ID_FIRMWARE,
        .description = "image",
        .start_addr  = 0x00010000,
        .length      = 0x00100000, /* 1024K bytes */
        .options     = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    {
        .id          = PARTITION_ID_BLE_SETTINGS,
        .description = "BLE Bond",
        .start_addr  = 0x00110000,
        .length      = 0x00001000, /* 4K bytes */
        .options     = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    {
        .id          = PARTITION_ID_PLATFORM,
        .description = "Vendor parameter",
        .start_addr  = 0x00111000,
        .length      = 0x00001000, /* 4K bytes */
        .options     = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    {
        .id          = PARTITION_ID_FACTORY,
        .description = "VSI Configuration",
        .start_addr  = 0x00112000,
        .length      = 0x00001000, /* 4K bytes */
        .options     = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    {
        .id          = PARTITION_ID_PRIVATE,
        .description = "User private region for OTA",
        .start_addr  = 0x00113000,
        .length      = 0x00080000, /* 512K bytes */
        .options     = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    {
        .id          = PARTITION_ID_DATA,
        .description = "User Data",
        .start_addr  = 0x00193000,
        .length      = 0x0006D000, /* 436K bytes */
        .options     = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
};

static const FlashPartition flash_partitions_4MB[] = {
    {
        .id          = PARTITION_ID_BOOTLOADER,
        .description = "Bootloader",
        .start_addr  = 0x00000000,
        .length      = 0x00010000, /* 64K bytes */
#if CONFIG_BUILD_BOOTLOADER
        .options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
#else
        .options = PAR_OPT_READ_EN,
#endif
    },
    {
        .id          = PARTITION_ID_FIRMWARE,
        .description = "image",
        .start_addr  = 0x00010000,
        .length      = 0x00100000, /* 1024K bytes */
        .options     = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    {
        .id          = PARTITION_ID_BLE_SETTINGS,
        .description = "BLE Bond",
        .start_addr  = 0x00110000,
        .length      = 0x00001000, /* 4K bytes */
        .options     = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    {
        .id          = PARTITION_ID_PLATFORM,
        .description = "Vendor parameter",
        .start_addr  = 0x00111000,
        .length      = 0x00001000, /* 4K bytes */
        .options     = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    {
        .id          = PARTITION_ID_FACTORY,
        .description = "VSI Configuration",
        .start_addr  = 0x00112000,
        .length      = 0x00001000, /* 4K bytes */
        .options     = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    {
        .id          = PARTITION_ID_PRIVATE,
        .description = "User private region for OTA",
        .start_addr  = 0x00113000,
        .length      = 0x00080000, /* 512K bytes */
        .options     = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    {
        .id          = PARTITION_ID_DATA,
        .description = "User Data",
        .start_addr  = 0x00193000,
        .length      = 0x0026D000, /* 2484K bytes */
        .options     = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
};

static const FlashPartInfo main_flash_part_table[] = {
    {
        .total_size = 2 * 1024 * 1024,
        .partinfo   = flash_partitions_2MB,
        .partnums =
            sizeof(flash_partitions_2MB) / sizeof(flash_partitions_2MB[0]),
    },
    {
        .total_size = 4 * 1024 * 1024,
        .partinfo   = flash_partitions_4MB,
        .partnums =
            sizeof(flash_partitions_4MB) / sizeof(flash_partitions_4MB[0]),
    },
};

/* Logic partition on external flash device */
static const FlashPartition ext_partitions[] = {
    {
        .id          = PARTITION_ID_DATA,
        .description = "Ext User Data",
        .start_addr  = 0x00000000,
        .length      = 0x00200000, /* 2048K bytes */
        .options     = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    {
        .id          = PARTITION_ID_DATA_EXT,
        .description = "U-DISK Area",
        .start_addr  = 0x00200000,
        .length      = 0x00200000, /* 2048K bytes */
        .options     = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
};

static const FlashPartInfo ext_flash_part_table[] = {
    {
        .total_size = 4 * 1024 * 1024,
        .partinfo   = ext_partitions,
        .partnums   = sizeof(ext_partitions) / sizeof(ext_partitions[0]),
    },
};

#ifdef __cplusplus
}
#endif

#endif // _FLASH_PARTITION_H_
