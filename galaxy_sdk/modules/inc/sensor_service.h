/**
 * Copyright (C) 2021 VeriSilicon Holdings Co., Ltd.
 *
 * @file sensor_service.h
 * @brief GATT Sensor Service
 * @author Lucas Wang <lucas.wang@verisilicon.com>
 */

#ifndef _SENSOR_SERVICE_H_
#define _SENSOR_SERVICE_H_

#include "vs_protocol.h"

/** @addtogroup SENSOR_SERVICE
 *  @ingroup BLE
 *  @{
 */

/* Event ID between sensor service and manager */
#define EVENT_CONNECT_STATE 0x00 /**< BLE connect state update */
#define EVENT_PDU_TRANSFER_STATUS \
    0x01 /**< Packet transfer status update, param @see SensorRawDataStatus */
#define EVENT_BLE_DECODE_PACKET 0x02 /**< Received packet */
#define EVENT_BLE_PARAM_UPDATED 0x03 /**< BLE param is new updated */

/**
 * @brief Sensor raw data status struct
 * @note Packet is indicated by trans_id, receiver should compare the trans_id
 * to find out corresponding buffer or packet
 */
typedef struct SensorRawDataStatus {
    uint16_t trans_id; /**< Transfer ID of the sensor data packet, @see
                          SensorTransInfo */
    uint8_t status; /**< Transfer status of packet, @see SensorRawDataState */
} __attribute((__packed__)) SensorRawDataStatus;

/**
 * @brief - Enum of Sensor raw data state bit
 */
typedef enum SensorRawDataState {
    RAW_DATA_SUCCESS = 0x0, /**< Data was transferred successfully */
    RAW_DATA_TIMEOUT /**< Data transfer was timed out */
} SensorRawDataState;

/**
 * @brief - BLE connect parameter struct
 */
struct BleConnParam {
    uint16_t interval; /**< actual applied interval */
    uint16_t interval_min; /**< expected min interval */
    uint16_t interval_max; /**< expected max interval */
    uint16_t latency; /**< expected/actual latency */
    uint16_t timeout; /**< expected/actual timeout */
};

/**
 * @brief - Enum of BLE transfer state bit
 */
typedef enum BleTransferStatus { BLE_IDLE = 0x0, BLE_BUSY } BleTransferStatus;

/**
 * @brief - Enum of BLE sensor events state bit
 */
typedef enum BleSensorEventsStatus {
    BLE_EVENTS_OK = 0x0, /**< BLE Event generic OK */
    BLE_QUEUE_FULL /**< Event queue is full */
} BleSensorEventsStatus;

/**
 * @brief - Sensor service callback typedef
 */
typedef int (*sensor_callback_t)(uint8_t id, uint8_t *data);

/**
 * @brief - Get ble connect state
 * @return connect status @see BleStateId
 */
uint8_t ble_get_connect_state(void);

/**
 * @brief - Sensor service to notify manager
 * @note The Sensor service will send the received data to the Sensor Manager
 * through the registered callback function.
 * @param event_id sensor event id
 * @param data Data transferred by sensor event
 */
void ble_sensor_service_notify_manager(uint8_t event_id, uint8_t *data);

/**
 * @brief - Sensor manager send data to service
 * @note The Sensor Manager sends sensor data that needs to be sent to the
 * Sensor service, which will be sent over BLE.
 * @param sen_data data transferred by sensor event
 * @return transfer status
 */
uint8_t ble_sensor_packet_send(BleTransmitData *sen_data);

/**
 * @brief - Register sensor service callback function to gatt
 * @note This registered callback function will be called by ble service
 * function.
 * @param callback sensor service callback
 */
void ble_sensor_service_callback_register(sensor_callback_t callback);

/**
 * @brief - Unregister sensor service callback function to gatt
 */
void ble_sensor_service_callback_unregister(void);

/**
 * @brief - Update BLE parameter when sensor data transfer
 *
 * @param param BLE connect parameter
 * @return Update result
 */
int ble_sensor_update_le_param(struct BleConnParam *param);

/** @} */

#endif /* _SENSOR_SERVICE_H_ */
