/**
 *
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_codec.h
 * @brief Public head file of CODEC HAL
 * @author Haiyong Liu<haiyong.liu@verisilicon.com>
 *
 */

#ifndef _HAL_CODEC_H
#define _HAL_CODEC_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CODEC
 *  CODEC HAL API and definition
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#include <stdint.h>
#include "vsd_common.h"
#include "hal_audio.h"

/**
 * @brief Max codec instance
 */
#define CODEC_PORT_MAX (2)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

/**
 * @enum CodecDef
 * @brief ID definition of codec interface
 */
typedef enum CodecDef {
    CODEC_ID_0 = 0,
    CODEC_ID_1,
    CODEC_ID_MAX,
} CodecDef;

/**
 * @brief Parameters of the codec
 */
typedef struct CodecParams {
    uint32_t sample_rate; /**< sample rate */
    uint32_t data_width; /**< data width */
    uint32_t sclk_rate; /**<system clock rate */
    uint32_t fmt; /**<format, @see SND_SOC_DAIFMT_* */
    uint8_t ch_num; /**<channel number */
} CodecParams;

/**
 * @brief Codec hardware configuration
 */
typedef struct CodecHWConfig {
    uint8_t id; /**<codec instance id, @see CodecDef */
} CodecHWConfig;

/**
 * @struct CodecDevice
 * @brief Define codec controller driver
 */
typedef struct CodecDevice {
    BusDevice bus_dev; /**< bus device */
    BusConfig bus_cfg; /**< bus device configurations */
    const CodecHWConfig *hw_config; /**< Codec hardware configurations */
    const void *ops; /**< codec operations */
    void *private_data; /**< driver private data */
} CodecDevice;

typedef struct CodecOperation {
    /**
     * Codec operation for config
     */
    int (*config)(const CodecDevice *codec, CodecParams *adf);

    /**
     * Codec operation for mute
     */
    int (*mute)(const CodecDevice *codec, uint8_t mute);

    /**
     * Codec operation for set gain value, unit in DB
     */
    int (*set_gain)(const CodecDevice *codec, int db_value);

    /**
     * Codec operation for close
     */
    int (*close)(const CodecDevice *codec);
} CodecOperation;

/**
 * @brief Add codec device instance
 * @param[in] dev codec device to be added
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_codec_add_dev(CodecDevice *dev);

/**
 * @brief Remove codec device instance
 * @param[in] dev codec device to be removed
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_codec_remove_dev(CodecDevice *dev);

/**
 * @brief Get codec device instance
 * @param[in] dev_id The device ID of codec, @see CodecDef
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
CodecDevice *hal_codec_get_device(uint8_t dev_id);

/**
 * @brief Config the codec
 * @param[in] dev codec device
 * @param params codec parameters
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_codec_config(const CodecDevice *dev, CodecParams *params);

/**
 * @brief Codec digital mute or unmute
 * @param[in] dev codec device
 * @param mute 1: mute, 0: unmute
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_codec_mute(const CodecDevice *dev, uint8_t mute);

/**
 * @brief Set digital volume gain.
 * @param[in] dev codec device
 * @param db_value gain value in DB
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_codec_set_gain(const CodecDevice *dev, int db_value);

/**
 * @brief Close the codec device
 * @param codec codec device
 * @return int @see VSD_SUCCESS for success, otherwise for error
 */
int hal_codec_close(const CodecDevice *codec);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
