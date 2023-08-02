#include "gpio.h"
#include "common_struct_def.h"

void task_test(void *param)
{
    // test gpio
    // if(gpio_init(&led_blue_port)!=VSD_SUCCESS){
    // 	uart_printf("gpio:gpio_init error\r\n");
    // 	return ;
    // }

    // int ret = gpio_toggle(&led_blue_port);
    // if(ret!=VSD_SUCCESS){
    //     uart_printf("gpio:gpio_toggle error\r\n");
    // 	return ;
    // }
    // // green:low  blue:high
    // while(gpio_high(&led_blue_port)==VSD_SUCCESS){
    //     osal_sleep(1000);
    // }

    while (1) {
        #ifdef BLE_TEST
        if (ble_get_connect_state() == BLE_STATE_CONNECTED) {
            // for test
            // bt_bas_set_led_level(1);
            bt_bas_set_wearing_level(0);
            uart_printf("set led success");
            osal_sleep(100);
        }
        // uint8_t ret ;
        // = bt_bas_get_led_level();
        uint8_t ret1 = bt_bas_get_wearing_level();
        // uart_printf("ret:%u",ret);
        uart_printf("ret:%u", ret1);
        #endif

        #ifdef TEST_TASK
        bt_bas_set_wearing_level(1);
        uart_printf("set 1\r\n");
        osal_sleep(1000);
        // bt_bas_set_wearing_level(0);
        // uart_printf("set 0\r\n");
        #endif
        // osal_sleep(1000);
    }
}