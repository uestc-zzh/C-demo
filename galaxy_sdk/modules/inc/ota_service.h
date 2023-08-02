/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 *
 * @file ota_service.h
 * @brief GATT OTA Service
 * @author Lucas Wang <lucas.wang@verisilicon.com>
 */

#ifndef _OTA_SERVICE_H_
#define _OTA_SERVICE_H_

#include "vs_protocol.h"

/** @addtogroup OTA_SERVICE
 *  @ingroup BLE
 *  @{
 */

/* Event ID between OTA service and manager */
#define EVENT_BLE_DECODE_PACKET 0x02

/**
 * @brief - Enum of BLE OTA events state bit
 */
typedef enum BleOtaEventsStatus {
    BLE_OTA_EVENTS_OK = 0x0, /**< OTA event generic OK */
    BLE_OTA_NOTIFY_FAIL, /**< OTA notify fail */
} BleOtaEventsStatus;

/**
 * @brief - OTA service callback typedef
 */
typedef int (*ota_callback_t)(uint8_t id, uint8_t *data);

/**
 * @brief - OTA manager send data to service
 *
 * @param ota_data data transferred by OTA event
 * @return transfer status
 */
uint8_t ble_ota_packet_send(BleTransmitData *ota_data);

/**
 * @brief - Unregister OTA service callback function to gatt
 */
void ble_ota_service_callback_unregister(void);

/**
 * @brief - Register OTA service callback function to gatt
 *
 * @param callback OTA service callback
 */
void ble_ota_service_callback_register(ota_callback_t callback);

/**
 * @brief - OTA service to notify manager
 *
 * @param event_id OTA event id
 * @param data Data transferred by OTA event
 */
void ble_ota_service_notify_manager(uint8_t event_id, uint8_t *data);

/** @} */

#endif /* _OTA_SERVICE_H_ */
