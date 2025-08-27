#include "gpio_output.h"

/**
 * @brief ��ʼ�� GPIO ���
 * @param output: GPIO ����ṹ��ָ��
 * @retval ��
 */
void GPIO_Output_Init(GPIO_Output_TypeDef* output) {
    GPIO_InitTypeDef GPIO_InitStructure;
    uint32_t RCC_APB2Periph_GPIOx = 0;

    // ���� GPIO �˿�ȷ�� RCC ʱ��
    if (output->config.GPIOx == GPIOA) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOA;
    } else if (output->config.GPIOx == GPIOB) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOB;
    } else if (output->config.GPIOx == GPIOC) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOC;
    } else if (output->config.GPIOx == GPIOD) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOD;
    } else if (output->config.GPIOx == GPIOE) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOE;
    }

    // ʹ�� GPIO ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);

    // ���� GPIO
    GPIO_InitStructure.GPIO_Pin = output->config.GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = output->config.GPIO_Mode;
    GPIO_InitStructure.GPIO_Speed = output->config.GPIO_Speed;
    GPIO_Init(output->config.GPIOx, &GPIO_InitStructure);

    // ���ó�ʼ״̬Ϊ�͵�ƽ
    GPIO_Output_SetState(output, GPIO_OUTPUT_LOW);
}

/**
 * @brief ���� GPIO ���״̬
 * @param output: GPIO ����ṹ��ָ��
 * @param state: Ҫ���õ����״̬
 * @retval ��
 */
void GPIO_Output_SetState(GPIO_Output_TypeDef* output, GPIO_Output_State state) {
    if (state == GPIO_OUTPUT_HIGH) {
        GPIO_SetBits(output->config.GPIOx, output->config.GPIO_Pin);
    } else {
        GPIO_ResetBits(output->config.GPIOx, output->config.GPIO_Pin);
    }
    output->state = state;
}

/**
 * @brief ��ȡ GPIO ���״̬
 * @param output: GPIO ����ṹ��ָ��
 * @retval GPIO ���״̬
 */
GPIO_Output_State GPIO_Output_GetState(GPIO_Output_TypeDef* output) {
    return output->state;
}
