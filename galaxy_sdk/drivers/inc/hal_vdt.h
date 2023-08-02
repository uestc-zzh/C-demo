#ifndef _HAL_VDT_H
#define _HAL_VDT_H

#include <stdint.h>

/** @addtogroup VDT
 *  VDT HAL API and definition
 *  @ingroup HAL
 *  Hardware Abstraction Layer
 *  @{
 */

#define VDT_MAX_DEVICE (1)
#define VDT_ID_0 (0)

typedef void (*VdtCallback)(void *param);

/**
 * @brief Structure of VDT hardware configuration
 *
 */
typedef struct VdtHwConfig {
    uint32_t base; /**< Base address */
    uint8_t clk_id; /**< Clock id */
    uint16_t irq_id; /**< Interrupt id */
} VdtHwConfig;

/**
 * @brief Structure of VDT hardware device
 *
 */
typedef struct VdtDevice {
    uint8_t hw_id; /**< Hardware id */
    const VdtHwConfig *hw_config; /**< Hardware config */
    const void *ops; /**< Vdt operations */
    void *ctx; /**< Private context */
} VdtDevice;

/**
 * @brief Structure of VDT operation
 *
 */
typedef struct VdtOperations {
    int (*trigger_work_mode)(VdtDevice *device);
    int (*set_mode)(VdtDevice *device, uint8_t mode, uint8_t param);
    int (*set_callback)(VdtDevice *device, VdtCallback hook,
                        void *callback_arg);
    int (*power_ctrl)(VdtDevice *device, uint8_t channel, uint8_t enable);
    void (*irq_handler)(VdtDevice *device);
} VdtOperations;

/**
 * @brief VDT HAL function, used to add vdt device
 *
 * @param device vdt device
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_vdt_add_device(VdtDevice *device);

/**
 * @brief VDT HAL function, used to remove vdt device
 *
 * @param device vdt device
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_vdt_remove_device(VdtDevice *device);

/**
 * @brief VDT HAL function, used to get device by hardware id
 *
 * @param hw_id hardware id
 * @return VdtDevice pointor
 */
VdtDevice *hal_vdt_get_device(uint8_t hw_id);

/**
 * @brief VDT HAL function, used to trigger device to process work mode
 *
 * @param device vdt device
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_vdt_trigger_work_mode(VdtDevice *device);

/**
 * @brief VDT HAL function, used to set vdt param
 *
 * @param device vdt device
 * @param mode vdt mode
 * @param param mode value
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_vdt_set_mode(VdtDevice *device, uint8_t mode, uint8_t param);

/**
 * @brief VDT HAL function, used to set call back function
 *
 * @param device vdt device
 * @param hook callback hook
 * @param callback_arg callback arg
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_vdt_set_callback(VdtDevice *device, VdtCallback hook,
                         void *callback_arg);

/**
 * @brief VDT HAL function, used to enable/disable vdt power down
 *
 * @param device vdt device
 * @param channel vdt channel
 * @param enable  enable/disable
 * @return VSD_SUCCESS for success,others for fail
 */
int hal_vdt_power_ctrl(VdtDevice *device, uint8_t channel, uint8_t enable);

/**
 * @brief VDT HAL function, used to interrupt handler
 *
 * @param device vdt device
 */
void hal_vdt_irq_handler(VdtDevice *device);

/** @} */

#endif //_HAL_VDT_H
