#include"stm32f10x.h"
#include"PWM.h"
#include"Delay.h"
#include"RGB.h"
#include<stdio.h>
#include "Exti.h"
int motor_status;
void EXTI_Config(void){
    GPIO_InitTypeDef	GPIO_InitStructure; 
	EXTI_InitTypeDef	EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
	
	/* GPIO config*/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //内部上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* EXTI line mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

}

void NVIC_Config(void){
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //调速键
}

void KEY_EXTI_Config(void){
    EXTI_Config();
    NVIC_Config();
    EXTI_ClearFlag(EXTI_Line0);
}
int val[6] = {0, 10, 20, 30, 40, 49};
int time = 0;

void EXTI0_IRQHandler(void){
    EXTI->PR = EXTI_Line0;
    if((GPIOB->IDR & GPIO_Pin_0) != 0){
        motor_status = 0;
    }
}

/*void EXTI2_IRQHandler(void){
    Delay_ms(10); 
    if(EXTI_GetITStatus(EXTI_Line2)!=RESET){
        Delay_ms(10);
        time = (time + 1) % 6;
        PWM_SetCompare1(val[time]);
        EXTI_ClearITPendingBit(EXTI_Line2);
        Delay_ms(10);
    }
    Delay_ms(10);
    if(EXTI_GetITStatus(EXTI_Line2)!=RESET){
        Delay_ms(10);
        if(val == 0){
            val = 1;
            signal = (signal + 1)%2;
        }
        val = (val*2) % 16;
        if(signal == 1){
            EXTI_ClearITPendingBit(EXTI_Line2);
            while(1){
                set_rgb((val >> 1) & 1, (val >> 2) & 1, (val >> 3) & 1);
                Delay_ms(100);
                set_rgb(0,0,0);
                Delay_ms(100);
                if(EXTI_GetITStatus(EXTI_Line2)!=RESET){
                    Delay_ms(10);
                    break;
                }
            }
        }else{
            EXTI_ClearITPendingBit(EXTI_Line2);
            set_rgb((val >> 1) & 1, (val >> 2) & 1, (val >> 3) & 1);
            
        }
    }
}*/


