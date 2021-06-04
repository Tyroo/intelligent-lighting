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
	
	// ��ģ���ʼ��
	usart1_init(9600); 
	led_init();
	human_init();
	timer2_init(6999, 35999); // ��ʱ����ʱ3.5��
	light_init();

	while(1) {
		// ���յ������źź�תΪ���ֶ�ģʽ������
		if (usart1_flag) {
			usart1_flag = 0;
			
			if (!set_buff[1]) {
				// �ֶ�ģʽ����
				if (set_buff[4]) {
					c_model = 1;
					// �ֶ�����LED����ʾ
					led_display_manual(set_buff[3], set_buff[6]);
				}
			}
			// ���Ϳ�������豸������״̬
			send_all_status();
		}
		
		// Ĭ���ԣ��Զ�ģʽ������
		if (!set_buff[4]) {
			// �Զ�����LED����ʾ
			led_display_auto();
			c_model = 0;
 		}
		// ÿ��һ��ʱ�俪��һ�ι���ǿ�ȼ�⣨���Ը�Ϊ�ö�ʱ��ʵ�֣�
		adc1_count++;
		if (adc1_count>320000) {
			adc1_count = 0;
			if (adc1_flag) {
				ADC1->CR2 |= 1<<22; // ��ʼ����ͨ����ת��
				adc1_flag = 0;
			}
		}
		// �����豸״̬������ֵ
		send_switch_change_status();
	}
}
