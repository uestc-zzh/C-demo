/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file vpi_ringbuffer.h
 * @brief public head file of ring buffer
 * @author Shaobo Tu <Shaobo.Tu@verisilicon.com>
 */

#ifndef _VPI_RINGBUFFER_H_
#define _VPI_RINGBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BUFFER
 *  Ring buffer API.
 *  @ingroup VPI
 *  @{
 */

#include <stdint.h>
#include <stdbool.h>
/**
 * @brief Structure of ring buffer control
 */
typedef struct RingBufCtrl {
    uint16_t wt_idx; /**< Write offset */
    uint16_t rd_idx; /**< Read offset */
    uint8_t *buf; /**< Pointer of buffer */
    uint16_t capacity; /**< Total size of buffer */
    uint16_t linear_capacity; /**< Available linear size */
    bool continue_addr; /**< Continue address support of buffer */
} RingBufCtrl;

/**
 * @brief Ring buffer insert data callback handler
 */
typedef uint16_t (*RingBufInsertHandler)(uint8_t *wt_addr, void *param);

/**
 * @brief Initialize a ring buffer controller
 * @param[in]   buf             ring buffer address
 * @param[in]   capacity        ring buffer capacity
 * @param[in]   continue_addr   is continue address
 * @return  RingBufCtrl pointer: on success,  NULL : error
 */
RingBufCtrl *vpi_ringbuf_init(uint8_t *buf, uint16_t capacity,
                              bool continue_addr);

/**
 * @brief Flush a ring buffer controller
 * @param[in]   ring_ctrl   ring buffer controller
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_ringbuf_flush(RingBufCtrl *ring_ctrl);

/**
 * @brief get the ring buffer used space
 * @param[in]  ring_ctrl   ring buffer controller
 * @return  Used space
 */
uint16_t vpi_ringbuf_get_used_space(RingBufCtrl *ring_ctrl);

/**
 * @brief Insert data into ringbuffer
 * @param[in]  ring_ctrl        ring buffer controller
 * @param[in]  size             insert data size
 * @param[in]  insert_handler   insert data callback
 * @param[in]  param            insert data callback parameters
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_ringbuf_insert(RingBufCtrl *ring_ctrl, uint32_t size,
                       RingBufInsertHandler insert_handler, void *param);

/**
 * @brief Delete data from ringbuffer
 * @param[in]  ring_ctrl        ring buffer controller
 * @param[in]  size             deleted data size
 * @return  Read pointer of the ring buffer
 */
void *vpi_ringbuf_delete(RingBufCtrl *ring_ctrl, uint32_t size);

/**
 * @brief Get free space from ringbuffer
 * @param[in]  ring_ctrl        ring buffer controller
 * @return  Free space of the ring buffer
 */
uint16_t vpi_ringbuf_get_free_space(RingBufCtrl *ring_ctrl);

/**
 * @brief Push data to ringbuffer
 * @param[in]  ring_ctrl        ring buffer controller
 * @param[in]  size             pushed data size
 * @param[in]  data             the input data
 * @return  VPI_SUCCESS on success,  otherwise is error
 */
int vpi_ringbuf_push(RingBufCtrl *ring_ctrl, uint32_t size, uint8_t *data);

/**
 * @brief Pop data from ringbuffer
 * @param[in]  ring_ctrl        ring buffer controller
 * @param[in]  size             pop/remove data size
 * @return  VPI_SUCCESS on success,  otherwise is error
 */
int vpi_ringbuf_pop(RingBufCtrl *ring_ctrl, uint32_t size);

/**
 * @brief Peek data with expected size from ringbuffer
 * @param[in]  ring_ctrl        ring buffer controller
 * @param[in]  peek_size        data size to peek
 * @return  the pointer of data
 */
uint8_t *vpi_ringbuf_peek(RingBufCtrl *ring_ctrl, uint16_t peek_size);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _VPI_RINGBUFFER_H_ */
