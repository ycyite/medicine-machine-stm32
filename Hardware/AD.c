#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//开启adc时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//分频，ADCCLK=72m/6
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ain是adc专属模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//A0接带线热敏电阻，A1接热敏电阻模块
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);//配置adc通道，那个adc,那个通道，次序（第一个位置），多少采样周期
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;//单次转换，转换模式
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//数据对齐，右对齐
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//外部触发源，选择软件触发
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//单adc工作模式，单adc和多adc
	ADC_InitStructure.ADC_NbrOfChannel=1;//通道数目
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;//单次,非扫描模式，扫描模式
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);//开启adc
	
	ADC_ResetCalibration(ADC1);//复位校准
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//等待复位校准
	ADC_StartCalibration(ADC1);//开始校准
	while(ADC_GetCalibrationStatus(ADC1)==SET);//等待校准完成
	
}

uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件触发,开始转换
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);//等待规则组转换完成
	return ADC_GetConversionValue(ADC1);//获取值
}
