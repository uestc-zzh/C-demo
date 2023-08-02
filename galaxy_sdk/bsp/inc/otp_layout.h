/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 *
 * @file otp_layout.h
 * @brief the layout of OTP for each platform
 */
#include <stdint.h>
#include "sys_common.h"

#ifndef _OTP_LAYOUT_H
#define _OTP_LAYOUT_H

#define OTP_INVALID_ADDR (-1)
#define OTP_MP_ADDR_BASE (OTP_INVALID_ADDR)
#define PLATFORM_OTP_MP_BYTES (184)

#define PLATFORM_OTP_SIZE_BITS (4096)

#define OTP_MISC_ADDR (0)
#define OTP_MISC_BYTES (4)

#define OTP_FLASH_PRGMED_FLG_ADDR (OTP_MISC_ADDR + OTP_MISC_BYTES)
#define OTP_FLASH_PRGMED_FLG_BYTES (4)

#define OTP_RMA_ACK_FLG_ADDR \
    (OTP_FLASH_PRGMED_FLG_ADDR + OTP_FLASH_PRGMED_FLG_BYTES)
#define OTP_RMA_ACK_FLG_BYTES (4)

#define OTP_VDT_CTL_ADDR (OTP_RMA_ACK_FLG_ADDR + OTP_RMA_ACK_FLG_BYTES)
#define OTP_VDT_CTL_BYTES (4)

#define OTP_IWDT1_ADDR (OTP_VDT_CTL_ADDR + OTP_VDT_CTL_BYTES)
#define OTP_IWDT1_BYTES (4)

#define OTP_IWDT2_ADDR (OTP_IWDT1_ADDR + OTP_IWDT1_BYTES)
#define OTP_IWDT2_BYTES (4)

#define OTP_IWDT3_ADDR (OTP_IWDT2_ADDR + OTP_IWDT2_BYTES)
#define OTP_IWDT3_BYTES (4)

#define OTP_VDT_IWDT_ENB_ADDR (OTP_IWDT3_ADDR + OTP_IWDT3_BYTES)
#define OTP_VDT_IWDT_ENB_BYTES (4)

#define OTP_AES_KEY1_ADDR (OTP_VDT_IWDT_ENB_ADDR + OTP_VDT_IWDT_ENB_BYTES)
#define OTP_AES_KEY1_BYTES (32)

#define OTP_AES_KEY2_ADDR (OTP_AES_KEY1_ADDR + OTP_AES_KEY1_BYTES)
#define OTP_AES_KEY2_BYTES (32)

#define OTP_SM4_KEY1_ADDR (OTP_AES_KEY2_ADDR + OTP_AES_KEY2_BYTES)
#define OTP_SM4_KEY1_BYTES (16)

#define OTP_SM4_KEY2_ADDR (OTP_SM4_KEY1_ADDR + OTP_SM4_KEY1_BYTES)
#define OTP_SM4_KEY2_BYTES (16)

#define OTP_RSA_HASH_KEY_ADDR (OTP_SM4_KEY2_ADDR + OTP_SM4_KEY2_BYTES)
#define OTP_RSA_HASH_KEY_BYTES (32)

#define OTP_ECDSA_KEY1_ADDR (OTP_RSA_HASH_KEY_ADDR + OTP_RSA_HASH_KEY_BYTES)
#define OTP_ECDSA_KEY1_BYTES (32)

#define OTP_ECDSA_KEY2_ADDR (OTP_ECDSA_KEY1_ADDR + OTP_ECDSA_KEY1_BYTES)
#define OTP_ECDSA_KEY2_BYTES (32)

#define OTP_ECDSA_KEY3_ADDR (OTP_ECDSA_KEY2_ADDR + OTP_ECDSA_KEY2_BYTES)
#define OTP_ECDSA_KEY3_BYTES (32)

#define OTP_ECDSA_KEY4_ADDR (OTP_ECDSA_KEY3_ADDR + OTP_ECDSA_KEY3_BYTES)
#define OTP_ECDSA_KEY4_BYTES (32)

#define OTP_ROM_FEATURE_ADDR (OTP_ECDSA_KEY4_ADDR + OTP_ECDSA_KEY4_BYTES)
#define OTP_ROM_FEATURE_BYTES (4)

#define OTP_BLE_MAC_ADDR (OTP_ROM_FEATURE_ADDR + OTP_ROM_FEATURE_BYTES)
#define OTP_BLE_MAC_ADDR_BYTES (8)

#define OTP_SW_VERSION_ADDR (OTP_BLE_MAC_ADDR + OTP_BLE_MAC_ADDR_BYTES)
#define OTP_SW_VERSION_BYTES (8)

#define OTP_CHIP_FEATURE1_ADDR (OTP_SW_VERSION_ADDR + OTP_SW_VERSION_BYTES)
#define OTP_CHIP_FEATURE1_BYTES (4)

#define OTP_ULT_ADDR (OTP_CHIP_FEATURE1_ADDR + OTP_CHIP_FEATURE1_BYTES)
#define OTP_ULT_BYTES (12)

#define OTP_DEVICE_ID_ADDR (OTP_ULT_ADDR + OTP_ULT_BYTES)
#define OTP_DEVICE_ID_BYTES (8)

#define OTP_CHIP_VERSION_ADDR (OTP_DEVICE_ID_ADDR + OTP_DEVICE_ID_BYTES)
#define OTP_CHIP_VERSION_BYTES (4)

#define OTP_UNUSE1_ADDR (OTP_CHIP_VERSION_ADDR + OTP_CHIP_VERSION_BYTES)
#define OTP_UNUSE1_BYTES (24)

#define OTP_LDO_PMU_TRIM1_ADDR (OTP_UNUSE1_ADDR + OTP_UNUSE1_BYTES)
#define OTP_LDO_PMU_TRIM1_BYTES (4)

#define OTP_TNS_OCO_TRIM_ADDR (OTP_LDO_PMU_TRIM1_ADDR + OTP_LDO_PMU_TRIM1_BYTES)
#define OTP_TNS_OCO_TRIM_BYTES (4)

#define OTP_ZSP_ADDR (OTP_TNS_OCO_TRIM_ADDR + OTP_TNS_OCO_TRIM_BYTES)
#define OTP_ZSP_BYTES (4)

#define OTP_STRAP_ADDR (OTP_ZSP_ADDR + OTP_ZSP_BYTES)
#define OTP_STRAP_BYTES (4)

#define OTP_UNUSE2_ADDR (OTP_STRAP_ADDR + OTP_STRAP_BYTES)
#define OTP_UNUSE2_BYTES (108)

#define OTP_USR_WR_LOCK_ADDR (OTP_UNUSE2_ADDR + OTP_UNUSE2_BYTES)
#define OTP_USR_WR_LOCK_BYTES (4)

#define OTP_USR_OVERRIDE_LOCK_ADDR \
    (OTP_USR_WR_LOCK_ADDR + OTP_USR_WR_LOCK_BYTES)
#define OTP_USR_OVERRIDE_LOCK_BYTES (4)

#define OTP_RD_LOCK_ADDR \
    (OTP_USR_OVERRIDE_LOCK_ADDR + OTP_USR_OVERRIDE_LOCK_BYTES)
#define OTP_RD_LOCK_BYTES (4)

#define OTP_WR_LOCK1_ADDR (OTP_RD_LOCK_ADDR + OTP_RD_LOCK_BYTES)
#define OTP_WR_LOCK1_BYTES (4)

#define OTP_WR_LOCK2_ADDR (OTP_WR_LOCK1_ADDR + OTP_WR_LOCK1_BYTES)
#define OTP_WR_LOCK2_BYTES (4)

#define OTP_OVERRIDE_LOCK1_ADDR (OTP_WR_LOCK2_ADDR + OTP_WR_LOCK2_BYTES)
#define OTP_OVERRIDE_LOCK1_BYTES (4)

#define OTP_OVERRIDE_LOCK2_ADDR \
    (OTP_OVERRIDE_LOCK1_ADDR + OTP_OVERRIDE_LOCK1_BYTES)
#define OTP_OVERRIDE_LOCK2_BYTES (4)

union OtpMiscEfuse {
    struct {
        uint32_t test_indicator : 2;
        uint32_t flsh_gpio_pad_gate_off : 1;
        uint32_t test_dbg2_dis : 1;
        uint32_t reserved : 28;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpFlashPrgmedFlgEfuse {
    struct {
        uint32_t flash_pgm_flag : 1;
        uint32_t reserved : 31;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpRmaAckFlgEfuse {
    struct {
        uint32_t rma_ack_flag : 1;
        uint32_t reserved : 31;
    } __attribute((__packed__));
    uint32_t value;
};

union OtpVdtCtlEfuse {
    struct {
        uint32_t reserved1 : 2;
        uint32_t vdt1_vdet1_sel_efuse : 5;
        uint32_t vdt1_stable_time_efuse : 8;
        uint32_t vdt_clk_div_ratio_efuse : 3;
        uint32_t reserved2 : 14;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpIwdtEfuse1 {
    struct {
        uint32_t wtlr_efuse : 16;
        uint32_t reserved : 16;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpIwdtEfuse2 {
    struct {
        uint32_t prescale_divisor_efuse : 8;
        uint32_t reserved1 : 2;
        uint32_t timeout_range_efuse : 3;
        uint32_t reserved2 : 19;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpIwdtEfuse3 {
    struct {
        uint32_t wtrpr_efuse : 16;
        uint32_t reserved : 16;
    } __attribute((__packed__));
    uint32_t value;
};

union OtpVdtIwdtEnbEfuse {
    struct {
        uint32_t vdt1_en_efuse : 1;
        uint32_t wte_efuse : 1;
        uint32_t reserved : 30;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpRomFeature {
    struct {
        uint32_t injection_state : 8;
        uint32_t rom_feature_bit : 8;
        uint32_t reserved : 16;
    } __attribute((__packed__));
    uint32_t value;
};

struct OtpBleMacAddr {
    uint8_t ble_mac_address[5];
    uint8_t mp_flow_crc16[2];
    uint8_t reserved;
} __attribute((__packed__));

struct OtpSwVersion {
    uint8_t sw_version[8];
} __attribute((__packed__));

struct OtpChipFeature1 {
    uint32_t chip_feature1;
} __attribute((__packed__));

struct OtpUlt {
    uint8_t ult[10];
    uint8_t reserved[2];
} __attribute((__packed__));

struct OtpDeviceId {
    uint8_t device_id[8];
} __attribute((__packed__));

union OtpChipVersion {
    struct {
        uint32_t chip_version : 16;
        uint32_t reserved : 16;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpLdoPmuTrimEfuse1 {
    struct {
        uint32_t adcldo_avdd18_bgtm : 5;
        uint32_t ipldo_avdd18_bgtm : 5;
        uint32_t pmu_bgtm : 5;
        uint32_t reserved : 17;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpTnsOcoTrimEfuse {
    struct {
        uint32_t reserved1 : 15;
        uint32_t hoco_trim : 6;
        uint32_t moco_trim : 6;
        uint32_t reserved2 : 5;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpZspEfuse {
    struct {
        uint32_t dis_zsp_bmox_uc_reg : 1;
        uint32_t dis_host_jtag : 1;
        uint32_t dis_zsp_jtag : 1;
        uint32_t dis_cpu_fboot_svt : 1;
        uint32_t reserved : 28;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpStrapEfuse {
    struct {
        uint32_t strap_jtag_hw_dis : 1;
        uint32_t reserved : 31;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpUsrWrLockEfuse {
    struct {
        uint32_t vdt_wr_lock : 1;
        uint32_t iwdt1_wr_lock : 1;
        uint32_t iwdt2_wr_lock : 1;
        uint32_t iwdt3_wr_lock : 1;
        uint32_t vdt_iwdt_enb_wr_lock : 1;
        uint32_t usr_wr_lock_wr_lock : 1;
        uint32_t usr_override_lock_wr_lock : 1;
        uint32_t reserved : 25;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpUsrOverrideLockEfuse {
    struct {
        uint32_t vdt_override_lock : 1;
        uint32_t iwdt1_override_lock : 1;
        uint32_t iwdt2_override_lock : 1;
        uint32_t iwdt3_override_lock : 1;
        uint32_t vdt_iwdt_enb_override_lock : 1;
        uint32_t usr_wr_lock_override_lock : 1;
        uint32_t usr_burn_lock_override_lock : 1;
        uint32_t reserved : 25;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpRdLockEfuse {
    struct {
        uint32_t reserved1 : 8;
        uint32_t key1_rd_lock : 1;
        uint32_t key2_rd_lock : 1;
        uint32_t key3_rd_lock : 1;
        uint32_t key4_rd_lock : 1;
        uint32_t reserved2 : 20;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpWrLock1Efuse {
    struct {
        uint32_t misc_wr_lock : 1;
        uint32_t flsh_prgmed_wr_lock : 1;
        uint32_t rma_ack_flg_wr_lock : 1;
        uint32_t reserved1 : 5;
        uint32_t key1_wr_lock : 1;
        uint32_t key2_wr_lock : 1;
        uint32_t key3_wr_lock : 1;
        uint32_t key4_wr_lock : 1;
        uint32_t shadow_reg32_39_wr_lock : 1;
        uint32_t shadow_reg40_47_wr_lock : 1;
        uint32_t shadow_reg48_55_wr_lock : 1;
        uint32_t shadow_reg56_63_wr_lock : 1;
        uint32_t shadow_reg64_71_wr_lock : 1;
        uint32_t shadow_reg72_wr_lock : 1;
        uint32_t shadow_reg73_76_wr_lock : 1;
        uint32_t shadow_reg77_84_wr_lock : 1;
        uint32_t shadow_reg85_wr_lock : 1;
        uint32_t shadow_reg86_wr_lock : 1;
        uint32_t shadow_reg87_wr_lock : 1;
        uint32_t shadow_reg88_wr_lock : 1;
        uint32_t shadow_reg89_wr_lock : 1;
        uint32_t ldo_pmu_trim1_wr_lock : 1;
        uint32_t tns_oco_trim_wr_lock : 1;
        uint32_t zsp_efuse_wr_lock : 1;
        uint32_t strap_efuse_wr_lock : 1;
        uint32_t shadow_reg94_wr_lock : 1;
        uint32_t shadow_reg95_wr_lock : 1;
        uint32_t reserved : 1;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpWrLock2Efuse {
    struct {
        uint32_t shadow_reg96_wr_lock : 1;
        uint32_t shadow_reg97_wr_lock : 1;
        uint32_t shadow_reg98_wr_lock : 1;
        uint32_t shadow_reg99_wr_lock : 1;
        uint32_t shadow_reg100_wr_lock : 1;
        uint32_t shadow_reg101_wr_lock : 1;
        uint32_t shadow_reg102_wr_lock : 1;
        uint32_t shadow_reg103_wr_lock : 1;
        uint32_t shadow_reg104_wr_lock : 1;
        uint32_t shadow_reg105_wr_lock : 1;
        uint32_t shadow_reg106_wr_lock : 1;
        uint32_t shadow_reg107_wr_lock : 1;
        uint32_t shadow_reg108_wr_lock : 1;
        uint32_t shadow_reg109_wr_lock : 1;
        uint32_t shadow_reg110_wr_lock : 1;
        uint32_t shadow_reg111_wr_lock : 1;
        uint32_t shadow_reg112_wr_lock : 1;
        uint32_t shadow_reg113_wr_lock : 1;
        uint32_t shadow_reg114_wr_lock : 1;
        uint32_t shadow_reg115_wr_lock : 1;
        uint32_t shadow_reg116_wr_lock : 1;
        uint32_t shadow_reg117_wr_lock : 1;
        uint32_t shadow_reg118_wr_lock : 1;
        uint32_t shadow_reg119_wr_lock : 1;
        uint32_t shadow_reg120_wr_lock : 1;
        uint32_t reserved1 : 1;
        uint32_t reserved2 : 1;
        uint32_t rd_lock_wr_lock : 1;
        uint32_t wr_lock1_wr_lock : 1;
        uint32_t wr_lock2_wr_lock : 1;
        uint32_t burn_lock1_wr_lock : 1;
        uint32_t burn_lock2_wr_lock : 1;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpOverrideLock1Efuse {
    struct {
        uint32_t misc_override_lock : 1;
        uint32_t flash_prgmed_override_lock : 1;
        uint32_t rma_ack_flg_override_lock : 1;
        uint32_t reserved1 : 5;
        uint32_t key1_override_lock : 1;
        uint32_t key2_override_lock : 1;
        uint32_t key3_override_lock : 1;
        uint32_t key4_override_lock : 1;
        uint32_t efuse32_39_override_lock : 1;
        uint32_t efuse40_47_override_lock : 1;
        uint32_t efuse48_55_override_lock : 1;
        uint32_t efuse56_63_override_lock : 1;
        uint32_t efuse64_71_override_lock : 1;
        uint32_t efuse72_override_lock : 1;
        uint32_t efuse73_76_override_lock : 1;
        uint32_t efuse77_84_override_lock : 1;
        uint32_t efuse85_override_lock : 1;
        uint32_t efuse86_override_lock : 1;
        uint32_t efuse87_override_lock : 1;
        uint32_t efuse88_override_lock : 1;
        uint32_t efuse89_override_lock : 1;
        uint32_t ldo_pmu_trim1_overrride_lock : 1;
        uint32_t tns_oco_trim_override_lock : 1;
        uint32_t zsp_efuse_override_lock : 1;
        uint32_t strap_efuse_override_lock : 1;
        uint32_t efuse_94_override_lock : 1;
        uint32_t efuse_95_override_lock : 1;
        uint32_t reserved2 : 1;
    } __attribute((__packed__));
    uint32_t value;
};
union OtpOverrideLock2Efuse {
    struct {
        uint32_t efuse_96_override_lock : 1;
        uint32_t efuse_97_override_lock : 1;
        uint32_t efuse_98_override_lock : 1;
        uint32_t efuse_99_override_lock : 1;
        uint32_t efuse_100_override_lock : 1;
        uint32_t efuse_101_override_lock : 1;
        uint32_t efuse_102_override_lock : 1;
        uint32_t efuse_103_override_lock : 1;
        uint32_t efuse_104_override_lock : 1;
        uint32_t efuse_105_override_lock : 1;
        uint32_t efuse_106_override_lock : 1;
        uint32_t efuse_107_override_lock : 1;
        uint32_t efuse_108_override_lock : 1;
        uint32_t efuse_109_override_lock : 1;
        uint32_t efuse_110_override_lock : 1;
        uint32_t efuse_111_override_lock : 1;
        uint32_t efuse_112_override_lock : 1;
        uint32_t efuse_113_override_lock : 1;
        uint32_t efuse_114_override_lock : 1;
        uint32_t efuse_115_override_lock : 1;
        uint32_t efuse_116_override_lock : 1;
        uint32_t efuse_117_override_lock : 1;
        uint32_t efuse_118_override_lock : 1;
        uint32_t efuse_119_override_lock : 1;
        uint32_t efuse_120_override_lock : 1;
        uint32_t reserved1 : 1;
        uint32_t reserved2 : 1;
        uint32_t rd_lock_burn_lock : 1;
        uint32_t wr_lock1_burn_lock : 1;
        uint32_t wr_lock2_burn_lock : 1;
        uint32_t override_lock1_burn_lock : 1;
        uint32_t override_lock2_burn_lock : 1;
    } __attribute((__packed__));
    uint32_t value;
};

// INJECT_STATE
#define OTP_INJECTION_STATE_ADDR (OTP_ROM_FEATURE_ADDR)
#define OTP_INJECTION_STATE_BYTES (1)
typedef enum {
    OTP_INJ_DEVICE_ID = 0,
    OTP_INJ_ROOT_KEY,
    OTP_INJ_ECDSA_BOOT_KEY,
    OTP_INJ_ECDSA_EXCH_KEY,
    OTP_INJ_BLE_MAC,
    OTP_INJ_MAX
} OTP_INJECTION_ID;

typedef enum {
    OTP_ROM_FEAT_JTAG = 0,
    OTP_ROM_FEAT_UART,
    OTP_ROM_FEAT_SECURE_BOOT,
    OTP_ROM_FEAT_ANTI_RB,
    OTP_ROM_FEAT_DUAL_IMG,
    OTP_ROM_FEAT_BURN_OVER_UART,
} OTP_ROM_FEAT_ID;

union OtpRomFeatByte1 {
    struct {
        uint8_t jtag_disable : 1;
        uint8_t uart_disable : 1;
        uint8_t secure_boot : 1;
        uint8_t anti_roolback : 1;
        uint8_t dual_boot_img : 1;
        uint8_t burn_over_uart : 1;
        uint8_t reserved1 : 2;
    } __attribute((__packed__));

    uint8_t value;
};

union OtpInjectionState {
    struct {
        uint8_t device_id : 1;
        uint8_t root_key : 1;
        uint8_t ecdsa_boot_key : 1;
        uint8_t ecdsa_exch_key : 1;
        uint8_t ble_mac : 1;
        uint8_t reserved : 3;
    } __attribute((__packed__));

    uint8_t value;
};
union OtpChipFeatures {
    struct {
        uint32_t sip_flash : 8;
        uint32_t hw_security : 1;
        uint32_t hw_aes : 1;
        uint32_t hw_ecdsa : 1;
        uint32_t hw_rsa : 1;
        uint32_t hw_trng : 1;
        uint32_t hw_hash : 1;
        uint32_t reserved : 2;
        uint32_t ble_4p2 : 1;
        uint32_t ble_5p0 : 1;
        uint32_t ble_5p2 : 1;
        uint32_t ble_new : 1;
        uint32_t bt_classic : 1;
        uint32_t bt_dual_mode : 1;
        uint32_t bluetooth_rf : 1;
        uint32_t ble_mesh : 1;
        uint32_t fpu_on_zsp : 1;
        uint32_t qspi_xip : 1;
        uint32_t psram : 1;
        uint32_t cds : 1;
        uint32_t battery_charger : 1;
        uint32_t fgu : 1;
        uint32_t neural_network : 1;
        uint32_t reserved2 : 1;
    } __attribute((__packed__));

    uint32_t value;
};

#endif //_OTP_LAYOUT_H
