/**
 * Copyright (C) 2022. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file os_adapter.h
 * @brief Compatible with bare metal and different real-time operating systems,
 * such as FreeRTOS, RT-Thread, etc.
 */

#ifndef __OS_ADAPTER__
#define __OS_ADAPTER__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "vs_conf.h"

#if CONFIG_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"
typedef TimerHandle_t SwTimer;
#else
#define TimerHandle_t void *
typedef uint32_t TickType_t;
#define SemaphoreHandle_t void *
#define TaskHandle_t void *
#define xTimerGetExpiryTime(xTimer) 0
#define xTaskGetTickCount() 0
#define portTICK_PERIOD_MS 10
typedef struct xSTATIC_QUEUE {
} StaticQueue_t;
typedef StaticQueue_t StaticSemaphore_t;
typedef void *SwTimer;
#endif

#ifdef __cplusplus
}
#endif

#endif /* __OS_ADAPTER__ */
