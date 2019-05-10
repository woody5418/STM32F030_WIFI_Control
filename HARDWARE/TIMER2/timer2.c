//********************************************************************
//	 
//      代码功能：定时器2初始化代码	   
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************
#include "includes.h"


uint8_t FLAG_LED_delay = 0; //控制LED时间的延时变量
uint8_t FLAG_delay_s = 0; //控制上传数据变量
static uint8_t delay_times = 0;  // 发送计时计数函数
static uint8_t led_flag=0;



// arr = 1000      psr = 7199
void TIM2_Init(uint32_t arr,uint16_t psr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	//     TIM_Period = 1000 是1S
	TIM_TimeBaseStructure.TIM_Period = arr;  // 自动重装载寄存器周期的值(计数值) 
	TIM_TimeBaseStructure.TIM_Prescaler = psr;	//时钟预分频数 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;			//向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);			        // 清除溢出中断标志 
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
		if(FLAG_delay_s == 1) {
            delay_times++;
            FLAG_LED_delay++;
            if(FLAG_LED_delay == 20) { //500ms
                FLAG_LED_delay=0;
				if(led_flag == 0)
				{
					led_flag=1;
					LEDL;
				}else{
					led_flag=0;
					LEDH;
				}
            }
            if(delay_times == 40) {   //2S
                delay_times = 0;
                FLAG_CJSON_SEND=1;
            }
        }
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);				//清除中断标志位
}


