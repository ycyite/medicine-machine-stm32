#include "RGB.h"
#include "stm32f10x.h"

void rgb_init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);

}

void set_rgb(int red_status, int green_status, int blue_status){
    GPIO_ResetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
    if(red_status == 1){
         GPIO_SetBits(GPIOA, GPIO_Pin_3);
    }
    if(green_status == 1){
         GPIO_SetBits(GPIOA, GPIO_Pin_4);
    }
    if(blue_status == 1){
         GPIO_SetBits(GPIOA, GPIO_Pin_5);
    }

}
