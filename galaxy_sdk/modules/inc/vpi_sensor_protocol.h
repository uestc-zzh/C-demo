/**
 * Copyright (C) 2021. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file vpi_sensor_protocol.h
 * @brief public header for sensor protocol define
 */
#ifndef _VPI_SENSOR_PROTOCOL_H_
#define _VPI_SENSOR_PROTOCOL_H_

#include <stdint.h>
#include <stdbool.h>

/** @addtogroup SENSOR
 *  - Sensor protocol definition
 *  @ingroup VPI
 *  @{
 */

/**
 * @brief Sensor data compress property
 */
typedef enum {
    COMPRESS_DATA = 0, /**< Compressed data flag */
    RAW_DATA      = 1, /**< Raw data flag */
} SenDataCompProp;

/**
 * @brief Sensor data endian property
 */
typedef enum {
    LSB_FIRST = 0, /**< LSB format */
    MSB_FIRST = 1, /**< MSB format */
} SenDataEndProp;

/**
 * @brief Sensor data encrypted property
 */
typedef enum {
    UNENCRYPTED = 0, /**< Unencrypted data */
    ENCRYPTED   = 1, /**< Encrypted data */
} SenDataEncProp;

/**
 * @brief sensor types
 */
typedef enum SensorType {
    SEN_TYPE_NONE = 0, /**< None */
    SEN_TYPE_PPG, /**< PPG sensor */
    SEN_TYPE_ECG, /**< ECG sensor */
    SEN_TYPE_IMU, /**< IMU sensor */
    SEN_TYPE_TMP, /**< Temperature sensor */
    SEN_TYPE_MAX
} SensorType;

/**
 * @brief Ppg Data Subtype
 */
typedef enum PpgDataType {
    PPG_DATA_NONE      = 0, /**< Not available type */
    PPG_DATA_GREEN_RAW = 0, /**< GREEN Raw data */
    PPG_DATA_REDIR_RAW, /**< REDIR Raw data */
    PPG_DATA_REDIRGREEN_RAW, /**< REDIRGREEN Raw data  */
    PPG_DATA_GREEN_RB, /**< GREEN Remove baseline data */
    PPG_DATA_IR_RB, /**< IR Remove baseline data */
    PPG_DATA_MAX
} PpgDataType;

/**
 * @brief Ecg Data Subtype
 */
typedef enum EcgDataType {
    ECG_DATA_NONE = 0, /**< Not available type */
    ECG_DATA_RAW, /**< Raw ECG data type */
    ECG_DATA_RR, /**< R wave interval data */
    EEG_DATA_PREPROC, /**< eeg pre-process data */
    EEG_DATA_FREQ_SPEC, /**< eeg frequency spectrum data */
    ECG_DATA_MAX
} EcgDataType;

/**
 * @brief IMU Data Subtype
 */
typedef enum ImuDataType {
    IMU_DATA_NONE = 0, /**< Not available type */
    IMU_DATA_ACCL_RAW, /**< ACCL Raw IMU data type */
    IMU_DATA_GYRO_RAW, /**< GYRO Raw IMU data type */
    IMU_DATA_GYRO_ACCL_RAW, /**< GYRO ACCL Raw IMU data type */
    IMU_DATA_MAG_RAW, /**< MAG Raw IMU data type */
    IMU_DATA_SF, /**< Sleep Feature */
    IMU_DATA_MAX
} ImuDataType;

/**
 * @brief Tmp Data Subtype
 */
typedef enum TmpDataType {
    TMP_DATA_NONE = 0, /**< Not available type */
    TMP_DATA_RAW, /**< Raw TMP data type */
    TMP_DATA_MAX
} TmpDataType;

/**
 * @brief enum of sensor raw data format
 */
typedef enum SenRawDataFormat {
    DATA_FORMAT_U32_RAW = 0, /**< uint32_t format */
    DATA_FORMAT_S32_RAW, /**< int32_t format */
    DATA_FORMAT_U16_RAW, /**< uint16_t format */
    DATA_FORMAT_S16_RAW, /**< int16_t format */
    DATA_FORMAT_U24_RAW, /**< uint24_t (3 byte) format */
    DATA_FORMAT_S24_RAW, /**< int24_t (3 byte) format */
} SenRawDataFormat;

/**
 * @brief enum of result data type
 * @note If you add/modify any enum type, you need to modify the length of the
 * enum type corresponding to the sensor_internal.c/res_data_len array at the
 * same time.
 */
typedef enum SenResultDataType {
    RES_TYPE_INFO = 0, /**< Information */
    RES_TYPE_HR, /**< Heart rate */
    RES_TYPE_RR, /**< Respiratory rate */
    RES_TYPE_SPO2, /**< Oxygen saturation */
    RES_TYPE_TMP, /**< Temperature */
    RES_TYPE_STEP, /**< Step counter */
    RES_TYPE_BP, /**< Blood pressure */
    RES_TYPE_GLUCOSE, /**< Glucose level */
    RES_TYPE_SQ, /**< Sleep quality */
    RES_TYPE_PAI, /**< Physical Activity Index */
    RES_TYPE_HRMAX, /**< HR max */
    RES_TYPE_HRREST, /**< HR rest */
    RES_TYPE_CC, /**< Calories consumption */
    RES_TYPE_BMR, /**< Basal metabolic rate */
    RES_TYPE_SED, /**< Sedentary */
    RES_TYPE_HRV, /**< Heart Rate Variablity */
    RES_TYPE_AFIB, /**< Atrial Fibrillation */
    RES_TYPE_PPG_SQI, /**< Signal Quality Index of PPG */
    RES_TYPE_ATTENTION, /**< Attention concentration */
    RES_TYPE_MAX
} SenResultDataType;

/**
 * @brief enum of user physical status
 */
typedef enum UserPhyStatus {
    USER_PHY_STA_STILL = 0, /**< User is still/reset */
    USER_PHY_STA_WALK, /**< User is walking */
    USER_PHY_STA_SLEEP, /**< User status info */
    USER_PHY_STA_SPORT, /**< User is exercising */
    USER_PHY_STA_NAP /**< User is napping */
} UserPhyStatus;

/**
 * @brief enum of sport type
 */
typedef enum SportType {
    SPORT_TYPE_WALK = 0, /**< Walking */
    SPORT_TYPE_RUN, /**< Running */
    SPORT_TYPE_SWIM /**< Swimming */
} SportType;

/**
 * @brief enum of result info type
 */
typedef enum SenResInfoType {
    RES_INFO_TYPE_USER_STA = 0, /**< User status info */
    RES_INFO_TYPE_SPORT /**< Sport type */
} SenResInfoType;

/**
 * @brief Structure of result info
 */
typedef struct SenResInfo {
    uint8_t value : 4; /**< @see SenResUserState @see SenResSportType */
    uint8_t reserved : 1; /**< reserved bit */
    uint8_t abnormal : 1; /**< Abnormal flag */
    uint8_t category_id : 2; /**< Category ID, @see SenResInfoType */
} __attribute((__packed__)) SenResInfo;

/** @} */

#endif // _VPI_SENSOR_PROTOCOL_H_
