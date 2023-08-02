/**
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file cocopalmR_board.c
 * @brief Functions for cocopalmR board
 */
#include <malloc.h>
#include "vs_conf.h"
#include "cocopalmR_board.h"
#include "sys_common.h"
#include "vsd_error.h"
#include "TI_ADS1291.h"
#include "TI_ADS1299.h"
#include "hal_ecg.h"
#include "MAX3010X.h"
#include "hal_ppg.h"
#include "vs_conf.h"
#include "hal_battery.h"
#include "lipoly_battery.h"
#include "rt9426.h"
#include "TI_TMP117.h"
#include "hal_imu.h"
#include "hal_pwm.h"
#include "bmi160.h"
#include "hal_ppg_ecg_combo.h"
#include "MAX86150.h"
#include "hal_tmp.h"
#include "MAX30208.h"
#include "hal_charger.h"
#include "mp2662.h"
#include "bsp_sysctl.h"
#include "soc_pinmux.h"
#include "soc_pin_define.h"
#include "soc_init.h"
#include "hal_flash.h"
#include "flash_partition.h"
#include "hal_uart.h"
#include "hal_pmu.h"
#include "wm8960.h"

static void *device_list[MAX_DEVICE_ID];

#define MAX86150_DEFAULT_PPG_CAP (PPG_MODE_REDIR)
#define MAX86150_DEFAULT_ECG_CAP (ECG_CHANNEL_ID_0)
#define MAX86150_DEFAULT_ECG_SAMPLE_RATE (200)

static const SocPinCtl pin_mux_ctrl[] = {
    /* Ext interrupt pins */
    SOC_PAD(PIN_IRQ_DS4, SW_MUX(0x13)), /* INT, Wake-Up by BTN */
    SOC_PAD(PIN_IRQ_DS5, SW_MUX(0x13)), /* INT, INT1 from IMU, Wake */
    SOC_PAD(PIN_IRQ_DS6, SW_MUX(0x13)), /* INT, INT2 from IMU */
    SOC_PAD(PIN_IRQ_DS7, SW_MUX(0x13)), /* INT, from Charger */
    SOC_PAD(PIN_IRQ_DS12, SW_MUX(0x13)), /* INT, from ECG DRDY */
    SOC_PAD(PIN_IRQ_DS11, SW_MUX(0x13)), /* INT, from PPG */
    SOC_PAD(PIN_IRQ_DS13, SW_MUX(0x13)), /* INT, from TMP */

    /* GPIO pins */
    SOC_PAD(PIN_IRQ_DS8, SW_MUX(0x13)), /* GPIO, LED_Blue */
    SOC_PAD(PIN_IRQ_DS9, SW_MUX(0x13)), /* GPIO, LED_Green */
    SOC_PAD(PIN_IRQ_DS10, SW_MUX(0x13)), /* GPIO, ECG PWDN Ctrl */

    /* SPI0 */
    SOC_PAD(PIN_SPI_CS0, SW_MUX(0x07)), /* SPI_CS0 */
    SOC_PAD(PIN_SPI_MOSI0, SW_MUX(0x07)), /* SPI_MOSI0 */
    SOC_PAD(PIN_SPI_MISO0, SW_MUX(0x07)), /* SPI_MISO0 */
    SOC_PAD(PIN_SPI_CLK0, SW_MUX(0x07)), /* SPI_CLK0 */

    /* I2C0 */
    SOC_PAD(PIN_I2C_SCL0, SW_MUX(0x03)), /* I2C_SCL0 */
    SOC_PAD(PIN_I2C_SDA0, SW_MUX(0x03)), /* I2C_SDA0 */

    /* I2C1 */
    SOC_PAD(PIN_I2C_SCL1, SW_MUX(0x03)), /* I2C_SCL1 */
    SOC_PAD(PIN_I2C_SDA1, SW_MUX(0x03)), /* I2C_SDA1 */

    /* UART0 */
    SOC_PAD(PIN_UART_TXD0, SW_MUX(0x04)), /* UART_TXD0 */
    SOC_PAD(PIN_UART_RXD0, SW_MUX(0x04)), /* UART_RXD0 */

    /* UART3 */
    SOC_PAD(PIN_UART_CTS0, SW_MUX(0x01)), /* UART_TXD3 */
    SOC_PAD(PIN_UART_RTS0, SW_MUX(0x01)), /* UART_RXD3 */

    /* UART1 */
    SOC_PAD(PIN_UART_TXD1, SW_MUX(0x04)), /* UART_TXD1 */
    SOC_PAD(PIN_UART_RXD1, SW_MUX(0x04)), /* UART_RXD1 */

    /* UART2 */
    SOC_PAD(PIN_UART_TXD2, SW_MUX(0x04)), /* UART_TXD2 */
    SOC_PAD(PIN_UART_RXD2, SW_MUX(0x04)), /* UART_RXD2 */

    /* I2S */
    SOC_PAD(PIN_I2S_BCLK, SW_MUX(0x0d)), /* I2S bclk */
    SOC_PAD(PIN_I2S_LRCK, SW_MUX(0x0d)), /* I2S lrclk */
    SOC_PAD(PIN_I2S_DI, SW_MUX(0x0d)), /* I2S DI */
    SOC_PAD(PIN_I2S_DO, SW_MUX(0x0d)), /* I2S DO */
    SOC_PAD(PIN_CACREF, SW_MUX(0x01)), /* I2S mclk_out */

};

static const SensorHwConfig ecg_sensor_config = {
    .power_pin      = NULL,
    .data_ready_pin = &ecg_data_pin,
    .reset_pin      = &ecg_reset_pin,
    .device_id      = 0,
    .rx_data_cont   = true,
};

static const I2cConfig max30208_i2c_cfg = {
    .address_width = I2C_HAL_ADDRESS_WIDTH_7BIT,
    .freq          = I2C_BUS_SPEED_400K,
    .work_mode     = I2C_MODE_MASTER,
    .xfer_mode     = XFER_MODE_POLLING,
    .dev_addr      = 0x53,
    .reg_width     = 1,
};

static const I2cConfig tmp117_i2c_cfg = {
    .address_width = I2C_HAL_ADDRESS_WIDTH_7BIT,
    .freq          = I2C_BUS_SPEED_400K,
    .work_mode     = I2C_MODE_MASTER,
    .xfer_mode     = XFER_MODE_POLLING,
    .dev_addr      = 0x49,
    .reg_width     = 1,
};

static const I2cConfig ppg_i2c_cfg = {
    .address_width = I2C_HAL_ADDRESS_WIDTH_7BIT,
    .freq          = I2C_BUS_SPEED_400K,
    .work_mode     = I2C_MODE_MASTER,
    .xfer_mode     = XFER_MODE_POLLING,
    .dev_addr      = MAX3010X_ADDRESS,
    .reg_width     = 1,
};

static const I2cConfig ppg_ecg_combo_i2c_cfg = {
    .address_width = I2C_HAL_ADDRESS_WIDTH_7BIT,
    .freq          = I2C_BUS_SPEED_400K,
    .work_mode     = I2C_MODE_MASTER,
    .xfer_mode     = XFER_MODE_POLLING,
    .dev_addr      = MAX86150_ADDRESS,
    .reg_width     = 1,
};

static const I2cConfig battery_i2c_config = {
    .address_width = I2C_HAL_ADDRESS_WIDTH_7BIT,
    .freq          = I2C_BUS_SPEED_400K,
    .work_mode     = I2C_MODE_MASTER,
    .xfer_mode     = XFER_MODE_POLLING,
    .dev_addr      = RT9426_ADDRESS,
    .reg_width     = 1,
};

static const I2cConfig charger_i2c_config = {
    .address_width = I2C_HAL_ADDRESS_WIDTH_7BIT,
    .freq          = I2C_BUS_SPEED_400K,
    .work_mode     = I2C_MODE_MASTER,
    .xfer_mode     = XFER_MODE_POLLING,
    .dev_addr      = 0x07,
    .reg_width     = 1,
};

static const SensorHwConfig ppg_ecg_sensor_config = {
    .power_pin      = NULL,
    .data_ready_pin = &ppg_data_pin,
    .reset_pin      = NULL,
    .device_id      = 0,
    .rx_data_cont   = false,
};

#if CONFIG_APP_EEG_SENSOR_ENABLED
#define TI1299_DEFAULT_ECG_CAP (ECG_CHANNEL_ID_0 | ECG_CHANNEL_ID_1)
#define TI1299_DEFAULT_SAMPLE_RATE (250)
static SpiTransConfig ads1299_spi_cfg = {
    .speed_hz       = 16000000,
    .cs_chip        = SPI_CS_ID_0,
    .xfer_mode      = XFER_MODE_POLLING,
    .timeout        = HAL_WAIT_FOREVER,
    .cpol_cpha_mode = SPI_CPOL_CPHA_MODE1,
    .delay_in_us    = 10,
    .batch_enable   = 0x00,
};
#else
#define TI1291_DEFAULT_ECG_CAP (ECG_CHANNEL_ID_0 | ECG_CHANNEL_LOD)
#define TI1291_DEFAULT_SAMPLE_RATE (125)
static const SpiTransConfig ads1291_spi_cfg = {
    .speed_hz       = 16000000,
    .cs_chip        = SPI_CS_ID_0,
    .xfer_mode      = XFER_MODE_POLLING,
    .timeout        = HAL_WAIT_FOREVER,
    .cpol_cpha_mode = SPI_CPOL_CPHA_MODE1,
    .delay_in_us    = 10,
    .batch_enable   = 0,
};
#endif

static const SensorHwConfig ppg_sensor_config = {
    .power_pin      = NULL,
    .data_ready_pin = &ppg_data_pin,
    .reset_pin      = NULL,
    .device_id      = 0,
    .rx_data_cont   = false,
};

static const I2cConfig imu_i2c_cfg = {
    .address_width = I2C_HAL_ADDRESS_WIDTH_7BIT,
    .freq          = I2C_BUS_SPEED_400K,
    .work_mode     = I2C_MODE_MASTER,
    .xfer_mode     = XFER_MODE_POLLING,
    .dev_addr      = 0x68,
    .reg_width     = 1,
};

static const ImuConfig imu_sensor_config = {
    .power_pin = NULL,
    .data_pin  = &imu_data_pin,
    .wake_pin  = &imu_wake_pin,
};

static I2cConfig wm8960_i2c_cfg = {
    .address_width = I2C_HAL_ADDRESS_WIDTH_7BIT,
    .freq          = I2C_BUS_SPEED_400K,
    .work_mode     = I2C_MODE_MASTER,
    .xfer_mode     = XFER_MODE_POLLING,
    .dev_addr      = WM8960_I2C_ADDR,
    .reg_width     = 1,
};

static const CodecHWConfig codec_hw_cfg[CODEC_PORT_MAX] = {
    {
        .id = CODEC_ID_1, // wm8960
    },
};

APP_SECTION
static int power_init(void)
{
    return 0;
}

static inline int create_wm8960(void)
{
    CodecDevice *wm8960;
    int ret;

    ret = wm8960_device_init(&codec_hw_cfg[0]);
    if (VSD_SUCCESS != ret)
        return ret;

    wm8960 = hal_codec_get_device(CODEC_ID_1);
    if (!wm8960)
        return VSD_ERR_NON_EXIST;

    wm8960->bus_dev.type = BUS_TYPE_I2C;
    wm8960->bus_dev.i2c = hal_i2c_get_device(I2C_ID_0);
    wm8960->bus_cfg.i2c = &wm8960_i2c_cfg;
    return ret;
}

static inline int create_ecg_device(void)
{
    int ret;
    SocIfInfo info;
    UartDevice *uart_dev = NULL;
    EcgDevice *ecg_dev   = (EcgDevice *)malloc(sizeof(EcgDevice));
    if (!ecg_dev) {
        return VSD_ERR_NO_MEMORY;
    }

    // set the pin mode as SPI
    info.port = 0;
    soc_pinmux_set(SOC_IF_SPI, &info, 1);

    // try to scan the ADS1291
    memset(ecg_dev, 0, sizeof(EcgDevice));
    ecg_dev->bus_device.type    = BUS_TYPE_SPI;
    ecg_dev->bus_device.port_id = SPI_ID_SPI0;
    ecg_dev->type               = SENSOR_TYPE_ECG;
    ecg_dev->flag               = 0;
    ecg_dev->bus_device.spi =
        hal_spi_get_dev_by_id(ecg_dev->bus_device.port_id);
    ret = hal_spi_drv_init(ecg_dev->bus_device.spi);
    if (ret != VSD_SUCCESS) {
        return ret;
    }
#if CONFIG_APP_EEG_SENSOR_ENABLED
    ecg_dev->capability     = TI1299_DEFAULT_ECG_CAP;
    ecg_dev->default_sr     = TI1299_DEFAULT_SAMPLE_RATE;
    ecg_dev->dma_mode       = true;
    ecg_dev->bus_config.spi = &ads1299_spi_cfg;
    ret = ti_ads1299_device_init(ecg_dev, &ecg_sensor_config);
#else
    ecg_dev->capability     = TI1291_DEFAULT_ECG_CAP;
    ecg_dev->default_sr     = TI1291_DEFAULT_SAMPLE_RATE;
    ecg_dev->dma_mode       = true;
    ecg_dev->bus_config.spi = &ads1291_spi_cfg;
    ret = ti_ads1291_device_init(ecg_dev, &ecg_sensor_config);
#endif
    if (ret == VSD_SUCCESS) {
        goto exit;
    }
    // switch to UART for failure
    info.port = ecg_dev->bus_device.port_id;
    soc_pinmux_set(SOC_IF_UART, &info, 1);
    uart_dev = hal_uart_get_device(UART_DEV_ID_0);
    if (uart_dev) {
        hal_uart_fifo_flush(uart_dev);
    }
    // failed to create a valid ECG device
    device_list[ECG_SENSOR_ID] = NULL;
    free(ecg_dev);
    return VSD_ERR_HW;

exit:
    device_list[ECG_SENSOR_ID] = ecg_dev;
    return ret;
}

static inline int create_imu_device(void)
{
    int ret;
    /*init the bmi160 sensor driver*/
    ImuDevice *imu_dev = malloc(sizeof(ImuDevice));
    if (!imu_dev) {
        return VSD_ERR_NO_MEMORY;
    }
    memset(imu_dev, 0, sizeof(ImuDevice));
    imu_dev->bus_device.type    = BUS_TYPE_I2C;
    imu_dev->bus_device.port_id = 1;
    imu_dev->bus_device.i2c = hal_i2c_get_device(imu_dev->bus_device.port_id);
    imu_dev->bus_config.i2c = &imu_i2c_cfg;
    ret                     = bmi160_device_init(imu_dev, &imu_sensor_config);
    if (ret) {
        device_list[IMU_SENSOR_ID] = NULL;
        free(imu_dev);
        return VSD_ERR_HW;
    }
    device_list[IMU_SENSOR_ID] = imu_dev;
    return ret;
}

static inline int create_comb_device(PpgEcgComboDevice **comb_dev)
{
    int ret;

    if (!comb_dev)
        return VSD_ERR_INVALID_POINTER;

    /*init ppg ecg combo device*/
    PpgEcgComboDevice *max86150_dev =
        (PpgEcgComboDevice *)malloc(sizeof(PpgEcgComboDevice));
    if (!max86150_dev) {
        return VSD_ERR_NO_MEMORY;
    }
    memset(max86150_dev, 0, sizeof(PpgEcgComboDevice));
    max86150_dev->ppg_device = (PpgDevice *)malloc(sizeof(PpgDevice));
    max86150_dev->ecg_device = (EcgDevice *)malloc(sizeof(EcgDevice));
    if (!(max86150_dev->ppg_device) || !(max86150_dev->ecg_device)) {
        return VSD_ERR_NO_MEMORY;
    }
    memset(max86150_dev->ppg_device, 0, sizeof(PpgDevice));
    memset(max86150_dev->ecg_device, 0, sizeof(EcgDevice));
    max86150_dev->bus_device.type    = BUS_TYPE_I2C;
    max86150_dev->bus_device.port_id = 0;
    max86150_dev->bus_device.i2c =
        hal_i2c_get_device(max86150_dev->bus_device.port_id);
    max86150_dev->bus_config.i2c         = &ppg_ecg_combo_i2c_cfg;
    max86150_dev->ppg_device->capability = (MAX86150_DEFAULT_PPG_CAP);
    max86150_dev->ecg_device->capability = (MAX86150_DEFAULT_ECG_CAP);
    max86150_dev->ecg_device->default_sr = MAX86150_DEFAULT_ECG_SAMPLE_RATE;
    max86150_dev->ecg_device->dma_mode   = false;
    ret = max_86150_device_init(max86150_dev, &ppg_ecg_sensor_config);
    if (ret) {
        device_list[PPG_ECG_SENSOR_ID] = NULL;
        free(max86150_dev);
        return VSD_ERR_HW;
    }
    device_list[PPG_ECG_SENSOR_ID] = max86150_dev;
    *comb_dev                      = max86150_dev;
    return ret;
}

static inline int create_ppg_device(void)
{
    int ret;

    PpgDevice *ppg_dev = (PpgDevice *)malloc(sizeof(PpgDevice));
    if (!ppg_dev) {
        return VSD_ERR_NO_MEMORY;
    }
    memset(ppg_dev, 0, sizeof(PpgDevice));
    ppg_dev->bus_device.type    = BUS_TYPE_I2C;
    ppg_dev->bus_device.port_id = 0;
    ppg_dev->bus_device.i2c = hal_i2c_get_device(ppg_dev->bus_device.port_id);
    ppg_dev->bus_config.i2c = &ppg_i2c_cfg;
    ppg_dev->capability     = PPG_MODE_REDIRGREEN;
    ret = max_3010x_device_init(ppg_dev, &ppg_sensor_config);
    if (ret != VSD_SUCCESS) {
        device_list[PPG_SENSOR_ID] = NULL;
        free(ppg_dev);
        return VSD_ERR_HW;
    }
    device_list[PPG_SENSOR_ID] = ppg_dev;
    return VSD_SUCCESS;
}

static inline int create_tmp_device(void)
{
    int ret;
    /* init the tmp sensor driver */
    TmpDevice *tmp_dev = (TmpDevice *)malloc(sizeof(TmpDevice));
    if (!tmp_dev) {
        return VSD_ERR_NO_MEMORY;
    }

    // scan the max30208
    memset(tmp_dev, 0, sizeof(TmpDevice));
    tmp_dev->capability         = TMP_CAP_ONESHOT;
    tmp_dev->bus_device.type    = BUS_TYPE_I2C;
    tmp_dev->bus_device.port_id = 0;
    tmp_dev->bus_device.i2c = hal_i2c_get_device(tmp_dev->bus_device.port_id);
    tmp_dev->bus_config.i2c = &max30208_i2c_cfg;
    ret                     = max30208_device_init(tmp_dev);

    if (ret == VSD_SUCCESS)
        goto exit;

    // scan the tmp117
    memset(tmp_dev, 0, sizeof(TmpDevice));
    tmp_dev->capability =
        (TMP_CAP_ONESHOT | TMP_CAP_CONV_CONT | TMP_CAP_SHUTDOWN);
    tmp_dev->bus_device.type    = BUS_TYPE_I2C;
    tmp_dev->bus_device.port_id = 0;
    tmp_dev->bus_device.i2c = hal_i2c_get_device(tmp_dev->bus_device.port_id);
    tmp_dev->bus_config.i2c = &tmp117_i2c_cfg;
    ret                     = tmp117_device_init(tmp_dev);
    if (ret == VSD_SUCCESS)
        goto exit;

    // failed to create a valid TMP device
    device_list[TMP_SENSOR_ID] = NULL;
    free(tmp_dev);
    return VSD_ERR_HW;

exit:
    device_list[TMP_SENSOR_ID] = tmp_dev;
    return ret;
}

static inline int create_battery_device(void)
{
    int ret;

    /* initialize battery driver */
    BatteryDev *battery_dev = (BatteryDev *)malloc(sizeof(BatteryDev));
    if (battery_dev == NULL) {
        return VSD_ERR_NO_MEMORY;
    }
    memset((void *)battery_dev, 0, sizeof(BatteryDev));
    battery_dev->bus_device.type    = BUS_TYPE_I2C;
    battery_dev->bus_device.port_id = 1;
    battery_dev->bus_device.i2c =
        hal_i2c_get_device(battery_dev->bus_device.port_id);
    battery_dev->bus_config.i2c = &battery_i2c_config;
    ret                         = rt9426_device_init(battery_dev);
    if (ret != VSD_SUCCESS) {
        device_list[BATTERY_ID] = NULL;
        free(battery_dev);
        return ret;
    } else {
        device_list[BATTERY_ID] = battery_dev;
        hal_battery_create(&(battery_dev->opers));
    }
    return ret;
}

static inline int create_charger_device(void)
{
    int ret;

    /* initialize charger driver */
    ChargerDevice *charger_dev = (ChargerDevice *)malloc(sizeof(ChargerDevice));
    if (charger_dev == NULL) {
        return VSD_ERR_NO_MEMORY;
    }
    memset((void *)charger_dev, 0, sizeof(ChargerDevice));
    charger_dev->bus_device.type    = BUS_TYPE_I2C;
    charger_dev->bus_device.port_id = 1;
    charger_dev->bus_device.i2c =
        hal_i2c_get_device(charger_dev->bus_device.port_id);
    charger_dev->bus_config.i2c = &charger_i2c_config;
    charger_dev->irq_pin        = &charger_irq_pin;
    ret                         = mp2662_device_init(charger_dev);
    if (ret != VSD_SUCCESS) {
        device_list[CHARGER_ID] = NULL;
        free(charger_dev);
        return ret;
    } else {
        device_list[CHARGER_ID] = charger_dev;
    }
    return ret;
}

static inline int create_misc_devices(void)
{
    device_list[PWR_KEY_DEV_ID]   = (void *)&pwr_key_pin;
    device_list[LED_GPIO_DEV0_ID] = (void *)&blue_led_config;
    device_list[LED_GPIO_DEV1_ID] = (void *)&green_led_config;

    return VSD_SUCCESS;
}

static inline int create_drivers(void)
{
    int ret = VSD_SUCCESS;
    PpgEcgComboDevice *comb_dev;

    ret = create_wm8960();
    create_battery_device();
    create_charger_device();
    create_imu_device();
    create_ecg_device();
    create_tmp_device();
    create_ppg_device();
    create_misc_devices();

    /* check if need to create ppg ecg combo device */
    if (!device_list[PPG_SENSOR_ID] || !device_list[ECG_SENSOR_ID]) {
        memset(&comb_dev, 0, sizeof(comb_dev));
        ret = create_comb_device(&comb_dev);
        if (ret == VSD_SUCCESS) {
            if (!device_list[PPG_SENSOR_ID])
                device_list[PPG_SENSOR_ID] = comb_dev->ppg_device;
            else {
                free(comb_dev->ppg_device);
                comb_dev->ppg_device = NULL;
            }
            if (!device_list[ECG_SENSOR_ID]) {
                device_list[ECG_SENSOR_ID] = comb_dev->ecg_device;
            } else {
                free(comb_dev->ecg_device);
                comb_dev->ecg_device = NULL;
            }
        }
    }
    return ret;
}

/* Map pwr_key_pin to corresponding wakeup source */
APP_SECTION
static void power_key_to_wakeup_src(uint8_t port, uint8_t *wakeup_src)
{
    switch (port) {
    case PIN_IRQ_DS4:
        *wakeup_src = WAKEUP_SOURCE_EXT_IO_0;
        break;
    case PIN_IRQ_DS5:
        *wakeup_src = WAKEUP_SOURCE_EXT_IO_1;
        break;
    case PIN_IRQ_DS6:
        *wakeup_src = WAKEUP_SOURCE_EXT_IO_2;
        break;
    case PIN_IRQ_DS7:
        *wakeup_src = WAKEUP_SOURCE_EXT_IO_3;
        break;
    case PIN_IRQ_DS8:
        *wakeup_src = WAKEUP_SOURCE_EXT_IO_4;
        break;
    case PIN_IRQ_DS9:
        *wakeup_src = WAKEUP_SOURCE_EXT_IO_5;
        break;
    case PIN_IRQ_DS10:
        *wakeup_src = WAKEUP_SOURCE_EXT_IO_6;
        break;
    default:
        *wakeup_src = WAKEUP_SOURCE_UNKNOWN;
        break;
    }
}

APP_SECTION
static int cocopalmR_board_init(BoardDevice *board)
{
    FlashDevice *flash_dev = NULL;
    uint32_t ptt_num =
        sizeof(main_flash_part_table) / sizeof(main_flash_part_table[0]);

    power_init();
    soc_pinmux_config(pin_mux_ctrl, sizeof(pin_mux_ctrl) / sizeof(SocPinCtl));
    /* Init the flash partition */
    flash_dev = (FlashDevice *)hal_flash_get_device(FLASH_MAIN);
    hal_flash_init_partition(flash_dev, main_flash_part_table, ptt_num);
    board->name = cocopalmR_board_name;
    create_drivers();
    return VSD_SUCCESS;
}

APP_SECTION
void *cocopalmR_board_find_device(uint8_t device_id)
{
    return device_list[device_id];
}

const BoardOperations cocopalmR_board_ops = {
    .init           = cocopalmR_board_init,
    .find_device    = cocopalmR_board_find_device,
    .get_wakeup_src = power_key_to_wakeup_src,
};

APP_SECTION
const BoardOperations *board_get_ops(void)
{
    return &cocopalmR_board_ops;
}
