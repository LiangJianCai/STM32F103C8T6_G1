#include "led.h"

/**
 * @brief  ��ʼ��LED
 * @param  led: LED�ṹ��ָ��
 * @param  GPIOx: GPIO�˿�
 * @param  GPIO_Pin: GPIO����
 * @retval ��
 */
void LED_Init(LED_TypeDef* led, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
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
    
    // ��ʼ��LED�ṹ��
    led->GPIOx = GPIOx;
    led->GPIO_Pin = GPIO_Pin;
    led->state = LED_OFF;
    
    // Ĭ�Ϲر�LED
    GPIO_SetBits(GPIOx, GPIO_Pin);
}

/**
 * @brief  ��LED
 * @param  led: LED�ṹ��ָ��
 * @retval ��
 */
void LED_On(LED_TypeDef* led) {
    GPIO_ResetBits(led->GPIOx, led->GPIO_Pin);
    led->state = LED_ON;
}

/**
 * @brief  �ر�LED
 * @param  led: LED�ṹ��ָ��
 * @retval ��
 */
void LED_Off(LED_TypeDef* led) {
    GPIO_SetBits(led->GPIOx, led->GPIO_Pin);
    led->state = LED_OFF;
}

/**
 * @brief  �л�LED״̬
 * @param  led: LED�ṹ��ָ��
 * @retval ��
 */
void LED_Toggle(LED_TypeDef* led) {
    if (led->state == LED_ON) {
        LED_Off(led);
    } else {
        LED_On(led);
    }
}

/**
 * @brief  ��ȡLED״̬
 * @param  led: LED�ṹ��ָ��
 * @retval LED״̬
 */
LED_State LED_GetState(LED_TypeDef* led) {
    return led->state;
}
