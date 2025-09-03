#ifndef _KEY_H_
#define _KEY_H_

#include "system_config.h"


// 按键初始化结构体
typedef struct {
    uint32_t GPIO_CLK;
    GPIO_TypeDef* GPIO_PORT;
    uint16_t GPIO_PIN;
    uint8_t EXTI_PORT;
    uint8_t EXTI_PIN;
    uint32_t EXTI_LINE;
    uint8_t EXTI_IRQn;
	uint8_t NVIC_IRQChannelPreemptionPriority;  // 主优先级
    uint8_t NVIC_IRQChannelSubPriority;         // 子优先级
    EXTITrigger_TypeDef Trigger;  				// 触发方式：EXTI_Trigger_Rising 或 EXTI_Trigger_Falling
    GPIOMode_TypeDef GPIO_Mode;  				// GPIO模式
} Key_InitTypeDef;

// 按键初始化函数
void Key_Init(Key_InitTypeDef* keys, uint8_t key_num);

// 读取按键状态
uint8_t Key_Read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
