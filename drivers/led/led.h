#ifndef LED_H
#define LED_H

#include "system_config.h"


// LED状态枚举
typedef enum {
    LED_OFF = 0,
    LED_ON = 1
} LED_State;

// LED结构体定义
typedef struct {
    GPIO_TypeDef* GPIOx;       // GPIO端口
    uint16_t GPIO_Pin;         // GPIO引脚
    LED_State state;           // LED当前状态
} LED_TypeDef;

// 函数声明
void LED_Init(LED_TypeDef* led, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void LED_On(LED_TypeDef* led);
void LED_Off(LED_TypeDef* led);
void LED_Toggle(LED_TypeDef* led);
LED_State LED_GetState(LED_TypeDef* led);

#endif /* LED_H */
