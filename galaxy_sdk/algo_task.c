#include <stdint.h>
#include "vs_conf.h"
#include "soc_init.h"
#include "bsp.h"
#include "uart_printf.h"
#include "board.h"
#include "osal_task_api.h"
#include "vpi_error.h"
#include "algo_task.h"
#include "hal_imu.h"
#include "vpi_event.h"
#include <stdlib.h>
#include "osal_semaphore_api.h"
#include "osal_heap_api.h"
#include "common_message_def.h"
#include "sensor_service.h"
#include "vpi_event_def.h"
#include "vpi_packet_statistics.h"
#include "wearing_state.h"
#include "algorithm/alg_step_counter.h"
#include "vpi_fifo.h"
#include "common_struct_def.h"
#include "algorithm/alg_mlp_networks.h"
void *algo_task;
static ImuGyroAccelData *gyro_accel_data;
static OsalSemaphore sem;
#define data_len 25
typedef struct GA_Input {
    int16_t x[data_len];
    int16_t y[data_len];
    int16_t z[data_len];
    int16_t gyro_x[data_len];
    int16_t gyro_y[data_len];
    int16_t gyro_z[data_len];
} GA_Input;

GA_Input *ga_input;
uint16_t step_num = 0, step_total = 0, i;
AccInput acc_xyz_win;
uint32_t time = 0;

int algo_data_len = 0;
extern OsalSemaphore ble_action_sem;
extern FifoCtrl *fc_verify;
int16_t class;
// static uint32_t seq=0;

VitalSignData action_data;
BleTransmitData action_trans_data;
extern uint16_t packet_length;
extern uint8_t wearing_state_level;

msg_verify_packet_t msg_verify_packet_test;
msg_algo_packet_t msg_algo_packet;

uint16_t action_map[11] = { IMU_ACT_JUMPING_SQUAT, IMU_ACT_JUMPING_JACK,
                            IMU_ACT_JUMPING_LUNGE, IMU_ACT_SIT,
                            IMU_ACT_SQUAT,         IMU_ACT_TROT,
                            IMU_ACT_WALK,          IMU_ACT_WAVE,
                            IMU_ACT_LUNGE,         IMU_ACT_GOOD_MORNING,
                            IMU_ACT_TUCK_JUMPING }; // TODO:
int algoEventHandler(void *cobj, uint32_t event_id, void *param)
{
    gyro_accel_data = (ImuGyroAccelData *)param;
    osal_sem_post_isr(&sem);

    return 0;
}
// handle IMU data
void task_algo(void *param)
{
    ga_input           = (GA_Input *)malloc(sizeof(GA_Input));
    void *pAlgoManager = vpi_event_new_manager(COBJ_ALGO_MGR, algoEventHandler);

    int ret = vpi_event_register(EVENT_SYS_TEST, pAlgoManager);
    if (ret == EVENT_ERROR) {
        uart_printf("vpi_event_register fail!\r\n");
        osal_delete_task(algo_task);
        return;
    }

    ret = step_counter_init();
    if (ret != 0) {
        uart_printf("ALG:step_counter_init error\r\n");
        return;
    }
    ret = networks_init();
    if (ret != 0) {
        uart_printf("MLP:networks_init error\r\n");
        return;
    }
    //	init sem
    ret = osal_init_sem(&sem);
    if (ret != OSAL_TRUE) {
        uart_printf("IMU:osal_init_sem error\r\n");
        return;
    }

    while (1) {
        // recieve msg
        vpi_event_listen(pAlgoManager);
        if (!vpi_fifo_is_empty(fc_verify)) {
            if (vpi_fifo_read(fc_verify, &msg_verify_packet_test) !=
                VPI_SUCCESS) {
                uart_printf("vpi_fifo_get error\r\n");
                return;
            }
            gyro_accel_data = msg_verify_packet_test.gyro_accel_data;
            for (int i = 0; i < VERIFY_PACKET_SIZE; ++i) {
                if (algo_data_len >= ALG_PACKET_SIZE) {
                    algo_data_len      = 0;
                    acc_xyz_win.len    = ALG_PACKET_SIZE;
                    acc_xyz_win.gyro_x = ga_input->gyro_x;
                    acc_xyz_win.gyro_y = ga_input->gyro_y;
                    acc_xyz_win.gyro_z = ga_input->gyro_z;
                    acc_xyz_win.x      = ga_input->x;
                    acc_xyz_win.y      = ga_input->y;
                    acc_xyz_win.z      = ga_input->z;
#if 1
                    step_counter_process(&acc_xyz_win, &step_num, &class);
                    step_total += step_num;
                    if (time++ % 5 == 0) {
                        uart_printf("[%lu]              Total counts = %3d\n",
                                    time, step_total);
                    } else {
                        uart_printf("[%lu]\n", time);
                    }
#endif
                    if (ble_get_connect_state() == BLE_STATE_CONNECTED) {
                        action_data.type         = VITAL_SIGN_IMU_ACTION;
                        action_data.imu.act_val  = step_num;
                        action_data.imu.action   = action_map[class + 1];
                        action_trans_data.opcode = VS_VITAL_SIGNS_MSG;
                        action_trans_data.data   = (uint8_t *)&action_data;
                        action_trans_data.length = sizeof(action_data);
                        action_trans_data.flags  = FLAG_BIT_SEN_DATA;

                        uart_printf("ble_sensor_packet_send:%d\r\n",
                                    ble_sensor_packet_send(&action_trans_data));
                        osal_sleep(100);
                    } else {
                        uart_printf("ble disconnected");
                    }
                }
                ga_input->gyro_x[algo_data_len] = gyro_accel_data[i].gx;
                ga_input->gyro_y[algo_data_len] = gyro_accel_data[i].gy;
                ga_input->gyro_z[algo_data_len] = gyro_accel_data[i].gz;
                ga_input->x[algo_data_len]      = gyro_accel_data[i].ax;
                ga_input->y[algo_data_len]      = gyro_accel_data[i].ay;
                ga_input->z[algo_data_len]      = gyro_accel_data[i].az;
                algo_data_len++;
            }
        }
    }
    osal_delete_task(algo_task);
    return;
}
