#include "sensor_task.h"
#include "hal_imu.h"
#include "common_struct_def.h"
CrcDevice *crc_device;

uint32_t crc_init()
{
    crc_device = hal_crc_get_device();
    if (crc_device == NULL) {
        uart_printf("hal_crc_get_device error");
        return -1;
    }

    if (hal_crc_init() != VSD_SUCCESS) {
        uart_printf("hal_crc_init error");
        return -1;
    }
    return 0;
}
uint32_t crc_calculate(void *arr, uint32_t length)
{
    CrcInput crc8_input;
    crc8_input.p_buffer      = arr;
    crc8_input.buffer_length = length * sizeof(ImuGyroAccelData);
    crc8_input.poly          = CRC_POLYNOMIAL_CRC_8;
    crc8_input.cal_switch    = CRC_LMS_MSB;

    uint32_t crc_result;
    if (hal_crc_calculate(&crc8_input, &crc_result) != VSD_SUCCESS) {
        uart_printf("hal_crc_calculate error");
        return -1;
    }
    return crc_result;
}

void create_header(struct data_header_t *data_header, uint32_t crc_result) {}
