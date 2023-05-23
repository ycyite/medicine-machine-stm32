#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "I2C_LCD.h"
#include "Serial.h"
#include "PWM.h"
#include "Timer.h"
#include "RGB.h"
#include "Exti.h"
#include "HDL_WS2812B.h"
#include "MyI2C.h"
#include "PID.h"
#include "usart.h"
#include "PWMOUT.h"
#include "Lock.h"
#include "AD.h"
#include "Temp.h"
#include "motor.h"
#include "LED.h"
#include "Exti.h"
#include "Speed.h"
#include <stdlib.h>
//#include "sys.h"

#define PERIOD    400
#define PRESCALER 3600

//设定温度
#define SetTemperature 20
int Num;//计时，10s加1
u8 Process;//指示灯流程
int Connect_Flag;
void Motor_Reset(){
	Delay_ms(10);
	EXTI->PR = EXTI_Line0;
	if((GPIOB->IDR & GPIO_Pin_0) == 0){
		motor_status = 1;
	}
	while(1){
		if(motor_status==0){
			break;
		}
		Setp_Motor_CW(100);
	}
	Setp_Motor_Stop();
}

void Self_Test()
{
	Process=0;//红闪
	Num=0;
	AD_Init();
	TimePwm_init(PERIOD-1,PRESCALER);
	TIM_Cmd(TIM1, ENABLE);//开启定时
	
	float TempS,TempE;
	TempS=0;TempE=0;
	while(Num>=0&&Num<10)
	{
		//I2C_LCD_ShowHexNum(1,1,0x00,2);
		TIM_SetCompare2(TIM3,400);
		u16 AD1=AD_GetValue(ADC_Channel_1);
		float TS = Get_Kelvin_Temperature(AD1);
		
		if(TS>50)TIM_SetCompare2(TIM3,4);
		
		if(TempS==0)TempS=TS;
		if(TempS>TS)TempS=TS;
		if(TS>80)TS=TempE;
		if(TempE<TS)TempE=TS;
		//I2C_LCD_WriteFloat(2,7,TS);
		
		/*if(USART_RX_STA&0X8000)
		{
			float TS=(float)USART_RX_BUF[2]+(float)(USART_RX_BUF[3])/100;
			if(TempS==0)TempS=TS;
			if(TempS>TS)TempS=TS;
			if(TS>80)TS=TempE;
			if(TempE<TS)TempE=TS;
			I2C_LCD_WriteFloat(2,7,TS);
			USART_RX_STA=0;
		}*/

	}
	if(TempE > SetTemperature){
		Self_Flag = 1;
	}else{
		Self_Flag = 2;
	}
	while(TempE<SetTemperature)//小于设定温度报错
	{
		Process=4;//常红
	}
	//float V=(TempE-TempS);
	
	TIM_CCxCmd(TIM3, TIM_Channel_2, TIM_CCx_Disable);//关闭定时器通道2
	TIM_Cmd(TIM3, DISABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);//更新中断函数
	Process=1;
	while(!Start_Flag)
	{
		
		//常绿
		/*
		Serial_TxPacket[0]=0xAA;
		Serial_SendPacket();//给esp32发送自检结束
		*/
	}
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	TIM_Cmd(TIM1, DISABLE);        //关闭定时器
    TIM_SetCounter(TIM1, 0);        //清零计数器
}
float t1,t2,t3;
void Constant_Temp()//恒温
{	
	/*Motor_Distance(5,3);//上升5mm,频率10
	Setp_Motor_Stop();*/
	Num=0;
	TIM_Cmd(TIM1, ENABLE);//开启定时
	TIM_CCxCmd(TIM3, TIM_Channel_4, TIM_CCx_Disable);//关闭定时器通道2
	TIM_Cmd(TIM3, DISABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);//更新中断函数
	
	t1=0;t2=43;t3=43;
	
	AD_Init();
	PID_Init();
	Time_init();
	TimePwm_init(PERIOD-1,PRESCALER);
	uart_init(9600);
	while(Num>=0&&Num<20)
	{
		u16 AD1=AD_GetValue(ADC_Channel_1); //获取热敏电阻的AD值
		float temp1 = Get_Kelvin_Temperature(AD1); //获得加热片温度
		pid.Pv=temp1;
		if(sprintf(Heat_Temp,"%.2f",temp1) < 0){

		}
		PID_Calc(); //PID计算
		unsigned int num=0;
		num=(((pid.OUT*PERIOD)/pid.pwmcycle)-1); //将PID输出转换成PWM波的占空比
		TIM_SetCompare2(TIM3,num);
	}
	TIM_SetCompare2(TIM3,1);//关闭加热
	//I2C_LCD_WriteCmd(0x01);//清屏
	
	/*Motor_Distance1(5,3);//下降5mm，频率10
	Setp_Motor_Stop();*/
	
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	TIM_Cmd(TIM1, DISABLE);        //关闭定时器
    TIM_SetCounter(TIM1, 0);        //清零计数器
}
void Centrifugal()//离心
{
	Num=0;
	
	TIM_Cmd(TIM1, ENABLE);//开启定时
	
	TIM_CCxCmd(TIM3, TIM_Channel_2, TIM_CCx_Disable);//关闭定时器通道2
	TIM_Cmd(TIM3, DISABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);//更新中断函数
	
	PWM_Init();
	Speed_Config();
	PWM_SetCompare1(95);
	while(Num>=0&&Num<20)
	{
		u16 AD1=AD_GetValue(ADC_Channel_1);
		//float temp2 = Get_Kelvin_Temperature(AD0);
		float temp1 = Get_Kelvin_Temperature(AD1);
		if(sprintf(Heat_Temp,"%.2f",temp1) < 0){

		}
	}
	PWM_SetCompare1(1);//关闭离心
	NVIC_DisableIRQ(EXTI15_10_IRQn);
}
void Fluorescence()//荧光
{	
	TIM_CCxCmd(TIM3, TIM_Channel_4, TIM_CCx_Disable);//关闭定时器通道2
	TIM_Cmd(TIM3, DISABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);//更新中断函数
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	
	Constant_Temp();//90
	/*while(Num>=0&&Num<10)
	{
		I2C_LCD_WriteChar(1,4,(Num/100)+'0');
		I2C_LCD_WriteChar(1,5,(Num/10)%10+'0');
		I2C_LCD_WriteChar(1,6,(Num%10)+'0');
		I2C_LCD_ShowHexNum(1,1,0x03,2);
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
		
	}*/
	
	TIM_Cmd(TIM1, ENABLE);//开启定时
	Process=3;//常黄
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);//关闭荧光
	//TIM_Cmd(TIM1, DISABLE);        //关闭定时器
    //TIM_SetCounter(TIM1, 0);        //清零计数器
}
void End()
{
	TIM_CCxCmd(TIM3, TIM_Channel_2, TIM_CCx_Disable);//关闭定时器通道2
	TIM_CCxCmd(TIM3, TIM_Channel_3, TIM_CCx_Disable);//关闭定时器通道2
	TIM_CCxCmd(TIM3, TIM_Channel_4, TIM_CCx_Disable);//关闭定时器通道2
	TIM_Cmd(TIM3, DISABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);//更新中断函数
}
void Wait_Start(){
	Process = 1;
	TIM_Cmd(TIM1, ENABLE);//开启定时
	while(!Start_Flag);
	
		
		//常绿
		/*
		Serial_TxPacket[0]=0xAA;
		Serial_SendPacket();//给esp32发送自检结束
		*/
	
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	TIM_Cmd(TIM1, DISABLE);        //关闭定时器
    TIM_SetCounter(TIM1, 0);
}

void Check_Blue_Connect(){
	TIM_Cmd(TIM1, ENABLE);//开启定时
	Process = 0;
	Check_Connect();
	Connect_Flag = 1; //蓝牙连接成功
}
int main(void)
{
	/*Moto_Init();
	LED_Init();
	I2C_LCD_WriteInit();
	Timer_Init();//定时配置
	Serial_Init();

	while(1)
	{
		Self_Test();

		Process=2;//绿闪
		if((Serial_GetRxFlag() == 1)&&(Serial_RxPacket[0]==0x01))//接收到esp回复指令
		{
			Centrifugal();//90  96
			Constant_Temp();//90
			Centrifugal();//90  96
			GPIO_SetBits(GPIOB,GPIO_Pin_6);
			Delay_ms(20);
			while(Serial_RxPacket1[0]!=0x06)
			{
				Delay_ms(1000);
				//GPIO_SetBits(GPIOB,GPIO_Pin_6);
				Serial_TxPacket[0]=0x04;
				Serial_SendPacket();//给esp32发送拍照
			}
			TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
			TIM_Cmd(TIM1, DISABLE);        //关闭定时器
			TIM_SetCounter(TIM1, 0);         //清零计数器
			Fluorescence();
			//End();
			
			while(1)
			{
				Process=3;//常黄
			}
			End();
		}
	}*/
	/*while(!Connect_Status){

	}
	Delay_s(3);
	Serial_SendByte(Send_Flag_Temp);
	Heat_Temp[0] = '3';
	Heat_Temp[1] = '6';
	Heat_Temp[2] = '.';
	Heat_Temp[3] = '6';
	Heat_Temp[4] = '7';
	Serial_SendString(Heat_Temp);
	Delay_s(10);
	Serial_SendByte(Send_Flag_Motor);
	Motor_Speed[0] = '2';
	Motor_Speed[1] = '0';
	Motor_Speed[2] = '3';
	Motor_Speed[3] = '4';
	Serial_SendString(Motor_Speed);
	Delay_s(10);
	Serial_SendByte(Send_Flag_LED);
	Serial_SendByte(LED_Status);
	Delay_s(10);*/
	/*Serial_Init();
	Timer_Init();	
	float min = 10.0f;
    float max = 99.0f;
	float random_float = 0.0f;
	int random_speed = 0;
	while(!Start_Flag){
		
	}
	Num = 0;
	TIM_Cmd(TIM1, ENABLE);
	while(Num < 10){
    	random_float = (float)rand() / RAND_MAX * (max - min) + min;
		if(sprintf(Heat_Temp,"%.2f",random_float) < 0){
			return -1;
		}
		if(Num <= 3){
			continue;
		}
		random_speed = rand() % 2500;
		if(sprintf(Motor_Speed, "%d", random_speed) < 0){
			return -1;
		}
		if(Num <= 6){
			continue;
		}
		LED_Status[0] = '1';
	}*/
	Connect_Flag = 0;
	LED_Status[0] = '0';
	LED_Status[1] = '\0';
	Motor_Speed[0] = '0';
	Motor_Speed[1] = '\0';
	Serial_Init();
	Timer_Init();
	LED_Init();
	EXTI_Config();
	Check_Blue_Connect();
	
	Wait_Start();
	Process = 2;
	Constant_Temp();
	Centrifugal();

	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	TIM_Cmd(TIM1, DISABLE);        //关闭定时器
	TIM_SetCounter(TIM1, 0);
	LED_Status[0] = '1';
	LED_Status[1] = '\0';
	Fluorescence();
	End_Flag = 1;
	End();
	while(1);
}
void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		Num++;//定时累加
		if(Connect_Flag){
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==RESET){
				Delay_ms(5);
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==RESET){	
					NVIC_SystemReset();
				}
			}
		}//蓝牙自检
		switch(Process)
		{
			case 0:Flashing_Red();break;//红闪
			case 1:Solid_Green();break;//绿亮
			case 2:Flashing_Green();break;//绿闪
		}//状态切换
		
	}
}
