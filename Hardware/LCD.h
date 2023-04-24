#include "stm32f10x.h"  
//#include "io_bit.h"
#ifndef __LCD_H
#define __LCD_H


#define LCD_port_rcc   RCC_APB2Periph_GPIOC
#define LCD_port       GPIOC              
#define LCD_RS         GPIO_Pin_13         
#define LCD_RW         GPIO_Pin_14         
#define LCD_EN         GPIO_Pin_15     

#define LCD_E_H()		GPIO_SetBits(LCD_port,LCD_EN)
#define LCD_E_L()		GPIO_ResetBits(LCD_port,LCD_EN)
#define LCD_RS_H()		GPIO_SetBits(LCD_port,LCD_RS)
#define LCD_RS_L()		GPIO_ResetBits(LCD_port,LCD_RS)
#define LCD_RW_H()		GPIO_SetBits(LCD_port,LCD_RW)
#define LCD_RW_L()		GPIO_ResetBits(LCD_port,LCD_RW)
#define LCD_FLAG()		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)


void Display_GPIO_Conf(void);
void LCD_PUTS(unsigned char line,unsigned char col,unsigned char *s);
void LCD_INIT(void);
void LCD_Write_Data(unsigned char d);
void LCD_Write_Com(unsigned char c);
void LCD_delay(void);

void Lcd1602_DisplayChar(u8 row,u8 col,unsigned char ch);

#endif
