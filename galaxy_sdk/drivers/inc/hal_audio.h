/**
 *
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_audio.h
 * @brief Public head file of audio HAL
 * @author Haiyong Liu<haiyong.liu@verisilicon.com>
 *
 */

#ifndef _HAL_AUDIO_H
#define _HAL_AUDIO_H

/** @addtogroup AUDIO
 *  Audio HAL API and definition
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#define SND_SOC_DAIFMT_I2S 1 /**< I2S mode */
#define SND_SOC_DAIFMT_DSP_A 4 /**< L data MSB after FRM LRC */
#define SND_SOC_DAIFMT_PDM 7 /**< Pulse density modulation */

/*
 * DAI hardware clock masters.
 *
 * This is wrt the codec, the inverse is true for the interface
 * i.e. if the codec is clk and FRM master then the interface is
 * clk and frame slave.
 */
#define SND_SOC_DAIFMT_CBM_CFM (1 << 12) /**< codec clk & FRM master */
#define SND_SOC_DAIFMT_CBS_CFM (2 << 12) /**< codec clk slave & FRM master */
#define SND_SOC_DAIFMT_CBM_CFS (3 << 12) /**< codec clk master & FRM slave */
#define SND_SOC_DAIFMT_CBS_CFS (4 << 12) /**< codec clk & FRM slave */

#define SND_SOC_DAIFMT_FORMAT_MASK 0x000f
#define SND_SOC_DAIFMT_MASTER_MASK 0xf000

/** @} */

#endif /* _HAL_AUDIO_H */
