#include "nvic.h"

void Nvic_Init(u8 PreemptionPriority,u8 SubPriority,u8 Channel,u8 Group)
{
		u32 temp;    

    u8 IPRADDR  =Channel/4;  //每组只能存4个,得到组地址 
    u8 IPROFFSET = Channel%4;//在组内的偏移
    IPROFFSET = IPROFFSET * 8 + 4;    //得到偏移的确切位置
    Nvic_PriorityGroupConfig(Group);//设置分组
    temp = PreemptionPriority << ( 4 - Group);      
    temp |= SubPriority & (0x0f >> Group);
    temp &= 0xf;//取低四位

    if(Channel<32)
			NVIC->ISER[0] |= 1 << Channel;//使能中断位(要清除的话,相反操作就OK)
    else 
			NVIC->ISER[1] |= 1 <<(Channel - 32);    
    NVIC->IP[IPRADDR] |= temp << IPROFFSET;//设置响应优先级和抢断优先级            

}

void Nvic_PriorityGroupConfig(u8 Group)
{
		u16 temp,temp1;
    temp1 = (~Group) & 0x07;//取后三位
    temp1 <<= 8;
    temp = SCB->AIRCR;  //读取先前的设置
    temp &= 0X0000F8FF; //清空先前分组
    temp |= 0X05FA0000; //写入钥匙
    temp |= temp1;       
    SCB->AIRCR = temp;  //设置分组   
}



//不能在这里执行所有外设复位!否则至少引起串口不工作.		    
//把所有时钟寄存器复位		  
void MYRCC_DeInit(void)
{	
 	RCC->APB1RSTR = 0x00000000;//复位结束			 
	RCC->APB2RSTR = 0x00000000; 
	  
  RCC->AHBENR = 0x00000014;  //睡眠模式闪存和SRAM时钟使能.其他关闭.	  
  RCC->APB2ENR = 0x00000000; //外设时钟关闭.			   
  RCC->APB1ENR = 0x00000000;   
	RCC->CR |= 0x00000001;     //使能内部高速时钟HSION	 															 
	RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
	RCC->CIR = 0x00000000;     //关闭所有中断		 

}
//系统时钟初始化函数
//pll:选择的倍频数，从2开始，最大值为16		 
void Stm32_Clock_Init(u8 PLL)
{
	unsigned char temp=0;   
	MYRCC_DeInit();		  //复位并配置向量表
 	RCC->CR|=0x00010000;  //外部高速时钟使能HSEON
	while(!(RCC->CR>>17));//等待外部时钟就绪
	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
	PLL-=2;				  //抵消2个单位（因为是从2开始的，设置0就是2）
	RCC->CFGR|=PLL<<18;   //设置PLL值 2~16
	RCC->CFGR|=1<<16;	  //PLLSRC ON 
	FLASH->ACR|=0x32;	  //FLASH 2个延时周期
	RCC->CR|=0x01000000;  //PLLON
	while(!(RCC->CR>>25));//等待PLL锁定
	RCC->CFGR|=0x00000002;//PLL作为系统时钟	 
	while(temp!=0x02)     //等待PLL作为系统时钟设置成功
	{   
		temp=RCC->CFGR>>2;
		temp&=0x03;
	}    
}
