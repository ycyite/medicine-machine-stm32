#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "Serial.h"
#include <stdarg.h>
#include "core_cm3.h"
#include "Delay.h"
uint8_t Serial_TxPacket[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};				//FF 01 02 03 04 FE
uint8_t Serial_RxPacket[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//存储发送或接收的数据不存包头包尾
uint8_t Serial_RxPacket1[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//输出数组用于显示，做缓冲
uint8_t Serial_RxFlag;//数据包接收位
char Motor_Speed[5] = {0};
char Heat_Temp[6] = {0};
char LED_Status[2] = {0};
char Send_Flag_Motor = '1';
char Send_Flag_Temp = '2';
char Send_Flag_LED = '3';
int Start_Flag = 0;
int End_Flag = 0;
int Pause_Flag = 0;
int Self_Flag = 0;
int Sys_End_Flag = 0;
int wait_time = 0;
void Serial_Init(void)
{
	USART_RCC_APB;
	RCC_APB2PeriphClockCmd(SERIAL_PORT_RCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = SERIAL_TX_PIN;//引脚输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SERIAL_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = SERIAL_RX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SERIAL_PORT, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//双工tx输出，RX输入
	USART_InitStructure.USART_Parity = USART_Parity_No;//不校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位字长
	USART_Init(USART_I, &USART_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART_IRQN;//中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART_I, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART_I, ENABLE);//使能
}

void Serial_SendByte(uint8_t Byte)//从TX引脚发送一个数据
{
	USART_SendData(USART_I, Byte);//把Byte变量写入到TDR
	while (USART_GetFlagStatus(USART_I, USART_FLAG_TXE) == RESET);//等待标注位，等待移位完成
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)//发送一个数组
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)//发送字符串
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)//x的y次方
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)//发送数字
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}


void Serial_Printf(char *format, ...)//对sprintf函数进行封装 
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}


void Serial_SendPacket(void)//发送一组数据自动添加包头尾
{
	Serial_SendByte(0xFF);
	Serial_SendArray(Serial_TxPacket, 8);
	Serial_SendByte(0xFE);
}

uint8_t Serial_GetRxFlag(void)//读后自动清除flag标志位
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}
void USART_IRQHANDLER(void){
	if (USART_GetITStatus(USART_I, USART_IT_RXNE) == SET){
		char RxData = USART_ReceiveData(USART_I);//读完数据自动清除标志位
		if(End_Flag == 1){
			wait_time = 0;
			if(RxData == 'p'){
				NVIC_SystemReset();
			}
			USART_SendString("111,");
		}
		else if(RxData == 'l'){
			wait_time = 0;
			if(Start_Flag == 0){
				Start_Flag = 1;
			}
			if(Pause_Flag == 1){
				Pause_Flag = 0;
			}
			USART_SendString(Heat_Temp);
			USART_SendString(",");
			USART_SendString(Motor_Speed);
			USART_SendString(",");
			USART_SendString(LED_Status);
		}else if(RxData == 'p'){
			wait_time = 0;
			NVIC_SystemReset();
		}
	}
	
	
}

void USART_SendString(char *str) {
    while (*str) {
        while (USART_GetFlagStatus(USART_I, USART_FLAG_TXE) == RESET);
        USART_SendData(USART_I, *str++);
    }
	
}

char USART_Receive(void) {
    while (USART_GetFlagStatus(USART_I, USART_FLAG_RXNE) == RESET);
    return USART_ReceiveData(USART_I);
}

void JDY31_SendString(char *str) {
    USART_SendString(str);
    Delay_ms(500);
}
