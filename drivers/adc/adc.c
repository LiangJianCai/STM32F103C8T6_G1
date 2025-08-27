#include "adc.h"
#include "delay.h"
#include <stdlib.h>

uint16_t* ADC_ConvertedValue = NULL;  /**< ��̬�����ADCת��ֵ���� */
uint8_t ADC_ChannelCount = 0;         /**< ��ǰADCͨ������ */
static uint8_t* ADC_ChannelMap = NULL;  /**< ͨ��ӳ��� */

/**
 * @brief ��ʼ��ADC����
 * @details ���ݴ�������ýṹ���ʼ��ADC��������GPIO
 * @param config ָ��ADC���ýṹ���ָ��
 * @return ��ʼ�������0-�ɹ�����0-ʧ��
 */
uint8_t Adc_Init(ADC_ConfigTypeDef* config)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ��֤���ò���
    if (config == NULL || config->Channel_Num == 0 || config->Channel_Num > 16) {
        return 1;  // ������Ч
    }
    
    // �ͷ�֮ǰ�������Դ������У�
    Adc_DeInit();
    
    // ʹ��GPIO��ADCʱ��
    RCC_APB2PeriphClockCmd(config->GPIO_Clock | config->ADC_Clock, ENABLE);
    
    // ����ADCԤ��Ƶ��
    RCC_ADCCLKConfig(config->ADC_Prescaler);
    
    // ����GPIOΪģ������
    GPIO_InitStructure.GPIO_Pin = config->GPIO_Pins;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(config->GPIOx, &GPIO_InitStructure);
    
    // ����ADC
    ADC_DeInit(config->ADCx);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;  // ����ת��һ��ͨ��
    ADC_Init(config->ADCx, &ADC_InitStructure);
    
    // ʹ��ADC
    ADC_Cmd(config->ADCx, ENABLE);
    
    // У׼ADC
    ADC_ResetCalibration(config->ADCx);
    while(ADC_GetResetCalibrationStatus(config->ADCx));
    ADC_StartCalibration(config->ADCx);
    while(ADC_GetCalibrationStatus(config->ADCx));
    
    // ����������Ϣ
    ADC_ChannelCount = config->Channel_Num;
    
    // ��̬�����ڴ�洢ADCֵ��ͨ��ӳ���
    ADC_ConvertedValue = (uint16_t*)malloc(ADC_ChannelCount * sizeof(uint16_t));
    ADC_ChannelMap = (uint8_t*)malloc(ADC_ChannelCount * sizeof(uint8_t));
    
    if (ADC_ConvertedValue == NULL || ADC_ChannelMap == NULL) {
        Adc_DeInit();
        return 2;  // �ڴ����ʧ��
    }
    
    // ��ʼ��ADCֵ�����ͨ��ӳ���
    for (uint8_t i = 0; i < ADC_ChannelCount; i++) {
        ADC_ConvertedValue[i] = 0;
        ADC_ChannelMap[i] = config->Channels[i];  // ����ͨ��ӳ��
    }
    
    return 0;  // ��ʼ���ɹ�
}

/**
 * @brief ��ȡָ��ͨ����ADCֵ
 * @details ִ�е���ADCת�������ؽ��
 * @param channel �߼�ͨ���ţ�0~Channel_Num-1��
 * @return ADCת�����
 */
uint16_t Get_Adc(uint8_t channel)
{
    if (channel >= ADC_ChannelCount || ADC_ChannelMap == NULL) {
        return 0;  // ��Чͨ��
    }
    
    // ͨ��ӳ����ȡʵ������ͨ��
    uint8_t physical_channel = ADC_ChannelMap[channel];
    
    // ����ADCͨ��
    ADC_RegularChannelConfig(ADC1, physical_channel, 1, ADC_SampleTime_239Cycles5);
    
    // ����ת��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // �ȴ�ת�����
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    
    // ����ת�����
    return ADC_GetConversionValue(ADC1);
}

/**
 * @brief ��ȡָ��ͨ����ADCƽ��ֵ
 * @details ִ�ж��ADCת��������ƽ��ֵ
 * @param channel �߼�ͨ���ţ�0~Channel_Num-1��
 * @param times ��������
 * @return ADCת��ƽ��ֵ
 */
uint16_t Get_Adc_Average(uint8_t channel, uint8_t times)
{
    uint32_t sum = 0;
    uint8_t valid_times = (times > 0) ? times : 1;
    
    for (uint8_t i = 0; i < valid_times; i++) {
        sum += Get_Adc(channel);
        DelayMs(1);  // �������
    }
    
    return (uint16_t)(sum / valid_times);
}

/**
 * @brief ��������ͨ����ADCֵ
 * @details �����������õ�ͨ��������ADCֵ����
 */
void Update_All_Adc_Values(void)
{
    for (uint8_t i = 0; i < ADC_ChannelCount; i++) {
        ADC_ConvertedValue[i] = Get_Adc_Average(i, 5);  // ʹ���߼�ͨ������
    }
}

/**
 * @brief �ͷ�ADC��Դ
 * @details �ͷŶ�̬������ڴ沢����ADC����
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
    
    // ����ADC
    ADC_Cmd(ADC1, DISABLE);
}

/*********************************************END OF FILE**********************/
