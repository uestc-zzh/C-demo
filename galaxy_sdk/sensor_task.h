#ifndef _SENSOR_TASK_H_
#define _SENSOR_TASK_H_

#include "hal_crc.h"
#include "sensor_task.h"
#include "uart_printf.h"

void task_sensor(void *param);

void task_change_work_mode(void *param);

void task_ble_action(void *param);

uint32_t crc_init();

uint32_t crc_calculate(void* arr,uint32_t length);

//void create_header(struct data_header_t* data_header, uint32_t crc_result);
#endif
