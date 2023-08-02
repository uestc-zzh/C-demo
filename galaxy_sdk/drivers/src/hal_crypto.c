/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_crypto.c
 * @brief HAL for crypto driver
 */

#include <stdlib.h>
#include "hal_crypto.h"
#include "vsd_common.h"
#include "vsd_error.h"

static CryptoDevice *g_crypto_device[CRYPTO_MAX_DEVICE] = { NULL };

static inline CryptoOperations *get_ops(uint8_t hw_id)
{
    return (CryptoOperations *)g_crypto_device[hw_id]->ops;
}

int hal_crypto_add_device(CryptoDevice *device)
{
    uint32_t i;
    uint8_t ret = VSD_ERR_FULL;
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    for (i = 0; i < sizeof(g_crypto_device) / sizeof(g_crypto_device[0]); i++) {
        // make sure hw_id is the index,avoid to get other device ops
        if (g_crypto_device[i] == NULL && (i == (device->hw_id))) {
            g_crypto_device[i] = device;
            ret                = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

CryptoDevice *hal_crypto_get_device(uint8_t hw_id)
{
    uint32_t i;
    for (i = 0; i < sizeof(g_crypto_device) / sizeof(g_crypto_device[0]); i++) {
        if (g_crypto_device[i] && (g_crypto_device[i]->hw_id == hw_id)) {
            return g_crypto_device[i];
        }
    }
    return NULL;
}

int hal_crypto_remove_device(CryptoDevice *device)
{
    uint32_t i;
    int ret = VSD_ERR_NON_EXIST;
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    for (i = 0; i < sizeof(g_crypto_device) / sizeof(g_crypto_device[0]); i++) {
        if (g_crypto_device[i] == device) {
            g_crypto_device[i] = NULL;
            ret                = VSD_SUCCESS;
            break;
        }
    }
    return ret;
}

int hal_crypto_start(CryptoDevice *device)
{
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device->hw_id)->start) {
        return VSD_ERR_GENERIC;
    }
    return get_ops(device->hw_id)->start(device);
}

int hal_crypto_stop(CryptoDevice *device)
{
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device->hw_id)->stop) {
        return VSD_ERR_GENERIC;
    }
    return get_ops(device->hw_id)->stop(device);
}

int hal_crypto_reset(CryptoDevice *device)
{
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device->hw_id)->reset) {
        return VSD_ERR_GENERIC;
    }
    return get_ops(device->hw_id)->reset(device);
}

int hal_crypto_hash(CryptoDevice *device, CryptoParams *payload,
                    CryptoData *msg, CryptoData *output)
{
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device->hw_id)->hash) {
        return VSD_ERR_GENERIC;
    }

    return get_ops(device->hw_id)->hash(device, payload, msg, output);
}

int hal_crypto_mac(CryptoDevice *device, CryptoParams *payload, CryptoData *msg,
                   CryptoData *output)
{
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device->hw_id)->mac) {
        return VSD_ERR_GENERIC;
    }

    return get_ops(device->hw_id)->mac(device, payload, msg, output);
}

int hal_crypto_blkcipher(CryptoDevice *device, CryptoParams *payload,
                         CryptoData *msg, CryptoData *output)

{
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device->hw_id)->blkcipher) {
        return VSD_ERR_GENERIC;
    }

    return get_ops(device->hw_id)->blkcipher(device, payload, msg, output);
}

int hal_crypto_trng(CryptoDevice *device, CryptoParams *payload,
                    CryptoData *output)
{
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device->hw_id)->trng) {
        return VSD_ERR_GENERIC;
    }
    return get_ops(device->hw_id)->trng(device, payload, output);
}

int hal_crypto_ecc(CryptoDevice *device, CryptoParams *payload,
                   CryptoData *output)
{
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device->hw_id)->ecc) {
        return VSD_ERR_GENERIC;
    }
    return get_ops(device->hw_id)->ecc(device, payload, output);
}

int hal_crypto_rsa(CryptoDevice *device, CryptoParams *payload,
                   CryptoData *output)
{
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device->hw_id)->rsa) {
        return VSD_ERR_GENERIC;
    }
    return get_ops(device->hw_id)->rsa(device, payload, output);
}

int hal_crypto_srp(CryptoDevice *device, CryptoParams *payload,
                   CryptoData *output)
{
    if (!device) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_ops(device->hw_id)->srp) {
        return VSD_ERR_GENERIC;
    }
    return get_ops(device->hw_id)->srp(device, payload, output);
}

DRV_ISR_SECTION
void hal_crypto_irq_handler(CryptoDevice *device, uint8_t irq_id)
{
    if (!device) {
        return;
    }
    if (!get_ops(device->hw_id)->irq_handler) {
        return;
    }
    return get_ops(device->hw_id)->irq_handler(device, irq_id);
}
