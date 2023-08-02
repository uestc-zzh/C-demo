/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_usb.h
 * @brief Public head file of USB HAL
 */

#ifndef __HAL_USB_H
#define __HAL_USB_H

/** @addtogroup USB
 *  USB HAL API and definition
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#include <stdint.h>
#include <stdbool.h>

#define USB_IF_NUMS (USB_ID_MAX)

typedef enum UsbId {
    USB_ID_0,
    USB_ID_MAX,
} UsbId;

typedef enum UsbRole {
    USB_ROLE_NONE,
    USB_ROLE_HOST,
    USB_ROLE_DEVICE,
} UsbRole;

typedef enum UsbSpeedType {
    USB_SPEED_UNKNOWN,
    USB_SPEED_LOW,
    USB_SPEED_FULL,
    USB_SPEED_HIGH,
} UsbSpeedType;

typedef struct UsbHostCfg {
    uint8_t irq_id; /**< interrupt id of usb */
    uint8_t clk_id; /**< clock id of usb */
    uint8_t gpd_id; /**< gpd id */
    uint8_t speed; /**< usb speed, @see UsbSpeedType */
    bool dma_enable; /**< enable or disable DMA */
    bool sof_output; /**< enable or disable SOF */
    uint16_t mps; /**< max packet size */
    uint32_t reg_base; /**< base register address */
} UsbHostCfg;

typedef struct UsbDeviceCfg {
    uint8_t irq_id; /**< interrupt id of usb */
    uint8_t clk_id; /**< clock id of usb */
    uint8_t gpd_id; /**< gpd id */
    uint8_t speed; /**< usb speed, @see UsbSpeedType */
    uint8_t deveps;
    uint8_t mountflash;
    uint8_t partitionnum;
    bool dma_enable; /**< enable or disable DMA */
    uint16_t mps; /**< max packet size */
    uint32_t reg_base; /**< base register address */
} UsbDeviceCfg;

typedef struct UsbHwConfig {
    uint8_t port; /**< port of usb */
    UsbHostCfg host; /**< host configuration */
    UsbDeviceCfg device; /**< device configuration */
} UsbHwConfig;

typedef struct UsbDevice UsbDevice;
typedef struct UsbHostDevice UsbHostDevice;
typedef struct UsbDevDevice UsbDevDevice;

typedef struct MscUDiskOpers {
    int (*getstatus)(const UsbDevice *usb, bool *status);
    int (*init)(const UsbDevice *usb);
    int (*read)(const UsbDevice *usb, uint8_t *buff, uint32_t sector,
                uint32_t count);
    int (*write)(const UsbDevice *usb, const uint8_t *buff, uint32_t sector,
                 uint32_t count);
    int (*ioctl)(const UsbDevice *dev, uint8_t cmd, void *buff);
} MscUDiskOpers;

typedef struct UsbHostOpers {
    int (*init)(const UsbDevice *usb);
    int (*deinit)(const UsbDevice *usb);
    int (*device_connect)(const UsbDevice *usb, bool *status);
    int (*enumerate)(const UsbDevice *usb);
    void (*irq_handler)(const UsbDevice *usb);
    MscUDiskOpers *udiskopers;
} UsbHostOpers;

typedef struct UsbDevOpers {
    int (*init)(const UsbDevice *usb);
    int (*deinit)(const UsbDevice *usb);
    int (*process)(const UsbDevice *usb);
    void (*irq_handler)(const UsbDevice *usb);
} UsbDevOpers;

typedef struct UsbHostDevice {
    const UsbHostOpers *opers;
} UsbHostDevice;

typedef struct UsbDevDevice {
    const UsbDevOpers *opers;
} UsbDevDevice;

typedef union UsbActDevice {
    UsbHostDevice host;
    UsbDevDevice device;
} UsbActDevice;

typedef struct UsbDevice {
    uint8_t port;
    uint8_t workrole;
    const UsbHwConfig *cfg;
    UsbActDevice actdev;
    void *ctx;
} UsbDevice;

/**
 * @brief add usb device to device table
 * @param dev usb device to add
 * @retval VSD_SUCCESS for success, others for failure
 */
int hal_usb_add_device(UsbDevice *dev);

/**
 * @brief remove usb device from device table
 * @param dev usb device to remove
 * @retval VSD_SUCCESS for success, others for failure
 */
int hal_usb_remove_device(UsbDevice *dev);

/**
 * @brief get usb device from usb table
 * @param port port of usb device
 * @retval NULL for failure, others for success
 */
UsbDevice *hal_usb_get_device(uint8_t port);

/**
 * @brief initialize usb controller
 * @param dev usb device to initialize
 * @retval VSD_SUCCESS for success, others for failure
 */
int hal_usb_init(const UsbDevice *dev);

/**
 * @brief de-initialize usb controller
 * @param dev usb device to de-initialize
 * @retval VSD_SUCCESS for success, others for failure
 */
int hal_usb_deinit(const UsbDevice *dev);

/**
 * @brief get connect state of usb device which connected to usb host
 * @param dev instance of usb device
 * @param status connect status
 * @retval VSD_SUCCESS for success, others for failure
 */
int hal_usbh_device_connect(const UsbDevice *dev, bool *status);

/**
 * @brief usb host enumrate device
 * @param dev instance of usb device
 * @retval VSD_SUCCESS for success, others for failure
 */
int hal_usbh_enumerate(const UsbDevice *dev);

/**
 * @brief usb host initializes u-disk
 * @param dev instance of usb device
 * @retval VSD_SUCCESS for success, others for failure
 */
int hal_usbh_udisk_init(const UsbDevice *dev);

/**
 * @brief get u-disk status
 * @param dev instance of usb device
 * @param status u-disk status
 * @retval VSD_SUCCESS for success, others for failure
 */
int hal_usbh_udisk_status(const UsbDevice *dev, bool *status);

/**
 * @brief usb host reads u-disk
 * @param dev instance of usb device
 * @param buff data buffer
 * @param sector start sector to read
 * @param count sectors to read
 * @retval VSD_SUCCESS for success, others for failure
 */
int hal_usbh_udisk_read(const UsbDevice *dev, uint8_t *buff, uint32_t sector,
                        uint32_t count);

/**
 * @brief usb host writes u-disk
 * @param dev instance of usb device
 * @param buff data buffer
 * @param sector start sector to write
 * @param count sectors to write
 * @retval VSD_SUCCESS for success, others for failure
 */
int hal_usbh_udisk_write(const UsbDevice *dev, const uint8_t *buff,
                         uint32_t sector, uint32_t count);

/**
 * @brief get u-disk parameters
 * @param dev instance of usb device
 * @param cmd command of u-disk operations
 * @param param parameter
 * @retval VSD_SUCCESS for success, others for failure
 */
int hal_usbh_udisk_ioctl(const UsbDevice *dev, uint8_t cmd, void *param);

/**
 * @brief main process of usb device functions
 * @param dev instance of usb device
 */
int hal_usbd_process(const UsbDevice *dev);

/**
 * @brief interrupt handler of usb
 * @param dev instance of usb device
 */
void hal_usb_irq_handler(const UsbDevice *dev);

/** @} */

#endif // __HAL_USB_H
