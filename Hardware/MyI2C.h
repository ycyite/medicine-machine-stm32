#ifndef __MYI2C_H
#define __MYI2C_H

#include "stm32f10x.h"                  // Device header

void MyI2C_Init(void);
uint8_t MyI2C_ReceiveAck(void);
void MyI2C_SendAck(uint8_t AckBit);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendByte(uint8_t Byte);
void MyI2C_Stop(void);
void MyI2C_Start(void);
uint8_t MyI2C_R_SDA(void);
void MyI2C_W_SDA(uint8_t BitValue);
void MyI2C_W_SCL(uint8_t BitValue);

void MyI2C_Mode(u8 addr);

#endif
