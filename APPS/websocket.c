#include "includes.h"

/**
 * websocket包通过串口2发送到ESP8266
 */

void printfResult(char *message,int counter)
{
    int i;
    for (i = 0; i < counter; i++) {
        USART_SendData(USART1,message[i]);
        while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//等待发送结束
    }
}


/**
 * 发送websocket数据包
 */
void sendMessagePacket(char *message)
{
    char packet[300];
    unsigned long int length = 0;  // 数据包的总长度
    int counter = 0;  // 数据的坐标
    int i;

    WebSocketFrame webSocketFrame;  //web的 结构体
    webSocketFrame.fin = 1;  // 1 表示帧结束表示  0是上一帧
    webSocketFrame.rsvs = 0;  //RSV1 RSV2 RSV3  都是0
    webSocketFrame.opcode = 0x01;  // 0X01表示字符串
    webSocketFrame.masked = 1;     //是否使用掩码 1：使用  0：不使用
    webSocketFrame.payloadlen = strlen(message);   //要发送的消息长度
    webSocketFrame.maskkey[0] = 0x37;  //掩码
    webSocketFrame.maskkey[1] = 0xFA;  //掩码
    webSocketFrame.maskkey[2] = 0x21;  //掩码
    webSocketFrame.maskkey[3] = 0x3D;  //掩码

    length = webSocketFrame.payloadlen;  //求发送消息的长度

    packet[counter] = 0 | (webSocketFrame.fin << 7);   //计算fin位的字节存储到packet[0]
    packet[counter] |= (webSocketFrame.opcode);    //计算opcode位的字节存储到packet里
    counter++;
    packet[counter] = 0 | (webSocketFrame.masked << 7);  //掩码存储到packet里

    /**
    *如果数据的长度在0-125之间，就是payload的真实长度
    *如果数据的长度等于126小于65536，payloadlen=126，实际长度就是后面2个字节，是payload的真实长度
    *如果数据的长度大于65535，payloadlen=127，实际长度就是后面8个字节，是payload的真实长度
    */
    if (length < 126) { // 如果总长度小于126
        packet[counter] |= length;  // Payloadlength
        counter++;
        for (i = 0; i < 4; i++) {
            packet[counter] = webSocketFrame.maskkey[i];  //根据掩码计算 数据加密
            counter++;
        }
    } else if (length < 65536 && length > 125) {  //如果长度大于等于126并且小于65536
        packet[counter] |= 126;
        counter++;
        packet[counter] = (length >> 8) & 0xff;   //Payloadlength 如果是十进制的10000(0X2710)二进制位10011100010000 ，网络字节序要求先发送高八位，在发送低八位(0X1027)
        counter++;
        packet[counter] = length & 0xff;
        counter++;
        for (i = 0; i < 4; i++) {
            packet[counter] = webSocketFrame.maskkey[i];   //根据掩码计算 数据加密
            counter++;
        }
    }
    //原始数据第i个字节的值 XOR masking-key的第(i%4)个字节的值
    for (int i = 0; i < length; i++) {
        uint8_t temp = 0;
        temp = (message[i] ^ webSocketFrame.maskkey[i % 4]); //异或数据 根据掩码计算 数据加密
        packet[counter] = temp;
        counter++;
    }
    printfResult((char *)packet,counter); // 发送
}

