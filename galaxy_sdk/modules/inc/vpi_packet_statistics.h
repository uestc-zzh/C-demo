/**
 * Copyright (C) 2023. VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file vpi_packet_statistics.h
 * @brief public head file of packet statistics
 * @author Shaobo Tu <Shaobo.Tu@verisilicon.com>
 */
#ifndef _VPI_PACK_STAT_H_
#define _VPI_PACK_STAT_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup PACKET_STATISTICS
 *  PACKET_STATISTICS API.
 *  @ingroup VPI
 *  @{
 */

#include <stdint.h>
#include "vs_protocol.h"
#include "vpi_event_def.h"
/**
 * @brief Structure of packet statistics control
 */
struct PackStatisticsCtrl;

/**
 * @brief Initialize packet statistics modules
 * @return  PackStatisticsCtrl pointer: on success, NULL : error
 */
struct PackStatisticsCtrl *vpi_packet_statistics_init(void);

/**
 * @brief Process raw data packet statistics
 * @param[in] ctrl packet statistics controller
 * @param[in] data pointer to ble transmit data
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_packet_statistics_proc(struct PackStatisticsCtrl *ctrl,
                               BleTransmitData *data);

/**
 * @brief Reset raw data packet statistics
 * @param[in] ctrl packet statistics controller
 * @return  VPI_SUCCESS, on success,  otherwise is error
 */
int vpi_packet_statistics_reset(struct PackStatisticsCtrl *ctrl);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _VPI_PACK_STAT_H_ */
