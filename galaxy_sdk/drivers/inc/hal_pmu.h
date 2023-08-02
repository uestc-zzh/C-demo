/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_pmu.h
 * @brief Header of power management unit HAL
 */

#ifndef __HAL_PMU_H__
#define __HAL_PMU_H__

/** @addtogroup PMU
 *  PMU HAL API and definition
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#include <stdint.h>
#include <stdbool.h>

/**
 * @enum PmuType
 * @brief Type of power management
 */
enum PmuType {
    PMU_TYPE_BLE_SYSCTRL, /**< Controlled by BLE system */
    PMU_TYPE_IP, /**< Independent PMU IP */
    PMU_TYPE_COUNT
};

/**
 * @enum PmuWakeUpSource
 * @brief Wakeup source
 */
enum PmuWakeUpSource {
    WAKEUP_SOURCE_RTC,
    WAKEUP_SOURCE_EXT_IO_0, /**< External IRQ */
    WAKEUP_SOURCE_EXT_IO_1,
    WAKEUP_SOURCE_EXT_IO_2,
    WAKEUP_SOURCE_EXT_IO_3,
    WAKEUP_SOURCE_EXT_IO_4,
    WAKEUP_SOURCE_EXT_IO_5,
    WAKEUP_SOURCE_EXT_IO_6,
    WAKEUP_SOURCE_COUNT,
    WAKEUP_SOURCE_UNKNOWN,
};

/**
 * @enum PmuLpsstDsbyCfg
 * @brief Low power state switch trigger(LPSST) items for exiting hardware deep
 * software standby(DSBY) mode
 */
enum PmuLpsstDsbyCfg {
    DSBY_IRQ_DS_0 = 0,
    DSBY_IRQ_DS_1,
    DSBY_IRQ_DS_2,
    DSBY_IRQ_DS_3,
    DSBY_IRQ_DS_4,
    DSBY_IRQ_DS_5,
    DSBY_IRQ_DS_6,
    DSBY_IRQ_DS_7,
    DSBY_IRQ_DS_8,
    DSBY_IRQ_DS_9,
    DSBY_IRQ_DS_10,
    DSBY_IRQ_DS_11,
    DSBY_IRQ_DS_12,
    DSBY_IRQ_DS_13,
    DSBY_IRQ_DS_14,
    DSBY_IRQ_DS_15,
    DSBY_RTC, /* RTC and RTC wakeup interrupt */
    DSBY_NMI,
    DSBY_MAILBOX_ZSP_REQ,
    DSBY_IRQ_COUNT,
};

/**
 * @enum PmuLpsstSbySnzCfgPin
 * @brief Low power state switch trigger(LPSST) items about pins IRQ for exiting
 * hardware software standby(SBY) or snooze(SNZ) mode
 */
enum PmuLpsstSbySnzCfgPin {
    SBYSNZ_IRQ_DS_0 = 0,
    SBYSNZ_IRQ_DS_1,
    SBYSNZ_IRQ_DS_2,
    SBYSNZ_IRQ_DS_3,
    SBYSNZ_IRQ_DS_4,
    SBYSNZ_IRQ_DS_5,
    SBYSNZ_IRQ_DS_6,
    SBYSNZ_IRQ_DS_7,
    SBYSNZ_IRQ_DS_8,
    SBYSNZ_IRQ_DS_9,
    SBYSNZ_IRQ_DS_10,
    SBYSNZ_IRQ_DS_11,
    SBYSNZ_IRQ_DS_12,
    SBYSNZ_IRQ_DS_13,
    SBYSNZ_IRQ_DS_14,
    SBYSNZ_IRQ_DS_15,
    SBYSNZ_PIN_IRQ_16,
    SBYSNZ_PIN_IRQ_17,
    SBYSNZ_PIN_IRQ_18,
    SBYSNZ_PIN_IRQ_19,
    SBYSNZ_PIN_IRQ_20,
    SBYSNZ_PIN_IRQ_21,
    SBYSNZ_PIN_IRQ_22,
    SBYSNZ_PIN_IRQ_23,
    SBYSNZ_PIN_IRQ_24,
    SBYSNZ_PIN_IRQ_25,
    SBYSNZ_PIN_IRQ_26,
    SBYSNZ_PIN_IRQ_27,
    SBYSNZ_PIN_IRQ_28,
    SBYSNZ_PIN_IRQ_29,
    SBYSNZ_PIN_IRQ_30,
    SBYSNZ_PIN_IRQ_31,
    SBYSNZ_PIN_IRQ_COUNT,
};

/**
 * @enum PmuLpsstSbySnzCfgIpGroup1
 * @brief Low power state switch trigger(LPSST) items about IP IRQ for exiting
 * hardware software standby(SBY) or snooze(SNZ) mode
 */
enum PmuLpsstSbySnzCfgIpGroup1 {
    SBYSNZ_NMI,
    SBYSNZ_WDT,
    SBYSNZ_IWDT,
    SBYSNZ_USB,
    SBYSNZ_RTC,
    SBYSNZ_I2C,
    SBYSNZ_I3C,
    SBYSNZ_UART0,
    SBYSNZ_UART1,
    SBYSNZ_UART2,
    SBYSNZ_UART3,
    SBYSNZ_EFUSE,
    SBYSNZ_I2S,
    SBYSNZ_PWM,
    SBYSNZ_CAC,
    SBYSNZ_DMA,
    SBYSNZ_CRYP_INTR0,
    SBYSNZ_CRYP_INTR1,
    SBYSNZ_CRYP_INTR2,
    SBYSNZ_CRYP_INTR3,
    SBYSNZ_BLE,
    SBYSNZ_QSPI,
    SBYSNZ_SPI0,
    SBYSNZ_SPI1,
    SBYSNZ_SPI2,
    SBYSNZ_SPI3,
    SBYSNZ_GPIO0,
    SBYSNZ_ADC,
    SBYSNZ_I2C_A,
    SBYSNZ_PDM,
    SBYSNZ_IP_IRQ_GROUP1_COUNT
};

/**
 * @enum PmuLpsstSbySnzCfgIpGroup2
 * @brief Low power state switch trigger(LPSST) items about IP IRQ for exiting
 * hardware software standby(SBY) or snooze(SNZ) mode
 */
enum PmuLpsstSbySnzCfgIpGroup2 {
    SBYSNZ_MAILBOX_CPU_REQ,
    SBYSNZ_MAILBOX_ZSP_ACK,
    SBYSNZ_MAILBOX_ZSP_REQ,
    SBYSNZ_MAILBOX_CPU_ACK,
    SBYSNZ_TIMER16_INTR0,
    SBYSNZ_TIMER16_INTR1,
    SBYSNZ_TIMER16_INTR2,
    SBYSNZ_TIMER16_INTR3,
    SBYSNZ_TIMER16_INTR4,
    SBYSNZ_TIMER32_INTR0,
    SBYSNZ_TIMER32_INTR1,
    SBYSNZ_TIMER32_INTR2,
    SBYSNZ_TIMER32_INTR3,
    SBYSNZ_TIMER32_INTR4,
    SBYSNZ_DAC,
    SBYSNZ_GPIO1,
    SBYSNZ_GPIO2,
    SBYSNZ_IP_IRQ_GROUP2_COUNT,
};

/**
 * @enum PmuLpsstSnzReqCfgGroup0
 * @brief Low power state switch trigger items for Snooze request
 */
enum PmuLpsstSnzReqCfgGroup0 {
    SNZ_REQ_UART0_TX_DMA_REQ,
    SNZ_REQ_UART0_RX_DMA_REQ,
    SNZ_REQ_UART0_TX_DMA_SINGLE,
    SNZ_REQ_UART0_RX_DMA_SINGLE,
    SNZ_REQ_UART1_TX_DMA_REQ,
    SNZ_REQ_UART1_RX_DMA_REQ,
    SNZ_REQ_UART1_TX_DMA_SINGLE,
    SNZ_REQ_UART1_RX_DMA_SINGLE,
    SNZ_REQ_UART2_TX_DMA_REQ,
    SNZ_REQ_UART2_RX_DMA_REQ,
    SNZ_REQ_UART2_TX_DMA_SINGLE,
    SNZ_REQ_UART2_RX_DMA_SINGLE,
    SNZ_REQ_UART3_TX_DMA_REQ,
    SNZ_REQ_UART3_RX_DMA_REQ,
    SNZ_REQ_UART3_TX_DMA_SINGLE,
    SNZ_REQ_UART3_RX_DMA_SINGLE,
    SNZ_REQ_PIN_IRQ_16,
    SNZ_REQ_PIN_IRQ_17,
    SNZ_REQ_PIN_IRQ_18,
    SNZ_REQ_PIN_IRQ_19,
    SNZ_REQ_PIN_IRQ_20,
    SNZ_REQ_PIN_IRQ_21,
    SNZ_REQ_PIN_IRQ_22,
    SNZ_REQ_PIN_IRQ_23,
    SNZ_REQ_PIN_IRQ_24,
    SNZ_REQ_PIN_IRQ_25,
    SNZ_REQ_PIN_IRQ_26,
    SNZ_REQ_PIN_IRQ_27,
    SNZ_REQ_PIN_IRQ_28,
    SNZ_REQ_PIN_IRQ_29,
    SNZ_REQ_PIN_IRQ_30,
    SNZ_REQ_PIN_IRQ_31,
    SNZ_REQ_GROUP0_COUNT,
};

/**
 * @enum PmuLpsstSnzReqCfgGroup1
 * @brief Low power state switch trigger items for Snooze request
 */
enum PmuLpsstSnzReqCfgGroup1 {
    SNZ_REQ_RTC,
    SNZ_REQ_TIMER16_INTR0,
    SNZ_REQ_TIMER16_INTR1,
    SNZ_REQ_TIMER16_INTR2,
    SNZ_REQ_TIMER16_INTR3,
    SNZ_REQ_TIMER16_INTR4,
    SNZ_REQ_TIMER32_INTR0,
    SNZ_REQ_TIMER32_INTR1,
    SNZ_REQ_TIMER32_INTR2,
    SNZ_REQ_TIMER32_INTR3,
    SNZ_REQ_TIMER32_INTR4,
    SNZ_REQ_I2S_TX_DMA_REQ,
    SNZ_REQ_I2S_RX_DMA_REQ,
    SNZ_REQ_I2S_TX_DMA_SINGLE,
    SNZ_REQ_I2S_RX_DMA_SINGLE,
    SNZ_REQ_I2C_TX_DMA_REQ,
    SNZ_REQ_I2C_RX_DMA_REQ,
    SNZ_REQ_I2C_TX_DMA_SINGLE,
    SNZ_REQ_I2C_RX_DMA_SINGLE,
    SNZ_REQ_I3C_TX_DMA_REQ,
    SNZ_REQ_I3C_RX_DMA_REQ,
    SNZ_REQ_I3C_TX_DMA_SINGLE,
    SNZ_REQ_I3C_RX_DMA_SINGLE,
    SNZ_REQ_SPI0_TX_DMA_REQ,
    SNZ_REQ_SPI0_RX_DMA_REQ,
    SNZ_REQ_SPI1_TX_DMA_REQ,
    SNZ_REQ_SPI1_RX_DMA_REQ,
    SNZ_REQ_SPI2_TX_DMA_REQ,
    SNZ_REQ_SPI2_RX_DMA_REQ,
    SNZ_REQ_SPI3_TX_DMA_REQ,
    SNZ_REQ_SPI3_RX_DMA_REQ,
    SNZ_REQ_GROUP1_COUNT,
};

/**
 * @enum PmuLpsstSnzEnd
 * @brief Low power state switch trigger items for Snooze end
 */
enum PmuLpsstSnzEnd {
    SNZ_END_RTC,
    SNZ_END_DMA,
    SNZ_END_ADC,
    SNZ_END_TIMER16_INTR0,
    SNZ_END_TIMER16_INTR1,
    SNZ_END_TIMER16_INTR2,
    SNZ_END_TIMER16_INTR3,
    SNZ_END_TIMER16_INTR4,
    SNZ_END_TIMER32_INTR0,
    SNZ_END_TIMER32_INTR1,
    SNZ_END_TIMER32_INTR2,
    SNZ_END_TIMER32_INTR3,
    SNZ_END_TIMER32_INTR4,
    SNZ_END_COUNT,
};

/**
 * @struct ExtIoIrq
 * @brief External interrupts as wakeup sources
 */
typedef struct ExtIoIrq {
    uint8_t wakeup_src;
    uint8_t io_trigger_mode;
} ExtIoIrq;

/**
 * @union WakeUpSourceInfo
 * @brief Wakeup source information for all kinds of wakeup sources
 */
typedef union WakeUpSourceInfo {
    ExtIoIrq ext_io;
    uint8_t wakeup_src;
} WakeUpSourceInfo;

/**
 * @struct PmuHwCfg
 * @brief PMU hardware configure
 */
typedef struct PmuHwCfg {
    uint8_t type;
} PmuHwCfg;

/**
 * @enum LPStateSwitchCfgItem
 * @brief State switching configure items of low power modes from a hardware
 * perspective.
 * @note For example, LPSS_EXIT_SW_STANDBY indicates that the software wants to
 * configure the trigger condition for the system to exit the software standby
 * mode.
 */
typedef enum LPStateSwitchCfgItem {
    LPSS_EXIT_SBY_CFG_PIN, /** Exit software standby mode, used for configuring
                              pin IRQ */
    LPSS_EXIT_SBY_CFG_IP_GROUP1, /** Exit software standby mode, used for
                                    configuring IP IRQ group1 */
    LPSS_EXIT_SBY_CFG_IP_GROUP2, /** Exit software standby mode, used for
                                    configuring IP IRQ group2 */
    LPSS_EXIT_SNZ_CFG_PIN, /** Exit snooze mode, used for configuring pin IRQ */
    LPSS_EXIT_SNZ_CFG_IP_GROUP1, /** Exit snooze mode, used for configuring IP
                                    IRQ group1 */
    LPSS_EXIT_SNZ_CFG_IP_GROUP2, /** Exit snooze mode, used for configuring IP
                                    IRQ group2 */
    LPSS_SNZ_REQ_GROUP0, /** Snooze request, used for configuring IRQ group0 */
    LPSS_SNZ_REQ_GROUP1, /** Snooze request, used for configuring IRQ group1 */
    LPSS_SNZ_END, /** Snooze end */
    LPSS_EXIT_DSBY, /* Exit deep software standby mode */
    LPSS_ITEM_COUNT,
} LPStateSwitchCfgItem;

/**
 * @brief A function pointer to the interrupt handler
 */
typedef void (*PmuIrqHandler)(void);

/**
 * @struct PmuFuncs
 * @brief Prototype declaration of operations of the power management uint
 */
typedef struct PmuFuncs {
    /** Initialize the PMU with hardware config */
    int (*init)(const PmuHwCfg *config);
    /** Enable/disable the wakeup source */
    int (*wakeup_src_en)(const PmuHwCfg *config, WakeUpSourceInfo wake_info,
                         bool enable, PmuIrqHandler handler);
    /** The time between entering and exiting deep sleep mode in ms */
    uint32_t (*latency)(const PmuHwCfg *config);
    /** Flow executed before SoC enters low power mode. In low-power mode, the
     * system continues to work, but in a low-performance state. */
    int (*enter_low_power)(const PmuHwCfg *config);
    /** Flow executed after SoC exits low power mode */
    int (*exit_low_power)(const PmuHwCfg *config, uint8_t *const wakeup_src);
    /** Flow executed before SoC enters deep sleep mode. Choose whether to
     * enable fast boot (if supported) */
    int (*enter_deep_sleep)(const PmuHwCfg *config, uint32_t sleep_period,
                            bool en_fast_boot);
    /** Flow executed after SoC exits deep sleep mode */
    int (*exit_deep_sleep)(const PmuHwCfg *config, uint8_t *const wakeup_src);
    /** Flow executed before SoC enters sleep mode */
    int (*enter_sleep)(const PmuHwCfg *config);
    /** Flow executed after SoC exits sleep mode */
    int (*exit_sleep)(const PmuHwCfg *config, uint8_t *const wakeup_src);
    /** Flow executed before SoC enters snooze mode */
    int (*enter_snooze)(const PmuHwCfg *config);
    /** Flow executed after SoC exits snooze mode */
    int (*exit_snooze)(const PmuHwCfg *config, uint8_t *const wakeup_src);
    /** Configure the trigger conditions for low-power state switching */
    int (*lp_state_switch_cfg)(const PmuHwCfg *config,
                               uint8_t state_switch_item, uint64_t cfg_mask);
    /** Configure of DS mode */
    int (*ds_init)(const PmuHwCfg *config, WakeUpSourceInfo wake_info);
    /* ISR for an external interrupt which wake the system from deep sleep */
    void (*ds_irq_handler)(const PmuHwCfg *config, int irq_id);
} PmuFuncs;

/**
 * @struct PmuHw
 * @brief Power management unit's hardware config and operations
 */
typedef struct PmuHw {
    const struct PmuHwCfg *config; /**< Hardware config of the PMU */
    const struct PmuFuncs *funcs; /**< Operations of the PMU */
} PmuHw;

/**
 * @brief Initialise power management unit driver
 * @param pmu Handle of a PMU hardware
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pmu_init(const PmuHw *pmu);

/**
 * @brief Enable/disable the specific wakeup source
 * @param pmu Handle of a PMU hardware
 * @param wake_info Wakeup source information, @see WakeUpSourceInfo
 * @param enable Enable or not
 * @param handler Interrupt handler, NULL for disabling the wakeup source
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pmu_en_wakeup_src(const PmuHw *pmu, WakeUpSourceInfo wake_info,
                          bool enable, PmuIrqHandler handler);

/**
 * @brief Flow executed before SoC enters deep sleep mode
 * @param pmu Handle of a PMU hardware
 * @param sleep_period Sleep period, in ms
 * @param en_fast_boot Whether to enable fast boot (if supported)
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pmu_enter_deep_sleep(const PmuHw *pmu, uint32_t sleep_period,
                             bool en_fast_boot);

/**
 * @brief Flow executed after SoC exits deep sleep mode
 * @param pmu Handle of a PMU hardware
 * @param wakeup_src Pointer to record wakeup source, @see PmuWakeUpSource
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pmu_exit_deep_sleep(const PmuHw *pmu, uint8_t *const wakeup_src);

/**
 * @brief The time between suspend and resume
 * @param pmu Handle of a PMU hardware
 * @return uint32_t Time in ms, and 0 for unknown
 */
uint32_t hal_pmu_latency_ms(const PmuHw *pmu);

/**
 * @brief Execution flow of the PMU before the system enters low power mode
 * @param pmu Handle of a PMU hardware
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pmu_enter_low_power(const PmuHw *pmu);

/**
 * @brief Execution flow of the PMU after the system exits low power mode
 * @param pmu Handle of a PMU hardware
 * @param wakeup_src Pointer to record wakeup source, @see PmuWakeUpSource
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pmu_exit_low_power(const PmuHw *pmu, uint8_t *const wakeup_src);

/**
 * @brief Execution flow of the PMU before the system enters sleep mode
 * @param pmu Handle of a PMU hardware
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pmu_enter_sleep(const PmuHw *pmu);

/**
 * @brief Execution flow of the PMU after the system exits sleep mode
 * @param pmu Handle of a PMU hardware
 * @param wakeup_src Pointer to record wakeup source, @see PmuWakeUpSource
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pmu_exit_sleep(const PmuHw *pmu, uint8_t *const wakeup_src);

/**
 * @brief Execution flow of the PMU before the system enters snooze mode
 * @param pmu Handle of a PMU hardware
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pmu_enter_snooze(const PmuHw *pmu);

/**
 * @brief Execution flow of the PMU after the system exits snooze mode
 * @param pmu Handle of a PMU hardware
 * @param wakeup_src Pointer to record wakeup source, @see PmuWakeUpSource
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pmu_exit_snooze(const PmuHw *pmu, uint8_t *const wakeup_src);

/**
 * @brief Configure trigger conditions for state switching in multiple low-power
 * modes
 * @param pmu Handle of a PMU hardware
 * @param state_switch_item Configurable items, that is, which state switching
 * trigger conditions can be configured by software. @see LPStateSwitchCfgItem
 * @param cfg_mask Mask value. A bit represents a LPStateSwitchTriggerItem.
 * The position of the bit is equal to the id of the item, and the value of the
 * bit indicates whether to enable the item. @see LPStateSwitchTriggerItem
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pmu_lp_state_switch_cfg(const PmuHw *pmu, uint8_t state_switch_item,
                                uint64_t cfg_mask);

/**
 * @brief Initialize external wakeup sources(IRQ_DS) of deep sleep mode
 * @param pmu Handle of a PMU hardware
 * @param wake_info Wakeup source information, @see WakeUpSourceInfo
 * @return int VSD_SUCCESS for succeed, others for failure
 */
int hal_pmu_ds_init(const PmuHw *pmu, WakeUpSourceInfo wake_info);

/**
 * @brief ISR of the interrupt which wake the system from deep sleep
 * @param pmu Handle of a PMU hardware
 * @param irq_id Interrupt id
 */
void hal_pmu_ds_irq_handler(const PmuHw *pmu, int irq_id);

/** @} */

#endif /* __HAL_PMU_H__ */
