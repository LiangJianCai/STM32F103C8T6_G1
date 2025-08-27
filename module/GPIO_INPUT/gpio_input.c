#include "gpio_input.h"

/**
 * @brief 初始化 GPIO 输入
 * @param input: GPIO 输入结构体指针
 * @param config: GPIO 配置结构体指针
 * @retval 无
 */
void GPIO_Input_Init(GPIO_Input_TypeDef* input) {
    GPIO_InitTypeDef GPIO_InitStructure;
    uint32_t RCC_APB2Periph_GPIOx = 0;
    
    // 根据 GPIO 端口确定 RCC 时钟
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
    
    // 使能 GPIO 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);
    
    // 配置 GPIO
    GPIO_InitStructure.GPIO_Pin = input->config.GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = input->config.GPIO_Mode;
    GPIO_InitStructure.GPIO_Speed = input->config.GPIO_Speed;
    GPIO_Init(input->config.GPIOx, &GPIO_InitStructure);
    
    // 更新初始状态
    GPIO_Input_UpdateState(input);
}

/**
 * @brief 获取 GPIO 输入状态
 * @param input: GPIO 输入结构体指针
 * @retval GPIO 输入状态
 */
GPIO_Input_State GPIO_Input_GetState(GPIO_Input_TypeDef* input) {
    return input->state;
}

/**
 * @brief 更新 GPIO 输入状态
 * @param input: GPIO 输入结构体指针
 * @retval 无
 */
void GPIO_Input_UpdateState(GPIO_Input_TypeDef* input) {
    if (GPIO_ReadInputDataBit(input->config.GPIOx, input->config.GPIO_Pin) == Bit_SET) {
        input->state = GPIO_INPUT_HIGH;
    } else {
        input->state = GPIO_INPUT_LOW;
    }
}
