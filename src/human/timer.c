#include "timer.h"
#include "human.h"

// ����ȫ�ֱ���
u8 timer2_flag = 0;

/*
*	���ã���ʱ��2��ʼ������
*	������arr������ֵ��psc����ʱ��2��Ƶ����
*	����ֵ����
*/
void timer2_init(u16 arr, u16 psc) {
	RCC->APB1ENR |= 0x01;
	TIM2->CR1 &= 0;
	TIM2->CR1 |= 0x01<<2;
	TIM2->CR1 |= 0x01<<7;
	TIM2->DIER |= 0x01;
	TIM2->ARR = arr;
	TIM2->PSC = psc;
	NVIC_EnableIRQ(TIM2_IRQn);	// ʹ��TIM2�жϺ���
//	Nvic_Init(2, 3, TIM2_IRQn, 2);
	
}

void start_timer2() {
	// pass ������ʱ��2
	TIM2->CNT = 0;
	TIM2->CR1 |= 0x01;
}

void stop_timer2() {
	// pass ֹͣ��ʱ��2
	TIM2->CR1 &= ~(0x0001);
	TIM2->CNT = 0;
}

void TIM2_IRQHandler(void) {
	short human_flag = get_human_flag();
	
	if (TIM2->SR & 0x01) {
		if (human_flag) {
			timer2_flag = 1;
			// �رպ��ٿ���������ģ��
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
