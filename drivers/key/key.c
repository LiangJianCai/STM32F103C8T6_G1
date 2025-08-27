#include "key.h"

/*
************************************************************
*	函数名称：	Key_Init
*
*	函数功能：	按键初始化
*
*	输入参数：	keys: 按键初始化结构体数组
*               key_num: 按键数量
*
*	返回参数：	无
*
*	说    明：	
************************************************************
*/
void Key_Init(Key_InitTypeDef* keys, uint8_t key_num)
{
    GPIO_InitTypeDef gpio_initstruct;
    EXTI_InitTypeDef exti_initstruct;
    NVIC_InitTypeDef nvic_initstruct;
    uint8_t i;

    // 使能所有按键的时钟
    uint32_t all_clocks = 0;
    for (i = 0; i < key_num; i++) {
        all_clocks |= keys[i].GPIO_CLK;
    }
    all_clocks |= RCC_APB2Periph_AFIO;
    RCC_APB2PeriphClockCmd(all_clocks, ENABLE);
	
    for (i = 0; i < key_num; i++) {
        // GPIO初始化
        gpio_initstruct.GPIO_Pin = keys[i].GPIO_PIN;
        gpio_initstruct.GPIO_Mode = keys[i].GPIO_Mode;
        GPIO_Init(keys[i].GPIO_PORT, &gpio_initstruct);

        // 初始化外部中断线
        GPIO_EXTILineConfig(keys[i].EXTI_PORT, keys[i].EXTI_PIN);

        // 初始化中断
        exti_initstruct.EXTI_Line = keys[i].EXTI_LINE;
        exti_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
        exti_initstruct.EXTI_Trigger = keys[i].Trigger;
        exti_initstruct.EXTI_LineCmd = ENABLE;
        EXTI_Init(&exti_initstruct);

        // 配置NVIC
        nvic_initstruct.NVIC_IRQChannel = keys[i].EXTI_IRQn;
        nvic_initstruct.NVIC_IRQChannelPreemptionPriority = keys[i].NVIC_IRQChannelPreemptionPriority;
        nvic_initstruct.NVIC_IRQChannelSubPriority = keys[i].NVIC_IRQChannelSubPriority;
        nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&nvic_initstruct);
    }
}

/*
************************************************************
*	函数名称：	Key_Read
*
*	函数功能：	读取按键状态
*
*	输入参数：	GPIOx: GPIO端口
*               GPIO_Pin: GPIO引脚
*
*	返回参数：	按键状态
*
*	说    明：	
************************************************************
*/
uint8_t Key_Read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return GPIO_ReadInputDataBit(GPIOx, GPIO_Pin);
}
