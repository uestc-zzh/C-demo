/* hci_vs.h - Bluetooth Host Control Interface Vendor Specific definitions */

/*
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef VERISILICON_INCLUDE_BLUETOOTH_HCI_VS_H_
#define VERISILICON_INCLUDE_BLUETOOTH_HCI_VS_H_

#include <zephyr/bluetooth/hci.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BT_VS_CMD_BIT_WRITE_TX_POWER                0
#define BT_VS_CMD_BIT_READ_TX_POWER                 1
#define BT_VS_CMD_BIT_SET_CODED_SELECT              2
#define BT_VS_CMD_BIT_TEST_TX_START                 4
#define BT_VS_CMD_BIT_TEST_RX_START                 5
#define BT_VS_CMD_BIT_TEST_END                      6
#define BT_VS_CMD_BIT_WRITE_BD_ADDR                 7
#define BT_VS_CMD_BIT_READ_BD_ADDR                  8
#define BT_VS_CMD_BIT_GET_RANDOM_VALUE              9
#define BT_VS_CMD_BIT_WRITE_SCAN_CH_MAP            10
#define BT_VS_CMD_BIT_READ_SCAN_CH_MAP             11


#define BT_HCI_VS_TX_POWER_LEVEL_HIGH           0x00
#define BT_HCI_VS_TX_POWER_LEVEL_MID            0x01
#define BT_HCI_VS_TX_POWER_LEVEL_LOW            0x02
#define BT_HCI_OP_VS_SET_TX_POWER				BT_OP(BT_OGF_VS, 0x0201)
struct bt_hci_cp_vs_set_tx_power {
	uint16_t conn_handle;
	uint8_t  tx_power_level;
} __packed;

struct bt_hci_rp_vs_set_tx_power {
    uint8_t  status;
	uint16_t conn_handle;
	int8_t   tx_power;
} __packed;

#define BT_HCI_OP_VS_GET_TX_POWER				BT_OP(BT_OGF_VS, 0x0202)
struct bt_hci_cp_vs_get_tx_power {
	uint16_t conn_handle;
} __packed;

struct bt_hci_rp_vs_get_tx_power {
	uint8_t  status;
	uint16_t conn_handle;
    int8_t   current_tx_power;
    int8_t   max_tx_power;
} __packed;

#define BT_HCI_OP_VS_SET_CODED_SELECT			BT_OP(BT_OGF_VS, 0x0203)
//unsupported now
struct bt_hci_rp_vs_set_coded_select {
} __packed;

#define BT_HCI_VS_CHANNEL(n)                    ((n) - 2402) / 2

#define BT_HCI_VS_PAYLOAD_PRBS9                 0x00
#define BT_HCI_VS_PAYLOAD_REPEATE_11110000      0x01
#define BT_HCI_VS_PAYLOAD_REPEATE_10101010      0x02
#define BT_HCI_VS_PAYLOAD_PRBS15                0x03
#define BT_HCI_VS_PAYLOAD_REPEATE_11111111      0x04
#define BT_HCI_VS_PAYLOAD_REPEATE_00000000      0x05
#define BT_HCI_VS_PAYLOAD_REPEATE_00001111      0x06
#define BT_HCI_VS_PAYLOAD_REPEATE_01010101      0x07

#define BT_HCI_VS_TX_LE_PHY_1M                  0x01
#define BT_HCI_VS_TX_LE_PHY_2M                  0x02
#define BT_HCI_VS_TX_LE_PHY_CODED_S8            0x03
#define BT_HCI_VS_TX_LE_PHY_CODED_S2            0x04

#define BT_HCI_VS_TEST_TX_OPTION_NON_MODUL      BIT(0)
#define BT_HCI_VS_TEST_TX_OPTION_CONT_TX        BIT(1)
#define BT_HCI_OP_VS_TEST_TX_START              BT_OP(BT_OGF_VS, 0x0205)
struct bt_hci_cp_vs_test_tx_start {
	uint8_t  channel;
    uint8_t  length;
    uint8_t  payload;
    uint8_t  phy;
    uint8_t  tx_power_level;
    uint8_t  option;
    uint8_t  packet_num;
} __packed;

struct bt_hci_rp_vs_test_tx_start {
	uint8_t  status;
} __packed;

#define BT_HCI_VS_RX_LE_PHY_1M                  0x01
#define BT_HCI_VS_RX_LE_PHY_2M                  0x02
#define BT_HCI_VS_RX_LE_PHY_CODED               0x03
#define BT_HCI_OP_VS_TEST_RX_START              BT_OP(BT_OGF_VS, 0x0206)
struct bt_hci_cp_vs_test_rx_start {
	uint8_t  channel;
    uint8_t  phy;
} __packed;

struct bt_hci_rp_vs_test_rx_start {
	uint8_t  status;
} __packed;

#define BT_HCI_OP_VS_TEST_END                   BT_OP(BT_OGF_VS, 0x0207)
struct bt_hci_rp_vs_test_end {
	uint8_t  status;
	uint16_t packet_num;
	uint16_t error_packet_num;
	int8_t   avg_rssi;
	int8_t   max_rssi;
	int8_t   min_rssi;
} __packed;

#define BT_HCI_VS_BD_ADDR_VOL_AREA              0x00
#define BT_HCI_VS_BD_ADDR_NONVOL_AREA           0x01

#define BT_HCI_VS_BD_ADDR_PUBLIC                0x00
#define BT_HCI_VS_BD_ADDR_RANDOM                0x01
#define BT_HCI_OP_VS_WRITE_BD_ADDR              BT_OP(BT_OGF_VS, 0x0208)
struct bt_hci_cp_vs_write_bd_addr {
	uint8_t  option;
	uint8_t  addr_type;
	uint8_t  addr_val[BT_ADDR_SIZE];
} __packed;

struct bt_hci_rp_vs_write_bd_addr {
	uint8_t  status;
} __packed;

#define BT_HCI_OP_VS_READ_BD_ADDR               BT_OP(BT_OGF_VS, 0x0209)
struct bt_hci_cp_vs_read_bd_addr {
	uint8_t  option;
	uint8_t  addr_type;
} __packed;

struct bt_hci_rp_vs_read_bd_addr {
	uint8_t  status;
	uint8_t  option;
	uint8_t  addr_type;
	uint8_t  addr_val[BT_ADDR_SIZE];
} __packed;

#define BT_HCI_OP_VS_GET_RANDOM_NUM             BT_OP(BT_OGF_VS, 0x020a)
struct bt_hci_cp_vs_get_random_num {
	uint8_t  rand_size;
} __packed;

struct bt_hci_rp_vs_get_random_num {
	uint8_t  status;
	uint8_t  rand_size;
	uint8_t  *p_rand;
} __packed;

#define BT_HCI_VS_CH_37_USED                    BIT(0)
#define BT_HCI_VS_CH_38_USED                    BIT(1)
#define BT_HCI_VS_CH_39_USED                    BIT(2)
#define BT_HCI_OP_VS_SET_SCAN_CH_MAP            BT_OP(BT_OGF_VS, 0x020b)
struct bt_hci_cp_vs_set_scan_ch_map {
	uint8_t  ch_map;
} __packed;

struct bt_hci_rp_vs_set_scan_ch_map {
	uint8_t  status;
} __packed;

#define BT_HCI_OP_VS_GET_SCAN_CH_MAP            BT_OP(BT_OGF_VS, 0x020c)
struct bt_hci_rp_vs_get_scan_ch_map {
	uint8_t  status;
	uint8_t  ch_map;
} __packed;


/* Events */

#ifdef __cplusplus
}
#endif

#endif /* VERISILICON_INCLUDE_BLUETOOTH_HCI_VS_H_ */
