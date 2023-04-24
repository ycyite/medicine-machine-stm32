#include "LED.h"
#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitIntructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitIntructure.GPIO_Mode=GPIO_Mode_Out_PP;//
	GPIO_InitIntructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitIntructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitIntructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	
	GPIO_InitIntructure.GPIO_Mode=GPIO_Mode_Out_PP;//
	GPIO_InitIntructure.GPIO_Pin=GPIO_Pin_15|GPIO_Pin_13|GPIO_Pin_14;//红绿黄
	GPIO_InitIntructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitIntructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_15);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	GPIO_ResetBits(GPIOC,GPIO_Pin_14);
}
void Solid_Green(void)//绿色常亮
{
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)1);
	GPIO_WriteBit(GPIOC, GPIO_Pin_14, (BitAction)0);
	GPIO_WriteBit(GPIOC, GPIO_Pin_15, (BitAction)0);
}
void Solid_Red(void)//红色常亮
{
	GPIO_WriteBit(GPIOC, GPIO_Pin_15, (BitAction)1);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)0);
	GPIO_WriteBit(GPIOC, GPIO_Pin_14, (BitAction)0);
}
void Solid_Yellow(void)//黄色常亮
{
	GPIO_WriteBit(GPIOC, GPIO_Pin_15, (BitAction)1);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)1);
	GPIO_WriteBit(GPIOC, GPIO_Pin_14, (BitAction)0);
}
void Flashing_Green(void)//绿色闪烁
{
	static u8 FLAG=0;
	FLAG++;
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(FLAG%2));
	GPIO_WriteBit(GPIOC, GPIO_Pin_14, (BitAction)0);
	GPIO_WriteBit(GPIOC, GPIO_Pin_15, (BitAction)0);
}
void Flashing_Red(void)//红色闪烁
{
	static u8 FLAG=0;
	FLAG++;
	GPIO_WriteBit(GPIOC, GPIO_Pin_15, (BitAction)(FLAG%2));
	GPIO_WriteBit(GPIOC, GPIO_Pin_14, (BitAction)0);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)0);
}
