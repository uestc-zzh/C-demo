/**
 * Copyright (C) 2023 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file cocopalmR_board.h
 * @brief Head file for cocopalmR board
 * @author Shaobo Tu <Shaobo.Tu@verisilicon.com>
 */

#ifndef _COCOPALM_R_BOARD_H_
#define _COCOPALM_R_BOARD_H_

#include <stddef.h>
#include "board.h"
#include "device.h"
#include "soc_pin_define.h"

static const char cocopalmR_board_name[] = "CocopalmR";

static const GpioPort ppg_data_pin = {
    .group      = 0,
    .irq_reload = 1,
    .invert     = 0,
    .port       = PIN_IRQ_DS11,
    .trigger    = IRQ_TRIGGER_FALLING_EDGE,
    .type       = IO_MODE_IRQ,
};

static const GpioPort ecg_data_pin = {
    .group      = 0,
    .irq_reload = 0, // can be enabled only for SPI DMA mode
    .invert     = 0,
    .port       = PIN_IRQ_DS12,
    .trigger    = IRQ_TRIGGER_FALLING_EDGE,
    .type       = IO_MODE_IRQ,
};

static const GpioPort ecg_reset_pin = {
    .group      = 0,
    .irq_reload = 0,
    .invert     = 0,
    .port       = PIN_IRQ_DS10,
    .trigger    = 0,
    .type       = IO_MODE_OUT_PULL,
};

static const GpioPort imu_data_pin = {
    .group      = 0,
    .irq_reload = 0,
    .invert     = 0,
    .port       = PIN_IRQ_DS6,
    .trigger    = IRQ_TRIGGER_LEVEL_LOW,
    .type       = IO_MODE_IRQ,
};

static const GpioPort imu_wake_pin = {
    .group      = 0,
    .irq_reload = 0,
    .invert     = 0,
    .port       = PIN_IRQ_DS5,
    .trigger    = IRQ_TRIGGER_FALLING_EDGE,
    .type       = IO_MODE_IRQ,
};

/* charger's interrupt request pin */
static const GpioPort charger_irq_pin = {
    .group      = 0,
    .irq_reload = 1,
    .invert     = 0,
    .port       = PIN_IRQ_DS7,
    .trigger    = IRQ_TRIGGER_FALLING_EDGE,
    .type       = IO_MODE_IRQ,
};

static const GpioPort blue_led_config = {
    .group      = 0,
    .irq_reload = 0,
    .invert     = 0,
    .port       = PIN_IRQ_DS8,
    .trigger    = IRQ_TRIGGER_NONE,
    .type       = IO_MODE_OUT_PULL,
};

static const GpioPort green_led_config = {
    .group      = 0,
    .irq_reload = 0,
    .invert     = 0,
    .port       = PIN_IRQ_DS9,
    .trigger    = IRQ_TRIGGER_NONE,
    .type       = IO_MODE_OUT_PULL,
};

static const GpioPort pwr_key_pin = {
    .group      = 0,
    .irq_reload = 1,
    .invert     = 0,
    .port       = PIN_IRQ_DS4,
    .trigger    = IRQ_TRIGGER_FALLING_EDGE,
    .type       = IO_MODE_IRQ,
};

#endif // _COCOPALM_R_BOARD_H_
