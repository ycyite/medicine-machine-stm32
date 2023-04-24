/***************************************************************************************
$ @ Copyright    : 
$ @ Author       : Krone
$ @ Data         : Do not edit
$ @ LastEditor   : 
$ @ LastData     : 
$ @ Describe     : 
*****************************************************************************************/
#include "stm32f10x.h"                  // Device header

#ifndef MOTOR_H__
#define MOTOR_H__
#include <stdio.h>
#include <string.h>

#define  MOTOR_CLK               (RCC_APB2Periph_GPIOB)
#define  MOTOR_PORT              (GPIOB)
#define  MOTOR_IN1_PIN           (GPIO_Pin_12)
#define  MOTOR_IN2_PIN           (GPIO_Pin_13)
#define  MOTOR_IN3_PIN           (GPIO_Pin_14)
#define  MOTOR_IN4_PIN           (GPIO_Pin_15)

#define MOTOR_A_HIGH GPIO_SetBits(MOTOR_PORT,MOTOR_IN1_PIN)
#define MOTOR_A_LOW GPIO_ResetBits(MOTOR_PORT,MOTOR_IN1_PIN)
#define MOTOR_B_HIGH GPIO_SetBits(MOTOR_PORT,MOTOR_IN2_PIN)
#define MOTOR_B_LOW GPIO_ResetBits(MOTOR_PORT,MOTOR_IN2_PIN)
#define MOTOR_C_HIGH GPIO_SetBits(MOTOR_PORT,MOTOR_IN3_PIN)
#define MOTOR_C_LOW GPIO_ResetBits(MOTOR_PORT,MOTOR_IN3_PIN)
#define MOTOR_D_HIGH GPIO_SetBits(MOTOR_PORT,MOTOR_IN4_PIN)
#define MOTOR_D_LOW GPIO_ResetBits(MOTOR_PORT,MOTOR_IN4_PIN)

extern u8 DropFLAG;//下降中
extern u8 RiseFLAG;//上升中
extern u8 Bottom;//底部
extern u8 Top;//顶部

void Motor_Rhythm_4_1_4(u8 step,u8 dly);
void Motor_Direction(u8 dir,u8 diy);
void Motor_Rhythm_4_8(u8 step,u8 dly);
void Motor_Direction_Angle(u8 dir,u8 angle,u8 diy);

void Moto_Init(void);

void Setp_Motor_Stop(void);
void Setp_Motor_CW(u8 diy);
void Setp_Motor_CCW(u8 diy);
void Motor_Distance(u8 distance,u8 diy);
void Motor_Distance1(u8 distance,u8 diy);
void Moto_Init(void);

#endif

