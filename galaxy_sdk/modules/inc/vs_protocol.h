/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file vs_protocol.h
 * @brief Common definition for vs protocol
 * @author Shaobo Tu <Shaobo.Tu@verisilicon.com>
 */

#ifndef _VS_PROTOCOL_H_
#define _VS_PROTOCOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** @addtogroup PROTOCOL
 *  @ingroup BLE
 *  @{
 */

/**
 * @brief Enum of flag bit for vs message header
 */
typedef enum VsMsgFlags {
    /**
     * Requires ACK flag
     * @note Set it if sender requires an ACK message from receiver for this
     * message
     */
    FLAG_BIT_REQ_ACK = 0,
    /**
     * Flag of a get message
     * @note Indicate it's a get/fetch message, intend to get status or value
     * from receiver
     */
    FLAG_BIT_GET,
    /**
     * Encrypted flag
     * @note Indicate it's a encrypted message, data needs to be decrypted
     * before using
     */
    FLAG_BIT_ENCRYPTED,
    /**
     * ACK message flag
     * @note Indicate this message is an ACK message for the specific message_id
     * from sender
     */
    FLAG_BIT_ACK,
    /**
     * Sensor data flag
     * @note Indicate it's sensor data message, could be raw or result data
     */
    FLAG_BIT_SEN_DATA,
} VsMsgFlags;

/**
 * @brief - BLE Transmit data struct
 */
typedef struct BleTransmitData {
    uint8_t flags; /**< Flags for message, @see VsMsgFlags */
    uint16_t opcode; /**< Message ID */
    uint16_t length; /**< Length of data in byte */
    uint8_t *data; /**< Pointer of output data */
} __attribute((__packed__)) BleTransmitData;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _VS_PROTOCOL_H_ */
