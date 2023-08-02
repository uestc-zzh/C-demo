/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 *
 * @file vsd_common.h
 * @brief Common definition for VeriSilicon Driver
 * @author Shaobo Tu <Shaobo.Tu@verisilicon.com>
 */

#ifndef __VSD_COMMON_H__
#define __VSD_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup DRV_COMMON
 *  @brief Driver common definition
 *  @ingroup DRIVER
 *
 *  @{
 */

#include <stdbool.h>
#include <stdint.h>
#include "sys_common.h"
#include "hal_i2c.h"
#include "hal_spi.h"
#include "hal_i3c.h"

/** bits */
#ifndef BIT
#define BIT(nr) ((1ul) << (nr))
#endif

#ifndef ROUND_DOWN
#define ROUND_DOWN(a, b) (((a) / (b)) * (b))
#endif

/** compare */
#ifndef MIN
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

/** divider */
#define DIV_ROUND_UP(n, d) (((n) + (d)-1) / (d))

typedef enum { BUS_TYPE_I2C, BUS_TYPE_SPI, BUS_TYPE_I3C, BYS_TYPE_MAX } BusType;

/**
 * @struct BusDevice
 * @brief Define bus device struct
 */
typedef struct BusDevice {
    BusType type; /**< Bus type, @see BusType */
    uint8_t port_id; /**< The port id of an interface */
    union {
        SpiDevice *spi; /**< Handle of SPI device */
        const I2cDevice *i2c; /**< Handle of I2C device */
        const I3cDevice *i3c; /**< Handle of I3C device */
    };
} BusDevice;

/**
 * @struct BusConfig
 * @brief Define bus configuration struct
 */
typedef union BusConfig {
    const SpiTransConfig *spi; /**< SPI port configuration */
    const I2cConfig *i2c; /**< I2C port configuration */
    const I3cConfig *i3c; /**< I3C port configuration */
} BusConfig;

/**
 * @brief   Create a contiguous bitmask starting at bit position l and ending
 * at position h.
 * example: GENMASK(31, 21) gives us the 32bit vector 0xffe00000.
 */
#ifndef GENMASK
#define GENMASK(h, l) \
    (((~(0ul)) - ((1ul) << (l)) + 1) & (~(0ul) >> (32 - 1 - (h))))
#endif

/**
 * @brief         cast a member of a structure out to the containing structure
 * @param ptr     the pointer to the member.
 * @param type    the type of the container struct this is embedded in.
 * @param member  the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member)                    \
    ({                                                     \
        const typeof(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type, member)); \
    })

/**
 * @brief Find first bit in word.
 * @param word The word to search
 * @note Undefined if no bit exists, so code should check against 0 first.
 * @return Return result
 * @retval First bit
 */
uint32_t __ffs(uint32_t word);

/**
 * @brief log2
 * @param n Input value
 * @return Return result
 * @retval log2(n)
 */
uint8_t vs_log2(uint32_t n);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __VSD_COMMON_H__ */
