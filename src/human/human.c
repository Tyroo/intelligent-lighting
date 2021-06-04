#include "human.h"
#include "timer.h"
#include "led.h"

// ����timer.c��ȫ�ֱ���
extern u8 timer2_flag;

/*
*	���ã���ʼ��������ģ��
*	��������
*	����ֵ����
*/
void human_init() {
	// GPIO_E
	RCC->APB2ENR |= 0x01<<6;
	// PE05����������
	GPIOE->CRL &= 0x00<<20;
	GPIOE->CRL |= 0x04<<20;
	// PE04�������������ʼ���Ϊ�͵�ƽ
	GPIOE->CRL &= 0xF0<<16;
	GPIOE->CRL |= 0x03<<16;
	GPIOE->ODR &= ~(0x01<<4);
}

/*
*	���ã�����������ģ�鿪/��
*	������sw��0Ϊ�رգ�1Ϊ����
*	����ֵ����
*/
void human_switch(short sw) {
	if (sw) {
		GPIOE->ODR &= ~(0x01<<4);
	} else {
		GPIOE->ODR |= 0x01<<4;
	}
}


/*
*	���ã���ȡ������ģ�����˲ʱֵ
*	��������
*	����ֵ��0�����ˣ�1������
*/
short get_human_flag() {
	// pass ��ȡ������ģ�������Ӧ�����ŵĵ�ƽ
	if (GPIOE->IDR & 1<<5) {
		return 1;
	}
	return 0;
}


/*
*	���ã�ͨ����ʱ��ѭ����⣬�ó�������ģ������ȶ�ֵ
*	��������
*	����ֵ��0�����ˣ�1������
*/
int human_detection() {
//	short led_flag = get_led_status();
	static int led_flag = 0;
	static int human_flag = 0;
	
	if (!timer2_flag) {
		human_flag = get_human_flag();
		if (human_flag) {
			led_flag = 1;
			start_timer2();
		} else if (led_flag) {
			led_flag = 0;
		} else {}
			
	}
	return led_flag;
}
