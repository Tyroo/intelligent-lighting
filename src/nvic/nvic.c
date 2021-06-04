#include "nvic.h"

void Nvic_Init(u8 PreemptionPriority,u8 SubPriority,u8 Channel,u8 Group)
{
		u32 temp;    

    u8 IPRADDR  =Channel/4;  //ÿ��ֻ�ܴ�4��,�õ����ַ 
    u8 IPROFFSET = Channel%4;//�����ڵ�ƫ��
    IPROFFSET = IPROFFSET * 8 + 4;    //�õ�ƫ�Ƶ�ȷ��λ��
    Nvic_PriorityGroupConfig(Group);//���÷���
    temp = PreemptionPriority << ( 4 - Group);      
    temp |= SubPriority & (0x0f >> Group);
    temp &= 0xf;//ȡ����λ

    if(Channel<32)
			NVIC->ISER[0] |= 1 << Channel;//ʹ���ж�λ(Ҫ����Ļ�,�෴������OK)
    else 
			NVIC->ISER[1] |= 1 <<(Channel - 32);    
    NVIC->IP[IPRADDR] |= temp << IPROFFSET;//������Ӧ���ȼ����������ȼ�            

}

void Nvic_PriorityGroupConfig(u8 Group)
{
		u16 temp,temp1;
    temp1 = (~Group) & 0x07;//ȡ����λ
    temp1 <<= 8;
    temp = SCB->AIRCR;  //��ȡ��ǰ������
    temp &= 0X0000F8FF; //�����ǰ����
    temp |= 0X05FA0000; //д��Կ��
    temp |= temp1;       
    SCB->AIRCR = temp;  //���÷���   
}



//����������ִ���������踴λ!�����������𴮿ڲ�����.		    
//������ʱ�ӼĴ�����λ		  
void MYRCC_DeInit(void)
{	
 	RCC->APB1RSTR = 0x00000000;//��λ����			 
	RCC->APB2RSTR = 0x00000000; 
	  
  RCC->AHBENR = 0x00000014;  //˯��ģʽ�����SRAMʱ��ʹ��.�����ر�.	  
  RCC->APB2ENR = 0x00000000; //����ʱ�ӹر�.			   
  RCC->APB1ENR = 0x00000000;   
	RCC->CR |= 0x00000001;     //ʹ���ڲ�����ʱ��HSION	 															 
	RCC->CFGR &= 0xF8FF0000;   //��λSW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;     //��λHSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //��λHSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;   //��λPLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
	RCC->CIR = 0x00000000;     //�ر������ж�		 

}
//ϵͳʱ�ӳ�ʼ������
//pll:ѡ��ı�Ƶ������2��ʼ�����ֵΪ16		 
void Stm32_Clock_Init(u8 PLL)
{
	unsigned char temp=0;   
	MYRCC_DeInit();		  //��λ������������
 	RCC->CR|=0x00010000;  //�ⲿ����ʱ��ʹ��HSEON
	while(!(RCC->CR>>17));//�ȴ��ⲿʱ�Ӿ���
	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
	PLL-=2;				  //����2����λ����Ϊ�Ǵ�2��ʼ�ģ�����0����2��
	RCC->CFGR|=PLL<<18;   //����PLLֵ 2~16
	RCC->CFGR|=1<<16;	  //PLLSRC ON 
	FLASH->ACR|=0x32;	  //FLASH 2����ʱ����
	RCC->CR|=0x01000000;  //PLLON
	while(!(RCC->CR>>25));//�ȴ�PLL����
	RCC->CFGR|=0x00000002;//PLL��Ϊϵͳʱ��	 
	while(temp!=0x02)     //�ȴ�PLL��Ϊϵͳʱ�����óɹ�
	{   
		temp=RCC->CFGR>>2;
		temp&=0x03;
	}    
}
