#include "PID.h"

PID pid;

void PID_Init()
{
  	/*pid.Sv=43; //用户设定温度
	pid.Kp=30;
	pid.T=200;//PID计算周期200
  	pid.Ti=20000;//积分时间4000000
	pid.Td=1000;//微分时间1000,考察1000ms的变换率
	pid.pwmcycle=200;//pwm周期200
	pid.OUT0=1;//防止为0处于失控状态
	pid.C1ms=0;*/
	pid.Sv=43; //用户设定温度
	pid.Kp=30;//24
	pid.T=200;//PID计算周期200
  	pid.Ti=400000;//积分时间4000000
	pid.Td=1000;//微分时间1000,考察1000ms的变换率
	pid.pwmcycle=200;//pwm周期200
	pid.OUT0=1;//防止为0处于失控状态
	pid.C1ms=0;
}

void PID_Calc()  //pid计算
{
 	float DelEk;
	float ti,ki;
	float td;
	float kd;
	float out;
 	if(pid.C1ms<(pid.T))  //计算周期未到
 	{
    	return ;
 	}
 
 	
 
	//if(pid.Pv>30)
	//{
		pid.Ek=pid.Sv-pid.Pv;   //得到当前的偏差值
		pid.Pout=pid.Kp*pid.Ek;      //比例输出
		pid.SEk+=pid.Ek;        //历史偏差总和
 
		DelEk=pid.Ek-pid.Ek_1;  //最近两次偏差之差
	 
		ti=pid.T/pid.Ti;
		ki=ti*pid.Kp;

		pid.Iout=ki*pid.SEk;  //积分输出
		
		td=pid.Td/pid.T;
 
		kd=pid.Kp*td;
	 
		pid.Dout=kd*DelEk;    //微分输出
		
		pid.Ek_1=pid.Ek;  //更新偏差
	//}
 	/*else
	{
		pid.Pout=200;
		pid.Iout=0;
		pid.Dout=0;
	}*/
 	
 	out= pid.Pout+ pid.Iout+ pid.Dout;
	//out= pid.Pout;
 
 	if(out>pid.pwmcycle)
 	{
  		pid.OUT=pid.pwmcycle;
 	}
 	else if(out<=0)
 	{
			pid.OUT=pid.OUT0; 
 	}
 	else 
 	{
  		pid.OUT=out;
 	}
 	
 	pid.C1ms=0;
}
