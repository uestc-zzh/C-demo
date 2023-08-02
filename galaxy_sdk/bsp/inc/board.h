/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file board.h
 * @brief Head file for board functions
 */
#ifndef _BOARD_H_
#define _BOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "device.h"

/** @addtogroup BOARD
 *  - Board API and definition
 *  @ingroup BSP
 *  @{
 */

/**
 * @brief Structure of board handler
 */
typedef struct BoardDevice {
    const char *name; /**< name of board */
    uint32_t reserved; /**< reserved */
    void *priv; /**< priv data */
} BoardDevice;

/**
 * @brief Structure of operations for board
 */
typedef struct BoardOperations {
    int (*init)(BoardDevice *board);
    void *(*find_device)(uint8_t device_id);
    void (*get_wakeup_src)(uint8_t port, uint8_t *wakeup_src);
} BoardOperations;

/**
 * @brief Get board ops
 * @return Return result
 * @retval Board ops
 */
const BoardOperations *board_get_ops(void);

/**
 * @brief Register the board
 * @param ops operations for board
 * @note This api must be called before all of board api
 * @return Return result
 * @retval 0 for succeed, others for failure
 */
int board_register(const BoardOperations *ops);

/**
 * @brief Init the board
 * @param board_cfg configuration of board
 * @return Return result
 * @retval 0 for succeed, others for failure
 */
int board_init(void *board_cfg);

/**
 * @brief Get device from board
 * @param device_id id of device
 * @return Return result
 * @retval Pointor to device which was found, NULL for no device
 */
void *board_find_device_by_id(uint8_t device_id);

/**
 * @brief Get wakeup source from board
 * @param port GPIO port id of power key
 * @param wakeup_src Wake up source from deepsleep mode
 */
void get_wakeup_src(uint8_t port, uint8_t *wakeup_src);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // _BOARD_H_
