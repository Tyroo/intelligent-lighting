#ifndef __NVIC_H__
#define __NVIC_H__

#include "stm32f10x.h"

void Stm32_Clock_Init(u8 PLL);
void MYRCC_DeInit(void);
void Nvic_PriorityGroupConfig(u8 Group);
void Nvic_Init(u8 PreemptionPriority,u8 SubPriority,u8 Channel,u8 Group);

#endif

