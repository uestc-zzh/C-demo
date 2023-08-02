/**
 * Copyright (C) 2021 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file vpi_pm.h
 * @brief VeriSilicon Platform Interface(VPI) for power management
 */

#ifndef __VPI_PM_H__
#define __VPI_PM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/**
 * @enum VpiPmEvent
 * @brief PM event from notifier callback (from a software perspective)
 */
typedef enum VpiPmEvent {
    PM_EVENT_SNOOZE, /**< Event of entering snooze mode */
    PM_EVENT_EXIT_SNOOZE, /**< Event of exiting snooze mode */
    PM_EVENT_SLEEP, /**< Event of entering sleep mode */
    PM_EVENT_EXIT_SLEEP, /**< Event of exiting sleep mode */
    PM_EVENT_DEEP_SLEEP, /**< Event of entering deep sleep mode */
    PM_EVENT_EXIT_DEEP_SLEEP, /**< Event of exiting deep sleep mode */
    PM_EVENT_LOW_POWER, /**< Event of entering low power mode */
    PM_EVENT_LOW_PERFORMANCE = PM_EVENT_LOW_POWER,
    PM_EVENT_EXIT_LOW_POWER, /**< Event of exiting low power mode */
    PM_EVENT_EXIT_LOW_PERFORMANCE = PM_EVENT_EXIT_LOW_POWER,
    PM_EVENT_FAILURE, /**< Failure occurred */
} VpiPmEvent;

/**
 * @enum VpiPmRequest
 * @brief PM client request
 */
typedef enum VpiPmRequest {
    PM_CLIENT_STAY_AWAKE, /**< Stay awake and don't sleep */
    PM_CLIENT_RELEASE_WAKE, /**< Ready to sleep but do not specify sleep
                                period, i.e. do not ask to be woken up */
    PM_CLIENT_BOOK_A_WAKE, /**< Ready to sleep and need to be waked up after a
                               while */
} VpiPmRequest;

/**
 * @enum VpiPmLowPowerMode
 * @brief The low power modes that the client can choose to enter
 */
typedef enum VpiPmLowPowerMode {
    PM_LPM_SNOOZE,
    PM_LPM_SLEEP,
    PM_LPM_DEEP_SLEEP,
    PM_LPM_LOW_POWER,
    PM_LPM_LOW_PERFORMANCE = PM_LPM_LOW_POWER,
    PM_LPM_COUNT,
} VpiPmLowPowerMode;

/**
 * @brief The prototype declaration for callback of PM notifier
 * @param event PM event, @see VpiPmEvent
 * @param priv Private data for client, NULL if not used
 */
typedef void (*pm_notifier)(uint8_t event, void *priv);

struct VpiPmClient;

/**
 * @brief Create a PM client
 * @param name Client's name
 * @param callback Notifiler callback, NULL indicates that no callback is
 * required
 * @return VpiPmClient* Handle of the client, NULL for failure
 */
struct VpiPmClient *vpi_pm_add_client(const char *name, pm_notifier callback);

/**
 * @brief Remove a PM client
 * @param client Handle of the PM client
 */
void vpi_pm_del_client(struct VpiPmClient *client);

/**
 * @brief PM client sends a request of some type
 * @param client Handle of the PM client
 * @param request Type of the request, @see VpiPmRequest
 * @param interval Only used when the request type is PM_CLIENT_BOOK_A_WAKE to
 * book a wake after an interval (in ms)
 * @param lp_mode The low power modes that the client can choose to enter @see
 * VpiPmLowPowerMode
 * @param en_fast_boot Whether to enable fast boot for deep sleep mode (if
 * supported)
 * @return int VPI_SUCCESS for succeed, others for failure
 */
int vpi_pm_client_request(struct VpiPmClient *client, uint8_t request,
                          uint32_t interval, uint8_t lp_mode,
                          bool en_fast_boot);

#ifdef __cplusplus
}
#endif

#endif /* __VPI_PM_H__ */
