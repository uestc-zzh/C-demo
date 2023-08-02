/*
 * common_struct_def.h
 *
 *  Created on: 2023年7月17日
 *      Author: swpu_
 */

#ifndef GALAXY_SDK_COMMON_STRUCT_DEF_H_
#define GALAXY_SDK_COMMON_STRUCT_DEF_H_

#include <stdint.h>
#include "common_message_def.h"
#include "vs_protocol.h"
#include "vpi_event_def.h"
#include "vpi_packet_statistics.h"
#include "sensor_service.h"
#include "vpi_event_def.h"
#include "vpi_packet_statistics.h"
#include "hal_gpio.h"
#include "soc_pin_define.h"
#include "vsd_error.h"
#include "hal_imu.h"
#include "uart_printf.h"
#include "osal_task_api.h"
#include "controller.h"

#define FLAG_CODE           0x1 << 7
#define BLOCK_TYPE          1
#define DATA_SUBTYPE        3;
#define DATA_FORMATE_FEILD1 2
#define DATA_FORMATE_FEILD2 10
#define SAMPLE_RATE         25
#define SAMPLE_COUNT        12
#define SENSOR_TYPE         3
#define COLLECTION_ID       1
#define ALG_PACKET_SIZE     25
#define VERIFY_PACKET_SIZE  12
#define TIMESTAMP_BITS      48
#define TIMESTAMP_LOW       32
#define MASK(n)             ((0x1ull << n) - 1)

// #define TEST_TASK

struct data_header_t {
    uint8_t block_type;
    uint8_t crc8;
    uint8_t sensor_type;
    uint8_t data_subtype;
    uint8_t version;
    struct {
        uint8_t field1 : 4, field2 : 4;
    } data_format;
    uint8_t flag;
    uint8_t collection_id;
    uint16_t sample_count;
    uint16_t sample_rate;
    uint32_t time_stamp_low32;
    uint16_t time_stamp_high16;
    uint16_t data_length;
};

struct msg_data_block_t {
    struct data_header_t data_header;
    // TODO:
    ImuGyroAccelData imu_data[12];
};
typedef struct msg_algo_packet {
    ImuGyroAccelData gyro_accel_data[ALG_PACKET_SIZE];
    int size;
} msg_algo_packet_t;

typedef struct msg_verify_packet {
    ImuGyroAccelData gyro_accel_data[VERIFY_PACKET_SIZE];
    int size;
} msg_verify_packet_t;

void task_test(void *param);

#endif /* GALAXY_SDK_COMMON_STRUCT_DEF_H_ */
