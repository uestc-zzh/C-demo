/** @file
 *  @brief GATT Battery Service
 */

/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "vsbt_config.h"
#include <errno.h>
#include <stdbool.h>

#include <bluetooth.h>
#include <conn.h>
#include <gatt.h>
#include <uuid.h>
#include "wearing_state.h"
#include "uart_printf.h"
#include "controller.h"
#include "gpio.h"
#define LOG_MODULE_NAME "controller"

extern GpioPort led_blue_port;
extern GpioPort led_green_port;
uint8_t wearing_state_level    = 1U;
static uint8_t led_state_level = 1U;

#define BT_UUID_CONTROLLER                                                     \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x00000001, 0x00FF, 0x1000, 0x8000, \
                                           0x00805F9B05B5))
#define BT_UUID_DETECTION                                                      \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x00000007, 0x00F1, 0x1000, 0x8000, \
                                           0x00805F9B05B5))
#define BT_UUID_LED                                                            \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x00000007, 0x00F2, 0x1000, 0x8000, \
                                           0x00805F9B05B5))

static void wearinglvl_ccc_cfg_changed(const struct bt_gatt_attr *attr,
                                       uint16_t value)
{
    (void)(attr);
    led_state_level = 3;
    uart_printf("-------line:%d", __LINE__);
}

static ssize_t read_wearing_state(struct bt_conn *conn,
                                  const struct bt_gatt_attr *attr, void *buf,
                                  uint16_t len, uint16_t offset)
{
    uint8_t lvl8 = wearing_state_level;
    uart_printf("read_wearing_state-------line:%d",__LINE__);
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &lvl8, sizeof(lvl8));
}

static ssize_t read_led_state(struct bt_conn *conn,
                              const struct bt_gatt_attr *attr, void *buf,
                              uint16_t len, uint16_t offset)
{
    uint8_t lvl8 = led_state_level;
    uart_printf("read_led_state--------line:%d",__LINE__);
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &lvl8, sizeof(lvl8));
}

static ssize_t write_led_state(struct bt_conn *conn,
                               const struct bt_gatt_attr *attr, const void *buf,
                               uint16_t len, uint16_t offset, uint8_t flags)
{
    attr = (struct bt_gatt_attr *)buf;
    uart_printf("write_led_state --------line:%d",__LINE__);
    // green:low  blue:high
    if(wearing_state_level == 1){
        uart_printf("gpio 1111--------------------------------------");
        // int ret = gpio_high(&led_blue_port);
        int ret = gpio_toggle(&led_green_port);
        if (ret != VSD_SUCCESS) {
            uart_printf("gpio_high error");
            return -1;
        }
    } else if (wearing_state_level == 0) {
        uart_printf("gpio 0000--------------------------------------");
        // int ret = gpio_low(&led_blue_port);
        int ret = gpio_toggle(&led_blue_port);
        if (ret != VSD_SUCCESS) {
            uart_printf("gpio_low error");
            return -1;
        }
    }

    return len;
}

// BT_UUID_GATT generic
BT_GATT_SERVICE_DEFINE(
    controller, BT_GATT_PRIMARY_SERVICE(BT_UUID_CONTROLLER),
    BT_GATT_CHARACTERISTIC(BT_UUID_DETECTION,
                           BT_GATT_CHRC_READ | BT_GATT_CCC_NOTIFY,
                           //   BT_GATT_CHRC_INDICATE,
                           BT_GATT_PERM_READ, read_wearing_state, NULL,
                           &wearing_state_level),
    BT_GATT_CCC(wearinglvl_ccc_cfg_changed,
                BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BT_UUID_LED, BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
                           BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
                           read_led_state, write_led_state, &led_state_level));

uint8_t bt_bas_get_led_level(void)
{
    return led_state_level;
}

int bt_bas_set_led_level(uint8_t level)
{
    int rc;

    if (level > 1U) {
        return -EINVAL;
    }

    led_state_level = level;

    rc = bt_gatt_notify(NULL, &controller.attrs[2], &level, sizeof(level));

    return rc == -ENOTCONN ? 0 : rc;
}

uint8_t bt_bas_get_wearing_level(void)
{
    return wearing_state_level;
}

int bt_bas_set_wearing_level(uint8_t level)
{
    int rc;

    if (level > 1U) {
        return -EINVAL;
    }

    wearing_state_level = level;

    rc = bt_gatt_notify(NULL, &controller.attrs[1], &level, sizeof(level));

    return rc == -ENOTCONN ? 0 : rc;
}
