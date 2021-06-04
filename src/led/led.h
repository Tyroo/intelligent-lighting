#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"

// function define
void led_init(void);
void led_display_manual(int df, int br);
void led_display_auto(void);
void led_display_adjust(int br);
short get_led_status(void);

#endif
