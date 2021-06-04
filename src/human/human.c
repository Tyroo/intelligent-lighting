#include "human.h"
#include "timer.h"
#include "led.h"

// 引用timer.c中全局变量
extern u8 timer2_flag;

/*
*	作用：初始化人体检测模块
*	参数：无
*	返回值：无
*/
void human_init() {
	// GPIO_E
	RCC->APB2ENR |= 0x01<<6;
	// PE05，浮空输入
	GPIOE->CRL &= 0x00<<20;
	GPIOE->CRL |= 0x04<<20;
	// PE04，推挽输出，初始输出为低电平
	GPIOE->CRL &= 0xF0<<16;
	GPIOE->CRL |= 0x03<<16;
	GPIOE->ODR &= ~(0x01<<4);
}

/*
*	作用：控制人体检测模块开/关
*	参数：sw：0为关闭，1为开启
*	返回值：无
*/
void human_switch(short sw) {
	if (sw) {
		GPIOE->ODR &= ~(0x01<<4);
	} else {
		GPIOE->ODR |= 0x01<<4;
	}
}


/*
*	作用：获取人体检测模块检测的瞬时值
*	参数：无
*	返回值：0：无人，1：有人
*/
short get_human_flag() {
	// pass 获取人体检测模块输出对应的引脚的电平
	if (GPIOE->IDR & 1<<5) {
		return 1;
	}
	return 0;
}


/*
*	作用：通过定时器循环检测，得出人体检测模块检测的稳定值
*	参数：无
*	返回值：0：无人，1：有人
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
