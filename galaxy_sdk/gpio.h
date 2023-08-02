#ifndef _GPIO_H_
#define _GPIO_H_
#include "common_struct_def.h"

int gpio_init(GpioPort *gpio);

int gpio_low(GpioPort *gpio);

int gpio_high(GpioPort *gpio);

int gpio_toggle(GpioPort *gpio);

#endif