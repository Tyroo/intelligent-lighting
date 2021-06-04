#include "light.h"
#include "uart.h"
#include "string.h"
/*******************************************
*作用：亮度模块初始化
*参数：无
*返回值：无
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
	RCC->CFGR |= 2<<14; // 设置ADC时钟预分频为12MHz（72MHz的6分频）
	
	GPIOA->CRL &= ~(0x0000000f); // 设置PA0口为模拟输入
	
	ADC1->CR1 &= 0xf0<<16;	// 设置为独立模式
	ADC1->CR1 |= 1<<5; 		// 开启ADC1中断
	ADC1->CR1 &= ~(1<<8);	// 关闭扫描模式
	
	ADC1->SQR1 &= ~(0XF<<20);
	ADC1->SQR1 |= 0<<20;	// 设置转换数量为1
	
//	ADC1->SQR3 &= 0xffffffe0;
//	ADC1->SQR3 |= 0;
	// 设置通道0的采样时间
	ADC1->SMPR2 &= ~(7<<0); //通道0采样时间清空
	ADC1->SMPR2 |= 7<<0;  //通道0 239.5周期，提高采样时间可以提高精确度
	
	ADC1->CR2 |= ~(1<<11); // 数据右对齐
	ADC1->CR2 |= 7<<17;		// 选择启动规则通道组转换的外部事件为SWSTART 
	ADC1->CR2 |= 1<<20;		// 使用外部触发启动规则通道的转换
	ADC1->CR2 &= ~(1<<1);	// 设置为单次转换
	ADC1->CR2 &= ~(1<<8);	// 关闭DMA直接访问数据
	
	ADC1->CR2 |= 1<<0;		 // 开启ADC转换器
	
	ADC1->CR2 |= 1<<3;	 // 开启校准
	while(ADC1->CR2 & 1<<3);
	
	ADC1->CR2 |= 1<<2;	 // 开启校准
	while(ADC1->CR2 & 1<<2);
	
	NVIC_EnableIRQ(ADC1_2_IRQn);	// 使能ADC1_2中断函数
}

/*
*作用：获取亮度等级	
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
