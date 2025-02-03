#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bridge.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver_twai.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "driver/twai.h"

const char* TAG = "CAN"; 
#define TX_CAN 25
#define RX_CAN 26
#define TICK 10

void init_twai()
{
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_CAN, RX_CAN, TWAI_MODE_NORMAL);
    g_config.rx_queue_len=20;
    g_config.tx_queue_len=130;
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    twai_driver_install(&g_config, &t_config, &f_config);
    twai_start();
}

void can_send_msg(twai_message_t msg)
{
    msg.extd=1;
    int err=twai_transmit(&msg,TICK); 
    if (err!=ESP_OK)
        ESP_LOGI(TAG,"ERR %d", err);
}

int can_recive_msg(twai_message_t *msg)
{
    msg->extd=1;
    if (twai_receive(msg,TICK)!=ESP_OK)
        return 0;
    if (!(msg->rtr)) 
        return msg->data_length_code;
    return 0;
}