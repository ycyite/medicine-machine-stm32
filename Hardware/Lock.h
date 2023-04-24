#ifndef __LOCK_H
#define __LOCK_H
#include "stm32f10x.h"                  // Device header

#define Lock_port_rcc   RCC_APB2Periph_GPIOA
#define Lock_port       GPIOA
#define Lock_pin        GPIO_Pin_0

#define Get_Lock_port_rcc   RCC_APB2Periph_GPIOA
#define Get_Lock_port       GPIOA
#define Get_Lock_pin        GPIO_Pin_1

void Lock_Init(void);
void Lock_Open(void);//开锁
u8 Get_Lock(void);//信号反馈1为开启状态2为上锁状态

#endif
