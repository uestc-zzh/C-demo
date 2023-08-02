#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include "vs_conf.h"
#include "platform.h"
#include "hal_uart.h"
#if CONFIG_QEMU_PLATFORM
#include "e203_uart.h"
#endif

#undef putchar
static UartDevice *uart_dev = NULL;
/**
 * @todo Replace uart sending function
 */
int putchar(int dat)
{
    if (dat == '\n') {
#if CONFIG_QEMU_PLATFORM
        uart_write(UART0, '\r');
#else
        hal_uart_put_char(uart_dev, '\r');
#endif
    }
#if CONFIG_QEMU_PLATFORM
    uart_write(UART0, dat);
#else
    hal_uart_put_string(uart_dev, (const char *)&dat);
#endif
    return dat;
}

__WEAK ssize_t _write(int fd, const void *ptr, size_t len)
{
    if (!isatty(fd)) {
        return -1;
    }
    if (__rv_unlikely(!uart_dev))
        uart_dev = hal_uart_get_device(UART_DEV_ID_0);

    const uint8_t *writebuf = (const uint8_t *)ptr;
    for (size_t i = 0; i < len; i++) {
        putchar((int)writebuf[i]);
    }
    return len;
}

__WEAK int _close(int fd)
{
    return -1;
}

__WEAK int _fstat(int file, void *st)
{
    return -1;
}

__WEAK int _getpid(void)
{
    return 1;
}

__WEAK int _isatty(int fd)
{
    return 1;
}

__WEAK int _kill(int pid, int sig)
{
    return -1;
}

__WEAK int _lseek(int file, int offset, int whence)
{
    return 0;
}

__WEAK ssize_t _read(int fd, void *ptr, size_t len)
{
    return -1;
}

__WEAK int _gettimeofday(struct timeval *tp, void *tzp)
{
    return 0;
}
