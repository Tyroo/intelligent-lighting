#ifndef __HUMAN_H__
#define __HUMAN_H__

#include "stm32f10x.h"

void human_init(void);
short get_human_flag(void);
int human_detection(void);
void human_switch(short sw);

#endif
