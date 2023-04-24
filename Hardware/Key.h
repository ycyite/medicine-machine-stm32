#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"                  // Device header

#include "Delay.h"
#include <stdint.h>

#define	KEY_TIME_IDLE		400		//按键动作空闲时间
#define	KEY_TIME_CONTINUS	500		//按键动作持续时间
#define	KEY_TIME_OUT		1000	//按键超时

//事件类型
#define EVENT_NONE_CLICK	0x00	//无动作
#define EVENT_SHORT_CLICK	0x01	//短按
#define EVENT_DOUBLE_CLICK	0x02	//连击
#define EVENT_LONG_CLICK	0x03	//长按

//按键状态
#define KEY_STATE_IDLE		0x00	//空闲
#define KEY_STATE_PRESS		0x01	//按下
#define KEY_STATE_RELEASE	0x02	//松开


typedef struct{
	struct{
		u8 check		:1;		//查询标志
		u8 key_state	:2;		//单次按键动作【0：无动作/1：按下/2：松开】
		u8 once_event	:1;		//一次完整按键事件，置1时可以进行事件识别，必须手动清零
		u8 press_time	:1;		//单次按键动作持续时间【0：短按/1：长按】
	}flag;                                       

	u8 event_current_type	:4;	//当前按键事件类型【00：无动作】【01：短按】【10：连击】【11：长按】
	u8 event_previous_type	:4;	//之前按键事件类型，需手动更新

	u8	press_cnt;				//按下次数，复位值为1，最大连击次数为256次
	
	u16 time_idle;				//按键空闲时间计数器
	u16 time_continus;			//按键动作持续时间计数器
	
}KEY_PROCESS_TypeDef;

void EXTI_GPIO_Config(void);

void TIMx_Config(TIM_TypeDef* TIMx);

static void NVIC_EXTI_GPIO_Config(void);

static void NVIC_TIM2_Config(void);

//void EXTI9_5_IRQHandler(void);

void KEY_Process(void);

void KEY_Scan(void);

void KEY_Config(void);

#endif
