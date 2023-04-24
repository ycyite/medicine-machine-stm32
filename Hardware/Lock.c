#include "Lock.h"
#include "Delay.h"

void Lock_Init(void)
{
	GPIO_InitTypeDef GPIO_InitIntructure;
	RCC_APB2PeriphClockCmd(Lock_port_rcc,ENABLE);
	GPIO_InitIntructure.GPIO_Mode=GPIO_Mode_Out_PP;//配置开锁mos管引脚
	GPIO_InitIntructure.GPIO_Pin=Lock_pin;
	GPIO_InitIntructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(Lock_port,&GPIO_InitIntructure);
	
	GPIO_InitIntructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitIntructure.GPIO_Pin=Get_Lock_pin;//配置电磁锁信号反馈线，一端接vcc一端接io口
	//GPIO_InitIntructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(Get_Lock_port,&GPIO_InitIntructure);
	
}

void Lock_Open(void)//开锁
{
	GPIO_WriteBit(Lock_port,Lock_pin,Bit_SET);//开锁,io置高电平,mos管导通给锁供电
	Delay_ms(500);
	GPIO_WriteBit(Lock_port,Lock_pin,Bit_RESET);//置低
}

u8 Get_Lock(void)//信号反馈1为开启状态0为上锁状态
{
	return GPIO_ReadInputDataBit(Get_Lock_port, Get_Lock_pin);
}
