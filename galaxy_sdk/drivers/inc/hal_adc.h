/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_adc.h
 * @brief Public head file of HAL for ADC
 */

#ifndef _HAL_ADC_H_
#define _HAL_ADC_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup ADC
 *  ADC HAL API and definition
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#include <stdint.h>
#include <stdbool.h>
#include "hal_common.h"
#include "hal_dmac.h"

#define ADC_DEV_MAX (ADC_ID_MAX) /**< Limit ADC id number */

/**
 * @enum AdcIdDef
 * @brief ID definition of ADC interface
 */
typedef enum AdcIdDef {
    ADC_ID_0,
    ADC_ID_MAX,
} AdcIdDef;

/**
 * @enum AdcRefSel
 * @brief ADC analog reference pair selection signals
 */
typedef enum {
    ADC_VREFH_VREFL, /**< Use a fixed voltage as the reference */
    ADC_AVDD_AGND, /**< Using analog voltage as the reference, the MCU AVDD
                      provides it */
} AdcRefSel;

/**
 * @enum AdcConvMode
 * @brief ADC conversion mode
 */
typedef enum {
    ADC_SINGLE_CONV_MODE, /**< A channel can only convert data once */
    ADC_CONTS_CONV_MODE, /**< A channel can continuously convert data */
} AdcConvMode;

/**
 * @enum AdcTrigMode
 * @brief ADC trigger mode
 */
typedef enum {
    ADC_SW_TRIGGER, /**< The software triggers the start of sampling */
    ADC_EXT_TRIGGER, /**< External hardware triggers the start of sampling */
} AdcTrigMode;

/**
 * @enum AdcInputMode
 * @brief ADC analog signals input mode, control power consumption
 */
typedef enum {
    ADC_IN_BUFFER_MODE, /**< Analog input signals go through the input buffer to
                           ADC core */
    ADC_BYPASS_MODE, /**< Analog input signals directly go to ADC core, the
                        input buffer is bypass */
} AdcInputMode;

/**
 * @enum AdcConvVol
 * @brief ADC self-diagnosis conversion voltage select
 */
typedef enum {
    ADC_VREFL_IN      = 1, /**< Low reference voltage */
    ADC_VREFH_IN_HALF = 2, /**< The half of high reference voltage */
    ADC_VREFH_IN      = 3, /**< High reference voltage */
} AdcConvVol;

/**
 * @enum AdcChannelId
 * @brief ADC channel id
 */
typedef enum {
    ADC_CHANNEL_ID_0, /**< Id of channel_0 */
    ADC_CHANNEL_ID_1, /**< Id of channel_1 */
    ADC_CHANNEL_ID_2, /**< Id of channel_2 */
    ADC_CHANNEL_ID_3, /**< Id of channel_3 */
    ADC_CHANNEL_ID_4, /**< Id of channel_4 */
    ADC_CHANNEL_ID_5, /**< Id of channel_5 */
    ADC_CHANNEL_ID_6, /**< Id of channel_6 */
    ADC_CHANNEL_ID_7, /**< Id of channel_7 */
    ADC_CHANNEL_ID_8, /**< Id of channel_8 */
    ADC_CHANNEL_ID_9, /**< Id of channel_9 */
    ADC_CHANNEL_ID_MAX, /**< Maximum number of channels */
} AdcChannelId;

/**
 * @enum AdcValueMode
 * @brief ADC channel A/D conversion value mode
 */
typedef enum {
    ADC_NORMAL_MODE, /**< Normal mode, the converted data is reflected by the
                        current sample */
    ADC_AVERAGE_MODE, /**< Average mode, the converted data is reflected the
                         average by the past several times(@see
                         AdcAverageCnt) sample */
    ADC_ADDITIONAL_MODE, /**< Additional mode, the converted data is reflected
                            the additional value by the past several times(@see
                         AdcAdditionCnt) sample */
} AdcValueMode;

/**
 * @enum AdcAverageCnt
 * @brief Select the average times
 */
typedef enum {
    ADC_AVR_TWO_TIMES, /**< 2 times */
    ADC_AVR_FOUR_TIMES, /**< 4 times */
    ADC_AVR_EIGHT_TIMES, /**< 8 times */
    ADC_AVR_INVAID, /**< No select ADC average mode, @see AdcValueMode */
} AdcAverageCnt;

/**
 * @enum AdcAdditionCnt
 * @brief Select the addition times
 */
typedef enum {
    ADC_ADD_TWO_TIMES, /**< 2 times */
    ADC_ADD_FOUR_TIMES, /**< 4 times */
    ADC_ADD_EIGHT_TIMES, /**< 8 times */
    ADC_ADD_INVAID, /**< No select ADC additional mode, @see AdcValueMode */
} AdcAdditionCnt;

/**
 * @enum AdcChannelUse
 * @brief Select channels use
 */
typedef enum {
    ADC_ONLY_READ, /**< Channels are only used to read data  */
    ADC_ONLY_COMPARE, /**< Channels are only used for window comparison */
    ADC_READ_COMPARE, /**< Channels ares used for both window comparison and
                         reading data */
} AdcChannelUse;

/**
 * @brief ADC interrupt callback function only for window compare
 * @param cb_ctx callback context
 */
typedef void (*AdcIrqCallback)(void *cb_ctx);

/**
 * @brief Define ADC sampling data
 */
typedef struct AdcSamplingData {
    uint32_t *buf; /**< Pointer of buffer */
    uint32_t buf_len; /**< Data size in buffer */
} AdcSamplingData;

/**
 * @brief Define ADC channel configuration
 */
typedef struct {
    uint8_t val_mode; /**< Value mode @see AdcValueMode */
    uint8_t avr_cnt; /**< The average times @see AdcAverageCnt */
    uint8_t add_cnt; /**< The addition times @see AdcAdditionCnt */

    /** If win_mode == 0, only LLV is valid, ULV is invalid. The A/D conversion
     * data is divided to two parts: <=LLV, or >LLV.
     * If win_mode == 1, both LLV and ULV are valid. The A/D conversion data is
     * divided to three parts: <= LLV, LLV < A/D conversion data <= ULV, > ULV.
     */
    uint8_t win_mode;

    /** If cond_mode == 0, it has two cases, (1)win_mode == 0, the condition
     * is defined as A/D conversion data <= LLV; (2)win_mode == 1, the condition
     * is defined as A/D conversion data <= LLV; or >= ULV.
     * If cond_mode == 1, it has two cases, (1)win_mode == 0, the condition is
     * defined as A/D conversion data >= LLV; (2)win_mode == 1, the condition is
     * defined as LLV <= A/D conversion data <= ULV.
     */
    uint8_t cond_mode;

    uint16_t llv; /**< Lower-Limit Value */
    uint16_t ulv; /**< Upper-Limit Value */
} AdcChannelConfig;

/**
 * @brief Define ADC channels configuration
 */
typedef struct {
    AdcChannelConfig *chn_cfg; /**< Configuration of selected channels
                                */
    AdcSamplingData *samp_data; /**< Sampling data */
    AdcIrqCallback cb; /**< ADC interrupt callback function */
    void *cb_context; /**< Callback context */
    uint16_t chn_mask; /** Indicate selected channels */
    uint8_t use_mode; /**< Channels use mode, @see AdcChannelUse */
    uint8_t xfer_mode; /**< Transfer mode, @see XferModeDef */
} AdcChannelsCfg;

/**
 * @brief Define ADC sampling configuration
 */
typedef struct {
    uint8_t ref_sel; /**< Reference pair selection signals @see AdcRefSel */
    uint8_t trig_mode; /**< Trigger mode @see AdcTrigMode */
    uint8_t input_mode; /**< Analog signals input mode @see AdcInputMode */
    uint8_t conv_mode; /**< Conversion mode @see AdcConvMode */
} AdcSamplingConfig;

/**
 * @brief Define ADC hardware configuration
 */
typedef struct {
    uint32_t addr; /**< Register base address */
    uint16_t irq_id; /**< Interrupt ID which defined by SoC */
    uint8_t id; /**< Id of ADC device, @see AdcIdDef */
    uint8_t clk_id; /**< Clock ID of ADC interface */
    uint8_t xfer_capability; /**< ADC capability, @see XferCapability */
    uint8_t dma_mux_id[ADC_CHANNEL_ID_MAX - 2]; /**< DMA hardware handshake
                                                    base id */
    uint8_t gpd_id; /**< ADC LDO gpd id */
} AdcHwConfig;

/**
 * @struct AdcDevice
 * @brief Structure of ADC device
 */
typedef struct AdcDevice {
    const AdcHwConfig *hw_config; /**< ADC hardware configuration */
    DmacDevice *dmac_dev; /**<  Instance of DMAC device */
    const void *ops; /**< Operations of ADC driver */
    void *ctx; /**< Context */
} AdcDevice;

/**
 * @brief Structure of operations for HAL of ADC
 */
typedef struct AdcOperations {
    /** Control ADC power */
    int (*power_ctrl)(const AdcDevice *device, bool pwr_ctrl);
    /** Calibrate */
    int (*calibrate)(const AdcDevice *device);
    /** Self-diagnosis */
    int (*self_diagnosis)(const AdcDevice *device, uint8_t vol_sel,
                          uint32_t *value);
    /** Configure the sampling */
    int (*config_sampling)(const AdcDevice *device,
                           const AdcSamplingConfig *samp_cfg);
    /** Read A/D conversion data from the selected channels */
    int (*read)(const AdcDevice *device, const AdcSamplingConfig *samp_cfg,
                const AdcChannelsCfg *channels_cfg);
    /** Start sampling */
    int (*start)(const AdcDevice *device, const AdcSamplingConfig *samp_cfg,
                 const AdcChannelsCfg *channels_cfg);
    /** Stop sampling */
    int (*stop)(const AdcDevice *device, const AdcSamplingConfig *samp_cfg,
                const AdcChannelsCfg *channels_cfg);
    /** ADC Interrupt handler function */
    void (*irq_handler)(const AdcDevice *device);
} AdcOperations;

/**
 * @brief Add an ADC device instance
 * @param device an instance of ADC device need to be added
 * @return Return result
 * @retval int VSD_SUCCESS for succeed, others for failure
 */
int hal_adc_add_dev(AdcDevice *device);

/**
 * @brief Remove an ADC device
 * @param device an instance of ADC device need to be removed
 * @return Return result
 * @retval int VSD_SUCCESS for succeed, others for failure
 */
int hal_adc_remove_dev(AdcDevice *device);

/**
 * @brief Get an ADC device
 * @param id id of an ADC device
 * @return Return result
 * @retval AdcDevice* an AdcDevice instance for succeed, NULL for failure
 */
AdcDevice *hal_adc_get_device(uint8_t id);

/**
 * @brief Control ADC power on or down
 * @param device an instance of ADC device
 * @param pwr_ctrl true: power on, false: power down
 * @return Return result
 * @retval int VSD_SUCCESS for succeed, others for failure
 */
int hal_adc_power(const AdcDevice *device, bool pwr_ctrl);

/**
 * @brief Calibrate
 * @param device an instance of ADC device
 * @return Return result
 * @retval int VSD_SUCCESS for succeed, others for failure
 */
int hal_adc_calibrate(const AdcDevice *device);

/**
 * @brief ADC self-diagnosis
 * @param device an instance of ADC device which starts self-diagnosis
 * @param vol_sel self-diagnosis conversion voltage, @see AdcConvVol
 * @param value storage A/D conversion data out of self-diagnosis
 * @return Return result
 * @retval int VSD_SUCCESS for succeed, others for failure
 */
int hal_adc_self_diagnosis(const AdcDevice *device, uint8_t vol_sel,
                           uint32_t *value);

/**
 * @brief Configure sampling
 * @param device an instance of ADC device
 * @param samp_cfg the configuration of sampling
 * @return Return result
 * @retval int VSD_SUCCESS for succeed, others for failure
 */
int hal_adc_config_sampling(const AdcDevice *device,
                            const AdcSamplingConfig *samp_cfg);

/**
 * @brief Start sampling, read data from the selected channels
 * @param device an instance of ADC device
 * @param samp_cfg the configuration of sampling
 * @param channels_cfg the configuration of channels
 * @return Return result
 * @retval int VSD_SUCCESS for succeed, others for failure
 */
int hal_adc_read(const AdcDevice *device, const AdcSamplingConfig *samp_cfg,
                 const AdcChannelsCfg *channels_cfg);

/**
 * @brief Start sampling
 * @param device an instance of ADC device
 * @param samp_cfg the configuration of sampling
 * @param channels_cfg the configuration of channels
 * @return Return result
 * @retval int VSD_SUCCESS for succeed, others for failure
 */
int hal_adc_start(const AdcDevice *device, const AdcSamplingConfig *samp_cfg,
                  const AdcChannelsCfg *channels_cfg);

/**
 * @brief Stop sampling
 * @param device an instance of ADC device
 * @param samp_cfg the configuration of sampling
 * @param channels_cfg the configuration of channels
 * @return Return result
 * @retval int VSD_SUCCESS for succeed, others for failure
 */
int hal_adc_stop(const AdcDevice *device, const AdcSamplingConfig *samp_cfg,
                 const AdcChannelsCfg *channels_cfg);

/**
 * @brief ADC interrupt handler
 * @param device an instance of ADC device
 */
void hal_adc_irq_handler(const AdcDevice *device);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // _HAL_ADC_H_
