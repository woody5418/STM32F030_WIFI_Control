/********************************************************************
 *@file        esp8266.h
 *@author      Woody      QQ：2490006131
 *@version     V1.0
 *@date        2017-07-26
 *@brief
 *******************************************************************/
#ifndef __ESP8266_H_
#define __ESP8266_H_
#include "stm32f0xx.h"
#include "stdbool.h"

/********************函数部分*********************/
bool Send_Order_RST(void);
bool Send_Search_Connect_AP(void);
bool Send_WiFi_Mode(void);
bool Send_Start_Airkiss(void);
bool Send_Stop_Airkiss(void);
bool Send_Set_Connect_TCP(void);
bool Send_CLOSE_TCP_UDP(void);
bool Send_Passthrough(void);
bool Send_Data_Passthrough(void);
bool Send_Request_header(void);
bool Send_Search_Local_MAC(void);
bool Quit_Order_TT(void);
/********************功能部分********************/
void f_to_string(char *buff_data,float values);
void f_to_string_gps(char *buff_data,float values);
char* checkCMD(char *str);
void sendCMD(char *cmd);
bool esp8266SendCMD(char *cmd,char *ack,uint16_t waittime);
char* Get_Search_Local_MAC(char *cmd,uint16_t waittime);
#endif



