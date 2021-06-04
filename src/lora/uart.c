#include "stdio.h"
#include "uart.h"

u8 set_buff[8] = {0, 0, '|', 0, 0, 0, 0, 0};	// UART1接收的数据

/** 接收数据set_buff的位说明
	set_buff[0]：	通信类型，{-5：post，-3：get}
	set_buff[1]：	请求说明，{0：设备控制类请求，1：获取状态类请求}
	set_buff[2]：	分 隔 符，值为“|”
	set_buff[3]：	小灯开关，初始关闭
	set_buff[4]：	工作模式，初始自动
	set_buff[5]：	通信模式，初始透传
	set_buff[6]：	亮度等级，初始为 0
**/

u8 usart1_flag = 0;	// UART1控制信号标志

void usart1_init(u32 bound)
{
  RCC->APB2ENR |=1<<14|1<<2; //使能USART1和PA时钟  
	GPIOA->CRH   &=~( 0XFF <<4); //清8个位0
	GPIOA->CRH   |= ( 0X4a <<4); //PA9复用推挽2M	  PA10浮空输入 
	//波特率
	USART1->BRR  =(float)72000000/bound+0.5;//(四舍五入)
	//NVIC配置
	SCB->AIRCR =0X05FA0500; //设置分组为2   有且仅有一个分组
	NVIC->IP[37]=0x70;  //抢占2位 1     响应2位 3    0111 0000
	NVIC->ISER[1] |= 1<< (37-32); //开启串口的NVIC
	//使能串口中断
	USART1->CR1 |=1<<5;//开启接收中断  
	//帧格式及使能串口和收发
	USART1->CR1  |=1<<13|1<<3|1<<2;
	
	// buff初始化
//	buff[0] = 0;
//	buff[1] = 0;
//	buff[2] = 0;
//	buff[3] = 0;
}


void usart1_send(u8 ch)
{
	while(!(USART1->SR & 1<<6));//等待发送完成
	USART1->DR = ch;//发送
}

void usart1_send_int(int ch)
{
	while(!(USART1->SR & 1<<6));//等待发送完成
	USART1->DR=ch;//发送
}


/*******************************************
*函数名    :usart1_send_str
*函数功能  :串口1发送一个字符串函数
*函数参数  :u8 *str
*函数返回值:无
*函数描述  :
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

//重定义fputc函数

int fputc(int ch,FILE *f)
{
    while(!(USART1->SR & (1<<6)))	;						//等待发送完成到
		USART1->DR = ch ;
		return ch;
}

void USART1_IRQHandler(void)
{
	 u8 ch;
	 static int i = 0;
   if(USART1->SR & 1<<5)
	 {
		   USART1->SR &= ~(1<<5);//清标志
	     ch=USART1->DR;//读取数据页可以清标志
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
