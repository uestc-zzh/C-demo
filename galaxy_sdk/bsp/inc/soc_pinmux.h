/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file soc_pinmux.h
 * @brief SoC pinmux definition & API
 */
#ifndef __SOC_PINMUX_H__
#define __SOC_PINMUX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#ifndef BIT
#define BIT(n) ((1ul) << (n))
#endif

/** @addtogroup SOC
 *  @brief SoC pinmux definition and API
 *  @ingroup BSP
 *
 *  @{
 */

#define SOC_PAD(i, cfg) \
    {                   \
        (i), (cfg)      \
    }

/* Pinmux bit set */
#define SW_DS(x) (x) /**< Pad drive strength control*/
#define SW_IN BIT(2) /**< Pad input force on */
#define SW_ST BIT(3) /**< Pad schmitt trigger enable */
#define SW_PD BIT(4) /**< Pad pull down enable */
#define SW_PU BIT(5) /**< Pad pull up enable */
#define SW_OD BIT(6) /**< Pad open drain enable */
#define SW_MUX(x) (x << 7) /**< Pad mux select */
#define SW_IOKEEP BIT(12) /**< IO keep enable */

typedef enum SocInterfaceDef {
    SOC_IF_GPIO = 0,
    SOC_IF_HOST_JTAG,
    SOC_IF_BLE_JTAG,
    SOC_IF_AUDIO_JTAG,
    SOC_IF_UART,
    SOC_IF_QSPI,
    SOC_IF_SPI,
    SOC_IF_I2C,
    SOC_IF_I3C,
    SOC_IF_PWM,
    SOC_IF_USB,
    SOC_IF_I2S,
    SOC_IF_PDM,
    SOC_IF_DAC_OUT,
    SOC_IF_ADC_IN,
    SOC_IF_MAX
} SocInterfaceDef;

/**
 * @brief SoC Interface Info
 * @note It's an unified definition for all kinds of interfaces
 */
typedef union SocIfInfo {
    /** PWM interface info struct */
    struct {
        uint16_t device_id;
        uint16_t channel;
    } pwm;
    /** Common interface port */
    uint32_t port;
} SocIfInfo;

typedef struct GpioMuxCfg {
    SocIfInfo info; /**< Port info */
    uint32_t value; /**< Value of pin setting */
} GpioMuxCfg;

typedef struct SocPinCtl {
    unsigned char idx; /* Pin control index or pin ID */
    uint32_t cfg; /* Config register of pinmux */
} SocPinCtl;

/**
 * @brief Init the SoC to default pinmux configuration
 * @return VSD_SUCCESS for success, others for failure
 */
int soc_pinmux_init(void);

/**
 * @brief Config pinmux for all necessary pins
 * @param pin_ctl The control table of all pins
 * @param pin_num Total pin numbers to control
 * @return VSD_SUCCESS for success, others for failure
 */
int soc_pinmux_config(const SocPinCtl *pin_ctl, uint32_t pin_num);

/**
 * @brief Configure pinmux for specific interface and port ID
 *
 * @param soc_if The interface ID, @see SocInterfaceDef
 * @param info The pointer of interface info which is defined by @see
 * SocIfInfo
 * @param value By default, 1 for enable, 0 for disable. GPIO has special
 * definition according to SoC pinmux design
 * @return VSD_SUCCESS for success, others for failure
 */
int soc_pinmux_set(uint32_t soc_if, SocIfInfo *info, uint32_t value);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
