#include <stdint.h>
#include <stdlib.h>
#include "vs_conf.h"
#include "soc_init.h"
#include "bsp.h"
#include "uart_printf.h"
#include "board.h"
#include "osal_task_api.h"
#include "vpi_error.h"
#include "sensor_task.h"
#include "hal_imu.h"
#include "osal_semaphore_api.h"
#include "vpi_event.h"
#include "osal_heap_api.h"
#include "imu/imu_init.h"
#include "common_struct_def.h"

#include <stdio.h>
#include "hal_rtc.h"
#include "controller.h"
#include "gpio.h"
#include "vpi_fifo.h"
#include "vsbt_config.h"
extern GpioPort led_blue_port;
extern GpioPort led_green_port;
static void *sensor_task;
ImuDevice *imu_dev;
static ImuGyroAccelData *gyro_accel_data;
// static int data_len=25;  //MICRO
int frame_request = 6; // less than 15
uint16_t available_frame;
static int start_index = 0;
static OsalSemaphore sem;
extern OsalSemaphore sem_mode;

uint32_t timeout = 1000;

// ImuSensorData* data;

extern uint16_t packet_length;
struct PackStatisticsCtrl *packStatisticsCtrl;
BleTransmitData ble_raw_data;
uint16_t packet_length = 12;

FifoCtrl *fc_verify = NULL;
int fifo_size       = 12;
msg_verify_packet_t msg_verify_packet;
// mirco
#define FLAG_CODE           0x1 << 7
#define BLOCK_TYPE          1
#define DATA_FORMATE_FEILD1 2
#define DATA_FORMATE_FEILD2 10
#define SAMPLE_RATE         25
#define SAMPLE_COUNT        12
#define SENSOR_TYPE         3
#define COLLECTION_ID       1

#define TIMESTAMP_BITS 48
#define TIMESTAMP_LOW  32
#define MASK(n)        ((0x1ull << n) - 1)

uint8_t wearing_status = 0;

void imuDataReadyHandler(void)
{
    osal_sem_post_isr(&sem);
}

void imuChangeToNormalHandler(void)
{
    osal_sem_post_isr(&sem_mode);
}

// read IMU data
void task_sensor(void *param)
{
    int ret;
    //	allocate data space
    gyro_accel_data =
        (ImuGyroAccelData *)osal_malloc(sizeof(ImuGyroAccelData) * 30);

    //	init sem
    ret = osal_init_sem(&sem);
    if (ret != OSAL_TRUE) {
        uart_printf("IMU:osal_init_sem error\r\n");
        return;
    }

    if (gpio_init(&led_blue_port) != VSD_SUCCESS) {
        uart_printf("gpio:gpio_init error\r\n");
        return;
    }
    uint8_t *fifo_addr_verify =
        (uint8_t *)osal_malloc(sizeof(struct msg_verify_packet) * fifo_size);
    fc_verify = vpi_fifo_init(fifo_addr_verify,
                              sizeof(struct msg_verify_packet), fifo_size);

    packStatisticsCtrl = vpi_packet_statistics_init();
    if (packStatisticsCtrl == NULL) {
        uart_printf("BLE:vpi_packet_statistics_init error\r\n");
        return;
    }

    if (crc_init()) {
        uart_printf("CRC:crc_init error\r\n");
        return;
    }

    imu_dev = (ImuDevice *)board_find_device_by_id(IMU_SENSOR_ID);
    ret     = imu_init(imu_dev);
    if (ret != VPI_SUCCESS) {
        uart_printf("IMU:imu_init error\r\n");
        return;
    }

    //	Enable data ready PIN interrupt with callback
    ret = hal_imu_enable_interrupt(imu_dev, IMU_WAKE_PIN, true,
                                   imuChangeToNormalHandler);
    if (ret != VPI_SUCCESS) {
        uart_printf("IMU:hal_imu_enable_interrupt error\r\n");
        return;
    }
    ret = hal_imu_enable_interrupt(imu_dev, IMU_DATA_PIN, true,
                                   imuDataReadyHandler);
    if (ret != VPI_SUCCESS) {
        uart_printf("IMU:hal_imu_enable_interrupt error\r\n");
        return;
    }

    struct data_header_t data_header;
    data_header.flag = FLAG_CODE | 0b100;

    while (1) {
        osal_sem_wait(&sem, OSAL_WAIT_FOREVER);

        int ret = hal_imu_read_gyro_accel(imu_dev,
                                          msg_verify_packet.gyro_accel_data +
                                              start_index,
                                          frame_request, &available_frame);
        // error is standable
        if (ret != VPI_SUCCESS) {
            uart_printf("IMU:hal_imu_read_gyro_accelerror\r\n");
        }
        if (available_frame != frame_request) {
            start_index = 0;
        }
        if (available_frame == 0) {
            wearing_status = 0;
        }
       

        start_index += available_frame;
        if (available_frame != frame_request) {
            uart_printf("low power mode……");
            start_index = 0;
        }
        // get enough data
        if (start_index >= packet_length) {
            if (vpi_event_notify(EVENT_SYS_TEST, (void *)gyro_accel_data) !=
                VPI_SUCCESS) {
                uart_printf("event:vpi_event_notify error\r\n");
                return;
            }
            if (!vpi_fifo_is_full(fc_verify)) {
                ret = vpi_fifo_write(fc_verify, (void *)(&msg_verify_packet));
                if (ret != VPI_SUCCESS) {
                    uart_printf("sensor:vpi_fifo_write error\r\n");
                    return;
                }
            }
            if (!vpi_fifo_is_empty(fc_verify)) {
                if (vpi_event_notify(EVENT_SYS_TEST, (void *)NULL) !=
                    EVENT_OK) {
                    uart_printf("event:osal_send_event_raw error\r\n");
                    return;
                }
            }
            start_index = 0;
            uint32_t crc_result =
                crc_calculate((void *)msg_verify_packet.gyro_accel_data,
                              packet_length);
            //		uart_printf("crc_calculate
            // result:%lx",crc_result);

            // uart_printf("data_header:%d",sizeof(data_header));
            data_header.block_type         = BLOCK_TYPE;
            data_header.crc8               = (uint8_t)crc_result & 0xff;
            data_header.sensor_type        = SENSOR_TYPE;
            data_header.data_subtype       = DATA_SUBTYPE;
            data_header.version            = 0x00;
            data_header.data_format.field1 = DATA_FORMATE_FEILD1;
            data_header.data_format.field2 = DATA_FORMATE_FEILD2;
            data_header.collection_id      = COLLECTION_ID;

            data_header.sample_rate  = SAMPLE_RATE;
            data_header.sample_count = SAMPLE_COUNT;
            data_header.data_length  = 12 * sizeof(ImuGyroAccelData);

            // prepare imu data
            struct msg_data_block_t msg_data_block;
            msg_data_block.data_header = data_header;
            memcpy((void *)msg_data_block.imu_data,
                   (void *)msg_verify_packet.gyro_accel_data,
                   packet_length * sizeof(ImuGyroAccelData));

            // send imu data
            if (ble_get_connect_state() == BLE_STATE_CONNECTED) {
                uart_printf("ble connected");
                SensorRawData tmp_data;
                uint64_t current_time;
                if (hal_get_utc_time(&current_time) != VSD_SUCCESS) {
                    uart_printf("hal_get_utc_time error");
                }

                msg_data_block.data_header.time_stamp_high16 =
                    ((current_time & (MASK(TIMESTAMP_BITS))) >> 32);
                msg_data_block.data_header.time_stamp_low32 =
                    (current_time & (MASK(TIMESTAMP_BITS)) &
                     (MASK(TIMESTAMP_LOW)));
                tmp_data.data       = (uint8_t *)&msg_data_block;
                ble_raw_data.data   = (uint8_t *)&tmp_data;
                ble_raw_data.length = sizeof(msg_data_block);
                ble_raw_data.flags  = FLAG_BIT_SEN_DATA;
                ble_raw_data.opcode = VS_SEN_RAW_DATA_MSG;
                // uart_printf("flag:%x",data_header.flag);

                int ret = vpi_packet_statistics_proc(packStatisticsCtrl,
                                                     &ble_raw_data);
                if (ret != VPI_SUCCESS) {
                    uart_printf("vpi_packet_statistics_proc error,%x", ret);
                    //				return ;
                }

                data_header.flag = FLAG_CODE;
                // osal_sleep(100);
            } else {
                uart_printf("ble disconnected");
            }

            // wait algo_task recieve all data:whether lost some data
            osal_sleep(50);
            start_index = 0;
            memset(gyro_accel_data, 0, packet_length);
            memset((void *)&ble_raw_data, 0, sizeof(ble_raw_data));
        }

        // no data
        // if (ble_get_connect_state() == BLE_STATE_CONNECTED) {
        //     if (available_frame == 0) {
        //         bt_bas_set_wearing_level(0);
        //         // uart_printf("set led green success:%d",__LINE__);
        //     } else {
        //         bt_bas_set_wearing_level(1);
        //         // uart_printf("set led blue success:%d",__LINE__);
        //     }
        // }

        ret = hal_imu_enable_interrupt(imu_dev, IMU_DATA_PIN, true,
                                       imuDataReadyHandler);
        if (ret != VPI_SUCCESS) {
            uart_printf("IMU:hal_imu_enable_interrupt error\r\n");
            return;
        }
    }

    osal_free(gyro_accel_data);

    osal_delete_task(sensor_task);
}
