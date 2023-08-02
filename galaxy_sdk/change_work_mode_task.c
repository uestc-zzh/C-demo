#include "change_work_mode_task.h"
#include "common_struct_def.h"
extern ImuDevice *imu_dev;
OsalSemaphore sem_mode;

void task_change_work_mode(void *param){
	int ret = osal_init_sem(&sem_mode);
	if(ret!=OSAL_TRUE){
		uart_printf("IMU:osal_init_sem error\r\n");
		return ;
	}
	while(1){
	osal_sem_wait(&sem_mode,OSAL_WAIT_FOREVER);
	int ret = hal_imu_set_work_mode(imu_dev,IMU_ACCEL_GYRO,IMU_SEN_MODE_NORMAL);
	if(ret!=VPI_SUCCESS){
		uart_printf("IMU:hal_imu_set_work_mode1 normal error\r\n");
		return;
	}
}

}
