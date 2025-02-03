#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bridge.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver_uart.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "driver_twai.h"

static uint8_t buff_uart[2560];
static uint8_t lung_buff=0;
static uint8_t address=0;
static twai_message_t can_msg;
static int id_number=0;
static bool is_msg_finished = false;
static int number=0;

void rs485_can()
{
    while (1)
    {
        int lenght = rs485_recive_msg(buff_uart,sizeof(buff_uart));
        for (int i=0;i<lenght;i+=8)
        {
            can_msg.identifier = (i/8)<<8 | buff_uart[0];
            can_msg.data_length_code=(lenght-i<8) ? lenght-i : 8;
            memcpy (can_msg.data, &buff_uart[i], can_msg.data_length_code);
            can_send_msg(can_msg);
        }
        vTaskDelay(1);   
    }
}

void can_rs485()
{
    while (1)
    {
        int lenght = 0 ;
        do
        {
            lenght = can_recive_msg(&can_msg);
            uint8_t mask = can_msg.identifier & 0xFF;
            if (lenght!=0)
            {
                if (can_msg.identifier==can_msg.data[0])
                {
                    memset(buff_uart,0x00,sizeof(buff_uart));
                    if (can_msg.data[1] >= 5)
                        lung_buff=can_msg.data[1];
                    else
                        lung_buff=0;
                    address=can_msg.data[0];
                    id_number=0;
                    number=0;
                }

                if (mask == address && id_number==((can_msg.identifier >> 8) & 0xFF))
                {
                    for (int i=0;i<can_msg.data_length_code;i++)
                        buff_uart[number++]=can_msg.data[i];
                    id_number++;
                    is_msg_finished = true;
                }
                
                if (lung_buff==number && is_msg_finished== true)
                {
                    rs485_send_msg(buff_uart,number);
                    is_msg_finished=false;
                }
            }
        } while (lenght!=0);     
        vTaskDelay(1);
    }
}