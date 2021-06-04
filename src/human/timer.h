#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f10x.h"

void timer2_init(u16 arr, u16 psc);
void start_timer2(void);
void stop_timer2(void);

#endif

