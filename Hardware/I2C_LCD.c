#include "I2C_LCD.h"
#include "MyI2C.h"
#include "Delay.h"

#define ADDRESS 0x4E //pcf8574t地址A1,2,0都是高电平

void I2C_PCF8574T_WriteByte(u8 Byte)//发送数据给LCD
{
	MyI2C_Start();
	MyI2C_SendByte(ADDRESS);//写地址
	u8 i=MyI2C_ReceiveAck();//获取从机应答
	MyI2C_SendByte(Byte);//写数据
	i=MyI2C_ReceiveAck();//获取从机应答
	MyI2C_Stop();
}

void I2C_LCD_WriteCmd(u8 cmd)//写指令16*2 5*7 4bit
{
	u8 cmd1,cmd2;//接收
	//0x28 
	cmd1=cmd | 0x0f;//前四位
	I2C_PCF8574T_WriteByte(cmd1 & 0xfc);//1111 1100 背光=1 en=1 rw=0 rs=0
	Delay_us(5);
	I2C_PCF8574T_WriteByte(0xf8);//关闭使能
	
	cmd2=cmd<<4;//移动前4位
	cmd2=cmd2 | 0x0f;//前四位
	I2C_PCF8574T_WriteByte(cmd2 & 0xfc);//1111 1100 背光=1 en=1 rw=0 rs=0
	Delay_us(5);
	I2C_PCF8574T_WriteByte(0xf8);//关闭使能
	
}

void I2C_LCD_WriteData(u8 data)//写数据
{
	u8 data1,data2;//接收
	data1=data | 0x0f;//前四位
	I2C_PCF8574T_WriteByte(data1 & 0xfd);//1111 1100 背光=1 en=1 rw=0 rs=1
	Delay_us(5);
	I2C_PCF8574T_WriteByte(0xf9);//背光=1 en=0 rw=0 rs=1
	
	data2=data<<4;//移动前4位
	data2=data2 | 0x0f;//前四位
	I2C_PCF8574T_WriteByte(data2 & 0xfd);//1111 1100 背光=1 en=1 rw=0 rs=1
	Delay_us(5);                
	I2C_PCF8574T_WriteByte(0xf8);//背光=1 en=0 rw=0 rs=1
	
}

void I2C_LCD_WriteInit(void)
{
	uint32_t i, j;	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	MyI2C_Init();//软件i2c初始化
	
	I2C_LCD_WriteCmd(0x28);//设置16*2 5*7（4线初始化）
	I2C_LCD_WriteCmd(0x33);//设置显示（4线）
	I2C_LCD_WriteCmd(0x32);//设置控制（4线）
	
	I2C_LCD_WriteCmd(0x06);//地址加1 数据不移动，地址移动
	I2C_LCD_WriteCmd(0x0C);//光标不闪烁
	//I2C_LCD_WriteCmd(0x0f);//显示闪烁
	
	I2C_LCD_WriteCmd(0x80);//起始地址
	I2C_LCD_WriteCmd(0x01);//清屏
	Delay_ms(5);
}

void I2C_LCD_WriteWoder(u8 com,u8 num,u8 *dat)//显示字符数组，com地址，num字符长度
{
	I2C_LCD_WriteCmd(com);//写地址
	for(u8 i=0;i<num;i++)
	{
		I2C_LCD_WriteData(*dat++);//写数据
		Delay_ms(200);
	}
}

void I2C_LCD_PUTS(unsigned char line,unsigned char col,unsigned char *s)//显示字符数组
{
unsigned char i,addr;
if(line==1) addr=0x80;
else if(line==2) addr=0x80+0x40;
I2C_LCD_WriteCmd(addr+col);
for(i=0;i<20;i++)
{
if(s[i]!=0)
I2C_LCD_WriteData(s[i]);
else
break;
}
}

void I2C_LCD_WriteChar(u8 row,u8 col,unsigned char ch)//显示单个字符
{
	int add = 0;
	if(row == 2)
	{
		add += 0x40;
	}
	add += 0x80 + col - 1;
	I2C_LCD_WriteCmd(add); 	
	I2C_LCD_WriteData(ch);	
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t I2C_LCD_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

void I2C_LCD_WriteFloat(u8 L,u8 C,float F)
{
	int temp3=F*100;
	I2C_LCD_WriteChar(L,C,temp3/1000+'0');//红外温度
	I2C_LCD_WriteChar(L,C+1,temp3/100%10+'0');
	I2C_LCD_WriteChar(L,C+2,'.');
	I2C_LCD_WriteChar(L,C+3,temp3/10%10+'0');
	I2C_LCD_WriteChar(L,C+4,temp3%10+'0');
}

void I2C_LCD_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)//显示16进制数
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / I2C_LCD_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			I2C_LCD_WriteChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			I2C_LCD_WriteChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

void I2C_LCD_WriteNum(u8 com,u8 num)//显示数字，com地址，num字符长度
{
	u8 Num[]={"0123456789"};
	I2C_LCD_WriteCmd(com);//写地址
	I2C_LCD_WriteData(Num[num/100]);//百
	I2C_LCD_WriteData(Num[num%100/10]);//十
	I2C_LCD_WriteData(Num[num%10]);//个
}


