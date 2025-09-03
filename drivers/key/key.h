#ifndef _KEY_H_
#define _KEY_H_

#include "system_config.h"


// ������ʼ���ṹ��
typedef struct {
    uint32_t GPIO_CLK;
    GPIO_TypeDef* GPIO_PORT;
    uint16_t GPIO_PIN;
    uint8_t EXTI_PORT;
    uint8_t EXTI_PIN;
    uint32_t EXTI_LINE;
    uint8_t EXTI_IRQn;
	uint8_t NVIC_IRQChannelPreemptionPriority;  // �����ȼ�
    uint8_t NVIC_IRQChannelSubPriority;         // �����ȼ�
    EXTITrigger_TypeDef Trigger;  				// ������ʽ��EXTI_Trigger_Rising �� EXTI_Trigger_Falling
    GPIOMode_TypeDef GPIO_Mode;  				// GPIOģʽ
} Key_InitTypeDef;

// ������ʼ������
void Key_Init(Key_InitTypeDef* keys, uint8_t key_num);

// ��ȡ����״̬
uint8_t Key_Read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
