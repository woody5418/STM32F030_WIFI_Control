//********************************************************************
//
//      代码功能：ESP8266驱动库代码
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************


#include "includes.h"


char *connStr = "AT+CIPSTART=\"TCP\",\"4dclass.net\",80"; //TCP连接服务器 端口80
/************************请求头的内容**************************/

/************************目前不带BASE64编码********************/
char *projectname="SWTZ_WifiModule_"; //通讯ID，标识
char *info1 = "GET http://4dclass.net/WebSocket/chat/"; //通讯URL
char *info2 = " HTTP/1.1\r\nHost: 4dclass.net\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nOrigin: http://4dclass.net\r\nSec-WebSocket-Protocol: chat, superchat\r\nSec-WebSocket-Version: 13\r\nUser-Agent: ESP8266\r\n";
/************************请求头的内容**************************/
char macbuf[18];  //MAC地址存储地址
char *macbufs="FFFFFFFFFFFFF";  //MAC地址指针
/*************************************************************/

/**
 * 利用串口1向8266发送数据
 */
void sendCMD(char *cmd)
{
    u1_printf("%s\r\n",cmd);
}

/***********************************************************************/

/**
 * 重置8266，也做上电自检使用
 * 成功返回true，失败返回false
 */
bool Send_Order_RST(void)
{
    bool state = true;
    u1_printf("+++");
    delay_ms(500);
    delay_ms(500);
    u1_printf("\r\n");
    delay_ms(10);
    state = esp8266SendCMD("AT+RST","OK",20);
    return state;
}
/**
 * 退出透传
 * 成功返回true，失败返回false
 */
bool Quit_Order_TT(void)
{
    bool state = true;
    u1_printf("+++");
    delay_ms(500);
    delay_ms(500);
    u1_printf("\r\n");
    delay_ms(10);
    state = esp8266SendCMD("AT","OK",20);
    return state;
}
/**
 * 获取8266连接状态，是否有AP连接
 * 有返回true，没有返回false
 */
bool Send_Search_Connect_AP(void)
{
    bool state = true;
    state = esp8266SendCMD("AT+CIPSTATUS","STATUS:2",50);
    return state;
}

/**
 * 选择连接模式
 * 成功返回true，失败返回false
 */
bool Send_WiFi_Mode(void)
{
    bool state = true;
    state = esp8266SendCMD("AT+CWMODE_DEF=1","OK",20);
    return state;
}

/**
 * 选择Airkiss模式,等待返回连接
 * 成功返回true，失败返回false
 */
bool Send_Start_Airkiss(void)
{
    bool state = true;
    //state = esp8266SendCMD("AT+CWJAP_DEF=\"imaker\",\"Wlgf@001\"","OK",1000); //等待最长时间10S
    state = esp8266SendCMD("AT+CWSTARTSMART=2","smartconfig connected wifi",30000); //等待最长时间5min
    return state;
}

/**
 * 停止Airkiss选择连接模式
 * 成功返回true，失败返回false
 */
bool Send_Stop_Airkiss(void)
{
    bool state = true;
    state = esp8266SendCMD("AT+CWSTOPSMART","OK",20);
    return state;
}

/**
 * 创建TCP连接
 * 成功返回true，失败返回false
 */
bool Send_Set_Connect_TCP(void)
{
    bool state = true;
    state = esp8266SendCMD(connStr,"CONNECT",200);
    return state;
}

/**
 * 断开AP连接，断开TCP连接
 * 成功返回true，失败返回false
 */
bool Send_CLOSE_TCP_UDP(void)
{
    bool state = true;
    state = esp8266SendCMD("AT+CIPCLOSE","OK",20);
    return state;
}

/**
 * 选择透传模式
 * 成功返回true，失败返回false
 */
bool Send_Passthrough(void)
{
    bool state = true;
    state = esp8266SendCMD("AT+CIPMODE=1","OK",20);
    return state;
}
/**
 * 进入到透传
 * 成功返回true，失败返回false
 */
bool Send_Data_Passthrough(void)
{
    bool state = true;
    state = esp8266SendCMD("AT+CIPSEND","OK",20);
    return state;
}

/**
 * 发送请求头 等待回复OK
 */
bool Send_Request_header(void)
{
    bool state = true;
    char handShake[336];  //拼接请求头的空间
    //请求头 info1
    strcpy(handShake,info1);
    strcat(handShake,projectname);
    strcat(handShake,(char *)macbuf);
    strcat(handShake,info2);
    state = esp8266SendCMD(handShake,"OK",50);
    return state;
}

/**
 * 记录MAC地址
 * 成功返回true，失败返回false
 */
bool Send_Search_Local_MAC(void)
{
    bool state = true;
    macbufs = Get_Search_Local_MAC("AT+CIFSR",50);
    if(macbufs) state=true;
    else state = false;
    return state;
}

/**
 * 向ATK-ESP8266发送命令
 * cmd:发送的命令字符串
 * ack:期待的应答结果,如果为空,则表示不需要等待应答
 * waittime:等待时间(单位:10ms)
 * 返回值:true,发送成功(得到了期待的应答结果)
 *        false,发送失败
 */
bool esp8266SendCMD(char *cmd,char *ack,uint16_t waittime)
{
    uint8_t res=true;

    USART1_RX_STA=0;
    sendCMD(cmd);							//发送命令
    if(ack&&waittime) {				//需要等待应答
        while(--waittime) {		//等待倒计时
            delay_ms(10);
            if(USART1_RX_STA&0X8000) {		//接收到期待的应答结果
                if(checkCMD((char *)ack)) {
                    break;		//得到有效数据
                }
                USART1_RX_STA=0;
            }
        }
        if(waittime==0) {
            res=false;
        }
    }
    return res;
}
/**
 * ATK-ESP8266发送命令后,检测接收到的应答
 * str:期待的应答结果
 * 返回值:0,没有得到期待的应答结果
 *     其他,期待应答结果的位置(str的位置)
 */
char* checkCMD(char *str)
{
    char *strx=0;
    if(USART1_RX_STA&0X8000) { 		//接收到一次数据了
        USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;		//添加结束符
        strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
    }
    return (char*)strx;
}
/**
 * 向ATK-ESP8266发送命令
 * cmd:发送的命令字符串
 * waittime:等待时间(单位:10ms)
 * 返回值:mac地址，不带双引号
 */
char* Get_Search_Local_MAC(char *cmd,uint16_t waittime)
{
    uint8_t n=0;
    char *mac;

    USART1_RX_STA=0;
    sendCMD(cmd);		//发送命令
    if("STAMAC"&&waittime) {		//需要等待应答
        while(--waittime) {			//等待倒计时
            delay_ms(10);
            if(USART1_RX_STA&0X8000) {		//接收到期待的应答结果
                if(checkCMD("STAMAC")) {
                    mac =  strtok(checkCMD("STAMAC"),",");
                    mac =  strtok(NULL,",");
                    mac =  strtok(mac,"\r\n");
                    for(n=0; n<17; n++) {
                        macbuf[n] = mac[n+1];
                    }
                    macbuf[17] = '\0';
                }
                USART1_RX_STA=0;
            }
        }
        if(waittime==0) {
        }
    }
    return (char *)macbuf;
}





