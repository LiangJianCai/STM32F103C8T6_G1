#ifndef __ADC_H
#define __ADC_H

#include "system_config.h"

/**
 * @brief ADC���ýṹ��
 * @details �洢ADC��ʼ��������������ò���������GPIO�˿ڡ����ź�ͨ������Ϣ
 */
typedef struct {
    GPIO_TypeDef* GPIOx;           /**< ADCʹ�õ�GPIO�˿� */
    uint32_t GPIO_Clock;           /**< GPIOʱ��ʹ��λ */
    uint32_t GPIO_Pins;            /**< ADCʹ�õ�GPIO���� */
    uint8_t Channel_Num;           /**< ADCͨ������ */
    uint8_t Channels[16];          /**< ADCͨ���б� */
    ADC_TypeDef* ADCx;             /**< ADC���� */
    uint32_t ADC_Clock;            /**< ADCʱ��ʹ��λ */
    uint32_t ADC_Prescaler;         /**< ADCԤ��Ƶֵ */
} ADC_ConfigTypeDef;

extern uint16_t* ADC_ConvertedValue;  /**< ��̬�����ADCת��ֵ���� */
extern uint8_t ADC_ChannelCount;      /**< ��ǰADCͨ������ */

/**
 * @brief ��ʼ��ADC����
 * @details ���ݴ�������ýṹ���ʼ��ADC��������GPIO
 * @param config ָ��ADC���ýṹ���ָ��
 * @return ��ʼ�������0-�ɹ�����0-ʧ��
 */
uint8_t Adc_Init(ADC_ConfigTypeDef* config);

/**
 * @brief ��ȡָ��ͨ����ADCֵ
 * @details ִ�е���ADCת�������ؽ��
 * @param channel ͨ����
 * @return ADCת�����
 */
uint16_t Get_Adc(uint8_t channel);

/**
 * @brief ��ȡָ��ͨ����ADCƽ��ֵ
 * @details ִ�ж��ADCת��������ƽ��ֵ
 * @param channel ͨ����
 * @param times ��������
 * @return ADCת��ƽ��ֵ
 */
uint16_t Get_Adc_Average(uint8_t channel, uint8_t times);

/**
 * @brief ��������ͨ����ADCֵ
 * @details �����������õ�ͨ��������ADCֵ����
 */
void Update_All_Adc_Values(void);

/**
 * @brief �ͷ�ADC��Դ
 * @details �ͷŶ�̬������ڴ沢����ADC����
 */
void Adc_DeInit(void);

#endif /* __ADC_H */
