/***************************************************************************************
$ @ Copyright    : 
$ @ Author       : Krone
$ @ Data         : Do not edit
$ @ LastEditor   : 
$ @ LastData     : 
$ @ Describe     : 
*****************************************************************************************/
#include "stm32f10x.h"                  // Device header
#include "motor.h"
#include "Delay.h"



/* IN4: PB9  d */
/* IN3: PB8  c */
/* IN2: PB7  b */ 
/* IN1: PB6  a */
u8 Top,Bottom,RiseFLAG,DropFLAG;
void Moto_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(MOTOR_CLK,ENABLE);

	 GPIO_InitStructure.GPIO_Pin = MOTOR_IN1_PIN | MOTOR_IN2_PIN | MOTOR_IN3_PIN | MOTOR_IN4_PIN ;//引脚按着INT1顺序接就行了
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB,&GPIO_InitStructure);
	 GPIO_ResetBits(MOTOR_PORT, MOTOR_IN1_PIN | MOTOR_IN2_PIN |MOTOR_IN3_PIN |MOTOR_IN4_PIN );
	
	Top=0;Bottom=1;RiseFLAG=0;DropFLAG=0;
}

void Setp1()
{
    MOTOR_A_LOW;
    MOTOR_B_LOW;
    MOTOR_C_LOW;
    MOTOR_D_HIGH;
}
void Setp2()
{
    MOTOR_A_LOW;
    MOTOR_B_HIGH;
    MOTOR_C_LOW;
    MOTOR_D_HIGH;
}
void Setp3()
{
    MOTOR_A_LOW;
    MOTOR_B_HIGH;
    MOTOR_C_LOW;
    MOTOR_D_LOW;
}
void Setp4()
{
    MOTOR_A_LOW;
    MOTOR_B_HIGH;
    MOTOR_C_HIGH;
    MOTOR_D_LOW;
}
void Setp5()
{
    MOTOR_A_LOW;
    MOTOR_B_LOW;
    MOTOR_C_HIGH;
    MOTOR_D_LOW;
}
void Setp6()
{
    MOTOR_A_HIGH;
    MOTOR_B_LOW;
    MOTOR_C_HIGH;
    MOTOR_D_LOW;
}
void Setp7()
{
    MOTOR_A_HIGH;
    MOTOR_B_LOW;
    MOTOR_C_LOW;
    MOTOR_D_LOW;
}
void Setp8()
{
    MOTOR_A_HIGH;
    MOTOR_B_LOW;
    MOTOR_C_LOW;
    MOTOR_D_HIGH;
}

void Setp_Motor_CCW(u8 diy)
{
    Setp1();
    Delay_ms(diy);
    Setp2();
    Delay_ms(diy);
    Setp3();
    Delay_ms(diy);
    Setp4();
    Delay_ms(diy);
    Setp5();
    Delay_ms(diy);
    Setp6();
    Delay_ms(diy);
    Setp7();
    Delay_ms(diy);
    Setp8();
    Delay_ms(diy);
}

/**********步经电机反转***************/
void Setp_Motor_CW(u8 diy)//下降
{
    Setp8();
    Delay_ms(diy);
    Setp7();
    Delay_ms(diy);
    Setp6();
    Delay_ms(diy);
    Setp5();
    Delay_ms(diy);
    Setp4();
    Delay_ms(diy);
    Setp3();
    Delay_ms(diy);
    Setp2();
    Delay_ms(diy);
    Setp1();
    Delay_ms(diy);
}
/**********步经电机Stop***************/
void Setp_Motor_Stop()
{
    MOTOR_A_LOW;
    MOTOR_B_LOW;
    MOTOR_C_LOW;
    MOTOR_D_LOW;
}

void Direction(u8 dir,u8 diy)//一拍0.025mm，每8拍走0.2mm
{
	if(dir)
	{
		Setp_Motor_CCW(diy);
	}
	else
	{
		Setp_Motor_CW(diy);
	}
}

void Motor_Distance(u8 distance,u8 diy)//上升 ,diy是频率用来控制转速,distance是距离
{
	 u8 FLAG=0;
	if(FLAG==0)
	{
		FLAG=1;
		for(u16 i=0;i<(distance*10);i++)
		{
			Setp_Motor_CCW(diy);
		}	
	}
}

void Motor_Distance1(u8 distance,u8 diy)//下降 ,diy是频率用来控制转速,distance是距离
{
	 u8 FLAG=0;
	if(FLAG==0)
	{
		FLAG=1;
		for(u16 i=0;i<(distance*10);i++)
		{
			Setp_Motor_CW(diy);
		}	
	}
}

void Motor_Reposition(void)//复位
{
	while(Bottom==0)//降到底部
	{
		if(RiseFLAG==1)//上升中
		{
			while(Top==0);//先上升到顶部
			Motor_Distance1(5,10);//再下降到底部
		}
	}
}

void Motor_Rhythm_4_1_4(u8 step,u8 dly)//4相单4拍dly是每一相转动的频率每次45度
{
	switch(step)
	{
		case 0: 
			;;
		break;
		case 1: 
			MOTOR_A_LOW;MOTOR_B_HIGH;MOTOR_C_HIGH;MOTOR_D_HIGH;
		break;
		case 2: 
			MOTOR_A_HIGH;MOTOR_B_LOW;MOTOR_C_HIGH;MOTOR_D_HIGH;
		break;
		case 3: 
			MOTOR_A_HIGH;MOTOR_B_HIGH;MOTOR_C_LOW;MOTOR_D_HIGH;
		break;
		case 4: 
			MOTOR_A_HIGH;MOTOR_B_HIGH;MOTOR_C_HIGH;MOTOR_D_LOW;
		break;
	}
	Delay_ms(dly);
}

void Motor_Direction(u8 dir,u8 diy)//转动1正转0反转 ,diy是频率
{
	if(dir)
	{
		for(u8 i=1;i<9;i++)
		{
			Motor_Rhythm_4_8(i,diy);
		}
	}
	else
	{
		for(u8 i=8;i>0;i--)
		{
			Motor_Rhythm_4_8(i,diy);
		}
	}
}

void Motor_Direction_Angle(u8 dir,u8 angle,u8 diy)//角度转动1正转0反转 ,diy是频率
{
	static u8 FLAG=0;
	if(FLAG==0)
	{
		FLAG=1;
		for(u16 i=0;i<(64*angle/45);i++)
		{
			Motor_Direction(dir,diy);
		}	
	}
}

/*void Motor_Rhythm_4_2_4(u8 step,u8 dly)//4相双4拍dly是每一相转动的频率
{
	switch(step)
	{
		case 0: 
			;;
		break;
		case 1: 
			MOTOR_A_LOW;MOTOR_B_HIGH;MOTOR_C_HIGH;MOTOR_D_LOW;
		break;
		case 2: 
			MOTOR_A_LOW;MOTOR_B_LOW;MOTOR_C_HIGH;MOTOR_D_HIGH;
		break;
		case 3: 
			MOTOR_A_HIGH;MOTOR_B_LOW;MOTOR_C_LOW;MOTOR_D_HIGH;
		break;
		case 4: 
			MOTOR_A_HIGH;MOTOR_B_HIGH;MOTOR_C_LOW;MOTOR_D_LOW;
		break;
	}
	Delay_ms(dly);
}*/
void Motor_Rhythm_4_8(u8 step,u8 dly)//4相八拍dly是每一相转动的频率每次90度
{
	switch(step)
	{
		case 0: 
			;;
		break;
		case 1: 
			MOTOR_A_LOW;MOTOR_B_HIGH;MOTOR_C_HIGH;MOTOR_D_HIGH;
		break;
		case 2: 
			MOTOR_A_LOW;MOTOR_B_LOW;MOTOR_C_HIGH;MOTOR_D_HIGH;
		break;
		case 3: 
			MOTOR_A_HIGH;MOTOR_B_LOW;MOTOR_C_HIGH;MOTOR_D_HIGH;
		break;
		case 4: 
			MOTOR_A_HIGH;MOTOR_B_LOW;MOTOR_C_LOW;MOTOR_D_HIGH;
		break;
		case 5: 
			MOTOR_A_HIGH;MOTOR_B_HIGH;MOTOR_C_LOW;MOTOR_D_HIGH;
		break;
		case 6: 
			MOTOR_A_HIGH;MOTOR_B_HIGH;MOTOR_C_LOW;MOTOR_D_LOW;
		break;
		case 7: 
			MOTOR_A_HIGH;MOTOR_B_HIGH;MOTOR_C_HIGH;MOTOR_D_LOW;
		break;
		case 8: 
			MOTOR_A_LOW;MOTOR_B_HIGH;MOTOR_C_HIGH;MOTOR_D_LOW;
		break;
	}
	Delay_ms(dly);
}







