#ifndef __EXTI_H
#define __EXTI_H
void EXTI_Config(void);

void NVIC_Config(void);

void Connect_EXTI_Config(void);

void EXTI0_IRQHandler(void);

void Check_Connect(void);

extern int motor_status;

#endif
