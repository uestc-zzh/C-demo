/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 *
 * @file common_message_def.h
 * @brief common message define
 * @author Shaobo Tu <Shaobo.Tu@verisilicon.com>
 */

#ifndef _COMMON_MESSAGE_DEF_H_
#define _COMMON_MESSAGE_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup MESSAGE
 *  common message Define.
 *  @ingroup BLE
 *  @{
 */

#include <stdint.h>

#define BT_DEV_NAME_LENGTH 16 /**< BT device name length */
#define BT_DEV_MAC_LENGTH  6 /**< BT device MAC address length */

/* SDK internal messages definition */
/*
 * System messages
 */
#define VS_CONNECT_STATE_MSG 0x0001 /**< Connect state, @see BleStateId */
#define VS_SEN_DATA_MSG      0x0002 /**< Sensor messages */
#define VS_SYNC_TIME_MSG     0x0003 /**< Sync time with UTC format */
#define VS_ALERTING_MSG      0x0004 /**< Alerting message */

/* OTA messages, it's over a separate service, but should not use same ID */
#define VS_OTA_START_MSG  0x0010 /**< OTA start message */
#define VS_OTA_FLUSH_MSG  0x0011 /**< OTA flush message */
#define VS_OTA_FINISH_MSG 0x0012 /**< OTA finish message */
#define VS_OTA_REBOOT_MSG 0x0013 /**< OTA reboot message */
#define VS_OTA_CANCEL_MSG 0x0014 /**< OTA cancel message */

/*
 * BLE messages
 */
#define VS_BLE_BIND_CTRL 0x0080 /**< 0 for unbind, 1 for bind */
#define VS_BLE_NAME_MAC  0x0081 /**< BLE config name and MAC message */

/*
 * Secure messages
 */
#define VS_SDK_KEY_MSG 0x300 /**< Key message for SDK */

/*
 * Auto test messages
 */
#define VS_TEST_GEN_RESULT    0x400 /**< Generate result packet */
#define VS_TEST_GEN_RAW       0x401 /**< Generate raw packet */
#define VS_TEST_DEV_READY_MSG 0x402 /**< Device ready for test */
#define VS_TEST_RESULT_MSG    0x403 /**< Test result message */
#define VS_TEST_FINISH_MSG    0x404 /**< Test finish  message */
#define VS_TEST_SEN_SQI_MSG   0x405 /**< sensor signal quality index test */

/*
 * Debug messages
 */
#define VS_DEBUG_MONITOR_MSG 0x500 /**< Debug monitor */
#define VS_DEBUG_INFO_MSG    0x501 /**< Debug info */

/*
 * Sensor relating messages
 */

/** Vital signs message, data struct @see VitalSignData */
#define VS_VITAL_SIGNS_MSG 0x600

/** Message to transmit sensor raw data, data format should follow
 * VeriHealth data protocol for sensor raw data */
#define VS_SEN_RAW_DATA_MSG 0x601

/** Sensor raw data critical attributes message, data struct @see
 * PackStatisticsData */
#define VS_SEN_RAW_DATA_CRIT_ATTR_MSG 0x602

/**
 * SDK messages end
 */
#define VS_SDK_MSG_END (0x1000)

/**
 * Custom messages start
 */
#define VS_CUSTOM_MSG_START 0x2000

/**
 * @brief Alerting type enum
 */
typedef enum AlertTypeEnum {
    ALERT_TYPE_CLEAR, /**< To clear the last alerting */
    ALERT_TYPE_FALL, /**< Fall detected */
    ALERT_TYPE_SEDENTARY, /**< Sedentary */
    ALERT_TYPE_AFib, /**< Atrial fibrillation */
    ALERT_TYPE_WEAR, /**< Wear detected */
    ALERT_TYPE_MAX, /**< MAX of alert types */
} AlertTypeEnum;

/**
 * @brief Alerting parameter struct
 */
typedef struct AlertParamSt {
    uint8_t type; /**< Type of alert, definition @see AlertTypeEnum */
    uint32_t param; /**< Parameter for specific type */
} __attribute((__packed__)) AlertParamSt;

/**
 * @brief Ota Start structure
 */
typedef struct OtaStartParam {
    uint32_t total_len; /**< Total length of the new firmwawre image */
    uint16_t fw_version; /**< Version of the new firmware */
} __attribute((__packed__)) OtaStartParam;

/**
 * @brief Ota Flush structure
 */
typedef struct BleOtaFlushParam {
    uint32_t offset; /**< Offset address of the flush data in firmware */
    uint16_t len; /**< The length of flush data */
} __attribute((__packed__)) BleOtaFlushParam;

/**
 * @brief BT config information(name and MAC address)
 */
typedef struct BTConfigNameMac {
    uint8_t bt_name[BT_DEV_NAME_LENGTH]; /**< The name of BT device */
    uint8_t bt_mac[BT_DEV_MAC_LENGTH]; /**< The MAC address of BT device */
} __attribute((__packed__)) BTConfigNameMac;

/**
 * @brief Debug info struct
 */
typedef struct DebugInfoSt {
    uint8_t module; /**< ID of module, @see ModuleIdDef */
    uint8_t type; /**< Type in the module, @see AlgoTypeDef for ALGO */
    uint8_t func; /**< Function ID, @see FuncTypeDef */
    uint32_t param; /**< Param of this info */
    int16_t err_code; /**< Error code from function invoking */
} __attribute((__packed__)) DebugInfoSt;

/**
 * @brief Debug monitor struct
 */
typedef struct DebugMonitorSt {
    uint32_t heap; /**< Heap size in bytes */
    uint16_t ppg_added_irq; /**< ppg irq counter */
    uint16_t ecg_added_irq; /**< ecg irq counter */
    uint16_t imu_added_irq; /**< imu irq counter */
} __attribute((__packed__)) DebugMonitorSt;

/**
 * @brief Auto test struct for result
 */
typedef struct TestResultSt {
    uint8_t success;
} __attribute((__packed__)) TestResultSt;

/**
 * @brief Test struct for generating result packet
 */
typedef struct TestGenResultSt {
    uint8_t type; /**< Result type, @see SenResultDataType */
    uint32_t value; /**< value of specified result type */
} __attribute((__packed__)) TestGenResultSt;

/**
 * @brief Test struct for generating raw packet
 */
typedef struct TestGenRawSt {
    uint8_t type; /**< Sensor type, @see SensorType */
    uint32_t length; /**< Length of raw data */
} __attribute((__packed__)) TestGenRawSt;

/**
 * @brief Enum for ppg led type
 */
typedef enum DevPpgLedType {
    DEV_PPG_LED_GREEN = 0,
    DEV_PPG_LED_RED,
    DEV_PPG_LED_IR,
    DEV_PPG_LED_MAX
} DevPpgLedType;

/**
 * @brief Test struct for senor signal quality index
 */
typedef struct TestSenSqiSt {
    uint8_t type; /**< @see SensorType */
    uint32_t value[DEV_PPG_LED_MAX]; /**< @see DevPpgLedType */
} __attribute((__packed__)) TestSenSqiSt;

/**
 * @brief Enum for vital signal type
 */
typedef enum VitalSignType {
    VITAL_SIGN_START = 128, /**< The value cannot be greater than
                               RES_TYPE_MAX(definition @see SenResultDataType)
                               defined by the VeriSilicon private protocol
                                */
    VITAL_SIGN_IMU_ACTION, /**< IMU action */
} VitalSignType;

/**
 * @brief Enum for imu action type
 */
typedef enum ImuActionType {
    IMU_ACT_NONE, /**< None action */
    IMU_ACT_JUMPING_SQUAT, /**< Jumping squat action */
    IMU_ACT_JUMPING_JACK, /**< Jumping jack action */
    IMU_ACT_JUMPING_LUNGE, /**< Jumping lunge action */
    IMU_ACT_SIT, /**< Sit action */
    IMU_ACT_SQUAT, /**< Squat action */
    IMU_ACT_TROT, /**< Trot action */
    IMU_ACT_WALK, /**< Walk action */
    IMU_ACT_WAVE, /**< Wave action */
    IMU_ACT_LUNGE, /**< Lunge action */
    IMU_ACT_GOOD_MORNING, /**< Good morning action */
    IMU_ACT_TUCK_JUMPING, /**< Tuck jumping action */
} ImuActionType;
/**
 * @brief Data struct for vital signs
 */
typedef struct VitalSignData {
    uint8_t type; /**< Vital sign type, definition @see SenResultDataType and
                     @see VitalSignType */
    union {
        uint32_t value; /**< Value of specific vital sign type */
        struct {
            uint16_t action; /**< Action type of IMU, definition @see
                                ImuActionType */
            uint16_t act_val; /**< Value of imu action */
        } imu;
    };
    uint16_t trans_id; /**< The transmission ID of the current data */
} __attribute__((__packed__)) VitalSignData;

/**
 * @brief Data struct for raw data packet statistics
 */
typedef struct PackStatisticsData {
    uint32_t bytes_num; /**< Bytes number of raw data  */
    uint16_t accuracy; /**< Accuracy times of raw data, expand by 100x */
    uint16_t actual_sps; /**< Actual sample per second of raw data, expand by
                            10x*/
} __attribute__((__packed__)) PackStatisticsData;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _COMMON_MESSAGE_DEF_H_ */
