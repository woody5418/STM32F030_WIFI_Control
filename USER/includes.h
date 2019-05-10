/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM32F030 Evaluation Board
*
* Filename      : includes.h
* Version       : V1.10
* Programmer(s) : BAN
*************************************************************************************************/

#ifndef __INCLUDES_H
#define __INCLUDES_H

#include "stm32f0xx.h"



extern char  *macbufs;             //mac 地址控制

extern uint8_t Dis_Number;         //初始化以及配网流程控制变量
extern uint8_t NetNumber;          //进入Airkiss控制流程变量
extern uint8_t Control_Flag_Mode;  // 控制模块模式切换
extern uint8_t FLAG_CJSON_SEND;    // 定时器发送cJson控制变量
extern uint8_t FLAG_delay_s;       //控制上传数据变量


extern uint32_t  FirstRECVTime;     //第一次接受的话检测是否存在这个值如果不存在就报错
extern uint8_t   FirstRECVWeek;     //第一次接受的话检测是否存在这个值如果不存在就报错
extern uint8_t   NewWeekNum;         //当前周数
extern uint8_t   mode_status_value; //模式的值
extern uint32_t  repeat_t1_value;
extern uint32_t  repeat_t2_value;
extern uint8_t   ADC_CP_status;     //ADC打开关闭脉冲模式
/*
 *常用头文件
 */
#include "delay.h"

/*
 *wifi部分头文件
 */
#include "websocket.h"
#include "esp8266.h"
#include "espjson.h"
#include "Airkiss.h"

/*
 *驱动库头文件
 */
#include "USART1.h"
#include "timer2.h"
#include "timer3.h"
#include "timer14.h"
#include "led.h"
#include "key.h"
#include "ADC1.h"

/*
 *C语言库头文件
 */
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include "stdbool.h"


#endif
/**********************************     END     **********************************/









