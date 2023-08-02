#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#include "stdint.h"
    int bt_bas_set_led_level(uint8_t level);
    uint8_t bt_bas_get_led_level(void);
    uint8_t bt_bas_get_wearing_level(void);

int bt_bas_set_wearing_level(uint8_t level);
#endif