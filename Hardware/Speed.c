#include "Speed.h"
#include "Serial.h"
#include <stdio.h>
#include <string.h>
#include "Delay.h"
SpeedInfo speed_info1;

void TIMx_Config1(TIM_TypeDef* TIMx){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 自动重装载寄存器周的值(计数值)
	TIM_TimeBaseStructure.TIM_Prescaler= 7199;					// 时钟预分频数为 71，则驱动计数器的时钟 CK_CNT = CK_INT / (71+1)=1M
	TIM_TimeBaseStructure.TIM_Period=10000;						// 累计 TIM_Period 个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		// 时钟分频因子 ，基本定时器没有，不用管
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	// 计数器计数模式，基本定时器只能向上计数，没有计数模式的设置
	// 初始化定时器
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

	TIM_ARRPreloadConfig(TIMx, ENABLE);			//使能TIM重载寄存器ARR
	
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);		// 清除计数器中断标志位

	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);	// 开启计数器中断

	TIM_Cmd(TIMx, DISABLE);						// 关闭定时器的时钟，等待使用
    
}

void WAVE_EXTI_Config(void){
    GPIO_InitTypeDef	GPIO_InitStructure; 
	EXTI_InitTypeDef	EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 
	
	/* GPIO config*/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //内部上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* EXTI line mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
}


void NVIC_TIM2_Config(void){
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void NVIC_WAVE_Config(void){
    NVIC_InitTypeDef	NVIC_InitStructure;
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* 配置中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void Speed_Config(void){
    speed_info1.wave_nums = 0;
    TIMx_Config1(TIM2);
    WAVE_EXTI_Config();
    NVIC_TIM2_Config();
    NVIC_WAVE_Config();
    TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void){
    TIM2->SR = 0x0000;	//清除中断标志位
    speed_info1.speed = speed_info1.wave_nums*10; //根据每秒获得的方波数进行计算获得转速
    speed_info1.wave_nums = 0; //方波数清零
    sprintf(Motor_Speed, "%d", speed_info1.speed);
}
void EXTI15_10_IRQHandler(void){
    EXTI->PR = EXTI_Line15;
	Delay_us(10);
    if((GPIOA->IDR & GPIO_Pin_15) != 0){
        speed_info1.wave_nums++; //方波来一次，方波数增加一
    }
      
}
