#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bridge.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver_uart.h"

#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define TX_RS485 1
#define RX_RS485 3
#define TX_BASIC 17
#define RX_BASIC 16
#define RS 27
#define PORT_NUM 0
#define BAUD_RATE 115200
#define BUF_SIZE (5120)
#define TICKS 0
const int uart_number = 0;
const int uart2_number = 1;

void init_uart()
{
    uart_config_t uart_config =
    {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    int intr_alloc_flags = ESP_INTR_FLAG_SHARED | ESP_INTR_FLAG_LOWMED;
    ESP_ERROR_CHECK(uart_driver_install(uart_number, BUF_SIZE, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(uart_number, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uart_number, TX_RS485, RX_RS485, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

void init_uart2()
{
    uart_config_t uart_config =
    {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    int intr_alloc_flags = ESP_INTR_FLAG_SHARED | ESP_INTR_FLAG_LOWMED;
    ESP_ERROR_CHECK(uart_driver_install(uart2_number, BUF_SIZE, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(uart2_number, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uart2_number, TX_BASIC, RX_BASIC, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

void rs485_send_msg(const uint8_t *data, size_t length)
{
    uart_write_bytes(PORT_NUM, data, length);
    ESP_ERROR_CHECK(uart_wait_tx_idle_polling(PORT_NUM));
}

int rs485_recive_msg(const uint8_t *data, size_t length)
{
    size_t len = uart_read_bytes(PORT_NUM, data, length, TICKS);
    return (int)len;
}