#include "adc.h"
#include "delay.h"
#include <stdlib.h>

uint16_t* ADC_ConvertedValue = NULL;  /**< 动态分配的ADC转换值数组 */
uint8_t ADC_ChannelCount = 0;         /**< 当前ADC通道数量 */
static uint8_t* ADC_ChannelMap = NULL;  /**< 通道映射表 */

/**
 * @brief 初始化ADC外设
 * @details 根据传入的配置结构体初始化ADC外设和相关GPIO
 * @param config 指向ADC配置结构体的指针
 * @return 初始化结果：0-成功，非0-失败
 */
uint8_t Adc_Init(ADC_ConfigTypeDef* config)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 验证配置参数
    if (config == NULL || config->Channel_Num == 0 || config->Channel_Num > 16) {
        return 1;  // 配置无效
    }
    
    // 释放之前分配的资源（如果有）
    Adc_DeInit();
    
    // 使能GPIO和ADC时钟
    RCC_APB2PeriphClockCmd(config->GPIO_Clock | config->ADC_Clock, ENABLE);
    
    // 配置ADC预分频器
    RCC_ADCCLKConfig(config->ADC_Prescaler);
    
    // 配置GPIO为模拟输入
    GPIO_InitStructure.GPIO_Pin = config->GPIO_Pins;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(config->GPIOx, &GPIO_InitStructure);
    
    // 配置ADC
    ADC_DeInit(config->ADCx);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;  // 单次转换一个通道
    ADC_Init(config->ADCx, &ADC_InitStructure);
    
    // 使能ADC
    ADC_Cmd(config->ADCx, ENABLE);
    
    // 校准ADC
    ADC_ResetCalibration(config->ADCx);
    while(ADC_GetResetCalibrationStatus(config->ADCx));
    ADC_StartCalibration(config->ADCx);
    while(ADC_GetCalibrationStatus(config->ADCx));
    
    // 保存配置信息
    ADC_ChannelCount = config->Channel_Num;
    
    // 动态分配内存存储ADC值和通道映射表
    ADC_ConvertedValue = (uint16_t*)malloc(ADC_ChannelCount * sizeof(uint16_t));
    ADC_ChannelMap = (uint8_t*)malloc(ADC_ChannelCount * sizeof(uint8_t));
    
    if (ADC_ConvertedValue == NULL || ADC_ChannelMap == NULL) {
        Adc_DeInit();
        return 2;  // 内存分配失败
    }
    
    // 初始化ADC值数组和通道映射表
    for (uint8_t i = 0; i < ADC_ChannelCount; i++) {
        ADC_ConvertedValue[i] = 0;
        ADC_ChannelMap[i] = config->Channels[i];  // 保存通道映射
    }
    
    return 0;  // 初始化成功
}

/**
 * @brief 获取指定通道的ADC值
 * @details 执行单次ADC转换并返回结果
 * @param channel 逻辑通道号（0~Channel_Num-1）
 * @return ADC转换结果
 */
uint16_t Get_Adc(uint8_t channel)
{
    if (channel >= ADC_ChannelCount || ADC_ChannelMap == NULL) {
        return 0;  // 无效通道
    }
    
    // 通过映射表获取实际物理通道
    uint8_t physical_channel = ADC_ChannelMap[channel];
    
    // 配置ADC通道
    ADC_RegularChannelConfig(ADC1, physical_channel, 1, ADC_SampleTime_239Cycles5);
    
    // 启动转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // 等待转换完成
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    
    // 返回转换结果
    return ADC_GetConversionValue(ADC1);
}

/**
 * @brief 获取指定通道的ADC平均值
 * @details 执行多次ADC转换并返回平均值
 * @param channel 逻辑通道号（0~Channel_Num-1）
 * @param times 采样次数
 * @return ADC转换平均值
 */
uint16_t Get_Adc_Average(uint8_t channel, uint8_t times)
{
    uint32_t sum = 0;
    uint8_t valid_times = (times > 0) ? times : 1;
    
    for (uint8_t i = 0; i < valid_times; i++) {
        sum += Get_Adc(channel);
        DelayMs(1);  // 采样间隔
    }
    
    return (uint16_t)(sum / valid_times);
}

/**
 * @brief 更新所有通道的ADC值
 * @details 遍历所有配置的通道并更新ADC值数组
 */
void Update_All_Adc_Values(void)
{
    for (uint8_t i = 0; i < ADC_ChannelCount; i++) {
        ADC_ConvertedValue[i] = Get_Adc_Average(i, 5);  // 使用逻辑通道索引
    }
}

/**
 * @brief 释放ADC资源
 * @details 释放动态分配的内存并禁用ADC外设
 */
void Adc_DeInit(void)
{
    if (ADC_ConvertedValue != NULL) {
        free(ADC_ConvertedValue);
        ADC_ConvertedValue = NULL;
    }
    
    if (ADC_ChannelMap != NULL) {
        free(ADC_ChannelMap);
        ADC_ChannelMap = NULL;
    }
    
    ADC_ChannelCount = 0;
    
    // 禁用ADC
    ADC_Cmd(ADC1, DISABLE);
}

/*********************************************END OF FILE**********************/
