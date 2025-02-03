#ifndef __DRIVER_UART_H__
#define __DRIVER_UART_H__
#include "stdint.h"
#include "stdbool.h"
#include "driver/uart.h"

void init_uart();
void init_uart2();
void rs485_send_msg(const uint8_t *data, size_t length);
int rs485_recive_msg(const uint8_t *data, size_t length);

#endif