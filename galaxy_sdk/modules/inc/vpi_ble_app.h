/**
 * Copyright (C) 2023 VeriSilicon Holdings Co., Ltd.
 *
 * @file vpi_ble_app.h
 * @brief public head file of ble app
 * @author Shaobo Tu <Shaobo.Tu@verisilicon.com>
 */

#ifndef _VPI_BLE_APP_H
#define _VPI_BLE_APP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BLE
 *  VeriSilicon BLE API.
 *  @ingroup VPI
 *  @{
 */

/**
 * @brief Start verisilicon ble
 * @note This function will only run once
 * @return execution status of this function
 * @retval VPI_SUCCESS for success, otherwise is error
 */
int vpi_ble_start(void);

/**
 * @brief Set name and MAC of Bluetooth
 * @param[in] name name of Bluetooth
 * @param[in] mac_addr MAC address of Bluetooth
 * @return execution status of this function
 * @retval VSD_SUCCESS for success
 */
int vpi_bt_set_name_mac(char *name, uint8_t *mac_addr);

/**
 * @brief Get name and MAC of Bluetooth
 * @param[out] name name of Bluetooth
 * @param[out] mac_addr MAC address of Bluetooth
 * @return execution status of this function
 * @retval VSD_SUCCESS for success, name and mac_addr
 * are valid when retval is VSD_SUCCESS.
 */
int vpi_bt_get_name_mac(char *name, uint8_t *mac_addr);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _VPI_BLE_APP_H */
