#include "stm32f10x.h"                  // Device header

#ifndef __Temp_H
#define __Temp_H

#define T25 298.15    //电压转温度的公式的采用
#define R25 10
#define B	3950
#define B1	3435

float temp_Get_R(u16 adct);
double myln(double a);
float Get_Kelvin_Temperature(u16 t);
float Get_Kelvin_Temperature1(u16 t);
#endif
