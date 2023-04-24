
#include "HDL_WS2812B.h"
#include "Delay.h"	//延时函数加自己对应的头文件，在复位处使用

/* 单个灯珠的编码对应的比较值数组 */
uint16_t Single_LED_Buffer[DATA_SIZE*LED_NUM];

/*
*作用：通用定时器2 TIM2_CH2 PWM输出+DMA初始化 
*参数：arr：自动重装值
*说明：定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
*      Ft=定时器工作频率,单位:Mhz
*/
void PWM_WS2812B_Init(uint16_t arr)
{
	//结构体变量
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	//使能RCC时钟
	RCC_APB2PeriphClockCmd(WS2812B_PORT_RCC, ENABLE);		//使能PORTA时钟	
	RCC_APB1PeriphClockCmd(WS2812B_TIM_RCC,ENABLE);			//使能TIM2时钟    
	RCC_AHBPeriphClockCmd(WS2812B_DMA_RCC,ENABLE);				//使能DMA1时钟    
	
	//初始化GPIO口
	GPIO_InitStructure.GPIO_Pin = WS2812B_PIN;					//GPIO口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出                               
	GPIO_Init(WS2812B_PORT,&GPIO_InitStructure);				//根据指定的参数初始化GPIO口
	
	//初始化TIM2
	TIM_TimeBaseStructure.TIM_Prescaler=0;											//定时器分频：(0+1)=1,不分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		//向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;												//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;				//时钟分割
	TIM_TimeBaseInit(WS2812B_TIM,&TIM_TimeBaseStructure);							//根据指定的参数初始化TIM2
	
	//初始化TIM2 Channel2 PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 						//选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	//TIM_OCInitStructure.TIM_Pulse = 0;													//待装入捕获比较寄存器的脉冲值(此程序不用加初值)
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//输出极性:TIM输出比较极性高
	WS2812B_TIM_CH_INIT;  										//根据指定的参数初始化外设TIM2 Channel2
	/* 此处为知识点所描述处的对应代码，一定要有。移植时注意0C2代表定时器通道2*/
	WS2812B_TIM_CH_CONFIG;  						//使能TIM2在CCR2上的预装载寄存器
	
	TIM_Cmd(WS2812B_TIM, DISABLE);  									//失能TIM2，防止第一个脉冲异常
	
	TIM_DMACmd(WS2812B_TIM, WS2812B_TIM_CH, ENABLE);		//使能TIM2_CH2的DMA功能(CC2对应通道2)
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(WS2812B_DMA_CCR); 				//设置DMA目的地址
  	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Single_LED_Buffer;					//设置DMA源地址
 	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;													//方向：从内存SendBuff到内存ReceiveBuff  
 	DMA_InitStructure.DMA_BufferSize = LED_NUM*DATA_SIZE; 															//一次传输大小DMA_BufferSize=SendBuffSize   
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 						//ReceiveBuff地址不增
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;											//SendBuff地址自增
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//ReceiveBuff数据单位,16bit
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;					//SENDBUFF_SIZE数据单位,16bit
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;																//DMA模式：正常模式(传输一次)
  	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;												//优先级：中 
  	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 																//内存到内存的传输
  	DMA_Init(WS2812B_DMA_Channel, &DMA_InitStructure);																//配置DMA1的7通道(不同定时器的通道不一样)
    
  	DMA_Cmd(WS2812B_DMA_Channel, DISABLE);					//失能DMA1的7通道，因为一旦使能就开始传输
	//WS2812B_Reset();
	PWM_WS2812B_Black(16);//防止掉电保存16不知道为啥
}
//复位灯带
void WS2812B_Reset(void)
{
	TIM_Cmd(WS2812B_TIM, DISABLE);
	WS2812B_LOW;
	Delay_ms(1);	//此处延时时间最小值大于50us即可
}
//写数据编码
void PWM_WS2812B_Write_24Bits(uint16_t num,uint32_t GRB_Data)
{
  uint32_t i,j,h;h=0;
  for(j = 0; j < num; j++)
  {
  	for(i = 0; i < DATA_SIZE; i++)
	{
		/*因为数据发送的顺序是GRB，高位先发，所以从高位开始判断，判断后比较值先放入缓存数组*/
		Single_LED_Buffer[h] = ((GRB_Data << i) & 0x800000) ? T1H : T0H;
		h++;
	}
  }
}

//点亮灯珠
void PWM_WS2812B_Show(uint16_t num)
{
	//DMA_Cmd(DMA1_Channel7,DISABLE);//先失能DMA
	/* 移植时此处对应的通道和中断标志都需要更改 */
  	DMA_SetCurrDataCounter(WS2812B_DMA_Channel, num*DATA_SIZE);
  	DMA_Cmd(WS2812B_DMA_Channel, ENABLE);
  	TIM_Cmd(WS2812B_TIM, ENABLE);
  	while(DMA_GetFlagStatus(WS2812B_DMA_FLAG) != SET);
  	DMA_Cmd(WS2812B_DMA_Channel, DISABLE);
  	DMA_ClearFlag(WS2812B_DMA_FLAG);
  	TIM_Cmd(WS2812B_TIM, DISABLE);
}

//N个灯珠发红光
void PWM_WS2812B_Red(uint16_t num)
{
	PWM_WS2812B_Write_24Bits(num,0x00ff00);
	PWM_WS2812B_Show(num);
}
//N个灯珠发绿光
void PWM_WS2812B_Green(uint16_t num)
{
	PWM_WS2812B_Write_24Bits(num,0xff0000);
	PWM_WS2812B_Show(num);
}
//N个灯珠发蓝光
void PWM_WS2812B_Blue(uint16_t num,uint32_t L)
{
	PWM_WS2812B_Write_24Bits(num,L);
	PWM_WS2812B_Show(num);
}
void PWM_WS2812B_Black(uint16_t num)
{
	uint32_t i,j,h;h=0;
	for(j = 0; j < num; j++)
	{
		for(i = 0; i < DATA_SIZE; i++)
		{
			/*因为数据发送的顺序是GRB，高位先发，所以从高位开始判断，判断后比较值先放入缓存数组*/
			Single_LED_Buffer[h] = 30;
			h++;
		}
	}
	PWM_WS2812B_Show(num);
}
