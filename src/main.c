#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "bridge.h"
#include "driver_uart.h"
#include "driver_twai.h"

void gpio_conf()
{
  gpio_config_t gpio_conf = {};
  gpio_conf.intr_type = GPIO_INTR_DISABLE;
  gpio_conf.pin_bit_mask = 1 << 27;
  gpio_conf.mode = GPIO_MODE_OUTPUT;
  gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  gpio_conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&gpio_conf);
  gpio_set_level(27,1);
}

void app_main() 
{
	ESP_ERROR_CHECK(nvs_flash_init());
	gpio_conf();
	//init_uart2();
  init_twai();
  init_uart();
	xTaskCreate(&rs485_can,"RS485-CAN",20000,NULL,4,NULL);
	xTaskCreate(&can_rs485,"CAN-RS485",20000,NULL,4,NULL);
}