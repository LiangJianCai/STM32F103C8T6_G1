#include "key.h"

/*
************************************************************
*	�������ƣ�	Key_Init
*
*	�������ܣ�	������ʼ��
*
*	���������	keys: ������ʼ���ṹ������
*               key_num: ��������
*
*	���ز�����	��
*
*	˵    ����	
************************************************************
*/
void Key_Init(Key_InitTypeDef* keys, uint8_t key_num)
{
    GPIO_InitTypeDef gpio_initstruct;
    EXTI_InitTypeDef exti_initstruct;
    NVIC_InitTypeDef nvic_initstruct;
    uint8_t i;

    // ʹ�����а�����ʱ��
    uint32_t all_clocks = 0;
    for (i = 0; i < key_num; i++) {
        all_clocks |= keys[i].GPIO_CLK;
    }
    all_clocks |= RCC_APB2Periph_AFIO;
    RCC_APB2PeriphClockCmd(all_clocks, ENABLE);
	
    for (i = 0; i < key_num; i++) {
        // GPIO��ʼ��
        gpio_initstruct.GPIO_Pin = keys[i].GPIO_PIN;
        gpio_initstruct.GPIO_Mode = keys[i].GPIO_Mode;
        GPIO_Init(keys[i].GPIO_PORT, &gpio_initstruct);

        // ��ʼ���ⲿ�ж���
        GPIO_EXTILineConfig(keys[i].EXTI_PORT, keys[i].EXTI_PIN);

        // ��ʼ���ж�
        exti_initstruct.EXTI_Line = keys[i].EXTI_LINE;
        exti_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
        exti_initstruct.EXTI_Trigger = keys[i].Trigger;
        exti_initstruct.EXTI_LineCmd = ENABLE;
        EXTI_Init(&exti_initstruct);

        // ����NVIC
        nvic_initstruct.NVIC_IRQChannel = keys[i].EXTI_IRQn;
        nvic_initstruct.NVIC_IRQChannelPreemptionPriority = keys[i].NVIC_IRQChannelPreemptionPriority;
        nvic_initstruct.NVIC_IRQChannelSubPriority = keys[i].NVIC_IRQChannelSubPriority;
        nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&nvic_initstruct);
    }
}

/*
************************************************************
*	�������ƣ�	Key_Read
*
*	�������ܣ�	��ȡ����״̬
*
*	���������	GPIOx: GPIO�˿�
*               GPIO_Pin: GPIO����
*
*	���ز�����	����״̬
*
*	˵    ����	
************************************************************
*/
uint8_t Key_Read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return GPIO_ReadInputDataBit(GPIOx, GPIO_Pin);
}
