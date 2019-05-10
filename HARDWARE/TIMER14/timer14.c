//********************************************************************
//	 
//      代码功能：定时器14初始化代码 （1s定时）	   
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************
#include "includes.h"


uint8_t NewWeekNum = 0x01;  //当前周数
uint32_t NewTimeH = 0;   //当前pwm计数值
uint32_t NewTimeL = 0;   //当前pwm计数值

void TIM14_Init(uint32_t arr,uint16_t psr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr;  // 自动重装载寄存器周期的值(计数值) 
	TIM_TimeBaseStructure.TIM_Prescaler = psr;	//时钟预分频数 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;			//向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM14, TIM_FLAG_Update);			        // 清除溢出中断标志 
	TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM14, ENABLE);
}

void TIM14_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM14, TIM_FLAG_Update);
/********************************     自动控制     ********************************/
  /***************************** 控制时间只有一分钟 *****************************/
		FirstRECVTime++; //秒针
		if(FirstRECVTime >= 86400)  //等于24H的时候清零0
		{
			FirstRECVTime=0;  //重新开始新的一天
			NewWeekNum = (NewWeekNum<<1);
			if((NewWeekNum &0x80) != 0 )   //周
				NewWeekNum = 0x01;
		}
		if((mode_status_value&0x01)==0)//不存在AD任务
		{
			ADC_CP_status=1; //打开脉冲模式
		}
		if(mode_status_value&0x04)
		{
			if(NewTimeL==0)
				NewTimeH++;
			if(NewTimeH == 0) 
				NewTimeL++;
			
			if(NewTimeH >= repeat_t1_value)
			{
				NewTimeH=0;
				NewTimeL=1;
				if(ADC_CP_status == 1)
					OUTH;  //后一级输出高
			}
			if(NewTimeL >= repeat_t2_value)
			{
				NewTimeL=0;
				if(ADC_CP_status == 1)
					OUTL;  //后一级输出高
			}
		}else
		{
			NewTimeL=0;
			NewTimeH=0;
		}
	}
	TIM_ClearITPendingBit(TIM14, TIM_FLAG_Update);
}


