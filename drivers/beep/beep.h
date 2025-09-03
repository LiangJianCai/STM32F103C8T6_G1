#ifndef BEEP_H
#define BEEP_H

#include "system_config.h"


// 蜂鸣器状态枚举
typedef enum {
    BEEP_OFF = 0,
    BEEP_ON = 1
} Beep_State;

// 蜂鸣器结构体定义
typedef struct {
    GPIO_TypeDef* GPIOx;     // GPIO端口
    uint16_t GPIO_Pin;       // GPIO引脚
    Beep_State state;        // 蜂鸣器当前状态
} Beep_TypeDef;

// 函数声明
void Beep_Init(Beep_TypeDef* beep, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Beep_On(Beep_TypeDef* beep);
void Beep_Off(Beep_TypeDef* beep);
void Beep_Toggle(Beep_TypeDef* beep);
Beep_State Beep_GetState(Beep_TypeDef* beep);

#endif /* BEEP_H */  
