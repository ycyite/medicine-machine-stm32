#ifndef __EXTI_H
#define __EXTI_H
void EXTI_Config(void);

void NVIC_Config(void);

void KEY_EXTI_Config(void);

void EXTI0_IRQHandler(void);

extern int motor_status;

#endif
