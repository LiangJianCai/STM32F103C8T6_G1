#include "gpio_output.h"

/**
 * @brief 初始化 GPIO 输出
 * @param output: GPIO 输出结构体指针
 * @retval 无
 */
void GPIO_Output_Init(GPIO_Output_TypeDef* output) {
    GPIO_InitTypeDef GPIO_InitStructure;
    uint32_t RCC_APB2Periph_GPIOx = 0;

    // 根据 GPIO 端口确定 RCC 时钟
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

    // 使能 GPIO 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);

    // 配置 GPIO
    GPIO_InitStructure.GPIO_Pin = output->config.GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = output->config.GPIO_Mode;
    GPIO_InitStructure.GPIO_Speed = output->config.GPIO_Speed;
    GPIO_Init(output->config.GPIOx, &GPIO_InitStructure);

    // 设置初始状态为低电平
    GPIO_Output_SetState(output, GPIO_OUTPUT_LOW);
}

/**
 * @brief 设置 GPIO 输出状态
 * @param output: GPIO 输出结构体指针
 * @param state: 要设置的输出状态
 * @retval 无
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
 * @brief 获取 GPIO 输出状态
 * @param output: GPIO 输出结构体指针
 * @retval GPIO 输出状态
 */
GPIO_Output_State GPIO_Output_GetState(GPIO_Output_TypeDef* output) {
    return output->state;
}
