#ifndef PWMOUT_H
#define PWMOUT_H
#include "stm32f10x.h"                  // Device header

//#include "sys.h"

void Time_init(void);
void TimePwm_init(int arr,int psc);

#endif
