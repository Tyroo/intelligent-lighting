/*
lora.c

通信类型：get: -，post: +。
通信描述：｛0：发送设备所有状态，1：发送设备开关状态｝
发送格式：通信类型 通信描述 | 传递的数据(-0|)
*/
#include "lora.h"
#include "string.h"
#include "led.h"
#include "uart.h"
#include "light.h"

extern u8 set_buff[8];
extern int light_value;

extern int led_status_old;	// LED的旧状态
extern int led_status_new;	// LED的新状态


// 根据请求参数返回设备的状态（暂时保留）
void send_status(short req) {
   switch (req) {
    case 0: send_all_status();break;// 发送设备所有状态;
    case 1: break;// 发送设备开关状态;break;
		default: break;
  }
}


// 发送设备的所有状态
void send_all_status() {
		// start_key = "all_status";
		char send_str[50];
		int status_switch = get_led_status();
		int status_communication_mode = 0;
		int status_control_mode = set_buff[4];
		int status_env_light = get_env_light_grade();
		int status_device_light = get_device_light_grade();
	
		if(set_buff[4] == 1) {
			status_device_light = set_buff[6];
		}
		sprintf(send_str, "+all_status|%d%d%d%d%d", status_switch, status_control_mode,
			status_communication_mode, status_device_light, status_env_light);
		usart1_send_str((u8*)send_str);
}


// 发送设备开关状态变更后的值
void send_switch_change_status() {
	led_status_new = get_led_status();
	if (led_status_new != led_status_old) {
		send_switch_status(led_status_new);
		led_status_old = led_status_new;
	}
}


// 发送设备当前开关状态
void send_switch_status(int state) {
   // send_key = "switch_status";
		char send_str[50];
		sprintf(send_str, "+switch_status|%d", state);
		usart1_send_str((u8*)send_str);
}
