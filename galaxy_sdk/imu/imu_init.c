#include <stdint.h>
#include <stdlib.h>
#include "hal_imu.h"
#include "uart_printf.h"
#include "vpi_error.h"
#include "common_struct_def.h"

static uint8_t fifo_wm = 30;

int imu_init(ImuDevice *imu_dev){
	int ret;

	ret=hal_imu_init(imu_dev);
	if(ret!=VPI_SUCCESS){
		uart_printf("IMU:hal_imu_init error\r\n");
		return -1;
	}

	ret = hal_imu_enable_power(imu_dev,true);
	if(ret!=VPI_SUCCESS){
		uart_printf("IMU:hal_imu_enable_power error\r\n");
//		return -1;
	}


	IMUHighGCondition highg_condition={
				.threshold = 1,
				.duration = 30,
				.hysteresis = 0
	};
	hal_imu_set_highg_threshold(imu_dev, highg_condition);

//	IMU_ACC_GYRO_FIFO_WATERMARK_INTERRUPT

	ret = hal_imu_cfg_interrupt(imu_dev,true,IMU_ACC_HIGH_G_INTERRUPT ,NULL);
	if(ret!=VPI_SUCCESS){
		uart_printf("IMU:hal_imu_cfg_interrupt error IMU_ACC_HIGH_G_INTERRUPT\r\n");
		return -1;
	}
	ret = hal_imu_cfg_interrupt(imu_dev,true,IMU_ACC_GYRO_FIFO_WATERMARK_INTERRUPT ,NULL);
	if(ret!=VPI_SUCCESS){
		uart_printf("IMU:hal_imu_cfg_interrupt error\r\n");
		return -1;
	}



	//set FIFO watermark
	ret = hal_imu_set_fifo_wm(imu_dev,fifo_wm);
	if(ret!=VPI_SUCCESS){
		uart_printf("IMU:hal_imu_set_fifo_wm error\r\n");
		return -1;
	}

	//config FIFO data format with ACCEL,GYRO&TIME data
	ret = hal_imu_set_fifo_cfg(imu_dev,IMU_FIFO_ACCEL|IMU_FIFO_GYRO|IMU_FIFO_TIME,true);
	if(ret!=VPI_SUCCESS){
		uart_printf("IMU:hal_imu_set_fifo_cfg error\r\n");
		return -1;
	}
	//	flush FIFO
	ret = hal_imu_flush_fifo(imu_dev);
	if(ret!=VPI_SUCCESS){
		uart_printf("IMU:hal_imu_flush_fifo error\r\n");
		return -1;
	}
	//	set workmode to NORMAL(if high-g,set IMU_SEN_MODE_LOW_PWR)
	ret = hal_imu_set_work_mode(imu_dev,IMU_ACCEL_GYRO,IMU_SEN_MODE_LOW_PWR);
//	ret = hal_imu_set_work_mode(imu_dev,IMU_ACCEL_GYRO,IMU_SEN_MODE_NORMAL);
	if(ret!=VPI_SUCCESS){
		uart_printf("IMU:hal_imu_set_work_mode1 error\r\n");
		return -1;
	}
	return 0;
 }
