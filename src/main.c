#include "led.h"
#include "uart.h"
#include "human.h"
#include "timer.h"
#include "light.h"
#include "lora.h"

/** uart.c	**/
extern u8 set_buff[8];
extern u8 usart1_flag;

extern short adc1_flag;
extern u8 timer2_flag;

extern int c_model;


int main() {
	long adc1_count = 0;
	
	// 各模块初始化
	usart1_init(9600); 
	led_init();
	human_init();
	timer2_init(6999, 35999); // 定时器定时3.5秒
	light_init();

	while(1) {
		// 接收到控制信号后转为（手动模式工作）
		if (usart1_flag) {
			usart1_flag = 0;
			
			if (!set_buff[1]) {
				// 手动模式工作
				if (set_buff[4]) {
					c_model = 1;
					// 手动控制LED灯显示
					led_display_manual(set_buff[3], set_buff[6]);
				}
			}
			// 发送控制完后设备的所有状态
			send_all_status();
		}
		
		// 默认以（自动模式工作）
		if (!set_buff[4]) {
			// 自动控制LED灯显示
			led_display_auto();
			c_model = 0;
 		}
		// 每隔一段时间开启一次光照强度检测（可以改为用定时器实现）
		adc1_count++;
		if (adc1_count>320000) {
			adc1_count = 0;
			if (adc1_flag) {
				ADC1->CR2 |= 1<<22; // 开始规则通道的转换
				adc1_flag = 0;
			}
		}
		// 发送设备状态变更后的值
		send_switch_change_status();
	}
}
