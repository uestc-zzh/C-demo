/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file soc_pin_define.h
 * @brief Header of SoC pin define
 */

#ifndef _SOC_PIN_DEFINE_H_
#define _SOC_PIN_DEFINE_H_

/**
 * @brief Gpio pins
 */
typedef enum PinList {
    PIN_IRQ_DS0 = 0,
    PIN_IRQ_DS1,
    PIN_IRQ_DS2,
    PIN_IRQ_DS3,
    PIN_IRQ_DS4,
    PIN_IRQ_DS5,
    PIN_IRQ_DS6,
    PIN_IRQ_DS7,
    PIN_IRQ_DS8,
    PIN_IRQ_DS9,
    PIN_IRQ_DS10,
    PIN_IRQ_DS11,
    PIN_IRQ_DS12,
    PIN_IRQ_DS13,
    PIN_IRQ_DS14,
    PIN_IRQ_DS15,
    PIN_NMI,
    PIN_SPI_CS0,
    PIN_SPI_MOSI0,
    PIN_SPI_MISO0,
    PIN_SPI_CLK0,
    PIN_I2S_BCLK,
    PIN_I2S_LRCK,
    PIN_I2S_DI,
    PIN_I2S_DO,
    PIN_UART_TXD0,
    PIN_UART_RXD0,
    PIN_UART_CTS0,
    PIN_UART_RTS0,
    PIN_UART_TXD1,
    PIN_UART_RXD1,
    PIN_UART_TXD2,
    PIN_UART_RXD2,
    PIN_I2C_SCL0,
    PIN_I2C_SDA0,
    PIN_I2C_SCL1,
    PIN_I2C_SDA1,
    PIN_CACREF,
    PIN_MAX,
} PinList;

#endif // _SOC_PIN_DEFINE_H_
