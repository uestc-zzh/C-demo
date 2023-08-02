/**
 * Copyright (C) 2021 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_dmac.c
 * @brief HAL for DMAC
 */

#include <stdlib.h>
#include <string.h>
#include "hal_dmac.h"
#include "vsd_error.h"

static DmacDevice *g_dmac_dev[DMAC_ID_MAX] = { NULL };
static DmacXferCfg work_xfer_cfg[AHB_DMAC_CHN_NUM];

static inline DmacOperation *get_ops(const DmacDevice *device)
{
    return (DmacOperation *)device->ops;
}

static void dmac_ch_default_config(const DmacDevice *device, DmacDevType src,
                                   DmacDevType dst, DmacXferCfg *xfer_cfg)
{
    xfer_cfg->cfg_reg.fc_mode    = FCMODE_EN;
    xfer_cfg->ctl_reg.llp_src_en = DMA_LLI_DISABLE;
    xfer_cfg->ctl_reg.llp_dst_en = DMA_LLI_DISABLE;
    xfer_cfg->ctl_reg.chn_intr   = DMA_INTERRUPT_ENABLE;
    xfer_cfg->cfg_reg.prio       = DMA_PRIORITY_DEFAULT;

    /* Mem to Mem */
    if ((src == DMA_PERI_MEM) && (dst == DMA_PERI_MEM)) {
        xfer_cfg->ctl_reg.dinc  = DMA_ADDR_INC;
        xfer_cfg->ctl_reg.sinc  = DMA_ADDR_INC;
        xfer_cfg->ctl_reg.tt_fc = TT_M2M_FC_DMA;
        xfer_cfg->dir           = DMA_MEM_TO_MEM;
        xfer_cfg->src_is_mem    = DMA_PERI_MEM;
        xfer_cfg->dst_is_mem    = DMA_PERI_MEM;
    }
    /* Mem to Peripheral */
    else if ((src == DMA_PERI_MEM) && (dst != DMA_PERI_MEM)) {
        xfer_cfg->ctl_reg.dinc  = DMA_ADDR_FIX;
        xfer_cfg->ctl_reg.sinc  = DMA_ADDR_INC;
        xfer_cfg->ctl_reg.tt_fc = TT_M2P_FC_DMA;
        xfer_cfg->dir           = DMA_MEM_TO_DEV;
        xfer_cfg->src_is_mem    = DMA_PERI_MEM;
        xfer_cfg->dst_is_mem    = DMA_PERI_NON_MEM;
    }
    /* Peripheral to Mem */
    else if ((src != DMA_PERI_MEM) && (dst == DMA_PERI_MEM)) {
        xfer_cfg->ctl_reg.dinc  = DMA_ADDR_INC;
        xfer_cfg->ctl_reg.sinc  = DMA_ADDR_FIX;
        xfer_cfg->ctl_reg.tt_fc = TT_P2M_FC_DMA;
        xfer_cfg->dir           = DMA_DEV_TO_MEM;
        xfer_cfg->src_is_mem    = DMA_PERI_NON_MEM;
        xfer_cfg->dst_is_mem    = DMA_PERI_MEM;
    }
}

int hal_dmac_add_dev(DmacDevice *device)
{
    int ret = VSD_ERR_FULL;
    uint8_t i;

    for (i = 0; i < sizeof(g_dmac_dev) / sizeof(g_dmac_dev[0]); i++) {
        if (g_dmac_dev[i] == NULL) {
            g_dmac_dev[i]    = device;
            device->xfer_cfg = &work_xfer_cfg[0];
            ret              = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

int hal_dmac_remove_dev(DmacDevice *device)
{
    int ret = VSD_ERR_NON_EXIST;
    uint8_t i;

    for (i = 0; i < sizeof(g_dmac_dev) / sizeof(g_dmac_dev[0]); i++) {
        if (g_dmac_dev[i] == device) {
            g_dmac_dev[i] = NULL;
            ret           = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

DmacDevice *hal_dmac_get_device(uint8_t device_id)
{
    DmacDevice *dmac = NULL;
    uint8_t i;

    for (i = 0; i < sizeof(g_dmac_dev) / sizeof(g_dmac_dev[0]); i++) {
        if (g_dmac_dev[i] && (g_dmac_dev[i]->device_id == device_id)) {
            dmac = g_dmac_dev[i];
            break;
        }
    }
    return dmac;
}

DmacXferCfg *hal_dmac_get_resource(const DmacDevice *device, DmacDevType src,
                                   DmacDevType dst, uint32_t fifo_width,
                                   uint32_t mux_id, uint32_t block_ts)
{
    uint8_t chn_id;
    uint8_t channel_sum   = device->hw_cfg->ch_sum;
    DmacChnMap *ch_map    = device->ch_map;
    DmacXferCfg *xfer_cfg = NULL;

    if (!device || !ch_map || (block_ts > device->max_blk_ts) || (block_ts < 0))
        return xfer_cfg;
    if (!((device->hw_cfg->width_capability) & (1 << fifo_width)) ||
        !get_ops(device)->get_resource)
        return xfer_cfg;

    chn_id = get_ops(device)->get_resource(device, src, dst);

    if (chn_id != channel_sum) {
        xfer_cfg                         = &work_xfer_cfg[chn_id];
        xfer_cfg->chn_id                 = ch_map[chn_id].ch_id;
        xfer_cfg->block_ts               = block_ts;
        xfer_cfg->ctl_reg.src_xfer_width = fifo_width;
        xfer_cfg->ctl_reg.dst_xfer_width = fifo_width;
        xfer_cfg->mux_id                 = mux_id;
        xfer_cfg->cfg_reg.src_hs_id      = xfer_cfg->chn_id;
        xfer_cfg->cfg_reg.dst_hs_id      = xfer_cfg->chn_id;
        dmac_ch_default_config(device, src, dst, xfer_cfg);
    }
    return xfer_cfg;
}

int hal_dmac_init(const DmacDevice *device, DmacXferCfg *xfer_cfg,
                  DmaInitCfg *init_cfg)
{
    if (!device || !xfer_cfg || !init_cfg)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->init)
        return VSD_ERR_UNSUPPORTED;

    xfer_cfg->src_addr    = init_cfg->src_addr;
    xfer_cfg->dst_addr    = init_cfg->dst_addr;
    xfer_cfg->len         = init_cfg->len;
    xfer_cfg->trigger_lvl = init_cfg->trigger_lvl;
    xfer_cfg->is_cyclic   = init_cfg->is_cyclic;

    return (get_ops(device)->init(device, xfer_cfg));
}

int hal_dmac_start(const DmacDevice *device, DmacXferCfg *xfer_cfg,
                   DmaCbAndParam *xfer_cb)
{
    if (!device || !xfer_cfg)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->start)
        return VSD_ERR_UNSUPPORTED;

    return (get_ops(device)->start(device, xfer_cfg, xfer_cb));
}

int hal_dmac_stop(const DmacDevice *device, DmacXferCfg *xfer_cfg)
{
    if (!device || !xfer_cfg)
        return VSD_ERR_INVALID_POINTER;
    if (!get_ops(device)->stop)
        return VSD_ERR_UNSUPPORTED;

    return (get_ops(device)->stop(device, xfer_cfg));
}

DRV_ISR_SECTION
void hal_dmac_isr_handler(const DmacDevice *device)
{
    if (!device || !get_ops(device)->isr_handler)
        return;
    return get_ops(device)->isr_handler(device);
}
