//********************************************************************
//	 
//      代码功能：串口1初始化代码 使用定时器3做超时判断	   
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************

#include "includes.h"

uint16_t USART1_RX_STA = 0;       //接收状态标记
uint8_t  USART1_RX_BUF[USART1_REC_LEN];     //缓冲数组 最大接收64字节
__align(8) uint8_t USART1_TX_BUF[USART1_SEN_LEN];//发送缓冲,最大USART3_MAX_SEND_LEN字节

/* 
 * USART初始化 
 */
void usart1_Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //使能GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  //使能USART的时钟
	/* USART1的端口配置 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); //配置PA9成第二功能引脚	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1); //配置PA10成第二功能引脚  RX

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART1的基本配置 */
	USART_InitStructure.USART_BaudRate = baud;              //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
					USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);           //使能接收中断
	USART_Cmd(USART1, ENABLE);                             //使能USART1

	/* USART1的NVIC中断配置 */
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

}
//=============================================================================
//文件名称：
//功能概要：USART1中断函数，使用中断3
//参数说明：无
//函数返回：无
//=============================================================================
void USART1_IRQHandler(void)
{
	uint8_t res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		res =USART_ReceiveData(USART1);
        if((USART1_RX_STA&(1<<15))==0) {		//接收完的一批数据,还没有被处理,则不再接收其他数据
            if(USART1_RX_STA<USART1_REC_LEN) {		//还可以接收数据
                TIM_SetCounter(TIM3,0);			//计数器清空
                if(USART1_RX_STA==0)
                    TIM_Cmd(TIM3, ENABLE);  	//使能定时器3
                USART1_RX_BUF[USART1_RX_STA++]=res;	//记录接收到的值
            }else {
                USART1_RX_STA|=1<<15;		//强制标记接收完成
            }
        }
	}
}
/*
 * 串口1,printf 函数
 * 确保一次发送数据不超过 USART1_SEN_LEN 字节
 */
void u1_printf(char* fmt,...)
{
    uint16_t i,j;
    va_list ap;
    va_start(ap,fmt);
    vsprintf((char*)USART1_TX_BUF,fmt,ap);
    va_end(ap);
    i=strlen((const char*)USART1_TX_BUF);  //此次发送数据的长度
    for(j=0; j<i; j++) {																																		//循环发送数据
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);	//等待上次传输完成
        USART_SendData(USART1,(uint8_t)USART1_TX_BUF[j]);	//发送数据到串口3
    }
}






