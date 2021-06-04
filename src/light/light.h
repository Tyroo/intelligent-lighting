#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "stm32f10x.h"

void light_init(void);
int get_device_light_grade(void);
int get_env_light_grade(void);
#endif

