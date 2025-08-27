#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

// 包含所有必要的STM32标准库头文件
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// 其他常用头文件
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

// GPIO 配置结构体
typedef struct {
    GPIO_TypeDef* GPIOx;           // GPIO 端口
    uint16_t GPIO_Pin;             // GPIO 引脚
    GPIOMode_TypeDef GPIO_Mode;    // GPIO 模式
    GPIOSpeed_TypeDef GPIO_Speed;  // GPIO 速度
} GPIO_Config_TypeDef;

// 如果没有定义NULL，手动定义
#ifndef NULL
#define NULL ((void*)0)
#endif

#endif /* SYSTEM_CONFIG_H */    
