/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file main.c
 * @brief Functions for main
 * @author Shaobo Tu <Shaobo.Tu@verisilicon.com>
 */

#include <stdint.h>
#include "vs_conf.h"
#include "soc_init.h"
#include "bsp.h"
#include "uart_printf.h"
#include "board.h"
#include "osal_task_api.h"
#include "vpi_error.h"
#include "vpi_ble_app.h"
#include "vpi_storage.h"
#include "vpi_error.h"
#include "sensor_task.h"
#include "algo_task.h"
#include "common_struct_def.h"

static BoardDevice board_dev;
static void *init_task;
static void *sensor_task;
static void *algo_task;
static void *change_work_mode_task;

static void task_init_app(void *param)
{
    /* Initialize board */
    board_register(board_get_ops());
    board_init((void *)&board_dev);
    if (board_dev.name)
        uart_printf("Board: %s", board_dev.name);
    vpi_storage_show_img_info();
    uart_printf("Hello VeriHealth!\r\n");
    vpi_ble_start();
    sensor_task = osal_create_task(task_sensor, "task_sensor", 512, 4, NULL);
    algo_task   = osal_create_task(task_algo, "task_algo", 512, 3, NULL);
    change_work_mode_task =
        osal_create_task(task_change_work_mode, "task_change_work_mode", 512, 4,
                         NULL);
    osal_delete_task(init_task);
}

int main(void)
{
    int ret;

    /* Initialize soc */
    ret = soc_init();
    if (vsd_to_vpi(ret) != VPI_SUCCESS) {
        if (vsd_to_vpi(ret) == VPI_ERR_LICENSE)
            uart_printf("Invalid SDK license!\r\n");
        else
            uart_printf("Soc Init Error!\r\n");
        return vsd_to_vpi(ret);
    }
    /* Initialize uart */
    uart_debug_init();
    /* Initialize bsp */
    bsp_init();
    /* Create init task */
    init_task = osal_create_task(task_init_app, "init_app", 512, 1, NULL);
    /* Start os */
    osal_start_scheduler();

    return 0;
}
