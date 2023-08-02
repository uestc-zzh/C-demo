/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 *
 * @file battery_service.h
 * @brief GATT Battery Service
 * @author Lucas Wang <lucas.wang@verisilicon.com>
 */

#ifndef _BATTERY_SERVICE_H_
#define _BATTERY_SERVICE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BATTERY_SERVICE
 *  @ingroup BLE
 *  @{
 */

typedef struct BatteryInfo {
    uint8_t level;
    uint8_t state;
} __attribute__((__packed__)) BatteryInfo;

/**
 * @brief Define the events for battery service
 */
enum BatteryEventDef {
    /** The event for requesting battery level */
    BATTERY_EVENT_LEVEL_REQ = 0x0001,
    /** The event for requesting battery state */
    BATTERY_EVENT_STATE_REQ = 0x0002,
    /** The event for indicating battery level */
    BATTERY_EVENT_LEVEL_IND = 0x0003,
    /** The event for indicating battery state */
    BATTERY_EVENT_STATE_IND = 0x0004
};
/** The minimum value of battery level */
#define BATTERY_LEVEL_MIN 0
/** The maxmum value of battery level */
#define BATTERY_LEVEL_MAX 100
#define BAS_NOTIFY_DISABLE false

/**
 * @brief Enum of ble battery events status bit
 */
enum ble_battery_events_status {
    BLE_BATTERY_EVENTS_OK = 0x0, /**< Battery event return is ok */
    BLE_NOT_BATTERY_EVENT /**< Error for not a battery event */
};

/**
 * @brief - Battery service callback typedef
 * @param id The event id of battery service, @see BatteryEventDef
 * @param data The pointer of data for specific event, the struct of data
 * depends on event
 */
typedef int (*battery_callback_t)(uint8_t id, uint8_t *data);

/**
 * @brief - Register battery service callback function to gatt
 *
 * @param callback battery service callback
 */
void ble_battery_service_callback_register(battery_callback_t callback);

/**
 * @brief - Unregister battery service callback function to gatt
 */
void ble_battery_service_callback_unregister(void);

/**
 * @brief - Battery service to notify manager
 *
 * @param id battery event id
 * @param data Data transferred by battery event
 */
void ble_battery_service_notify_manager(uint8_t id, uint8_t *data);

/**
 * @brief - Battery manager send data to service
 *
 * @param id battery event id, @see BatteryEventDef
 * @param data Data transferred by battery event
 * @return transfer status
 */
uint8_t ble_battery_events_send(uint8_t id, uint8_t *data);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _BATTERY_SERVICE_H_ */
