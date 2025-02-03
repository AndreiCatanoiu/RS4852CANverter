#ifndef __DRIVER_TWAI_H__
#define __DRIVER_TWAI_H__
#include "stdint.h"
#include "stdbool.h"
#include <driver/twai.h>

void init_twai();
void can_send_msg(twai_message_t msg);
int can_recive_msg(twai_message_t *msg);

#endif
