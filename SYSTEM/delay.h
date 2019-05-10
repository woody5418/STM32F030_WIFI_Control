/********************************************************************
 *@file        delay.h
 *@author      Woody      QQ£º2490006131
 *@version     V1.0
 *@date        2017-07-26
 *@brief       
 *******************************************************************/
#ifndef _DELAY_H_
#define _DELAY_H_
#include "stm32f0xx.h"

void delay_init(void);
void delay_us(int32_t nus);
void delay_ms(int16_t nms);

#endif





