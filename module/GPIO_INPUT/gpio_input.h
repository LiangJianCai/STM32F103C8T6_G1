#ifndef GPIO_INPUT_H
#define GPIO_INPUT_H

#include "system_config.h"

// GPIO 输入状态枚举
typedef enum {
    GPIO_INPUT_LOW = 0,
    GPIO_INPUT_HIGH = 1
} GPIO_Input_State;

// GPIO 输入结构体定义
typedef struct {
    GPIO_Config_TypeDef config;    // GPIO 配置
    GPIO_Input_State state;        // GPIO 当前状态
} GPIO_Input_TypeDef;

// 函数声明
void GPIO_Input_Init(GPIO_Input_TypeDef* input);
GPIO_Input_State GPIO_Input_GetState(GPIO_Input_TypeDef* input);
void GPIO_Input_UpdateState(GPIO_Input_TypeDef* input);

#endif /* GPIO_INPUT_H */
