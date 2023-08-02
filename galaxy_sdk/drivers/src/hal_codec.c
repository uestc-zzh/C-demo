/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_codec.c
 * @brief HAL for codec driver
 * @author Haiyong Liu<haiyong.liu@verisilicon.com>
 */

#include <string.h>
#include "hal_codec.h"
#include "vsd_error.h"

static CodecDevice *g_codec_dev[CODEC_PORT_MAX] = { NULL };

static inline CodecOperation *get_ops(const CodecDevice *dev)
{
    return (CodecOperation *)dev->ops;
}

int hal_codec_add_dev(CodecDevice *dev)
{
    int ret   = VSD_ERR_FULL;
    uint8_t i = 0;

    for (i = 0; i < ARRAY_SIZE(g_codec_dev); i++) {
        if (g_codec_dev[i] == NULL) {
            g_codec_dev[i] = dev;
            ret            = VSD_SUCCESS;
            break;
        }
    }

    return ret;
}

int hal_codec_remove_dev(CodecDevice *dev)
{
    int ret   = VSD_ERR_NON_EXIST;
    uint8_t i = 0;

    for (i = 0; i < ARRAY_SIZE(g_codec_dev); i++) {
        if (g_codec_dev[i] == dev) {
            g_codec_dev[i] = NULL;
            ret            = VSD_SUCCESS;
            break;
        }
    }

    return ret;
}

CodecDevice *hal_codec_get_device(uint8_t dev_id)
{
    uint8_t i;

    for (i = 0; i < ARRAY_SIZE(g_codec_dev); i++) {
        if (g_codec_dev[i] && g_codec_dev[i]->hw_config &&
            g_codec_dev[i]->hw_config->id == dev_id)
            return g_codec_dev[i];
    }

    return NULL;
}

int hal_codec_config(const CodecDevice *dev, CodecParams *params)
{
    if (get_ops(dev)->config)
        return get_ops(dev)->config(dev, params);

    return VSD_SUCCESS;
}

int hal_codec_mute(const CodecDevice *dev, uint8_t mute)
{
    if (get_ops(dev)->mute)
        return get_ops(dev)->mute(dev, mute);

    return VSD_SUCCESS;
}

int hal_codec_set_gain(const CodecDevice *dev, int db_value)
{
    if (get_ops(dev)->set_gain)
        return get_ops(dev)->set_gain(dev, db_value);

    return VSD_SUCCESS;
}

int hal_codec_close(const CodecDevice *dev)
{
    if (get_ops(dev)->close)
        return get_ops(dev)->close(dev);

    return VSD_SUCCESS;
}
