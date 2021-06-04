#include "light.h"
#include "uart.h"
#include "string.h"
/*******************************************
*���ã�����ģ���ʼ��
*��������
*����ֵ����
*********************************************/
short adc1_flag = 1;
int light_value = 0;
int light_grade = 0;

void light_init() {
	
	RCC->APB2ENR |= 1<<2;
	RCC->APB2ENR |= 1<<9;
	RCC->APB2RSTR |= 1<<9;
	RCC->APB2RSTR &= 0<<9;
	
	RCC->CFGR &= ~(3<<14);
	RCC->CFGR |= 2<<14; // ����ADCʱ��Ԥ��ƵΪ12MHz��72MHz��6��Ƶ��
	
	GPIOA->CRL &= ~(0x0000000f); // ����PA0��Ϊģ������
	
	ADC1->CR1 &= 0xf0<<16;	// ����Ϊ����ģʽ
	ADC1->CR1 |= 1<<5; 		// ����ADC1�ж�
	ADC1->CR1 &= ~(1<<8);	// �ر�ɨ��ģʽ
	
	ADC1->SQR1 &= ~(0XF<<20);
	ADC1->SQR1 |= 0<<20;	// ����ת������Ϊ1
	
//	ADC1->SQR3 &= 0xffffffe0;
//	ADC1->SQR3 |= 0;
	// ����ͨ��0�Ĳ���ʱ��
	ADC1->SMPR2 &= ~(7<<0); //ͨ��0����ʱ�����
	ADC1->SMPR2 |= 7<<0;  //ͨ��0 239.5���ڣ���߲���ʱ�������߾�ȷ��
	
	ADC1->CR2 |= ~(1<<11); // �����Ҷ���
	ADC1->CR2 |= 7<<17;		// ѡ����������ͨ����ת�����ⲿ�¼�ΪSWSTART 
	ADC1->CR2 |= 1<<20;		// ʹ���ⲿ������������ͨ����ת��
	ADC1->CR2 &= ~(1<<1);	// ����Ϊ����ת��
	ADC1->CR2 &= ~(1<<8);	// �ر�DMAֱ�ӷ�������
	
	ADC1->CR2 |= 1<<0;		 // ����ADCת����
	
	ADC1->CR2 |= 1<<3;	 // ����У׼
	while(ADC1->CR2 & 1<<3);
	
	ADC1->CR2 |= 1<<2;	 // ����У׼
	while(ADC1->CR2 & 1<<2);
	
	NVIC_EnableIRQ(ADC1_2_IRQn);	// ʹ��ADC1_2�жϺ���
}

/*
*���ã���ȡ���ȵȼ�	
*/
int get_device_light_grade() {
	if (400 < light_value && light_value < 1800) {
		return light_grade;
	}
	light_grade = (int)((light_value + 600) / 1000);
	if (light_value >= 3800) {
		light_grade = 5;
	}
	return light_grade;
}

int get_env_light_grade() {
	int grade = 0;
	if (light_value>4060)
		return grade;
	else if (light_value<200)
		return 5;
	else
		return (int)((4050-light_value) / 760);
}


void ADC1_2_IRQHandler() {
//	char light_grade_str[50];

	if (ADC1->SR & 1<<1) {
		light_value = ((int)ADC1->DR);
		light_grade = get_device_light_grade();
//		sprintf(light_grade_str, "+light_grade|%d", light_grade);
//		usart1_send_str((u8 *)light_grade_str);
		adc1_flag = 1;
	}
}
