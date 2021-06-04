#ifndef _USART_H__
#define _USART_H__
#include "stm32f10x.h"                  // Device header
#include "stdio.h"
u8 usart1_get(void);
void usart1_init(u32 bound);  
void usart1_send(u8 ch);
void usart1_send_int(int ch);
void usart1_send_str(u8 *str);
int fputc(int ch,FILE *f);
void usart1_riv_str(void);
#endif
