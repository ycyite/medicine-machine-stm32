#ifndef __SPEED_H
#define __SPEED_H
#include "stm32f10x.h"
void TIMx_Config1(TIM_TypeDef* TIMx);
void NVIC_TIM2_Config(void);
void Speed_Config(void);
void WAVE_EXTI_Config(void);
void NVIC_WAVE_Config(void);
typedef struct{
    int wave_nums;
    char motor_speed[10];
    int speed;

}SpeedInfo;

#endif
