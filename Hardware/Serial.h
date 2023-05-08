#include "stm32f10x.h"                  // Device header
#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>
#define SERIAL_PORT_RCC      RCC_APB2Periph_GPIOA
#define SERIAL_RX_PIN        GPIO_Pin_3
#define SERIAL_PORT          GPIOA
#define SERIAL_TX_PIN        GPIO_Pin_2

#define USART_RCC_APB        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
#define USART_I       	     USART2
#define USART_IRQN       	 USART2_IRQn
#define USART_IRQHANDLER(void)            USART2_IRQHandler(void)

extern uint8_t Serial_TxPacket[];//声明外部可调用数组
extern uint8_t Serial_RxPacket[];
extern uint8_t Serial_RxPacket1[];
extern char Motor_Speed[];
extern char Heat_Temp[];
extern char LED_Status[];
extern char Send_Flag_Motor;
extern char Send_Flag_Temp;
extern char Send_Flag_LED;
extern int Start_Flag; //开始标志
extern int End_Flag;
extern int Pause_Flag;
extern int Self_Flag;
extern int Sys_End_Flag;
extern int wait_time;
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);

void Serial_SendPacket(void);
uint8_t Serial_GetRxFlag(void);

void USART_SendString(char *str);
char USART_Receive(void);
void JDY31_SendString(char *str);

#endif
