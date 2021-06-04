#ifndef __LORA_H__
#define __LORA_H__

#include "stm32f10x.h"

void send_status(short req);
void send_all_status(void);
void send_switch_status(int state);
void send_switch_change_status(void);

#endif
