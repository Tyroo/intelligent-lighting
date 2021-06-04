#include "timer.h"
#include "human.h"

// 定义全局变量
u8 timer2_flag = 0;

/*
*	作用：定时器2初始化函数
*	参数：arr：计数值，psc：定时器2分频因子
*	返回值：无
*/
void timer2_init(u16 arr, u16 psc) {
	RCC->APB1ENR |= 0x01;
	TIM2->CR1 &= 0;
	TIM2->CR1 |= 0x01<<2;
	TIM2->CR1 |= 0x01<<7;
	TIM2->DIER |= 0x01;
	TIM2->ARR = arr;
	TIM2->PSC = psc;
	NVIC_EnableIRQ(TIM2_IRQn);	// 使能TIM2中断函数
//	Nvic_Init(2, 3, TIM2_IRQn, 2);
	
}

void start_timer2() {
	// pass 开启定时器2
	TIM2->CNT = 0;
	TIM2->CR1 |= 0x01;
}

void stop_timer2() {
	// pass 停止定时器2
	TIM2->CR1 &= ~(0x0001);
	TIM2->CNT = 0;
}

void TIM2_IRQHandler(void) {
	short human_flag = get_human_flag();
	
	if (TIM2->SR & 0x01) {
		if (human_flag) {
			timer2_flag = 1;
			// 关闭后再开启人体检测模块
			human_switch(0);
			human_switch(1);
		} else {
			timer2_flag = 0;
			stop_timer2();
		}
	}
	TIM2->SR &= ~(1<<0);
	return;
}
