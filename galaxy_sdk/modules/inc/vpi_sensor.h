/**
 * Copyright (C) 2021. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file vpi_sensor.h
 * @brief Interfaces to Sensor Manager.
 *
 * This header define the interfaces of SDK level Sensor Manager API
 */

#ifndef __VPI_SENSOR_H__
#define __VPI_SENSOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "vpi_event_def.h"
#include "vpi_sensor.h"
#include "vpi_ringbuffer.h"
#include "vpi_fifo.h"
#include "vpi_sensor_def.h"
#include "vpi_sensor_protocol.h"

/** @addtogroup SENSOR
 *  - Sensor API
 *  @ingroup VPI
 *  @{
 */

/**
 * Raw data control
 */

/**
 * @brief Initialize a raw data buffer controller
 * @param[in]  raw       raw data pointer
 * @param[in]  buf_num   buffer number
 * @param[in]  proc_len  process data length
 * @param[in]  padding   padding data length
 * @return  SenRawDataBufCtrl pointer: on success,  NULL : error
 */
SenRawDataBufCtrl *vpi_sensor_rawdata_init(uint8_t *raw, uint16_t buf_num,
                                           uint16_t proc_len, uint16_t padding);

/**
 * @brief Reset a raw data buffer controller
 * @param[in]  ctrl   raw data buffer controller
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_sensor_rawdata_reset(SenRawDataBufCtrl *ctrl);

/**
 * @brief Get rawdata write address
 * @param[in]  ctrl   raw data buffer controller
 * @param[out] wt_addr  the pointer of write address
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_sensor_rawdata_get_write_addr(SenRawDataBufCtrl *ctrl,
                                      uint8_t **wt_addr);

/**
 * @brief  Raw data buffer control
 * @param[in]  ctrl            raw data buffer controller
 * @param[in]  avail_cnt       number of bytes need read
 * @param[in]  data_callback   raw data sampling callback
 * @param[out] sta             raw data status
 * @param[in]  max_size        the max length of packet data
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_sensor_rawdata_push(SenRawDataBufCtrl *ctrl, uint16_t avail_cnt,
                            SenDataHandler data_callback,
                            VpiSensorRawStaPointArrHandle sta,
                            uint16_t max_size);

/**
 * @brief Destroy a raw data buffer controller
 * @param[in]  ctrl   raw data buffer controller
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_sensor_rawdata_destroy(SenRawDataBufCtrl *ctrl);

/**
 * @brief Initialize a sensor data process controller
 * @param[in]  samp_size   sensor sampling size
 * @return  SenDataProcCtrl pointer: on success,  NULL : error
 */
SenDataProcCtrl *vpi_sensor_data_process_init(uint16_t samp_size);

/**
 * @brief Flush sensor data process controller
 * @param[in]  ctrl   sensor data process controller
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_sensor_data_process_flush(SenDataProcCtrl *ctrl);

/**
 * @brief  Raw data buffer control
 * @param[in]  ctrl            sensor data process controller
 * @param[in]  data            data pointer
 * @param[in]  data_len        data length
 * @param[out] data_callback   data process callback
 * @param[in]  arg             arg pointer
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_sensor_data_process(SenDataProcCtrl *ctrl, uint8_t *data,
                            uint16_t data_len, SenDataProcHandler data_callback,
                            void *arg);
/**
 * @brief Destroy sensor data process controller
 * @param[in]  ctrl   sensor data process controller
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_sensor_data_process_destroy(SenDataProcCtrl *ctrl);

/**
 * Data packet common
 */

/**
 * @brief  Initialize a data packet process buffer
 * @param[in]  buf          buffer address
 * @param[in]  itemsize     each packet size
 * @param[in]  itemtotal    packet number
 * @return  FifoCtrl pointer: on success,  NULL : error
 */
FifoCtrl *vpi_sensor_packet_init(uint8_t *buf, uint16_t itemsize,
                                 uint16_t itemtotal);

/**
 * @brief  Determine if packet fifo is empty
 * @param[in]  fifo   fifo controller
 * @return  True is empty,false is not empty
 */
bool vpi_sensor_packet_is_empty(FifoCtrl *fifo);

/**
 * @brief  Determine if packet fifo is full
 * @param[in]  fifo   fifo controller
 * @return  True is full,false is not full
 */
bool vpi_sensor_packet_is_full(FifoCtrl *fifo);

/**
 * @brief Parse trans information
 * @param[in]  trans_info   trans information
 * @param[in]  packet_type  trans packet type,see SenPacketType
 * @param[in]  data_type    trans data type,see SenDataType
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_parse_trans_info(SensorTransInfo *trans_info,
                                SenPacketType *packet_type, uint8_t *data_type);

/**
 * @brief Set raw data test flag
 * @param[in]  data   raw data packet
 * @param[in]  test   test flag
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_set_raw_test_flag(void *data, bool test);

/**
 * @brief Set result data test flag
 * @param[in]  data   raw data packet
 * @param[in]  test   test flag
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_set_res_test_flag(void *data, bool test);

/**
 * Raw data packet
 */

/**
 * @brief  Initialize a raw data process buffer
 * @param[in]  buf        buffer address
 * @param[in]  capacity   buffer capacity
 * @return  RingBufCtrl pointer: on success,  NULL : error
 */
RingBufCtrl *vpi_sensor_ringbuf_init(uint8_t *buf, uint16_t capacity);

/**
 * @brief  Set user compress function
 * @param[in]  compress   compress function pointer
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_sensor_set_compress_fun(DataCompressHandler compress);

/**
 * @brief  Set user encrypt function
 * @param[in]  encrypted   encrypt function pointer
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_sensor_set_encrypt_fun(DataEncryptHandler encrypted);

/**
 * @brief Make a raw packet
 * @param[in]  type       Sensor type
 * @param[in]  ringctrl   Ring buffer
 * @param[in]  packctrl   FIFO control for pack
 * @param[in]  prop       Property for raw data
 * @param[in]  sensor_cfg Configuration for sensor
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_make_raw_pack(SensorType type, RingBufCtrl *ringctrl,
                             FifoCtrl *packctrl, VpiSenRawProp *prop,
                             VpiSensorCfg *sensor_cfg);

/**
 * @brief Peek a raw packet
 * @param[in]  fifo   FIFO control for pack
 * @param[in]  num    number of packet
 * @param[in]  item   the pointer to raw packet
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_packet_peek(FifoCtrl *fifo, uint8_t num, void *item);

/**
 * @brief Release a raw packet
 * @param[in]  ring         Ring buffer
 * @param[in]  fifo         FIFO control for pack
 * @param[out] trans_info   current trans information, NULL means no need
 *                          return this value
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_rel_raw_pack(RingBufCtrl *ring, FifoCtrl *fifo,
                            uint16_t *trans_info);

/**
 * @brief Flush sensor raw packet
 * @param[in]  ring   Ring buffer
 * @param[in]  fifo   FIFO control for pack
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_flush_raw_pack(RingBufCtrl *ring, FifoCtrl *fifo);

/**
 * @brief Send empty raw packet
 * @param[in]  type   Sensor type
 * @param[in]  cfg    vital sensors configuration
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_send_empty_raw_pack(SensorType type, VpiSensorCfg *cfg);

/**
 * Result data packet
 */

/**
 * @brief Init result set
 * @param[in]  ctrl the pointer of result set control table
 * @param[in]  len the length of control table
 * @return  VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_result_set_init(SenResSetCtrl *ctrl, uint8_t len);

/**
 * @brief Add data to result buf
 * @param[in]  ctrl  the pointer of result set control
 * @param[in]  type  result type
 * @param[in]  val   pointer of result value
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_sensor_result_add_data(SenResSetCtrl *ctrl, SenResultDataType type,
                               void *val);

/**
 * @brief Generate result packet
 * @param[in]  ctrl        result set control
 * @param[in]  grp         result group
 * @param[in]  packctrl    FIFO control for pack
 * @param[in]  resprop     result property
 * @param[in]  datapack    result data packet
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_gen_result_packet(SenResSetCtrl *ctrl, uint8_t *grp,
                                 FifoCtrl *packctrl, VpiSenResultProp *resprop,
                                 SensorDataPacket **datapack);

/**
 * @brief  Release result data packet
 * @param[in]  packctrl  fifo controller
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_sensor_rel_result_packet(FifoCtrl *packctrl);

/**
 * Storage data packet
 */

/**
 * @brief Init sensor storage
 */
int vpi_sensor_storage_init(void);

/**
 * @brief Storage sensor raw packet
 * @param[in]  packet   raw data packet
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_stg_raw_pack(SensorDataPacket *packet);

/**
 * @brief Storage sensor result packet
 * @param[in]  ctrl     result set control
 * @param[in]  grp      result group
 * @param[in]  prop     result property
 * @param[in]  pack_sz  result packet size
 * @param[in]  flush    directly storage sensor result packet
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_stg_res_pack(SenResSetCtrl *ctrl, uint8_t *grp,
                            VpiSenResultProp *prop, uint16_t pack_sz,
                            bool flush);

/**
 * @brief Init storage packet control
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_init_stg_pack_ctrl(void);

/**
 * @brief Reset storage packet control
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_reset_stg_pack_ctrl(void);

/**
 * @brief Is storage packet empty
 * @return true for empty, others for no empty
 */
bool vpi_sensor_is_stg_pack_empty(void);

/**
 * @brief Print storage packet control message
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_print_stg_pack_ctrl_msg(void);

/**
 * @brief Send packet in storage
 * @param[in]  fifo          FIFO control for pack
 * @param[in]  packet_num    Send number
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_send_pack_in_stg(FifoCtrl *fifo, uint8_t packet_num);

/**
 * @brief Ack_storage packet
 * @param[in]   stg_fifo     FIFO control for pack
 * @param[in]   ack_val      storage ack value
 * @param[out]  trans_info   current trans information, NULL means no need
 *                           return this value
 * @return VPI_SUCCESS for success, others for failure
 */
int vpi_sensor_ack_stg_pack(FifoCtrl *stg_fifo, int ack_val,
                            uint16_t *trans_info);

/**
 * @brief  Storage all result packets cached through the
 * vpi_sensor_stg_res_pack interface when ble connected
 * @param[in]  ctrl       the pointer of result set control
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_sensor_stg_all_res_pack(SenResSetCtrl *ctrl);

/**
 * @brief  Get storage result slice number
 * @param[in]   group_id    result group parameters
 * @param[out]  num         storage result slice number
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_sensor_get_res_stg_slices(uint8_t group_id, uint16_t *num);

/**
 * @brief  Get realtime result slice number
 * @param[in]   group_id    result group parameters
 * @param[out]  num         realtime result slice number
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_sensor_get_res_rt_slices(uint8_t group_id, uint16_t *num);

/**
 * Others
 */

/**
 * @brief  Get body temperature
 * @param[in]   tmp1   the first tmp sensor value
 * @param[in]   tmp2   the second tmp sensor value
 * @param[out]  tmp    Core temperature
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_sensor_get_core_temperature(uint16_t tmp1, uint16_t tmp2,
                                    uint16_t *tmp);

/** @} */
#ifdef __cplusplus
}
#endif

#endif
