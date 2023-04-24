#include "stm32f10x.h"                  // Device header
#include "Timer.h"

/*
//比较值
u16 CCR1_Val = 32768;
u16 CCR2_Val = 16384;
u16 CCR3_Val = 8192;
u16 CCR4_Val = 4096;

void TIM2_PWM_Init( u16 arr, u16 psc )
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

        //使能时钟
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );

    //设置IO口
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//tim3ch1,ch2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//tim3ch3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    //设置中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    //设置定时器基本参数
    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                                //只有高级定时器需要设置，其他定时器可以不设置。
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM3, &TIM_TimeBaseInitStructure );

    //设置工作模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;                                        //设置定时器工作在翻转模式
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    //设置4个通道 比较寄存器值
    TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
    TIM_OC1Init( TIM3, &TIM_OCInitStructure );                                                        //TIM2_OC1   PA0

    TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
    TIM_OC2Init( TIM3, &TIM_OCInitStructure );                                                        //TIM2_OC2   PA1

    TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
    TIM_OC3Init( TIM3, &TIM_OCInitStructure );                                                        //TIM2_OC1   PA2

    //TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
    //TIM_OC4Init( TIM3, &TIM_OCInitStructure );                                                        //TIM2_OC1   PA3

    //禁止自动预装载
    TIM_OC1PreloadConfig( TIM3, TIM_OCPreload_Disable );
    TIM_OC2PreloadConfig( TIM3, TIM_OCPreload_Disable );
    TIM_OC3PreloadConfig( TIM3, TIM_OCPreload_Disable );
    //TIM_OC4PreloadConfig( TIM3, TIM_OCPreload_Disable );

    //使能定时器
    TIM_Cmd( TIM3, ENABLE );
    TIM_ITConfig( TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 , ENABLE );//各通道使能
}

//占空比 0 --- 100
u16 CCR1_dc = 20;
u16 CCR2_dc = 40;
u16 CCR3_dc = 60;
//u16 CCR4_dc = 80;

u32 capture = 0;
u8 flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0;
u16 setcap = 0;

void TIM3_IRQHandler( void )
{
    if( TIM_GetITStatus( TIM3, TIM_IT_CC1 ) != RESET )
    {
        TIM_ClearITPendingBit( TIM3, TIM_IT_CC1 );
        capture = TIM_GetCapture1( TIM3 );
        //设置占空比
        if( flag1 == 0 )
        {
            flag1 = 1;
            setcap = capture + ( u32 )CCR1_Val * CCR1_dc / 100;
        }
        else
        {
            flag1 = 0;
            setcap = capture + ( u32 )CCR1_Val  * ( 100 - CCR1_dc ) / 100;
        }
        TIM_SetCompare1( TIM3, setcap  );
    }

    if( TIM_GetITStatus( TIM3, TIM_IT_CC2 ) != RESET )
    {
        TIM_ClearITPendingBit( TIM3, TIM_IT_CC2 );
        capture = TIM_GetCapture2( TIM3 );
        if( flag2 == 0 )
        {
            flag2 = 1;
            setcap = capture + CCR2_Val * CCR2_dc / 100;
        }
        else
        {
            flag2 = 0;
            setcap = capture + CCR2_Val  * ( 100 - CCR2_dc ) / 100;
        }
        TIM_SetCompare2( TIM3, setcap );
    }


    if( TIM_GetITStatus( TIM3, TIM_IT_CC3 ) != RESET )
    {
        TIM_ClearITPendingBit( TIM3, TIM_IT_CC3 );
        capture = TIM_GetCapture3( TIM3 );

        if( flag3 == 0 )
        {
            flag3 = 1;
            setcap = capture + CCR3_Val * CCR3_dc / 100;
        }
        else
        {
            flag3 = 0;
            setcap = capture + CCR3_Val  * ( 100 - CCR3_dc ) / 100;
        }
        TIM_SetCompare3( TIM3, setcap );
    }

    if( TIM_GetITStatus( TIM3, TIM_IT_CC4 ) != RESET )
    {
        TIM_ClearITPendingBit( TIM3, TIM_IT_CC4 );
        capture = TIM_GetCapture4( TIM3 );

        if( flag4 == 0 )
        {
            flag4 = 1;
            setcap = capture + CCR4_Val * CCR4_dc / 100;
        }
        else
        {
            flag4 = 0;
            setcap = capture + CCR4_Val  * ( 100 - CCR4_dc ) / 100;
        }
        TIM_SetCompare4( TIM3, setcap );
    }

}*/
/*
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
void Pwm_Init(void)
{
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);     

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM3_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

	TIM_TimeBaseStructure.TIM_Period = 400-1; 
	TIM_TimeBaseStructure.TIM_Prescaler =3600-1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  
	TIM_Cmd(TIM3, ENABLE);
}

void Pwm_Disposition(int arr,int psc)
{
	TIM_Cmd(TIM3, DISABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);//更新中断函数
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_Cmd(TIM3, ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//更新中断函数
}*/
void Timer_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能
	
	TIM_TimeBaseStructure.TIM_Period = 2500 - 1; //设置自动重装载寄存器周期值
	TIM_TimeBaseStructure.TIM_Prescaler =28800 - 1;//设置预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//重复计数设置
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //参数初始化
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);//清中断标志位
	
	TIM_ITConfig(      //使能或者失能指定的TIM中断
    TIM1,            //TIM1
    TIM_IT_Update  | //TIM 更新中断源
    TIM_IT_Trigger,  //TIM 触发中断源 
    ENABLE  	     //使能
    );
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	   //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}




