#include "led.h"
#include "human.h"
#include "light.h"
#include<math.h>
#include "string.h"
#include "uart.h"



int led_status_old = 0;	// LED�ľ�״̬
int led_status_new = 0;	// LED����״̬

int c_br = 0;	// ��ǰled�����ȵȼ�
int c_model = 0;	// ��ǰ����ģʽ

extern int light_grade;

/* 
*	���ã�LED��ʼ������(GPIO_C)
*	��������
*	����ֵ����
*/
void led_init() {
	// GPIO_C
	RCC->APB2ENR |= 1<<4;
	GPIOC->CRL &= 0<<28;
	GPIOC->CRL |= 3<<28;
	// GPIO_B
	RCC->APB2ENR |= 1<<3;
//	GPIOB->CRL &= 0x000<<20;
//	GPIOB->CRL |= 0x333<<20;
	GPIOB->CRH &= 0;
	GPIOB->CRH |= 0x33333<<8;
	
	GPIOC->ODR &= 0<<7;
	GPIOB->ODR |= 31<<10;
	
}


/* 
*	���ã���ȡ��ǰled��ʾ״̬ 
*	��������
*	����ֵ��1��������0���ر�
*/
short get_led_status() {
	// ��ȡled״̬������
	if (!(GPIOB->ODR & 1<<10)) {
		return 1;
	}
	
	// test
//	if (GPIOC->ODR & 1<<7) {
//		return 1;
//	}
	
	return 0;
}


/* 
*	���ã��ֶ�����LED��ʾ���� 
*	������df: LED��ʾ��־, br������ϵ��
*	����ֵ��success�����óɹ���error������ʧ��
*/
void led_display_manual(int df, int br) {
	// ��dfΪ0ʱ����������ϵ��Ϊ0���ر�LED��ʾ
	if (!df) {
		led_display_adjust(df);
		return;
	}	
	// ʹLED�������ȵ��ڵ�ֵ����ʾ
	led_display_adjust(br);
}


/* 
*	���ã��Զ�����LED��ʾ���� 
*	��������
*	����ֵ��success�����óɹ���error������ʧ��
*/
void led_display_auto() {
	// ��ȡ����ģ���������ֵ��0/1��
	int br = human_detection();
//	if (br) {
//		GPIOC->ODR |= 1<<7;
//	} else {
//		GPIOC->ODR &= 0<<7;
//	}
	// ������ģ�����ֵΪ1ʱ����br���ڹ���ģ�����ֵ
	if (!br) 
		light_grade = 0;
	
	if (light_grade != c_br || c_model) {
		c_br = light_grade;
		led_display_adjust(light_grade);
	}
}


/* 
*	���ã�LED���ȵ��ں��� 
*	������grade�����ȵ���ϵ������Χ0-5
*/
void led_display_adjust(int br) {
	GPIOB->ODR |= 31<<10;
	if (br) {
		GPIOB->ODR &= (32-(int)(pow(2, br)))<<10;
	}
//	if (br>3) {
//		GPIOC->ODR |= 1<<7;
//	} else {
//		GPIOC->ODR &= 0<<7;
//	}
}
