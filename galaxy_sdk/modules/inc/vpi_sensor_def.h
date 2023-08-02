/**
 * Copyright (C) 2021. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file vpi_sensor_def.h
 * @brief public header for sensor define
 */
#ifndef _VPI_SENSOR_DEF_H_
#define _VPI_SENSOR_DEF_H_

#include <stdint.h>
#include <stdbool.h>
#include "vpi_sensor_protocol.h"

/** @addtogroup SENSOR
 *  - Sensor definition
 *  @ingroup VPI
 *  @{
 */

#define RAW_DATA_HEADER_SIZE (20)
#define RESULT_DATA_HEADER_SIZE (12)

/* max number of result group */
#define MAX_RES_GRP_NUM (16)
/* max number of result group members */
#define MAX_RES_GRP_MEMBER_NUM (12)

/**
 * @brief Structure of raw data buffer control status
 */
struct VpiSensorRawSta {
    SensorType type; /**< Sensor type, @see SensorType */
    uint8_t *raw; /**< Pointer of raw data */
    uint8_t ready; /**< Data ready flag */
    int8_t mode; /**< @see e_ppg_workmode */
    uint16_t len; /**< Length of data */
    uint64_t time; /**< Timestamp in UTC */
    uint8_t subtype; /**< @see PpgDataType @see EcgDataType @see ImuDataType
                      */
};

/**
 * @brief Structure of sensor data packet
 */
typedef struct SensorDataPacket SensorDataPacket;

/**
 * @brief Structure of sensor raw data state
 */
typedef struct VpiSensorRawSta VpiSensorRawSta;
/**
 * @brief Type of VpiSensorRawSta point array
 */
typedef struct VpiSensorRawSta **VpiSensorRawStaPointArr;

/**
 * @brief Type of VpiSensorRawStaPointArr point
 */
typedef VpiSensorRawStaPointArr *VpiSensorRawStaPointArrHandle;

/**
 * @brief sensor raw data sampling callback handler
 */
typedef uint16_t (*SenDataHandler)(uint8_t *raw_data, uint16_t avail_cnt);

/**
 * @brief sensor data process callback handler
 */
typedef int (*SenDataProcHandler)(uint8_t *data, uint16_t samp_num, void *arg);

/**
 * @brief data compress callback handler
 */
typedef int (*DataCompressHandler)(uint8_t *in, uint16_t in_len, uint8_t *out,
                                   uint16_t *out_len);

/**
 * @brief data encrypt callback handler
 * @param in the pointer of in data
 * @param in_len the length of in data
 * @param out the pointer of out data
 */
typedef int (*DataEncryptHandler)(uint8_t *in, uint16_t in_len, uint8_t *out);

/**
 * @brief sensor packet types
 */
typedef enum SenPacketType {
    SEN_PACKET_EMPTY = 0, /**< Empty packet for state update */
    SEN_PACKET_RT_RAW, /**< Realtime packet of raw data */
    SEN_PACKET_RT_RESULT, /**< Realtime packet of result data */
    SEN_PACKET_CACHED, /**< Indicate it's packet for cached data */
    SEN_PACKET_MAX /**< Max type of sensor packet type */
} SenPacketType;

/**
 * @brief Sensor data types
 */
typedef enum {
    SEN_DATA_TYPE_RAW, /**< Raw data */
    SEN_DATA_TYPE_RESULT, /**< Result data */
    SEN_DATA_TYPE_MAX, /**< Max type of sensor data type */
} SenDataType;

/**
 * @brief Structure of raw data buffer control
 */
typedef struct SenRawDataBufCtrl {
    uint8_t *buffer; /**< Pointer of buffer */
    uint16_t proc_len; /**< Default process length */
    uint8_t buf_num; /**< Number of buffers */
    uint16_t padding; /**< padding size  */
    uint16_t rec_len; /**< Record current length of data */
    uint8_t *rec_idx; /**< Pointer of last data */
    uint8_t buf_sw; /**< Buffer switch index */
    uint64_t time; /**< Timestamp of raw data */
    VpiSensorRawStaPointArr rawsta; /**< Raw data buffer control status */
} SenRawDataBufCtrl;

/**
 * @brief Structure of sensor data process control
 */
typedef struct SenDataProcCtrl {
    uint8_t *buffer; /**< Pointer of buffer */
    uint16_t index; /**< Data index in buffer */
    uint16_t samp_size; /**< Sensor sampling size */
} SenDataProcCtrl;

/**
 * @brief Structure of sensor result control
 */
struct SenResSetCtrlSt {
    uint8_t grp_id; /**< Group id of the result set */
    uint32_t grp_mb; /**< Member of the result set */
    uint32_t index; /**< Index of the result set */
    uint16_t len; /**< Data length of the result set */
    uint8_t *data; /**< Data of the result set */
};

typedef struct SenResSetCtrlSt SenResSetCtrl;

typedef struct SensorTransInfo {
    uint8_t packet_type : 4; /**< @see SenPacketType */
    uint8_t data_type : 4; /**< @see SensorType for real-time data, @see
                              SenHeaderBlockType for cached data */
    uint8_t trans_id; /**< increasing id for each type */
} __attribute((__packed__)) SensorTransInfo;

/**
 * @brief Vital sensors configuration
 */
typedef struct {
    uint32_t sample_rate; /**< Sample rate */
    int mode; /**< enum e_ppg_workmode for PPG */
    SenRawDataFormat data_fmt; /**< Data format */
    uint8_t channels; /**<  0 is default for 1 channel */
} VpiSensorCfg;

/**
 * @brief Data structure for packing
 */
typedef struct {
    uint8_t *in_raw; /**< the pointer to sensor raw data */
    uint32_t in_raw_size; /**< the size of sensor raw data */
    uint8_t *out_buf; /**< the pointer to compressed and encrypted buffer */
    uint32_t out_size; /**< the size of out_buf */
    bool compress; /**< compress or raw @see SenDataCompProp */
    bool encrypt; /**< encrypt or decrypt @see SenDataEncProp */
    bool msb; /**< msb or lsb @see SenDataEndProp */
    bool start; /**< packet start */
    bool end; /**< packet end */
    uint64_t timestamp; /**< timestamp unit:ms */
    uint8_t type; /**< raw data type @see EcgDataType, ImuDataType ... */
} VpiSenRawProp;

/**
 * @brief Data structure for result data packing
 */
typedef struct {
    bool encrypt; /**< encrypt or decrypt @see SenDataEncProp */
    bool start; /**< packet start */
    bool end; /**< packet end */
    uint32_t timestamp; /**< timestamp unit:s */
    uint16_t duration; /**< duration unit:s */
} VpiSenResultProp;

/** @} */

#endif // _VPI_SENSOR_DEF_H_
