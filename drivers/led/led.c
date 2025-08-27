#include "led.h"

/**
 * @brief  初始化LED
 * @param  led: LED结构体指针
 * @param  GPIOx: GPIO端口
 * @param  GPIO_Pin: GPIO引脚
 * @retval 无
 */
void LED_Init(LED_TypeDef* led, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    GPIO_InitTypeDef GPIO_InitStructure;
    uint32_t RCC_APB2Periph_GPIOx = 0;
    
    // 根据GPIO端口确定RCC时钟
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
    
    // 使能GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);
    
    // 配置GPIO为推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
    
    // 初始化LED结构体
    led->GPIOx = GPIOx;
    led->GPIO_Pin = GPIO_Pin;
    led->state = LED_OFF;
    
    // 默认关闭LED
    GPIO_SetBits(GPIOx, GPIO_Pin);
}

/**
 * @brief  打开LED
 * @param  led: LED结构体指针
 * @retval 无
 */
void LED_On(LED_TypeDef* led) {
    GPIO_ResetBits(led->GPIOx, led->GPIO_Pin);
    led->state = LED_ON;
}

/**
 * @brief  关闭LED
 * @param  led: LED结构体指针
 * @retval 无
 */
void LED_Off(LED_TypeDef* led) {
    GPIO_SetBits(led->GPIOx, led->GPIO_Pin);
    led->state = LED_OFF;
}

/**
 * @brief  切换LED状态
 * @param  led: LED结构体指针
 * @retval 无
 */
void LED_Toggle(LED_TypeDef* led) {
    if (led->state == LED_ON) {
        LED_Off(led);
    } else {
        LED_On(led);
    }
}

/**
 * @brief  获取LED状态
 * @param  led: LED结构体指针
 * @retval LED状态
 */
LED_State LED_GetState(LED_TypeDef* led) {
    return led->state;
}
