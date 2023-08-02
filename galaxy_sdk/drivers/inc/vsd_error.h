/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 *
 * @file vsd_error.h
 * @brief Error codes for VeriSilicon Driver
 * @author Alex Lin <alex.lin@verisilicon.com>
 */

#ifndef _VSD_ERROR_H_
#define _VSD_ERROR_H_

/** @addtogroup DRIVER
 *  - Error code definition
 *
 *  @{
 */

/** Error codes for VeriSilicon Driver (VSD)*/
typedef enum VsdError {
    VSD_ERR_GENERIC         = -1, /**< Generic errors */
    VSD_SUCCESS             = 0, /**< Success */
    VSD_ERR_NOT_INITIALIZED = 1, /**< Required initialization not complete */
    VSD_ERR_INVALID_POINTER = 2, /**< Pointer to invalid memory location */
    VSD_ERR_INVALID_PARAM   = 3, /**< Invalid input parameter */
    VSD_ERR_INVALID_DATA    = 4, /**< Invalid input data */
    VSD_ERR_INVALID_CHANNEL = 5, /**< Selected channel does not exist */
    VSD_ERR_INVALID_STATE   = 6, /**< API or command not valid now */
    VSD_ERR_UNSUPPORTED     = 7, /**< unsupported by this API */
    VSD_ERR_IO              = 8, /**< IO error */
    VSD_ERR_BUSY            = 9, /**< Channel/peripheral is running/busy */
    VSD_ERR_NO_MEMORY       = 10, /**< No enough memory */
    VSD_ERR_FULL            = 11, /**< Hardware overflow/full */
    VSD_ERR_EMPTY           = 12, /**< Hardware underflow/empty */
    VSD_ERR_ABORTED         = 13, /**< An operation was aborted */
    VSD_ERR_TIMEOUT         = 14, /**< Timeout error */
    VSD_ERR_NON_EXIST       = 15, /**< Hardware non-exist */
    VSD_ERR_HW              = 16, /**< Hardware error */
    VSD_ERR_LICENSE         = 17, /**< SDK license error */
} VsdError;
/** @} */
#endif // _VSD_ERROR_H_
