#include "beep.h"

/**
 * @brief  ��ʼ��������
 * @param  beep: �������ṹ��ָ��
 * @param  GPIOx: GPIO�˿�
 * @param  GPIO_Pin: GPIO����
 * @retval ��
 */
void Beep_Init(Beep_TypeDef* beep, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    GPIO_InitTypeDef GPIO_InitStructure;
    uint32_t RCC_APB2Periph_GPIOx = 0;
    
    // ����GPIO�˿�ȷ��RCCʱ��
    if (GPIOx == GPIOA) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOA;
    } else if (GPIOx == GPIOB) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOB;
    } else if (GPIOx == GPIOC) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOC;
    } else if (GPIOx == GPIOD) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOD;
    } else if (GPIOx == GPIOE) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOE;
    }
    
    // ʹ��GPIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);
    
    // ����GPIOΪ�������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
    
    // ��ʼ���������ṹ��
    beep->GPIOx = GPIOx;
    beep->GPIO_Pin = GPIO_Pin;
    beep->state = BEEP_OFF;
    
    // Ĭ�Ϲرշ�����
    GPIO_SetBits(GPIOx, GPIO_Pin);
}

/**
 * @brief  �򿪷�����
 * @param  beep: �������ṹ��ָ��
 * @retval ��
 */
void Beep_On(Beep_TypeDef* beep) {
    GPIO_ResetBits(beep->GPIOx, beep->GPIO_Pin);
    beep->state = BEEP_ON;
}

/**
 * @brief  �رշ�����
 * @param  beep: �������ṹ��ָ��
 * @retval ��
 */
void Beep_Off(Beep_TypeDef* beep) {
    GPIO_SetBits(beep->GPIOx, beep->GPIO_Pin);
    beep->state = BEEP_OFF;
}

/**
 * @brief  �л�������״̬
 * @param  beep: �������ṹ��ָ��
 * @retval ��
 */
void Beep_Toggle(Beep_TypeDef* beep) {
    if (beep->state == BEEP_ON) {
        Beep_Off(beep);
    } else {
        Beep_On(beep);
    }
}

/**
 * @brief  ��ȡ������״̬
 * @param  beep: �������ṹ��ָ��
 * @retval ������״̬
 */
Beep_State Beep_GetState(Beep_TypeDef* beep) {
    return beep->state;
}
