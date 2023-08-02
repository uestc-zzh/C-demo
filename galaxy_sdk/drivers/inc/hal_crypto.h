/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_crypto.h
 * @brief public head file of crypto driver
 */

#ifndef _HAL_CRYPTO_H
#define _HAL_CRYPTO_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <inttypes.h>

/** @addtogroup CRYPTO
 *  Crypto hal API.
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#define CRYPTO_MAX_DEVICE (2)
#define CRYPTO_MAX_CAP (7)

#define CRYPTO_CAP_HASH (1 << 0)
#define CRYPTO_CAP_BLKCIPHER (1 << 1)
#define CRYPTO_CAP_MAC (1 << 2)
#define CRYPTO_CAP_ECC (1 << 3)
#define CRYPTO_CAP_RSA (1 << 4)
#define CRYPTO_CAP_SRP (1 << 5)
#define CRYPTO_CAP_TRNG (1 << 6)

/**
 *
 * @brief Crypto device property
 *
 */
typedef enum CryptoDevProp {
    CRYPTO_DEV_PROP_HW, /**< Crypto hardware device*/
    CRYPTO_DEV_PROP_SW, /**< Crypto software device*/
} CryptoDevProp;

/**
 * @brief Crypto id
 *
 */
typedef enum CryptoId {
    CRYPTO_ID_0,
    CRYPTO_ID_1,
} CryptoId;

/**
 * @brief  Structure of crypto hardware config
 *
 */
typedef struct CryptoHwConfig {
    uint32_t base; /**< Device base address */
    uint8_t clk_id; /**< Clock id */
    uint8_t irq_id; /**< Irq id*/
    uint8_t hw_capability; /**< Hardware capability */
} CryptoHwConfig;

/**
 * @brief  Structure of crypto device
 *
 */
typedef struct CryptoDevice {
    uint8_t hw_id; /**< Crypto hardware id */
    uint8_t dev_prop; /**< Crypto device type(HW/SW), @see CryptoDevProp */
    const CryptoHwConfig *hw_config; /**< Hardware config */
    const void *ops; /**< Device operations */
} CryptoDevice;

/**
 * @brief  Structure of crypto data used for input/output data and size
 *
 */
typedef struct CryptoData {
    uint8_t *bytes; /**< Data ponitor which ponit to message or cipher text */
    uint32_t size; /**< Data size should in bytes */
} CryptoData;

/**
 * @brief Payload for addition data like keys,aad etc
 *
 */
typedef void CryptoParams;

/**
 * @brief Crypto operation init list
 *
 */
typedef void (*CryptoOps[CRYPTO_MAX_CAP])(const CryptoHwConfig *, uint8_t);

/**
 * @brief Structure of crypto operations
 *
 */
typedef struct CryptoOperations {
    int (*start)(CryptoDevice *device);
    int (*stop)(CryptoDevice *device);
    int (*reset)(CryptoDevice *device);
    int (*hash)(CryptoDevice *device, CryptoParams *payload, CryptoData *msg,
                CryptoData *output);
    int (*mac)(CryptoDevice *device, CryptoParams *payload, CryptoData *msg,
               CryptoData *output);
    int (*blkcipher)(CryptoDevice *device, CryptoParams *payload,
                     CryptoData *msg, CryptoData *output);
    int (*trng)(CryptoDevice *device, CryptoParams *payload,
                CryptoData *output);
    int (*srp)(CryptoDevice *device, CryptoParams *payload, CryptoData *output);
    int (*rsa)(CryptoDevice *device, CryptoParams *payload, CryptoData *output);
    int (*ecc)(CryptoDevice *device, CryptoParams *payload, CryptoData *output);

    void (*irq_handler)(CryptoDevice *device, uint8_t irq_id);
} CryptoOperations;

/**
 * @brief CRYPTO HAL function,used to start crypto device
 *
 * @param device crypto device
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_crypto_start(CryptoDevice *device);

/**
 * @brief CRYPTO HAL function,used to stop crypto device running
 *
 * @param device crypto device
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_crypto_stop(CryptoDevice *device);

/**
 * @brief CRYPTO HAL function,used to add crypto device
 *
 * @param device crypto device
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_crypto_add_device(CryptoDevice *device);

/**
 * @brief CRYPTO HAL function,used to remove device from device list
 *
 * @param device crypto device
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_crypto_remove_device(CryptoDevice *device);

/**
 * @brief CRYPTO HAL function,used to get ctypro device by hardware id
 *
 * @param hw_id crypto hardware id
 * @return crypto device,NULL for can not find the device
 */
CryptoDevice *hal_crypto_get_device(uint8_t hw_id);

/**
 * @brief CRYPTO HAL function,used to reset crypto device
 *
 * @param device crypto device
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_crypto_reset(CryptoDevice *device);

/**
 * @brief CRYPTO HAL function,used to running hash operations
 *
 * @param device crypto device
 * @param payload crypto payload
 * @param msg  input message pointer and size
 * @param output used to output data and data size
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_crypto_hash(CryptoDevice *device, CryptoParams *payload,
                    CryptoData *msg, CryptoData *output);

/**
 * @brief CRYPTO HAL function,used to running mac operations
 *
 * @param device crypto device
 * @param payload crypto payload
 * @param msg  input message pointer and size
 * @param output used to output data and data size
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_crypto_mac(CryptoDevice *device, CryptoParams *payload, CryptoData *msg,
                   CryptoData *output);

/**
 * @brief CRYPTO HAL function,used to running block cipher operations
 *
 * @param device crypto device
 * @param payload crypto payload
 * @param msg  input message pointer and size
 * @param output used to output data and data size
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_crypto_blkcipher(CryptoDevice *device, CryptoParams *payload,
                         CryptoData *msg, CryptoData *output);

/**
 * @brief CRYPTO HAL function,used to running true random number operations
 *
 * @param device crypto device
 * @param payload crypto payload
 * @param output used to output data and data size
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_crypto_trng(CryptoDevice *device, CryptoParams *payload,
                    CryptoData *output);

/**
 * @brief CRYPTO HAL function,used to running ecc operations
 *
 * @param device crypto device
 * @param payload crypto payload
 * @param output used to output data and data size
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_crypto_ecc(CryptoDevice *device, CryptoParams *payload,
                   CryptoData *output);
/**
 * @brief CRYPTO HAL function,used to running rsa operations
 *
 * @param device crypto device
 * @param payload crypto payload
 * @param output used to output data and data size
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_crypto_rsa(CryptoDevice *device, CryptoParams *payload,
                   CryptoData *output);
/**
 * @brief CRYPTO HAL function,used to running secure remote password operations
 *
 * @param device crypto device
 * @param payload crypto payload
 * @param output used to output data and data size
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_crypto_srp(CryptoDevice *device, CryptoParams *payload,
                   CryptoData *output);

/**
 * @brief CRYPTO HAL function,used to interrupt handler
 *
 * @param device crypto device
 * @param irq_id irq id
 */
void hal_crypto_irq_handler(CryptoDevice *device, uint8_t irq_id);

#ifdef __cplusplus
}
#endif
/** @} */
#endif //_HAL_CRYPTO_H
