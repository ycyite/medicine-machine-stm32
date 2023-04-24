#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "LCD.h"
#include "Delay.h"
unsigned char LCD_PORT;

void LCD_delay(void)
{
unsigned int i,j;
for(i=0;i<800;i++)
for(j=0;j<800;j++);
}

 


void LCD_Write_Com(unsigned char c)
{
u16 f;
register unsigned char i;
GPIO_SetBits(GPIOA,GPIO_Pin_7);
LCD_RS_L();
LCD_RW_H();
do
{
LCD_E_H();
f=LCD_FLAG();
LCD_E_L();
}while(f==1);

LCD_RW_L();


i=c;
i&=0xf0;
GPIO_Write(GPIOA,i|0);
LCD_E_H();
Delay_us(2);
LCD_E_L();


i=c<<4;
i&=0xf0;
GPIO_Write(GPIOA,i);
LCD_E_H();
Delay_us(2);
LCD_E_L();

Delay_ms(15);
}


void LCD_Write_Data(unsigned char d)
{
u16 f;
register unsigned char i;
GPIO_SetBits(GPIOA,GPIO_Pin_7);
LCD_RS_L();
LCD_RW_H();
do
{
LCD_E_H();
f=LCD_FLAG();
LCD_E_L();
}while(f==1);

LCD_RS_H();
LCD_RW_L();


i=d;
i&=0xf0;
GPIO_Write(GPIOA,i);
LCD_E_H();
Delay_us(2);
LCD_E_L();


i=(d<<4);
i&=0xf0;
GPIO_Write(GPIOA,i);
LCD_E_H();
Delay_us(2);
LCD_E_L();

Delay_ms(15);
}

 


void LCD_INIT(void)
{
Display_GPIO_Conf();
Delay_ms(15);
LCD_Write_Com(0x28);
LCD_Write_Com(0x01);
LCD_Write_Com(0x06);
LCD_Write_Com(0x0f);
}


void LCD_PUTS(unsigned char line,unsigned char col,unsigned char *s)
{
unsigned char i,addr;
if(line==1) addr=0x80;
else if(line==2) addr=0x80+0x40;
LCD_Write_Com(addr+col);
for(i=0;i<20;i++)
{
if(s[i]!=0)
LCD_Write_Data(s[i]);
else
break;
}
}

void Lcd1602_DisplayChar(u8 row,u8 col,unsigned char ch)
{
	//Дʾλ׃
	int add = 0;
	//Ɛ׏ˇŇһѐ
	if(row == 2)
	{
		//֚׾ѐք˗ַ֘ 0x40
		add += 0x40;
	}
	//֚һѐք˗ַ֘ 0x80
	add += 0x80 + col - 1;
	LCD_Write_Com(add); 
	//ДʾŚɝ	
	LCD_Write_Data(ch);	
}

/*****************主函数中的端口设置***********************/

void Display_GPIO_Conf(void)
{

GPIO_InitTypeDef GPIO_InitIntructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);

GPIO_InitIntructure.GPIO_Mode=GPIO_Mode_Out_PP;
GPIO_InitIntructure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_InitIntructure.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
GPIO_Init(GPIOC,&GPIO_InitIntructure);

GPIO_InitIntructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
GPIO_Init(GPIOA,&GPIO_InitIntructure);

GPIO_InitIntructure.GPIO_Pin=GPIO_Pin_7;
GPIO_InitIntructure.GPIO_Mode=GPIO_Mode_Out_OD;
GPIO_Init(GPIOA,&GPIO_InitIntructure);


}

