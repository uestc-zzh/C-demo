/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_flash.h
 * @brief public head file of Flash HAL
 * @author Ma.Shaohui <Ma.Shaohui@verisilicon.com>
 */

#ifndef __HAL_FLASH_H
#define __HAL_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup FLASH
 *  Flash hal API
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#include <stdint.h>
#include "hal_spi.h"

/**< Max supported SPI flash device number */
#define FLASH_DEVICE_MAX (3u)

#define NOR_MAX_ID_LEN 2

/**
 * @enum FlashRole
 * @brief Flash role definition, the system supports multiple flash devices
 */
typedef enum {
    FLASH_MAIN = 0, /**< the major flash which stores code and data */
    FLASH_EXT, /**< the extended flash which stores data */
    FLASH_EXT2, /**< the extended flash which used for spi test */
} FlashRole;

/* Define for options set */
#define PAR_OPT_READ_POS (0) /**< Read attribute bit in options */
#define PAR_OPT_WRITE_POS (1) /**< write attribute bit in options */

#define PAR_OPT_READ_MASK \
    (0x1u << PAR_OPT_READ_POS) /**< Read attribute mask bit */
#define PAR_OPT_WRITE_MASK \
    (0x1u << PAR_OPT_WRITE_POS) /**< Write attribute mask bit */

#define PAR_OPT_READ_EN (0x1u << PAR_OPT_READ_POS) /**< Read enable bit set */
#define PAR_OPT_WRITE_EN \
    (0x1u << PAR_OPT_WRITE_POS) /**< write enable bit set */

/* hal flash partition define */
typedef enum {
    PARTITION_ID_INVALID = -1,
    PARTITION_ID_FACTORY, /**< manufactory info & settings */
    PARTITION_ID_PLATFORM, /**< platform configuration & status */
    PARTITION_ID_BOOTLOADER, /**< bootloader */
    PARTITION_ID_FIRMWARE, /**< single firmware image */
    PARTITION_ID_IMAGE_A, /**< image A for dual image system */
    PARTITION_ID_IMAGE_B, /**< image B for dual image system */
    PARTITION_ID_BLE_SETTINGS, /**< BLE settings */
    PARTITION_ID_DATA, /**< user data */
    PARTITION_ID_DATA_EXT, /**< extended user data */
    PARTITION_ID_PRIVATE, /**< private region of user */
    PARTITION_ID_MAX,
} PartitionId;

/**
 * @brief Define for flash types
 */
typedef enum {
    FLASH_TYPE_NOR, /**< Nor flash */
    FLASH_TYPE_NAND, /**< Nand flash */
    FLASH_TYPE_MAX,
} FlashType;

typedef enum {
    FLASH_PWR_DYNAMIC, /**< Dynamic control the flash power in driver */
    FLASH_PWR_SLEEP, /**< Flash keeps on deep sleep mode */
    FLASH_PWR_ON, /**< Flash keeps on normal working mode */
    FLASH_PWR_DEFAULT = FLASH_PWR_ON, /**< Default power mode */
    FLASH_PWR_MAX,
} FlashPowerMode;

/* Hal flash partition manage struct */
typedef struct FlashPartition {
    PartitionId id; /**< Partition ID, definition @see PartitionId */
    const char *description; /**< Partition description */
    uint32_t start_addr; /**< Start address of partition */
    uint32_t length; /**< Length of partition in bytes */
    uint32_t options; /**< Read/write enable or disable */
} FlashPartition;

typedef struct FlashPartInfo {
    uint32_t total_size;
    const FlashPartition *partinfo;
    uint32_t partnums;
} FlashPartInfo;

/**
 * @brief Structure of Nor flash info
 */
typedef struct NorFlashInfo {
    const char *name; /**< nor flash name */
    uint8_t id[NOR_MAX_ID_LEN]; /**< chip id, get it by reading databook */
    uint8_t id_len; /**< the len of chip id is generally equal to 3 */
    uint8_t type; /**< @see FlashType */
    uint8_t addr_width; /**< address width */
    uint32_t pagesize; /**< page size in byte */
    uint32_t sectorsize; /**< sector size in byte */
    uint32_t blocksize; /**< block size in byte */
    uint32_t devicesize; /**< device size in byte */
} NorFlashInfo;

typedef struct FlashDevice {
    uint8_t dev_id; /**< Device ID */
    SpiDevice *qspi_dev; /**< Instance of QSPI/SPI device */
    const void *ops; /**< Flash operations */
    const FlashPartition *partitions; /**< Pointer of flash partitions */
    uint32_t partition_num; /**< Flash partition numbers */
    const NorFlashInfo *flash_info; /**< Flash info table */
    void *ctx; /**< Private context */
} FlashDevice;

/**
 * @brief Structure of operations for HAL of FLASH
 */
typedef struct FlashOperations {
    int (*init)(FlashDevice *device);
    int (*program)(const FlashDevice *device, uint32_t adr, uint32_t size,
                   const uint8_t *buf);
    int (*read)(const FlashDevice *device, uint32_t adr, uint32_t size,
                uint8_t *buf);
    int (*erase)(const FlashDevice *device, uint32_t adr, uint32_t size);
    int (*erase_chip)(const FlashDevice *device);
    int (*power_mode)(const FlashDevice *device, uint8_t mode);
    int (*get_factory_info)(const FlashDevice *device, uint8_t *uid,
                            uint8_t *csid);
    int (*reset)(const FlashDevice *device);
} FlashOperations;

typedef struct NorFlashDev {
    const NorFlashInfo *chip;
} NorFlashDev;

typedef struct NorFactoryInfo {
    uint8_t uid[16]; /**< get norflash uid by read */
    uint8_t csid[2]; /**< get norflash csid by read */
} NorFactoryInfo;

typedef struct NorFactoryDev {
    NorFactoryInfo *chip;
} NorFactoryDev;

/**
 * @brief add an instance to flash device table
 * @param device instance of flash to add
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_add_dev(FlashDevice *device);

/**
 * @brief remove an instance from flash device table
 * @param device instance of flash to remove
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_remove_dev(FlashDevice *device);

/**
 * @brief get an instance from flash device table according to ID
 * @param dev_id ID of flash device to get
 * @return NULL for failure, others for success
 */
const FlashDevice *hal_flash_get_device(uint8_t dev_id);

/**
 * @brief get flash partition information
 * @param device instance of flash
 * @param in_partition partition ID
 * @param partition partition information
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_info_get(const FlashDevice *device, PartitionId in_partition,
                       FlashPartition **partition);

/**
 * @brief initialize flash
 * @param device instance of flash
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_init(FlashDevice *device);

/**
 * @brief add flash partition information to flash device
 * @param device instance of flash
 * @param info flash partition table
 * @param items intems of flash partition table
 * @note it will select and register one partition from partition table
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_init_partition(FlashDevice *device, const FlashPartInfo *info,
                             uint32_t items);
/**
 * @brief write data to a partition
 * @param device instance of flash
 * @param pno partition ID
 * @param off offset to start address of partition
 * @param buf write data buffer
 * @param size data size to write
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_partition_write(const FlashDevice *device, PartitionId pno,
                              uint32_t off, const uint8_t *buf, uint32_t size);

/**
 * @brief write data to absolute address of flash
 * @param device instance of flash
 * @param addr absolute address to write data
 * @param size data size to write
 * @param buf write data buffer
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_write(const FlashDevice *device, uint32_t addr, uint32_t size,
                    const uint8_t *buf);

/**
 * @brief read data from a partition
 * @param device instance of flash
 * @param pno partition ID
 * @param off offset to start address of partition
 * @param buf read data buffer
 * @param size data size to read
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_partition_read(const FlashDevice *device, PartitionId pno,
                             uint32_t off, uint8_t *buf, uint32_t size);

/**
 * @brief read data from absolute address of flash
 * @param device instance of flash
 * @param addr absolute address to read data
 * @param size data size to read
 * @param buf read data buffer
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_read(const FlashDevice *device, uint32_t addr, uint32_t size,
                   uint8_t *buf);

/**
 * @brief erase one region of partition
 * @param device instance of flash
 * @param pno partition ID
 * @param off offset to start address of partition
 * @param size region size to erase
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_partition_erase(const FlashDevice *device, PartitionId pno,
                              uint32_t off, uint32_t size);

/**
 * @brief erase one absolute region of flash
 * @param device instance of flash
 * @param addr absolute address to erase
 * @param size region size to erase
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_erase(const FlashDevice *device, uint32_t addr, uint32_t size);

/**
 * @brief switch power mode of flash
 * @param device instance of flash
 * @param mode power mode to switch
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_power_mode(const FlashDevice *device, FlashPowerMode mode);

/**
 * @brief get norflash hardware information
 * @param device instance of flash
 * @param nor norflash hardware information
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_get_size(const FlashDevice *device, NorFlashDev *nor);

/**
 * @brief get factory information of norflash
 * @param device instance of flash
 * @param nor norflash factory information
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_get_factory_info(const FlashDevice *device, NorFactoryDev *nor);

/**
 * @brief reset flash
 * @param device instance of flash
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_reset(const FlashDevice *device);

/**
 * @brief erase all flash regions
 * @param device instance of flash
 * @return VSD_SUCCESS for success, others for failure
 */
int hal_flash_erase_chip(const FlashDevice *device);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __HAL_FLASH_H */
