#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void MyI2C_W_SCL(uint8_t BitValue)//I2C写，scl引脚翻转
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_8,(BitAction)BitValue);
	Delay_us(10);
}
void MyI2C_W_SDA(uint8_t BitValue)//I2C写，sda引脚翻转
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_9,(BitAction)BitValue);
	Delay_us(10);
}

uint8_t MyI2C_R_SDA(void)//I2C读
{
	uint8_t BitValue;
	BitValue=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9);
	Delay_us(10);
	return BitValue;
}


void MyI2C_Init(void)//软件i2c用gpio模拟输出
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//全配输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;//8是scl,9是sda
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void MyI2C_Mode(u8 addr)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(addr)//1 输出
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//全配输出
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//8是scl,9是sda
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	else//0输入
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//8是scl,9是sda
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	
	GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);
}

void MyI2C_Start(void)//设置I2C起始条件
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)//设置I2C终止条件
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)//发送一字节数据
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}
uint8_t MyI2C_ReceiveByte(void)//只有scl高电平，从机才能写入数据
{
	uint8_t i, Byte = 0x00;
	MyI2C_W_SDA(1);
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}
		MyI2C_W_SCL(0);
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)//主机发送应答
{
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}
uint8_t MyI2C_ReceiveAck(void)//从机发送接收应答
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}
