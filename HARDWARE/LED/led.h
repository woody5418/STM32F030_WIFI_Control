/********************************************************************
 *@file        led.h
 *@author      Woody      QQ£º2490006131
 *@version     V1.0
 *@date        2017-07-26
 *@brief       
 *******************************************************************/
#ifndef __LED_H
#define	__LED_H

#include "stm32f0xx.h"


#define  OUTH   GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define  OUTL   GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define  Read_pin  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7)

//#define  LEDH   GPIO_SetBits(GPIOA,GPIO_Pin_3)
//#define  LEDL   GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define  LEDL   GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define  LEDH   GPIO_SetBits(GPIOA,GPIO_Pin_3)
void LED_Init(void);

#endif






