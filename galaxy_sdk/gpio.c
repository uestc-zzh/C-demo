#include "gpio.h"
#include "common_struct_def.h"

GpioPort led_blue_port = {
    .group      = 0,
    .irq_reload = 0,
    .invert     = 0,
    .port       = PIN_IRQ_DS8,
    .trigger    = 0,
    .type       = IO_MODE_OUT_PULL,
};

GpioPort led_green_port = {
    .group      = 0,
    .irq_reload = 0,
    .invert     = 0,
    .port       = PIN_IRQ_DS9,
    .trigger    = 0,
    .type       = IO_MODE_OUT_PULL,
};

int gpio_init(GpioPort *gpio)
{
    return hal_gpio_init(gpio);
}

int gpio_low(GpioPort *gpio)
{
    return hal_gpio_output_low(gpio);
}

int gpio_high(GpioPort *gpio)
{
    return hal_gpio_output_high(gpio);
}

int gpio_toggle(GpioPort *gpio)
{
    return hal_gpio_output_toggle(gpio);
}