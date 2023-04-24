//#include "sys.h"
#include "usart.h"	  
#include "PID.h"

#if 1
#pragma import(__use_no_semihosting)             
//±ê×¼¿âÐèÒªµÄÖ§³Öº¯Êý                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//¶¨Òå_sys_exit()ÒÔ±ÜÃâÊ¹ÓÃ°ëÖ÷»úÄ£Ê½    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//ÖØ¶¨Òåfputcº¯Êý 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

void USART1_SendData(u8 data)
{
	while((USART1->SR&0X40)==0); 
	USART1->DR = data;
}

 
#if EN_USART1_RX   //Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
//×¢Òâ,¶ÁÈ¡USARTx->SRÄÜ±ÜÃâÄªÃûÆäÃîµÄ´íÎó   	
u8 USART_RX_BUF[USART_REC_LEN];     //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
//½ÓÊÕ×´Ì¬
//bit15£¬	½ÓÊÕÍê³É±êÖ¾
//bit14£¬	½ÓÊÕµ½0x0d
//bit13~0£¬	½ÓÊÕµ½µÄÓÐÐ§×Ö½ÚÊýÄ¿
u16 USART_RX_STA=0;       //½ÓÊÕ×´Ì¬±ê¼Ç	  
u16 USART_RX_LEN=0;
u16 USART_STA=0;
void uart_init(u32 bound){
  //GPIO¶Ë¿ÚÉèÖÃ
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//Ê¹ÄÜUSART1£¬GPIOAÊ±ÖÓ
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//¸´ÓÃÍÆÍìÊä³ö
  GPIO_Init(GPIOA, &GPIO_InitStructure);//³õÊ¼»¯GPIOA.9
   
  //USART1_RX	  GPIOA.10³õÊ¼»¯
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//¸¡¿ÕÊäÈë
  GPIO_Init(GPIOA, &GPIO_InitStructure);//³õÊ¼»¯GPIOA.10  

  //Usart1 NVIC ÅäÖÃ
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷
  
   //USART ³õÊ¼»¯ÉèÖÃ

	USART_InitStructure.USART_BaudRate = bound;//´®¿Ú²¨ÌØÂÊ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½

  USART_Init(USART1, &USART_InitStructure); //³õÊ¼»¯´®¿Ú1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//¿ªÆô´®¿Ú½ÓÊÜÖÐ¶Ï
  USART_Cmd(USART1, ENABLE);                    //Ê¹ÄÜ´®¿Ú1 
		USART1_SendData(0XFA);
		USART1_SendData(0XC6);
		USART1_SendData(0XC0);
}

void USART1_IRQHandler(void)                	//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //æŽ¥æ”¶ä¸­æ–­(æŽ¥æ”¶åˆ°çš„æ•°æ®å¿…é¡»æ˜¯0x0d 0x0aç»“å°¾
	{
		Res =USART_ReceiveData(USART1);	////è¯»å–æŽ¥æ”¶åˆ°çš„æ•°æ®
		if((USART_RX_STA&0x8000)==0)//æŽ¥æ”¶æœªå®Œæˆ
		{
			if(Res==0XFE)//æŽ¥æ”¶åˆ°å‘½ä»¤å¤´
			{
				USART_STA=1;
				USART_RX_LEN=0;
			}
		  if(USART_STA)
			{
				USART_RX_BUF[USART_RX_LEN]=Res;
				USART_RX_LEN++;
				if(USART_RX_LEN>8)////æŽ¥æ”¶8ä½æ•°æ®æŽ¥æ”¶å®Œæ¯•
				{
					USART_STA=0;
					USART_RX_STA|=0x8000;////æŽ¥æ”¶å·²å®Œæˆ
				}
			}
		}   		 
	}
	//pid.Pv=(float)USART_RX_BUF[2]+(float)(USART_RX_BUF[3])/100;
	//USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	//if(pid.Pv>60)pid.Pv=0;
} 
#endif	

