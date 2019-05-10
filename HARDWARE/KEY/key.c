//********************************************************************
//	 
//      代码功能：按键初始化代码	   
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************


#include "includes.h"




static uint8_t KeyNumFlag = 0; //按键计数函数



//PA8按键
void KeyInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* 配置KEY相应引脚PA4*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


// 按键PA4 上来输入   按下去3S
void KEY_SCAN(void)
{
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == Bit_RESET) { 
        delay_ms(500); // 去抖动
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == Bit_RESET) {
            KeyNumFlag++;
            if(KeyNumFlag == 6) {  //3s
                KeyNumFlag=0;
				LEDL;
                Control_Flag_Mode = 2;  //进入Airkiss模式
                NetNumber=0;
                LEDL;//1红灯
                delay_ms(230);
                LEDH;//0红灯
                delay_ms(230);
                LEDL; //1红灯
                delay_ms(230);
                LEDH;//0红灯
                delay_ms(230);
            }
        }
    }
    KeyNumFlag=0;
}





