#ifndef __ADC_H
#define __ADC_H

#include "system_config.h"

/**
 * @brief ADC配置结构体
 * @details 存储ADC初始化所需的所有配置参数，包括GPIO端口、引脚和通道等信息
 */
typedef struct {
    GPIO_TypeDef* GPIOx;           /**< ADC使用的GPIO端口 */
    uint32_t GPIO_Clock;           /**< GPIO时钟使能位 */
    uint32_t GPIO_Pins;            /**< ADC使用的GPIO引脚 */
    uint8_t Channel_Num;           /**< ADC通道数量 */
    uint8_t Channels[16];          /**< ADC通道列表 */
    ADC_TypeDef* ADCx;             /**< ADC外设 */
    uint32_t ADC_Clock;            /**< ADC时钟使能位 */
    uint32_t ADC_Prescaler;         /**< ADC预分频值 */
} ADC_ConfigTypeDef;

extern uint16_t* ADC_ConvertedValue;  /**< 动态分配的ADC转换值数组 */
extern uint8_t ADC_ChannelCount;      /**< 当前ADC通道数量 */

/**
 * @brief 初始化ADC外设
 * @details 根据传入的配置结构体初始化ADC外设和相关GPIO
 * @param config 指向ADC配置结构体的指针
 * @return 初始化结果：0-成功，非0-失败
 */
uint8_t Adc_Init(ADC_ConfigTypeDef* config);

/**
 * @brief 获取指定通道的ADC值
 * @details 执行单次ADC转换并返回结果
 * @param channel 通道号
 * @return ADC转换结果
 */
uint16_t Get_Adc(uint8_t channel);

/**
 * @brief 获取指定通道的ADC平均值
 * @details 执行多次ADC转换并返回平均值
 * @param channel 通道号
 * @param times 采样次数
 * @return ADC转换平均值
 */
uint16_t Get_Adc_Average(uint8_t channel, uint8_t times);

/**
 * @brief 更新所有通道的ADC值
 * @details 遍历所有配置的通道并更新ADC值数组
 */
void Update_All_Adc_Values(void);

/**
 * @brief 释放ADC资源
 * @details 释放动态分配的内存并禁用ADC外设
 */
void Adc_DeInit(void);

#endif /* __ADC_H */
