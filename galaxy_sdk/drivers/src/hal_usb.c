/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file hal_usb.c
 * @brief HAL interface for USB driver
 */

#include <stddef.h>
#include "hal_usb.h"
#include "vsd_error.h"
#include "sys_common.h"

static UsbDevice *usb_dev_table[USB_IF_NUMS];

static inline UsbHostOpers *get_host_ops(const UsbHostDevice *dev)
{
    return (UsbHostOpers *)dev->opers;
}

static inline UsbDevOpers *get_dev_ops(const UsbDevDevice *dev)
{
    return (UsbDevOpers *)dev->opers;
}

int hal_usb_add_device(UsbDevice *dev)
{
    uint8_t i;

    for (i = 0; i < USB_IF_NUMS; ++i) {
        if (usb_dev_table[i] == NULL) {
            usb_dev_table[i] = dev;
            return VSD_SUCCESS;
        }
    }
    return VSD_ERR_FULL;
}

int hal_usb_remove_device(UsbDevice *dev)
{
    uint8_t i;

    for (i = 0; i < USB_IF_NUMS; ++i) {
        if (usb_dev_table[i] == dev) {
            usb_dev_table[i] = NULL;
            return VSD_SUCCESS;
        }
    }
    return VSD_ERR_NON_EXIST;
}

UsbDevice *hal_usb_get_device(uint8_t port)
{
    uint8_t i;

    for (i = 0; i < USB_IF_NUMS; ++i) {
        if (usb_dev_table[i] && (usb_dev_table[i]->port == port)) {
            return usb_dev_table[i];
        }
    }
    return NULL;
}

int hal_usb_init(const UsbDevice *dev)
{
    int ret;
    const UsbHostDevice *usbh;
    const UsbDevDevice *usbd;

    if (!dev) {
        return VSD_ERR_INVALID_POINTER;
    }
    usbh = &(dev->actdev.host);
    usbd = &(dev->actdev.device);
    if (dev->workrole == USB_ROLE_HOST) {
        if (!usbh || !get_host_ops(usbh) || !get_host_ops(usbh)->init) {
            return VSD_ERR_INVALID_POINTER;
        }
        ret = get_host_ops(usbh)->init(dev);
    } else {
        if (!usbd || !get_dev_ops(usbd) || !get_dev_ops(usbd)->init) {
            return VSD_ERR_INVALID_POINTER;
        }
        ret = get_dev_ops(usbd)->init(dev);
    }
    return ret;
}

int hal_usb_deinit(const UsbDevice *dev)
{
    int ret;
    const UsbHostDevice *usbh;
    const UsbDevDevice *usbd;

    if (!dev) {
        return VSD_ERR_INVALID_POINTER;
    }
    usbh = &(dev->actdev.host);
    usbd = &(dev->actdev.device);
    if (dev->workrole == USB_ROLE_HOST) {
        if (!usbh || !get_host_ops(usbh) || !get_host_ops(usbh)->deinit) {
            return VSD_ERR_INVALID_POINTER;
        }
        ret = get_host_ops(usbh)->deinit(dev);
    } else {
        if (!usbd || !get_dev_ops(usbd) || !get_dev_ops(usbd)->deinit) {
            return VSD_ERR_INVALID_POINTER;
        }
        ret = get_dev_ops(usbd)->deinit(dev);
    }
    return ret;
}

int hal_usbh_device_connect(const UsbDevice *dev, bool *connect)
{
    int ret;
    const UsbHostDevice *usbh;

    if (!dev) {
        return VSD_ERR_INVALID_POINTER;
    }
    usbh = &(dev->actdev.host);
    if (!usbh || !get_host_ops(usbh) || !connect) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_host_ops(usbh)->device_connect) {
        return VSD_ERR_UNSUPPORTED;
    }
    ret = get_host_ops(usbh)->device_connect(dev, connect);
    return ret;
}

int hal_usbh_enumerate(const UsbDevice *dev)
{
    int ret;
    const UsbHostDevice *usbh;

    if (!dev) {
        return VSD_ERR_INVALID_POINTER;
    }
    usbh = &(dev->actdev.host);
    if (!usbh || !get_host_ops(usbh)) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_host_ops(usbh)->enumerate) {
        return VSD_ERR_UNSUPPORTED;
    }
    ret = get_host_ops(usbh)->enumerate(dev);
    return ret;
}

int hal_usbh_udisk_init(const UsbDevice *dev)
{
    int ret;
    const UsbHostDevice *usbh;

    if (!dev) {
        return VSD_ERR_INVALID_POINTER;
    }
    usbh = &(dev->actdev.host);
    if (!usbh || !get_host_ops(usbh) || !get_host_ops(usbh)->udiskopers) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_host_ops(usbh)->udiskopers->init) {
        return VSD_ERR_UNSUPPORTED;
    }
    ret = get_host_ops(usbh)->udiskopers->init(dev);
    return ret;
}

int hal_usbh_udisk_status(const UsbDevice *dev, bool *status)
{
    int ret;
    const UsbHostDevice *usbh;

    if (!dev) {
        return VSD_ERR_INVALID_POINTER;
    }
    usbh = &(dev->actdev.host);
    if (!usbh || !get_host_ops(usbh) || !get_host_ops(usbh)->udiskopers) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_host_ops(usbh)->udiskopers->getstatus) {
        return VSD_ERR_UNSUPPORTED;
    }
    ret = get_host_ops(usbh)->udiskopers->getstatus(dev, status);
    return ret;
}

int hal_usbh_udisk_read(const UsbDevice *dev, uint8_t *buff, uint32_t sector,
                        uint32_t count)
{
    int ret;
    const UsbHostDevice *usbh;

    if (!dev) {
        return VSD_ERR_INVALID_POINTER;
    }
    usbh = &(dev->actdev.host);
    if (!usbh || !get_host_ops(usbh) || !get_host_ops(usbh)->udiskopers) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_host_ops(usbh)->udiskopers->read) {
        return VSD_ERR_UNSUPPORTED;
    }
    ret = get_host_ops(usbh)->udiskopers->read(dev, buff, sector, count);
    return ret;
}

int hal_usbh_udisk_write(const UsbDevice *dev, const uint8_t *buff,
                         uint32_t sector, uint32_t count)
{
    int ret;
    const UsbHostDevice *usbh;

    if (!dev) {
        return VSD_ERR_INVALID_POINTER;
    }
    usbh = &(dev->actdev.host);
    if (!usbh || !get_host_ops(usbh) || !get_host_ops(usbh)->udiskopers) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_host_ops(usbh)->udiskopers->write) {
        return VSD_ERR_UNSUPPORTED;
    }
    ret = get_host_ops(usbh)->udiskopers->write(dev, buff, sector, count);
    return ret;
}

int hal_usbh_udisk_ioctl(const UsbDevice *dev, uint8_t cmd, void *param)
{
    int ret;
    const UsbHostDevice *usbh;

    if (!dev) {
        return VSD_ERR_INVALID_POINTER;
    }
    usbh = &(dev->actdev.host);
    if (!usbh || !get_host_ops(usbh) || !get_host_ops(usbh)->udiskopers) {
        return VSD_ERR_INVALID_POINTER;
    }
    if (!get_host_ops(usbh)->udiskopers->ioctl) {
        return VSD_ERR_UNSUPPORTED;
    }
    ret = get_host_ops(usbh)->udiskopers->ioctl(dev, cmd, param);
    return ret;
}

int hal_usbd_process(const UsbDevice *dev)
{
    const UsbDevDevice *usbd;

    if (!dev) {
        return VSD_ERR_INVALID_POINTER;
    }
    usbd = &(dev->actdev.device);
    if (!usbd || !get_dev_ops(usbd) || !get_dev_ops(usbd)->process) {
        return VSD_ERR_INVALID_POINTER;
    }
    get_dev_ops(usbd)->process(dev);
    return VSD_SUCCESS;
}

DRV_ISR_SECTION
void hal_usb_irq_handler(const UsbDevice *dev)
{
    const UsbHostDevice *usbh;
    const UsbDevDevice *usbd;

    if (!dev) {
        return;
    }
    usbh = &(dev->actdev.host);
    usbd = &(dev->actdev.device);
    if (dev->workrole == USB_ROLE_HOST) {
        if (!usbh || !get_host_ops(usbh) || !get_host_ops(usbh)->irq_handler) {
            return;
        }
        get_host_ops(usbh)->irq_handler(dev);
    } else {
        if (!usbd || !get_dev_ops(usbd) || !get_dev_ops(usbd)->irq_handler) {
            return;
        }
        get_dev_ops(usbd)->irq_handler(dev);
    }
}
