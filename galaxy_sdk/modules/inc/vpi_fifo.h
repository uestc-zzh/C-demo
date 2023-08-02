/**
 * Copyright (C) 2021. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file vpi_fifo.h
 * @brief public head file of fifo
 * @author Shaobo Tu <Shaobo.Tu@verisilicon.com>
 */
#ifndef _VPI_FIFO_H_
#define _VPI_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup FIFO
 *  FIFO API.
 *  @ingroup VPI
 *  @{
 */

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Structure of fifo control
 */
typedef struct FifoCtrl {
    uint8_t *buffer; /**< Pointer of buffer */
    uint16_t item_size; /**< The item size of FIFO */
    uint16_t item_total; /**< Number of items */
    uint16_t buffer_size; /**< Buffer size of FIFO */
    uint16_t item_stored; /**< Counter of stored items */
    uint16_t wt_idx; /**< Write offset of FIFO */
    uint16_t rd_idx; /**< Read offset of FIFO */
} FifoCtrl;

/**
 * @brief Structure of init fifo data
 */
typedef struct FifoInitData {
    uint32_t buffer_addr;
    uint16_t item_size;
    uint16_t item_total;
    uint16_t buffer_size;
} FifoInitData;

/**
 * @brief Fifo add data callback handler
 */
typedef void (*FifoAddHandler)(uint8_t *wt_addr, void *param);

/**
 * @brief Initialize a fifo controller
 * @param[in]   buf         fifo address
 * @param[in]   itemsize    fifo each item size
 * @param[in]   itemtotal   fifo total item
 * @return  FifoCtrl pointer: on success,  NULL : error
 */
FifoCtrl *vpi_fifo_init(uint8_t *buf, uint16_t itemsize, uint16_t itemtotal);

/**
 * @brief Write a data to FIFO
 * @param[in] fifo fifo controller
 * @param[in] data pointer to data
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_fifo_write(FifoCtrl *fifo, void *data);

/**
 * @brief Read a data from FIFO
 * @param[in] fifo fifo controller
 * @param[out] data pointer to data
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_fifo_read(FifoCtrl *fifo, void *data);

/**
 * @brief Add data into fifo
 * @param[in]  fifo          fifo controller
 * @param[in]  add_handler   add data callback
 * @param[in]  param         add data callback parameters
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_fifo_add(FifoCtrl *fifo, FifoAddHandler add_handler, void *param);

/**
 * @brief Get data from fifo
 * @param[in]   fifo   fifo controller
 * @param[out]  item   current item address
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_fifo_get(FifoCtrl *fifo, void *item);

/**
 * @brief Peek data from fifo
 * @note This VPI don't update the rd_idx of fifo
 * @param[in]   fifo   fifo controller
 * @param[in]   num    number of item
 * @param[out]  item   current item address
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_fifo_peek(FifoCtrl *fifo, uint8_t num, uint8_t **item);

/**
 * @brief Get the fifo stored item number
 * @param[in]  fifo   fifo controller
 * @return  Stored item number
 */
uint16_t vpi_fifo_get_stored_item(FifoCtrl *fifo);

/**
 * @brief Flush fifo
 * @param[in]   fifo   fifo controller
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_fifo_flush(FifoCtrl *fifo);

/**
 * @brief Get whether FIFO is full
 * @param[in] fifo fifo controller
 * @return true for full, false for not full
 */
bool vpi_fifo_is_full(FifoCtrl *fifo);

/**
 * @brief Get whether FIFO is empty
 * @param[in] fifo fifo controller
 * @return true for empty, false for not empty
 */
bool vpi_fifo_is_empty(FifoCtrl *fifo);

/**
 * @brief Initialize a fifo controller with customized shared memory address
 * @param[in] fifo       fifo controller
 * @param[in] init_fifo  init data for fifo
 * @return  VPI_SUCCESS for succeed, others for failure
 */
int vpi_fifo_static_init(FifoCtrl *fifo, FifoInitData *init_fifo);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _VPI_FIFO_H_ */
