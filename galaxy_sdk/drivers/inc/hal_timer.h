/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_timer.h
 * @brief Public head file of timer HAL
 */

#ifndef _HAL_TIMER_H
#define _HAL_TIMER_H

#include <inttypes.h>
#include "sys_common.h"

/** @addtogroup TIMER
 *  - Timer HAL API and definition
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */
#define TIMER_DEVICE_MAX (2)
#define TIMER_COUNTER_MAX (5)

/** Events that can trigger a callback function */
typedef enum {
    TIMER_EVENT_CYCLE_END, /**< Cycle end event */
} TimerEvent;

/** Callback function parameter data */
typedef struct TimerCallbackArgs {
    void *context; /**< The context of timer calblack */
    /** Used to identify who triggered the callback */
    TimerEvent event;
} TimerCallbackArgs;

/**
 * @brief Timer call back function
 */
typedef void (*TimerCallback)(TimerCallbackArgs *callback_memory,
                              void *context);

/**
 * @brief Timer hardware type
 */
typedef enum TimerType {
    TIMER_TYPE_NONE, /**< Invalid timer type */
    TIMER_TYPE_16BIT, /**< 16bit timer */
    TIMER_TYPE_32BIT /**< 32bit timer */
} TimerType;

/**
 * @brief Timer hardware version
 */
typedef enum VsTimerHwVer {
    VS_TIMER_HW_VER_0P1B, /**< Version 0.1b */
    VS_TIMER_HW_VER_0P2, /**< Version 0.2 */
} VsTimerHwVer;

/**
 * @brief Timer groups
 */
typedef enum TimerGroup {
    TIMER_GROUP0, /**< Timer group 0 */
    TIMER_GROUP1, /**< Timer group 1 */
} TimerGroup;

/**
 * @brief Structure of TIMER hardware configuration
 */
typedef struct TimerHwConfig {
    uint32_t base; /**< Base address of TIMER device */
    uint8_t clk_id; /**< Clock ID of the timer */
    uint8_t counters; /**< The count number of timers in this device */
    uint16_t irq_id[TIMER_COUNTER_MAX]; /**< Interrupt ID which defined by SoC*/
    uint8_t version; /**< Timer hardware version */
    uint32_t tclk_freq; /**< Target clock rate timer needed,used to driver
                        divisor if do not need set it 0*/
} TimerHwConfig;

/**
 * @brief Structure of TIMER hardware device
 */
typedef struct TimerDevice {
    uint8_t hw_id; /**< Port ID, the ID of timer hardware device */
    uint8_t type; /**< Timer type, @see TimerType */
    const TimerHwConfig *hw_config; /**< Timer hardware configuration */
    const void *ops; /**< Timer operations */
    void *ctx; /**< Private context */
} TimerDevice;

/**
 * @brief Structure of operations for HAL of TIMER
 */
typedef struct TimerOperations {
    /** Init the timer hardware */
    int (*init)(TimerDevice *device, uint8_t cnt_id);
    /** Enable the timer hardware  */
    int (*enable)(TimerDevice *device, uint8_t cnt_id);
    /** Disable the timer hardware  */
    int (*disable)(TimerDevice *device, uint8_t cnt_id);
    /** Start the timer */
    int (*start)(TimerDevice *device, uint8_t cnt_id);
    /** Stop the timer */
    int (*stop)(TimerDevice *device, uint8_t cnt_id);
    /** Reset the timer */
    int (*reset)(TimerDevice *device, uint8_t cnt_id);
    /** Set period value for timer */
    int (*periodset)(TimerDevice *device, uint8_t cnt_id, uint32_t timeout_ms);
    /** Set single mode for timer  */
    int (*singleset)(TimerDevice *device, uint8_t cnt_id, uint32_t timeout_ms);
    /** Get current count from timer */
    int (*getcurcount)(TimerDevice *device, uint8_t cnt_id, uint64_t *curcount);
    /** Convert time to count */
    int (*time2count)(TimerDevice *device, uint8_t cnt_id, uint64_t *timeout_ms,
                      uint64_t *time2count);
    /** Set callback of timer  */
    int (*callbackset)(TimerDevice *device, uint8_t cnt_id,
                       void (*hook)(TimerCallbackArgs *arg, void *context),
                       void *context, TimerCallbackArgs *callback_memory);
    /** set irq handler for timer */
    void (*irq_hanlder)(TimerDevice *device, uint8_t cnt_id);
} TimerOperations;

/**
 * @brief TIMER HAL function, used to add device to timer device list
 * @param device the instance of timer device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_timer_add_dev(TimerDevice *device);

/**
 * @brief TIMER HAL function, used to remove device from timer device list
 * @param device the instance of timer device
 * @return Return result
 * @retval VSD_SUCCESS for succeed, others for failure
 */
int hal_timer_remove_dev(TimerDevice *device);

/**
 * @brief TIMER HAL function, used to get total timer ports on the SoC system
 * @return Return total port numbers, 0 for no timer
 */
uint8_t hal_timer_get_ports(void);

/**
 * @brief TIMER HAL function, used to get timer device from device list
 * @param hw_id the ID of timer device need to be found
 * @return Return The instance of timer device, NULL for failure
 */
TimerDevice *hal_timer_get_device(uint8_t hw_id);

/**
 * @brief TIMER HAL function, used to init a new timer port according to timer
 * type
 * @param type timer type need to init
 * @param port the pointer to timer port which should be initialized by HAL
 * @note The HAL interface handles the port allocation, return an available port
 * @return VSD_SUCCESS for success, others for error
 */
int hal_timer_init(TimerType type, uint8_t *port);

/**
 * @brief TIMER HAL function, used to free the timer port after finished using
 * @note After free, the port can not be used anymore before new initialization
 * @param port the port of timer
 * @return VSD_SUCCESS for success, others for error
 */
int hal_timer_free(uint8_t port);

/**
 * @brief TIMER HAL function, used to enable timer interrupt
 * @param port the timer port
 * @return VSD_SUCCESS for success, others for error
 */
int hal_timer_enable_interrupt(uint8_t port);

/**
 * @brief TIMER HAL function, used to disable timer interrupt
 * @param port the timer port
 * @return VSD_SUCCESS for success, others for error
 */
int hal_timer_disable_interrupt(uint8_t port);

/**
 * @brief TIMER HAL function, used to start timer
 * @param port the timer port
 * @return VSD_SUCCESS for success, others for error
 */
int hal_timer_start(uint8_t port);

/**
 * @brief TIMER HAL function, used to stop timer
 * @param port the timer port
 * @return VSD_SUCCESS for success, others for error
 */
int hal_timer_stop(uint8_t port);

/**
 * @brief TIMER HAL function, used to reset timer
 * @param port the timer port
 * @return VSD_SUCCESS for success, others for error
 */
int hal_timer_reset(uint8_t port);

/**
 * @brief TIMER HAL function, used to set timer's period mode
 * @param port the timer port
 * @param count Time until count should expire
 * @return VSD_SUCCESS for success, others for error
 */
int hal_timer_set_period(uint8_t port, uint32_t count);

/**
 * @brief TIMER HAL function, used to set timer's single mode
 * @param port Timer port
 * @param count Time until count should expire
 * @return VSD_SUCCESS for success, others for error
 */
int hal_timer_set_single(uint8_t port, uint32_t count);

/**
 * @brief TIMER HAL function, used to get timer current count
 * @param port the timer port
 * @param curcount the timer current count
 * @return VSD_SUCCESS for success, others for error
 */
int hal_timer_get_count(uint8_t port, uint64_t *curcount);

/**
 * @brief TIMER HAL function, used to transfer ms to count
 * @param port the timer port
 * @param timeout_ms the timer current count
 * @param time2count the timeout_ms to count
 * @return VSD_SUCCESS for success, others for error
 */
int hal_timer_time2count(uint8_t port, uint64_t *timeout_ms,
                         uint64_t *time2count);

/**
 * @brief TIMER HAL function, used to set timer callback function
 * @param port the timer port
 * @param hook callback hook function to register
 * @param context  pointer to send to callback function
 * @param callback_memory pointer to memory where callback structure can be
 * allocated
 * @return VSD_SUCCESS for success, others for error
 */
int hal_timer_callbackset(uint8_t port,
                          void (*hook)(TimerCallbackArgs *arg, void *context),
                          void *context, TimerCallbackArgs *callback_memory);
/**
 * @brief TIMER HAL function ,used to interrupt handler
 *
 * @param device timer device
 * @param cnt_id timer counter id
 */
void hal_timer_irq_handler(TimerDevice *device, uint8_t cnt_id);

/** @} */

#endif /* _HAL_TIMER_H */
