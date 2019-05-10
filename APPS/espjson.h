/********************************************************************
 *@file        espjson.h
 *@author      Woody      QQ：2490006131
 *@version     V1.0
 *@date        2017-07-26
 *@brief
 *******************************************************************/
#ifndef __ESPJSON_H
#define __ESPJSON_H

#include "stm32f0xx.h"
#include <stdint.h>
#include "usart1.h"


/*-----------------------CJSON----------------------*/
//JSON 校验函数
char json_check(char *str);
char json_get_value(char *json, char *json_key, char *json_value);
char json_check_value(char *str1, char *str2);
void json_get_ItemObject(char *json, char *json_key, char *json_value);
void json_get_ItemObjectFirst(char *json, char *json_key, char *json_value);
void json_get_obj(char *json, char *json_key,char *json_objectItem);
double atof(const char *nptr);
char Received_GPS_DATA(char *weidu_value, char *jingdu_value);
void f_to_string(char *buff_data,float values);
void f_to_string_gps(char *buff_data,float values);
/*----------------------- 其他 ----------------------*/
char cJson_Pars_data(void);
//void Data_Read(void);
void Automatic_Flow_Control(void);
void Apps_Analysis_Data(void);
void send_data_user(void);
void send_data_sd_card(void);
void  Body_Pyroelectric(void);
void ControlProcessTime(uint8_t m);
void ControlProcessADC(void);
uint32_t stringtoint(char *a);
char* checkJSON(char *str);
#endif




