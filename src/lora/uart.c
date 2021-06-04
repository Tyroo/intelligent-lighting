#include "stdio.h"
#include "uart.h"

u8 set_buff[8] = {0, 0, '|', 0, 0, 0, 0, 0};	// UART1���յ�����

/** ��������set_buff��λ˵��
	set_buff[0]��	ͨ�����ͣ�{-5��post��-3��get}
	set_buff[1]��	����˵����{0���豸����������1����ȡ״̬������}
	set_buff[2]��	�� �� ����ֵΪ��|��
	set_buff[3]��	С�ƿ��أ���ʼ�ر�
	set_buff[4]��	����ģʽ����ʼ�Զ�
	set_buff[5]��	ͨ��ģʽ����ʼ͸��
	set_buff[6]��	���ȵȼ�����ʼΪ 0
**/

u8 usart1_flag = 0;	// UART1�����źű�־

void usart1_init(u32 bound)
{
  RCC->APB2ENR |=1<<14|1<<2; //ʹ��USART1��PAʱ��  
	GPIOA->CRH   &=~( 0XFF <<4); //��8��λ0
	GPIOA->CRH   |= ( 0X4a <<4); //PA9��������2M	  PA10�������� 
	//������
	USART1->BRR  =(float)72000000/bound+0.5;//(��������)
	//NVIC����
	SCB->AIRCR =0X05FA0500; //���÷���Ϊ2   ���ҽ���һ������
	NVIC->IP[37]=0x70;  //��ռ2λ 1     ��Ӧ2λ 3    0111 0000
	NVIC->ISER[1] |= 1<< (37-32); //�������ڵ�NVIC
	//ʹ�ܴ����ж�
	USART1->CR1 |=1<<5;//���������ж�  
	//֡��ʽ��ʹ�ܴ��ں��շ�
	USART1->CR1  |=1<<13|1<<3|1<<2;
	
	// buff��ʼ��
//	buff[0] = 0;
//	buff[1] = 0;
//	buff[2] = 0;
//	buff[3] = 0;
}


void usart1_send(u8 ch)
{
	while(!(USART1->SR & 1<<6));//�ȴ��������
	USART1->DR = ch;//����
}

void usart1_send_int(int ch)
{
	while(!(USART1->SR & 1<<6));//�ȴ��������
	USART1->DR=ch;//����
}


/*******************************************
*������    :usart1_send_str
*��������  :����1����һ���ַ�������
*��������  :u8 *str
*��������ֵ:��
*��������  :
*********************************************/
void usart1_send_str(u8 *str)
{
	while(*str != '\0')
	{
		usart1_send(*str);
		str++;
	}
	
}

u8 usart1_get(void)
{
	u8  ch;
   while(!(USART1->SR & (1<<5)));
   ch = USART1->DR;
   return ch;
}


void usart1_riv_str(void)
{
	 u8 ch, i = 0;
   while(1)
	 {
	     ch=usart1_get();
		   if(ch == '\r' || ch == '\n')
			 {
//				  set_buff[i] = '\0';
			    return;   
			 }
			 set_buff[i++] = ch;
	 }
}

//�ض���fputc����

int fputc(int ch,FILE *f)
{
    while(!(USART1->SR & (1<<6)))	;						//�ȴ�������ɵ�
		USART1->DR = ch ;
		return ch;
}

void USART1_IRQHandler(void)
{
	 u8 ch;
	 static int i = 0;
   if(USART1->SR & 1<<5)
	 {
		   USART1->SR &= ~(1<<5);//���־
	     ch=USART1->DR;//��ȡ����ҳ�������־
		   if(ch == '\r' || ch == '\n')
			 {
				  set_buff[i] = '\0';
				  i = 0;
				 
				  usart1_flag = 1;
				 
			    return;   
			 }
			 set_buff[i++]= (int)ch - 48;
	 }
}
