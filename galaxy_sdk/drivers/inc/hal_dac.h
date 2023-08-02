/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_dac.h
 * @brief public head file of DAC driver
 */

#ifndef _HAL_DAC_H_
#define _HAL_DAC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vsd_error.h"
#include "vsd_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup DAC
 *  DAC controller API
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

/**
 * @brief Max DAC instance
 */
#define DAC_PORT_MAX (1)

/**
 * @brief ID definition of DAC interface
 */
typedef enum DacIdDef {
    DAC_ID_0 = 0,
    DAC_ID_MAX,
} DacIdDef;

/** limit DAC id number */
#define DAC_DEV_MAX (DAC_ID_MAX)

/**DAC DMA id number */
#define DAC_CHANNEL_NUM (2)

/**
 * @brief DAC channel type and id
 */
typedef enum {
    DAC_SINGLE_CHANNEL_0,
    DAC_SINGLE_CHANNEL_1,
    DAC_DUAL_CHANNEL,
    DAC_CHANNEL_ID_MAX,
} DacChannelId;

/**
 * @brief DAC reference selection
 */
typedef enum {
    DAC_REF_INTERNAL,
    DAC_REF_EXTERNAL, /**< both channel_0 and channel_1 */
} DacRefSel;

/**
 * @brief DAC continues mode
 */
typedef enum {
    DAC_ONE_SHOT, /**< loop one samples */
    DAC_KEEP_LAST, /**< keep the last sample */
    DAC_LOOP, /**< loop all samples */
    DAC_LOOP_BACK, /**< used in AD/DA loopback test */
} DacConMode;

/**
 * @brief Structure for DAC inputs
 * */
typedef struct DacInput {
    uint32_t *p_buffer; /**< if dual channel, p_buffer is channel 0/1
                                  Cross Write */
    uint32_t length; /**< Length in word */
} DacInput;

/**
 * @brief Structure of DAC hardware configuration
 */
typedef struct DacHwConfig {
    uint8_t id; /**< Id of DAC device, @see DacIdDef */
    uint8_t xfer_capability; /**< DAC capability, @see XferCapability  */
    uint16_t clk_id; /**< clk id */
    uint16_t irq_id; /**< Interrupt ID which defined by SoC */
    uint32_t addr; /**< Base address of DAC device */
    uint8_t mux_id_list[DAC_CHANNEL_NUM]; /**< DMA hardware handshake base id */
} DacHwConfig;

/**
 * @brief Structure of DAC configuration
 */
typedef struct {
    uint8_t ref_sel; /**< DAC reference selection @see DacRefSel */
    uint8_t xfer_mode; /**< polling, intr or dma mode, @see XferMode */
    uint8_t cont_mode; /**< LOOP, KEEP_LAST or LOOP_BACK, @see DacConMode */
    uint8_t ch_id; /**< DAC channel id, @see DacChannelId */
} DacConfig;

/**
 * @brief Structure of DAC device
 */
typedef struct DacDevice {
    const DacHwConfig *hw_config;
    DmacDevice *dmac_dev; /**<  Instance of DMAC device */
    const void *ops; /**< dac operations */
    void *ctx;
} DacDevice;

/**
 * @brief  DAC complete callback
 * @return Return result
 * @retval int VSD_SUCCESS for succeed, others for failure
 */
typedef int (*DacCompleteCallback)(void);

/**
 * @brief Structure of operations for HAL of DAC
 */
typedef struct DacOperations {
    int (*init)(const DacDevice *device, DacConfig dac_config);
    int (*enable)(const DacDevice *device, bool enable, DacCompleteCallback cb);
    /** Write D/A conversion data from the selected channel */
    int (*write)(const DacDevice *device, DacInput *dac_input);
    /** DAC Interrupt handler function */
    void (*irq_handler)(const DacDevice *device);
} DacOperations;

/**
 * @brief Add DAC device instance
 * @param device an instance of DAC device
 * @return Return result
 * @retval int VSD_SUCCESS for succeed, others for failure
 */
int hal_dac_add_dev(DacDevice *device);

/**
 * @brief Remove DAC device instance
 * @param device an instance of DAC device
 * @return Return result
 * @retval int VSD_SUCCESS for succeed, others for failure
 */
int hal_dac_remove_dev(DacDevice *device);

/**
 * @brief Get DAC device instance
 * @param hw_id hardware id of a DAC device
 * @return Return result
 * @retval DacDevice* a DacDevice instance for succeed, NULL for failure
 */
DacDevice *hal_dac_get_device(uint8_t hw_id);

/**
 * @brief Initialize DAC
 * @param device an instance of DAC device
 * @param dac_config dac config of dac device
 * @return Return result
 * @retval int VSD_SUCCESS for succeed, others for failure
 */
int hal_dac_init(const DacDevice *device, DacConfig dac_config);

/**
 * @brief Enable / Disable DAC
 * @param device an instance of DAC device
 * @param enable true or false
 * @param cb complete callback
 * @return Return result
 * @retval int VSD_SUCCESS for succeed, others for failure
 */
int hal_dac_enable(const DacDevice *device, bool enable,
                   DacCompleteCallback cb);

/**
 * @brief Write D/A conversion data from the selected channel
 * @param device an instance of dac device
 * @param dac_input A pointer to structure for DAC inputs
 * @return Return result
 * @retval int VSD_SUCCESS for succeed, others for failure
 */
int hal_dac_write(const DacDevice *device, DacInput *dac_input);

/**
 * @brief DAC interrupt handler
 * @param device an instance of DAC device
 */
void hal_dac_irq_handler(const DacDevice *device);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // _HAL_DAC_H_
