//********************************************************************
//
//      代码功能：ESP8266 JSON解析代码
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************

#include "includes.h"


uint8_t   FLAG_CJSON_SEND = 0;     // 定时器发送cJson控制变量

uint8_t   js_swout_status[2]="l";      //开关状态值
uint8_t   js_vout_status[2]="l";       //ad任务的后一级输出状态值
uint8_t   js_mode[2];      //模式控制
uint8_t   js_time[6];      //当前时间的转存数组 每次控制的时候都对齐进行时间校验
uint8_t   js_ad_value[4];  //ADC的设置值
uint8_t   js_repeat_t1[6];  //repeat的t1设置值
uint8_t   js_repeat_t2[6];  //repeat的t2设置值
uint8_t   js_timetask_t1[6];  //timetask的t1设置值
uint8_t   js_timetask_t2[6];  //timetask的t2设置值
uint8_t   js_timetask_week[2];  //timetask的循环周期数 周一到周日
uint8_t   js_timetask_num[3];  //timetask的设置值  第几个任务值
uint8_t   js_timetask_week_remove[3];  //计算周数发送使用
uint8_t   mode_status_value=0;     //模式的值
uint16_t  adc_status_value;      //前一级ADC值
uint16_t  set_adc_value;         //ADC的这是阈值
uint8_t   adtask_vout_value=0;     //ADC的vout状态
uint32_t  repeat_t1_value = 0;
uint32_t  repeat_t2_value = 0;
uint8_t   timetask_num_value = 0;  //第几个任务
uint32_t  FirstRECVTime = 0;     //第一次接受的话检测是否存在这个值如果不存在就报错 当前时间
uint8_t   FirstRECVWeek = 0;     //第一次接受的话检测是否存在这个值如果不存在就报错 周数
uint8_t   ADC_CP_status=0;    //ADC打开关闭脉冲模式
#define  OneDimensional 10  //一维数 50行
#define  TwoDimensional 4 //二位数 4列


uint32_t  timetaskbuf[OneDimensional][TwoDimensional]; //t1/t2/cycle/num 最大可以创建50个任务



char json_get_temp[200] = "";  //二级Json存储 数组
char json_temp[70] = "";       //三级Json存储 数组

char asciibuf[] = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };

char weekbuf[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

/**
 * 接收到串口1的ESPP8266数据
 * 判断是否为JSON，并且执行相应的功能
 */
void Apps_Analysis_Data(void)
{
    uint8_t jsFirstTime[7];  //第一次接收的时间
    uint8_t jsFirstWeek[2];  //第一次接收的周数

    if (USART1_RX_STA & 0x8000) {
        if(cJson_Pars_data() == 0x01) { //数据解析  控制
            json_get_ItemObjectFirst((char *)USART1_RX_BUF,"welcome",json_get_temp);
            if (json_check(json_get_temp)  == 1) { //合法 为0  不合法为1
                json_get_value((char *)json_get_temp,"time",(char *)jsFirstTime);
                json_get_value((char *)json_get_temp,"week",(char *)jsFirstWeek);
                FirstRECVTime = stringtoint((char *)jsFirstTime);//转化服务器连接以后打回的时间参数
                FirstRECVWeek = (uint8_t)(jsFirstWeek[0]&0x0f);//转化服务器连接以后打回的周参数
                //FirstRECVWeek =1~7
                FirstRECVWeek = weekbuf[FirstRECVWeek-1];
                NewWeekNum = FirstRECVWeek; //设置当前周数
            }
            memset(USART1_RX_BUF,0,300);
        }
        USART1_RX_STA=0;
    }
}

/**
 * 接收到串口1的ESPP8266数据
 * 判断是否为JSON，并且执行相应的功能
 * {"id":"SWTZ_Cantilever_a0:20:a6:29:a7:dc","to":"user","services":{"strength1":"0.01","strength2":"0.02"}}
*/
char cJson_Pars_data(void)
{
    uint32_t  Remove_timeweek_status=0;

    memset(json_get_temp,0,200);//清空数组 写0 大小1024Byte
    memset(json_temp,0,70);//清空数组 写0 大小1024Byte
    //获取子json
    json_get_ItemObject((char *)USART1_RX_BUF,"services",(char *)json_get_temp); //获取子json

    if (json_check(json_get_temp)  == 1) { //合法 为1  不合法为0
        json_get_value( (char *)json_get_temp, "mode", (char *)js_mode);   //模式的键值
        json_get_value( (char *)json_get_temp, "swout", (char *)js_swout_status); //开关的键值
        json_get_value( (char *)json_get_temp, "time", (char *)js_time);  //当前时间的转存数组 每次控制的时候都对齐进行时间校验

        ////解析adtask
        json_get_obj( (char *)json_get_temp, "adtask", (char *)json_temp);  //获取AD任务下的json
        json_get_value( (char *)json_temp, "value", (char *)js_ad_value);   //提取AD设置的value值
        json_get_value( (char *)json_temp, "vout", (char *)js_vout_status);     //提取AD设置的输出高低电平
        ////解析repeat
        json_get_obj( (char *)json_get_temp, "repeat", (char *)json_temp);  //获取AD任务下的json
        json_get_value( (char *)json_temp, "t1", (char *)js_repeat_t1);   //提取AD设置的value值
        json_get_value( (char *)json_temp, "t2", (char *)js_repeat_t2);     //提取AD设置的输出高低电平
        ////解析timetask
        json_get_obj( (char *)json_get_temp, "timetask", (char *)json_temp);  //获取AD任务下的json
        json_get_value( (char *)json_temp, "t1", (char *)js_timetask_t1);   //提取AD设置的value值
        json_get_value( (char *)json_temp, "t2", (char *)js_timetask_t2);     //提取AD设置的输出高低电平
        json_get_value( (char *)json_temp, "week", (char *)js_timetask_week);   //提取AD设置的value值
        json_get_value( (char *)json_temp, "num", (char *)js_timetask_num);     //提取AD设置的输出高低电平

        ////手动的直接控制
        if( json_check_value( (char *)js_swout_status,"h" ) == 1 )  //如果是高
            OUTH;  //后一级输出高
        else
            OUTL;  //后一级输出低

        //获取模式的值
        mode_status_value = *js_mode;          //获取模式值
        //获取当前时间 用于更新当前时间
        FirstRECVTime = stringtoint((char *)js_time);  //更新时间值

        //adtask的键值解析
        set_adc_value = (uint16_t)((float)(stringtoint((char *)js_ad_value))*30); //获取ADC设置值
        if( json_check_value( (char *)js_vout_status,"h" ) == 1 )  //如果是高
            adtask_vout_value = 1;//ADC的阈值输出状态“H”
        else
            adtask_vout_value = 0;//ADC的阈值输出状态“L”

        //repeat的键值解析
        repeat_t1_value = stringtoint((char *)js_repeat_t1);  //PWM脉冲模式的值  开启时间
        repeat_t2_value = stringtoint((char *)js_repeat_t2);  //PWM脉冲模式的值  关闭时间

        //timetask键值解析
        timetask_num_value = stringtoint((char *)js_timetask_num); //任务号
        //填二维数组的值
        if(json_check_value( (char *)js_timetask_week, "-1") != 1) {	//没有关闭
            timetaskbuf[timetask_num_value-1][0] = timetask_num_value;  //任务号
            if(js_timetask_week[0] <= 0x40) {
                Remove_timeweek_status = (uint32_t)((js_timetask_week[0] & 0x0f) << 4);
            } else {
                Remove_timeweek_status = (uint32_t)(asciibuf[(js_timetask_week[0] & 0x0f)-1] << 4);
            }
            if(js_timetask_week[1] <= 0x40) {
                Remove_timeweek_status |= (uint32_t)(js_timetask_week[1] & 0X0F);
            } else {
                Remove_timeweek_status |= (uint32_t)(asciibuf[(js_timetask_week[1] & 0X0F)-1]);
            }
            timetaskbuf[timetask_num_value-1][1] = Remove_timeweek_status;
            timetaskbuf[timetask_num_value-1][2] = stringtoint((char *)js_timetask_t1);   //定时任务的关闭时间
            timetaskbuf[timetask_num_value-1][3] = stringtoint((char *)js_timetask_t2);   //定时任务的启用时间
        } else {
            if(timetask_num_value == 0) timetask_num_value=1;
            timetaskbuf[timetask_num_value-1][0] = 0; //任务号
            timetaskbuf[timetask_num_value-1][1] = 0; //周数
            timetaskbuf[timetask_num_value-1][2] = 0; //定时任务的关闭时间
            timetaskbuf[timetask_num_value-1][3] = 0; //定时任务的启用时间
        }
        //控制完成以后，直接发送数据到服务器
        send_data_user();  //发送json到服务器
        return 0;
    } else {
        if(checkJSON("Connection: close")) {
            TIM_Cmd(TIM2, DISABLE);//数据任务关
            Control_Flag_Mode=0;
            Dis_Number = 10;  //发送请求头
        }
        //灯效果+
        GPIO_ResetBits(GPIOA, GPIO_Pin_3); //1红灯1
        delay_ms(40);
        GPIO_SetBits(GPIOA, GPIO_Pin_3);//1红灯1
        delay_ms(40);
        GPIO_ResetBits(GPIOA, GPIO_Pin_3); //1红灯1
        delay_ms(40);
        GPIO_SetBits(GPIOA, GPIO_Pin_3);//1红灯1
        delay_ms(40);
        return 1;
    }
}

/**
 * 处理收集到的数据
 * 流程控制
 */
void ControlProcessTime(uint8_t m)
{
    if(timetaskbuf[m][0] != 0) { //如果有任务号
        if((timetaskbuf[m][1] & NewWeekNum)||(timetaskbuf[m][1] & 0X80)) { //判断当前周是否成立
            if((timetaskbuf[m][2] <= FirstRECVTime)&&(timetaskbuf[m][3]>= FirstRECVTime)) { //开始时间
                if(ADC_CP_status == 1)
                    OUTH;  //后一级输出高
            } else if((timetaskbuf[m][3] <= FirstRECVTime)&&((timetaskbuf[m][3]+10) >= FirstRECVTime)) { //结束时间
                if(ADC_CP_status == 1) {
                    OUTL;  //后一级输出低
                    if(timetaskbuf[m][1] & 0X80) {
                        timetaskbuf[m][0] = 0;
                        mode_status_value &=0x7f;
                    }
                }
            }
        }
    }
}

void ControlProcessADC(void)
{
    adc_status_value = ADC_GetValue();  //获取AD的值
    if(adtask_vout_value == 1) { //输出高
        if(adc_status_value >= set_adc_value) {
            if((mode_status_value&0x04)||(mode_status_value&0x02)) { //是否存在重复任务或者定时任务
                ADC_CP_status=1;  //打开脉冲
            } else {
                OUTH;             //后一级输出高
            }
        } else {
            OUTL;  //后一级输出高
            ADC_CP_status=0;//关闭脉冲
        }
    } else {
        if(adc_status_value >= set_adc_value) {
            OUTL;  //后一级输出低
            ADC_CP_status=0;//关闭脉冲
        } else {
            if((mode_status_value&0x04)||(mode_status_value&0x02)) { //是否存在重复任务或者定时任务
                ADC_CP_status=1;//打开脉冲
            } else
                OUTH;  //后一级输出高
        }
    }
}

/**
 * 发送数据到服务器
 * 发送的{\"id\":\"SWTZ_WifiModule_a0:20:a6:29:a7:dc\",\"to\":\"user\",\"services\":{\"switch\":\"off\",\"adc\":\"4096\"}}
 * 接收到的
 */
void send_data_user(void)
{
    char PRemove[32];
    uint8_t buffs_num=0;
    uint8_t adc_mode_value=0;
    uint16_t ADC_send_value=0;
    uint16_t ADC_send_set=0;
    uint8_t packingData[300];        //最大800字节

    strcpy((char *) packingData, "{\"id\":\"SWTZ_WifiModule_");
    strcat((char *) packingData, macbufs);
    strcat((char *) packingData, "\",\"to\":\"user\",");

    strcat((char *) packingData, "\"debug\":{\"variable\":\"");
    sprintf((char *)PRemove,"%d,%d,%d",NewWeekNum,NetNumber,Dis_Number);
    strcat((char *) packingData, PRemove);
    strcat((char *) packingData, "\"},");
    strcat((char *) packingData, "\"services\":{\"swout\":\"");
    if((json_check_value((char *)js_swout_status,"h") !=1)&&(json_check_value((char *)js_swout_status,"l") !=1))   //L=l
        sprintf((char *)js_swout_status,"%s","l");
    if(Read_pin == SET ) //输出状态为高
        sprintf((char *)js_swout_status,"%s","h");
    else
        sprintf((char *)js_swout_status,"%s","l");
    strcat((char *) packingData, (char *)js_swout_status);  //自动控制开关on/off.;

    strcat((char *) packingData, "\",\"adc\":\"");
    ADC_send_value = ADC_GetValue()/30;
    if(ADC_send_value >= 100) ADC_send_value=100;
    if(mode_status_value&0x01) adc_mode_value=1;
    else adc_mode_value=0;
    ADC_send_set = set_adc_value/30;
    sprintf(PRemove, "%d,%d,%d,%d", adc_mode_value, ADC_send_value,adtask_vout_value,ADC_send_set);   // ADC值7
    strcat((char *) packingData, PRemove);

    if(mode_status_value&0x04)
        strcat((char *) packingData, "\",\"repeat\":\"1,");
    else  strcat((char *) packingData, "\",\"repeat\":\"0,");

    sprintf(PRemove, "%d,%d", repeat_t1_value, repeat_t2_value);   // ADC值
    strcat((char *) packingData, PRemove);

    strcat((char *) packingData, "\",\"timetask\":\"");
    buffs_num =(timetaskbuf[0][0] != 0)? 1:0;
    sprintf(PRemove,"%d,%2d,%d,%d#",buffs_num,timetaskbuf[0][1],timetaskbuf[0][2],timetaskbuf[0][3]);
    strcat((char *) packingData, PRemove);
    buffs_num =(timetaskbuf[1][0] != 0)? 1:0;
    sprintf(PRemove,"%d,%2d,%d,%d#",buffs_num,timetaskbuf[1][1],timetaskbuf[1][2],timetaskbuf[1][3]);
    strcat((char *) packingData, PRemove);
    buffs_num =(timetaskbuf[2][0] != 0)? 1:0;
    sprintf(PRemove,"%d,%2d,%d,%d#",buffs_num,timetaskbuf[2][1],timetaskbuf[2][2],timetaskbuf[2][3]);
    strcat((char *) packingData, PRemove);
    buffs_num =(timetaskbuf[3][0] != 0)? 1:0;
    sprintf(PRemove,"%d,%2d,%d,%d#",buffs_num,timetaskbuf[3][1],timetaskbuf[3][2],timetaskbuf[3][3]);
    strcat((char *) packingData, PRemove);
    buffs_num =(timetaskbuf[4][0] != 0)? 1:0;
    sprintf(PRemove,"%d,%2d,%d,%d#",buffs_num,timetaskbuf[4][1],timetaskbuf[4][2],timetaskbuf[4][3]);
    strcat((char *) packingData, PRemove);
    buffs_num =(timetaskbuf[5][0] != 0)? 1:0;
    sprintf(PRemove,"%d,%2d,%d,%d#",buffs_num,timetaskbuf[5][1],timetaskbuf[5][2],timetaskbuf[5][3]);
    strcat((char *) packingData, PRemove);
    buffs_num =(timetaskbuf[6][0] != 0)? 1:0;
    sprintf(PRemove,"%d,%2d,%d,%d#",buffs_num,timetaskbuf[6][1],timetaskbuf[6][2],timetaskbuf[6][3]);
    strcat((char *) packingData, PRemove);
    buffs_num =(timetaskbuf[7][0] != 0)? 1:0;
    sprintf(PRemove,"%d,%2d,%d,%d#",buffs_num,timetaskbuf[7][1],timetaskbuf[7][2],timetaskbuf[7][3]);
    strcat((char *) packingData, PRemove);
    buffs_num =(timetaskbuf[8][0] != 0)? 1:0;
    sprintf(PRemove,"%d,%2d,%d,%d#",buffs_num,timetaskbuf[8][1],timetaskbuf[8][2],timetaskbuf[8][3]);
    strcat((char *) packingData, PRemove);
    buffs_num =(timetaskbuf[9][0] != 0)? 1:0;
    sprintf(PRemove,"%d,%2d,%d,%d",buffs_num,timetaskbuf[9][1],timetaskbuf[9][2],timetaskbuf[9][3]);
    strcat((char *) packingData, PRemove);

    strcat((char *) packingData, "\"}}");
    sendMessagePacket((char *)packingData);
}


////////////********************** JSON  数据处理 ***************************////////////////

/***************************************
 name:       JSON 校验函数
 input:      字符串
 output:     合法JAON 返回1 不合法JSON 返回0
 description:
 ***************************************/
char json_check(char *str)
{
    uint16_t str_length;//定义字符长度变量
    str_length = strlen(str);//计算字符长度
    if (str[0] == '{' && str[str_length - 1] == '}') { //通过首尾大括号判断是否为JSON
        return 1;//如果字符串为合法JSON 返回1
    } else {
        return 0;//如果字符串为合法JSON 返回0
    }
}

/***************************************
 name:       JSON 获取键值函数
 input:      JSON字符串 要获取的键名 获取键值的字符串
 output:     如果获取成功返回1 获取失败返回0
 description:
 ***************************************/
char json_get_value(char *json, char *json_key, char *json_value)
{
    char *json_key_start;//定义键名开始的位置
    char *json_key_end;//定义键名结束的位置
    char json_key_length;//定义键名长度
    char *json_value_start;//定义键值开始的位置
    char *json_value_end;//定义键值结束的位置
    char json_value_length;//定义键值长度

    json_key_start = strstr(json, json_key);//获取键名开始的位置
    json_key_length = strlen(json_key);//获取键名的长度
    json_key_end = json_key_start + json_key_length;//获取键名结束的位置
    if (json_key_start != 0 && *(json_key_start - 1) == '\"'
        && *(json_key_end) == '\"' && *(json_key_end + 1) == ':'
        && *(json_key_end + 2) == '\"') {
        json_value_start = json_key_end + 3;//获取键值开始的位置
        json_value_end = strstr(json_value_start, "\""); //获取键值结束的位置
        json_value_length = json_value_end - json_value_start;//获取键值的长度
        strncpy(json_value, json_value_start, json_value_length);//将键值存入指针数组
        json_value[json_value_length] = '\0';//指针最后一位补字符串结束符 \0
        return 1;//成功获取键值 返回1
    } else {
        json_value[0] = '\0';
        return 0;//失败获取键值 返回0
    }
}

/***************************************
 name:      JSON 提取子JSON键值的值（一级提取）
 input:     JSON字符串 要获取的键名 获取键值的值
 output:    无
 description:
 ***************************************/
void json_get_ItemObject(char *json, char *json_key, char *json_value)
{
    int  json_len;
    uint16_t jsoncls_0=0;
    char *json_get_Item_obj;
    char *ptr_obj;

    while(json[jsoncls_0++] != '\0');
    json[jsoncls_0-1] = 0x30;
    json_get_Item_obj = strstr(json, json_key);
    json_get_Item_obj = strstr(json_get_Item_obj, "{");
    ptr_obj = strrchr(json_get_Item_obj, ',');
    json_len = strlen(json_get_Item_obj) - strlen(ptr_obj);
    strncpy(json_value, json_get_Item_obj, json_len);
    json_value[json_len] = '\0';
}

/***************************************
 name:      JSON 提取子JSON键值的值
 input:     JSON字符串 要获取的键名 获取键值的值
 output:    无
 description:
 ***************************************/
void json_get_ItemObjectFirst(char *json, char *json_key, char *json_value)
{
    int  json_len_Fir;
    char *json_get_Item_obj_Fir;
    char *ptr_obj_Fir;

    json_get_Item_obj_Fir = strstr(json, json_key);
    json_get_Item_obj_Fir = strstr(json_get_Item_obj_Fir, "{");
    ptr_obj_Fir = strrchr(json_get_Item_obj_Fir, '}');
    json_len_Fir = strlen(json_get_Item_obj_Fir) - strlen(ptr_obj_Fir);
    strncpy(json_value, json_get_Item_obj_Fir, json_len_Fir);
    json_value[json_len_Fir] = '\0';
}


/***************************************
 name:      JSON 提取子JSON(二级提取)
 input:     JSON字符串 要获取的键名 获取子json的串
 output:    无
 description:
 ***************************************/
void json_get_obj(char *json, char *json_key,char *json_objectItem)
{
    int json_len_t;
    char *json_get_temp_obj_t;
    char *ptr_t;

    json_get_temp_obj_t = strstr(json,json_key);
    json_get_temp_obj_t = strstr(json_get_temp_obj_t,"{");
    ptr_t = strchr(json_get_temp_obj_t,'}');
    json_len_t =strlen(json_get_temp_obj_t)-strlen(ptr_t)+1;
    strncpy(json_objectItem, json_get_temp_obj_t, json_len_t);
    json_objectItem[json_len_t] = '\0';
}


/***************************************
 name:       JSON 键值对比函数
 input:      JSON 键值 要匹配的字符
 output:     如果匹配成功返回1 失败返回0
 description:
 ***************************************/
char json_check_value(char *str1, char *str2)
{
    if (strcmp(str1, str2) == 0) {
        return 1;//匹配成功返回1
    } else {
        return 0;//匹配成功返回0
    }
}


/***************************************
 name:       string类型转int类型
 input:      存储string的参数
 output:     如果转换成功返回数值 失败返回0
 description:
 ***************************************/
uint32_t stringtoint(char *a)
{
    uint32_t val=0;
    uint8_t i=0;
    while(1) {
        if( *(a+i) == '\0' )break;
        else {
            if(*(a+i)<='9'&&*(a+i)>='0')
                val=(val*10)+(*(a+(i++))&0x0F);
            else
                return 0;
        }
    }
    return val;
}

//
char* checkJSON(char *str)
{
    char *strx=0;
    strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
    return (char*)strx;
}

