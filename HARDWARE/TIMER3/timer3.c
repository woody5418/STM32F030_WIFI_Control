//********************************************************************
//	 
//      代码功能：定时器3初始化代码 用于串口1超时判断   
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************


#include "includes.h"




// arr = 1000      psr = 7199
void TIM3_Init(uint32_t arr,uint16_t psr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//     TIM_Period = 1000 是1S
	TIM_TimeBaseStructure.TIM_Period = arr;  // 自动重装载寄存器周期的值(计数值) 
	TIM_TimeBaseStructure.TIM_Prescaler = psr;	//时钟预分频数 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;			//向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);			        // 清除溢出中断标志 
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		USART1_RX_STA|=1<<15;
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
		TIM_Cmd(TIM3, DISABLE);
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);				//清除中断标志位
}


