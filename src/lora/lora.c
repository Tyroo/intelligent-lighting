/*
lora.c

ͨ�����ͣ�get: -��post: +��
ͨ����������0�������豸����״̬��1�������豸����״̬��
���͸�ʽ��ͨ������ ͨ������ | ���ݵ�����(-0|)
*/
#include "lora.h"
#include "string.h"
#include "led.h"
#include "uart.h"
#include "light.h"

extern u8 set_buff[8];
extern int light_value;

extern int led_status_old;	// LED�ľ�״̬
extern int led_status_new;	// LED����״̬


// ����������������豸��״̬����ʱ������
void send_status(short req) {
   switch (req) {
    case 0: send_all_status();break;// �����豸����״̬;
    case 1: break;// �����豸����״̬;break;
		default: break;
  }
}


// �����豸������״̬
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


// �����豸����״̬������ֵ
void send_switch_change_status() {
	led_status_new = get_led_status();
	if (led_status_new != led_status_old) {
		send_switch_status(led_status_new);
		led_status_old = led_status_new;
	}
}


// �����豸��ǰ����״̬
void send_switch_status(int state) {
   // send_key = "switch_status";
		char send_str[50];
		sprintf(send_str, "+switch_status|%d", state);
		usart1_send_str((u8*)send_str);
}
