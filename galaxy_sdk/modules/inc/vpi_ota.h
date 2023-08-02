/**
 * Copyright (C) 2021. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file vpi_ota.h
 * @brief Interfaces to Ota Manager.
 *
 * This header define the interfaces of SDK level bluetooth API
 */
#ifndef VPI_OTA_H
#define VPI_OTA_H

#ifdef __cplusplus
extern "C" {
#endif
#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Define OTA stage
 */
typedef enum {
    OTA_STAGE_IDLE, /**< Ota ready */
    OTA_STAGE_READY, /**< Ota ready */
    OTA_STAGE_TRANSMIT, /**< Load image file */
    OTA_STAGE_CERTIFY, /**< Integrity check and identification verification of
                          firmware */
    OTA_STAGE_UPDATE, /**< Write image to flash */
    OTA_STAGE_REBOOT, /**< Upgrade done, reboot chip */
    OTA_STAGE_CANCEL, /**< Upgrade cancel */
    OTA_STAGE_INVALID /**< End of valid value */
} VpiOtaStage;

/**
 * @brief Define error code for OTA.
 */
typedef enum {
    OTA_OK = 0, /**< No error */
    OTA_ERROR_TIMEOUT, /**< Timeout in receiving data */
    OTA_ERROR_TRANS, /**< Error in transmitting data */
    OTA_ERROR_CRC, /**< CRC error in checking image integrity */
    OTA_ERROR_CERT, /**< Certification not pass */
    OTA_ERROR_FLASH, /**< Error in updating flash or in reading back verify */
    OTA_ERROR_UNKNOWN /**< Other errors */
} VpiOtaError;

/**
 * @brief Define OTA status.
 */
typedef struct {
    VpiOtaStage stage; /**< Stage */
    VpiOtaError error; /**< Error status */
} VpiOtaStatus;

/**
 * @brief Define OTA generic configuration.
 */
typedef struct {
    uint8_t force_update; /**< Whether check version.
                           0: Only newer version.
                           1: Update regardless of version */
    uint8_t need_reboot; /**< Whether reboot automatically.
                          0: App call to reboot.
                          1: OTA manager reboot after updating flash*/
    uint32_t cert_type; /**< Certification type. TBD */
} VpiOtaConfig;

/**
 * @brief Source of OTA data
 * @details
 */
typedef enum {
    OTA_SOURCE_BT_RFCOMM, /**< Through VSI protocol by BT RFCOMM */
    OTA_SOURCE_BLE_SERVICE, /**< Through VSI protocol by BLE OTA service */
    OTA_SOURCE_UART, /**< Through VSI protocol by UART */
    OTA_SOURCE_USB, /**< Through USB */
    OTA_SOURCE_PRIVATE /**< Customer private protocol */
} VpiOtaSource;

/**
 * @brief Define OTA source configuration.
 * @todo
 */
typedef union {
    struct {
        uint32_t reserved; /**< Structure TDB */
    } bt; /**< BT config */
    struct {
        uint32_t reserved; /**< Structure TDB */
        uint32_t timeout_threshold;
        uint32_t max_data_length;
        uint32_t image_size;
    } ble; /**< BLE config */
    struct {
        uint32_t reserved; /**< Structure TDB */
        uint32_t image_size;
    } uart; /**< Uart config */
} VpiOtaSourceConfig;

/**
 * @brief Notification will callback with the OTA status update.
 * @param stage - See VpiOtaStage
 */
typedef void (*ota_stage_notify_t)(VpiOtaStage stage);

/**
 * @brief Notification for OTA error happening.
 * @param error - See VpiOtaError
 */
typedef void (*ota_error_notify_t)(VpiOtaError error);

/**
 * @brief Implement customer private transmitter.
 * @details
 *     OTA data will be transferred in private protocol. OTA manager waits
 *     for return of this function then move to next stage. Implementer should
 *     check CANCEL state with vpi_ota_get_status() periodically in case a
 *     cancel request comes in, and stop transmission when it happenes.
 * @return VpiOtaError. OTA_OK means data collection finishes successfully.
 */
typedef VpiOtaError (*ota_transmitter_private_t)(void);

/**
 * @brief Implement customer private certification protocol.
 * @details
 *     OTA data will be certified in private protocol. OTA manager waits
 *     for return of this function then move to next stage. Implementer should
 *     check CANCEL status with vpi_ota_get_status() periodically in case a
 *     cancel request comes in, and stop certification when it happenes.
 * @return VpiOtaError. OTA_OK means data certification finishes successfully.
 */
typedef VpiOtaError (*ota_cert_private_t)(void);

/**
 * @brief Callbacks of customer private protocol.
 */
typedef struct {
    ota_transmitter_private_t transmitter; /**< Private transmitter. Leave it to
                                              NONE if you don't have one */
    ota_cert_private_t certify; /**< Private certify. Leave it to NONE if you
                                   don't have one */
} VpiOtaPrivCB;

/**
 * @brief Get Init OTA Manager.
 * @param config - See VpiOtaConfig.
 * @return VPI_SUCCESS for success case. other for failed case.
 */
int vpi_ota_init(VpiOtaConfig *config);

/**
 * @brief Get OTA status.
 * @param status - Output of OTA status.
 * @return VPI_SUCCESS for success case. other for failed case.
 */
int vpi_ota_get_status(VpiOtaStatus *status);

/**
 * @brief Config the source of OTA data
 * @details
 *     - Ota data can be retrieved from one of VpiOtaSource.
 *          App should config one before start OTA.
 *     - If source is set to OTA_SOURCE_PRIVATE, OTA manager
 *          will not handle data collection. App need to get image
 *          data, then call vpi_ota_flash_firmware_private() to update.
 *
 * @param source - Type of OTA source
 * @param config - Configuration of OTA source pipeline.
 * @return VPI_SUCCESS for success case. other for failed case.
 */
int vpi_ota_config_source(VpiOtaSource source, VpiOtaSourceConfig *config);

/**
 * @brief Register callbacks of customer private protocol.
 * @param cb - Callback set
 * @return VPI_SUCCESS for success case. other for failed case.
 */
int vpi_ota_register_private(VpiOtaPrivCB cb);

/**
 * @brief Start transfer one image package
 * @return VPI_SUCCESS for success case. other for failed case.
 */
int vpi_ota_pkg_transfer(void);

/**
 * @brief Finish transfer one package of image
 * @param is_finish true for update finish, false for update continue
 * @return VPI_SUCCESS for success case. other for failed case.
 */
int vpi_ota_pkg_done(bool is_finish);

/**
 * @brief Verify image
 * @return VPI_SUCCESS for success case. other for failed case.
 */
int vpi_ota_certify(void);

/**
 * @brief Stop any process and reset OTA stage to "OTA_STAGE_IDLE".
 * @return VPI_SUCCESS for success case. other for failed case.
 */
int vpi_ota_reset(void);

/**
 * @brief Cancel OTA upgrade process and move OTA stage to "OTA_STAGE_READY".
 * @return VPI_SUCCESS for success case. other for failed case.
 */
int vpi_ota_cancel(void);

/**
 * @brief Switch boot firmware.
 * @details System should support A/B (Seamless) update to call this function.
 * @return VPI_SUCCESS for success case. other for failed case.
 */
int vpi_ota_switch_boot(void);

/**
 * @brief Reboot system to active firmware.
 * @return 0 for success case. other for failed case.
 */
int vpi_ota_reboot_system(void);

/**
 * @brief Get size of image.
 * @return Image size.
 */
int vpi_ota_get_image_size(void);

/**
 * @brief Request OTA Manager to copy image data to specified address.
 * @param buf - Destiny address.
 * @param len - Image data length.
 * @return 0 for success case. other for failed case.
 */
int vpi_ota_get_image_data(uint8_t *buf, size_t len);

/**
 * @brief Start to update firmware to flash.
 * @param data - Image data.
 * @param dest_addr - Destiny address in flash.
 * @param len - Image data size.
 * @return 0 for success case. other for failed case.
 */
int vpi_ota_update_firmware_private(const uint8_t *data, uint8_t *dest_addr,
                                    size_t len);

/**
 * @brief Send ota action request
 * @param action - Ota action, see OtaActionId.
 * @param offset - Packet offset
 * @param len - Packet size.
 * @param pdata - Packet data.
 * @return 0 for success case. other for failed case.
 */
int vpi_send_ota_action_req(uint8_t action, uint32_t offset, uint32_t len,
                            uint8_t *pdata);

/**
 * @brief Handle ota action request
 * @param data - Input data.
 * @param action - Ota action, see OtaActionId.
 * @param offset - Packet offset.
 * @param len - Packet size.
 * @param pdata - Packet data.
 */
void vpi_handle_ota_action_req(void *data, uint8_t *action, uint32_t *offset,
                               uint32_t *len, uint8_t **pdata);

/**
 * @brief set information of OTA stage2 boot
 * @param info_addr address of information in flash
 * @param ota_start start address of OTA stage2 image
 * @param ota_end end address of OTA stage2 image
 * @param otaed ota image exists or not
 * @return VSD_SUCCESS for success, others for failure
 */
int vpi_ota_set_stage2_info(uint32_t info_addr, uint32_t ota_start,
                            uint32_t ota_end, uint32_t otaed);

#ifdef __cplusplus
}
#endif

#endif
