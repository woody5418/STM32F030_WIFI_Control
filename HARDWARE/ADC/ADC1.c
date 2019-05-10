//********************************************************************
//	 
//      代码功能：ADC初始化代码	   
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************
#include "includes.h"


void ADC1_Init(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	ADC_InitTypeDef     ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
		
	ADC_StructInit(&ADC_InitStructure);//初始化ADC结构
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位精度
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对其为右对齐
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward; //ADC的扫描方向
    ADC_Init(ADC1, &ADC_InitStructure); 
	 
    ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_239_5Cycles);
	
	ADC_GetCalibrationFactor(ADC1);//校准
    ADC_Cmd(ADC1, ENABLE);         //使能			
}

uint16_t ADC_GetValue(void)
{
	uint16_t adcvalue=0;
	
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_ADEN) == RESET);
			ADC_StartOfConversion(ADC1); 
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);  //等待ADC完成 
			adcvalue = ADC_GetConversionValue(ADC1);
	return adcvalue;
}

