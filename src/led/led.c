#include "led.h"
#include "human.h"
#include "light.h"
#include<math.h>
#include "string.h"
#include "uart.h"



int led_status_old = 0;	// LED的旧状态
int led_status_new = 0;	// LED的新状态

int c_br = 0;	// 当前led的亮度等级
int c_model = 0;	// 当前工作模式

extern int light_grade;

/* 
*	作用：LED初始化函数(GPIO_C)
*	参数：无
*	返回值：无
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
*	作用：获取当前led显示状态 
*	参数：无
*	返回值：1：开启，0：关闭
*/
short get_led_status() {
	// 获取led状态并返回
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
*	作用：手动控制LED显示函数 
*	参数：df: LED显示标志, br：亮度系数
*	返回值：success：设置成功，error：设置失败
*/
void led_display_manual(int df, int br) {
	// 当df为0时，设置亮度系数为0，关闭LED显示
	if (!df) {
		led_display_adjust(df);
		return;
	}	
	// 使LED按照亮度调节的值来显示
	led_display_adjust(br);
}


/* 
*	作用：自动控制LED显示函数 
*	参数：无
*	返回值：success：设置成功，error：设置失败
*/
void led_display_auto() {
	// 获取红外模块人体检测的值（0/1）
	int br = human_detection();
//	if (br) {
//		GPIOC->ODR |= 1<<7;
//	} else {
//		GPIOC->ODR &= 0<<7;
//	}
	// 当红外模块检测的值为1时，令br等于光敏模块检测的值
	if (!br) 
		light_grade = 0;
	
	if (light_grade != c_br || c_model) {
		c_br = light_grade;
		led_display_adjust(light_grade);
	}
}


/* 
*	作用：LED亮度调节函数 
*	参数：grade：亮度调节系数，范围0-5
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
