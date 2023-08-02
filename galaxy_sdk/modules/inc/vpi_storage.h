/**
 * Copyright (C) 2021. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file vpi_storage.h
 * @brief Interfaces to storage service.
 *
 * This header define the interfaces of SDK level storage service API
 */
#ifndef VPI_STORAGE_H
#define VPI_STORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/** @addtogroup STORAGE
 *  - Storage definition and API
 *  @ingroup VPI
 *  @{
 */

typedef enum {
    STORAGE_PWR_DYNAMIC, /**< Dynamic control the storage power by driver */
    STORAGE_PWR_SLEEP, /**< Storage keeps on deep sleep mode */
    STORAGE_PWR_ALWAYS_ON, /**< Storage keeps on always on mode */
    STORAGE_PWR_DEFAULT = STORAGE_PWR_ALWAYS_ON, /**< Default power mode */
    STORAGE_PWR_MAX
} VpiStgPwrMode;

typedef enum {
    STORAGE_TYPE_NOR, /**< Nor flash */
    STORAGE_TYPE_NAND, /**< Nand flash */
    STORAGE_TYPE_MAX
} VpiStgType;

/**
 * @brief Define storage attribute/info
 */
typedef struct VpiStgInfo {
    VpiStgType type; /**< storage type */
    uint32_t page_size; /**< storage page size in byte */
    uint32_t sector_size; /**< storage sector size in byte */
    uint32_t block_size; /**< storage block size in byte */
    uint32_t device_size; /**< storage device total size in byte */
} VpiStgInfo;

/**
 * @brief Get storage info
 * @return Pointer to storage info, NULL for invalid info
 */
const VpiStgInfo *vpi_storage_get_info(void);

/**
 * @brief Erase area in storage memory
 * @param part_no the flash logical partition which should be erased
 * @param off_set start address of the erased flash area
 * @param length erase length(for flash, must be sector aligned)
 * @return VPI_SUCCESS for success case, other for failed case.
 */
int vpi_storage_erase(int8_t part_no, uint32_t off_set, uint32_t length);

/**
 * @brief Write data in storage memory
 * @param part_no the flash logical partition which should be written
 * @param off_set start address of the write area(for flash, must be page
 * aligned)
 * @param size write size
 * @param data write data pointer
 * @param aligned false for not page aligned, true for page aligned
 * @return VPI_SUCCESS for success case, other for failed case.
 */
int vpi_storage_write(int8_t part_no, uint32_t off_set, uint32_t size,
                      uint8_t *data, bool aligned);

/**
 * @brief Read data in storage memory
 * @param part_no the flash logical partition which should be read
 * @param off_set start address of the write area(for flash, must be page
 * aligned)
 * @param size read size
 * @param data read data pointer
 * @param aligned false for not page aligned, true for page aligned
 * @return VPI_SUCCESS for success case, other for failed case.
 */
int vpi_storage_read(int8_t part_no, uint32_t off_set, uint32_t size,
                     uint8_t *data, bool aligned);

/**
 * @brief Init storage
 * @param mode Power mode for storage, @see VpiStgPwrMode
 * @return VPI_SUCCESS for success case, other for failed case.
 */
int vpi_storage_init(VpiStgPwrMode mode);

/**
 * @brief Get image partition number in the storage
 * @param version Pointer to current version
 * @param curr_part_no Pointer to current partition number
 * @param old_part_no Pointer to old partition number
 * @return VPI_SUCCESS for success case, other for failed case.
 */
int vpi_storage_get_imginfo(char *version, int8_t *curr_part_no,
                            int8_t *old_part_no);

/**
 * @brief Print current image version & partition number
 * @param version Pointer to current version
 * @param part_no Current partition number
 */
void vpi_storage_print_imginfo(char *version, int8_t part_no);

/**
 * @brief Verify received image
 * @details At this stage, image payload has been stored, the header page
 * remains in memory. This function check sync word of header page and sha256
 * of image payload. After verification passed, upgrade process should store
 * header page to flash to finish whole procedure.
 *
 * @param img_header - the imager header
 * @return VPI_SUCCESS for success case, other for failed case.
 */
int vpi_storage_verify_img(uint8_t *img_header);

/**
 * @brief Set power mode to storage
 * @param mode Power mode for storage, @see VpiStgPwrMode
 * @return VPI_SUCCESS for success case, other for failed case.
 */
int vpi_storage_power_mode(VpiStgPwrMode mode);

/**
 * @brief show information of firmware image
 *
 */
void vpi_storage_show_img_info(void);

/** @} */
#ifdef __cplusplus
}
#endif

#endif
