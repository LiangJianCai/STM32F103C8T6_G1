#include "gpio_input.h"

/**
 * @brief ��ʼ�� GPIO ����
 * @param input: GPIO ����ṹ��ָ��
 * @param config: GPIO ���ýṹ��ָ��
 * @retval ��
 */
void GPIO_Input_Init(GPIO_Input_TypeDef* input) {
    GPIO_InitTypeDef GPIO_InitStructure;
    uint32_t RCC_APB2Periph_GPIOx = 0;
    
    // ���� GPIO �˿�ȷ�� RCC ʱ��
    if (input->config.GPIOx == GPIOA) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOA;
    } else if (input->config.GPIOx == GPIOB) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOB;
    } else if (input->config.GPIOx == GPIOC) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOC;
    } else if (input->config.GPIOx == GPIOD) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOD;
    } else if (input->config.GPIOx == GPIOE) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOE;
    }
    
    // ʹ�� GPIO ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);
    
    // ���� GPIO
    GPIO_InitStructure.GPIO_Pin = input->config.GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = input->config.GPIO_Mode;
    GPIO_InitStructure.GPIO_Speed = input->config.GPIO_Speed;
    GPIO_Init(input->config.GPIOx, &GPIO_InitStructure);
    
    // ���³�ʼ״̬
    GPIO_Input_UpdateState(input);
}

/**
 * @brief ��ȡ GPIO ����״̬
 * @param input: GPIO ����ṹ��ָ��
 * @retval GPIO ����״̬
 */
GPIO_Input_State GPIO_Input_GetState(GPIO_Input_TypeDef* input) {
    return input->state;
}

/**
 * @brief ���� GPIO ����״̬
 * @param input: GPIO ����ṹ��ָ��
 * @retval ��
 */
void GPIO_Input_UpdateState(GPIO_Input_TypeDef* input) {
    if (GPIO_ReadInputDataBit(input->config.GPIOx, input->config.GPIO_Pin) == Bit_SET) {
        input->state = GPIO_INPUT_HIGH;
    } else {
        input->state = GPIO_INPUT_LOW;
    }
}
