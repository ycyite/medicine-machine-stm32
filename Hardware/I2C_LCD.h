#ifndef __I2C_LCD_H
#define __I2C_LCD_H
#include "stm32f10x.h"                  // Device header

void I2C_PCF8574T_WriteByte(u8 Byte);
void I2C_LCD_WriteCmd(u8 cmd);
void I2C_LCD_WriteData(u8 data);
void I2C_LCD_WriteInit(void);
void I2C_LCD_WriteWoder(u8 com,u8 num,u8 *dat);
void I2C_LCD_WriteNum(u8 com,u8 num);
void I2C_LCD_PUTS(unsigned char line,unsigned char col,unsigned char *s);
void I2C_LCD_WriteChar(u8 row,u8 col,unsigned char ch);
void I2C_LCD_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void I2C_LCD_WriteFloat(u8 L,u8 C,float F);

#endif
