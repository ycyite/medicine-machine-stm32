#include "stm32f10x.h"                  // Device header
#include "Temp.h"
#include "AD.h"

//模拟量转电阻
float temp_Get_R(u16 adct)
{
	//单片机3.3v 供电 ==》 3v，先求出电压
	float v1 = (float)(adct*3.3)/4095;//ntc上的电压
	float v2 = 3.3 - v1;
	float r = (v1/v2)*10;
	
	return r;
}
//实现ln(x) 的计算
double myln(double a)
{
   int N = 15;//取了前15+1项来估算
   int k,nk;
   double x,xx,y;
   x = (a-1)/(a+1);
   xx = x*x;
   nk = 2*N+1;
   y = 1.0/nk;
   for(k=N;k>0;k--)
   {
     nk = nk - 2;
     y = 1.0/nk+xx*y;
     
   }
   return 2.0*x*y;
}
 
float Get_Kelvin_Temperature(u16 t)
{
	float N1,N2,N3,N4;
	float Rntc = temp_Get_R(t);
	N1 = (myln(R25)-myln(Rntc))/B;
	N2 = 1/T25 - N1;
	N3 = 1/N2;
	N4 = N3-273.15;//开尔文转摄氏度
	
	return N4;
}
float Get_Kelvin_Temperature1(u16 t)
{
	float N1,N2,N3,N4;
	float Rntc = temp_Get_R(t);
	N1 = (myln(R25)-myln(Rntc))/B1;
	N2 = 1/T25 - N1;
	N3 = 1/N2;
	N4 = N3-273.15;//开尔文转摄氏度
	
	return N4;
}
