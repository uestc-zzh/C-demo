/**
 * Copyright (C) 2022 VeriSilicon Holdings Co., Ltd.
 * All rights reserved.
 *
 * @file uart_printf.h
 * @brief public head file of uart print
 * @author Shaobo Tu <Shaobo.Tu@verisilicon.com>
 */

#ifndef _UART_PRINTF_H
#define _UART_PRINTF_H
#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup LOGGING
 *  uart printf API and definition.
 *  @ingroup VPI
 *  @{
 */

/*Check your function's parameter a & b by following function f's rule*/
#define CHECK_PARAM_FORMAT(f, a, b) __attribute__((__format__(f, a, b)))

/**
 * @brief Print string to UART
 * @param format C string that contains the text to be written to UART
 * @return On success, total number of characters written is returned
 */
int uart_printf(const char *format, ...) CHECK_PARAM_FORMAT(printf, 1, 2);
int uart_sprintf(char *out, const char *format, ...)
    CHECK_PARAM_FORMAT(printf, 2, 3);

/**
 * @brief API to init the UART interface for debug
 * @return return 0 for success
 */
int uart_debug_init(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _UART_PRINTF_H */
