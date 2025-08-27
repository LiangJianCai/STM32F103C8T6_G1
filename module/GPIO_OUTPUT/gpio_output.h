#ifndef GPIO_OUTPUT_H
#define GPIO_OUTPUT_H

#include "system_config.h"

// GPIO 输出状态枚举
/**
 * @brief 定义GPIO输出的状态
 */
typedef enum {
    GPIO_OUTPUT_LOW = 0,  /**< GPIO输出低电平 */
    GPIO_OUTPUT_HIGH = 1  /**< GPIO输出高电平 */
} GPIO_Output_State;

// GPIO 输出结构体
/**
 * @brief 定义GPIO输出的相关信息
 */
typedef struct {
    GPIO_Config_TypeDef config;    /**< GPIO配置 */
    GPIO_Output_State state;       /**< GPIO当前输出状态 */
} GPIO_Output_TypeDef;

// 函数声明
/**
 * @brief 初始化 GPIO 输出
 * @param output: GPIO 输出结构体指针
 * @retval 无
 */
void GPIO_Output_Init(GPIO_Output_TypeDef* output);

/**
 * @brief 设置 GPIO 输出状态
 * @param output: GPIO 输出结构体指针
 * @param state: 要设置的输出状态
 * @retval 无
 */
void GPIO_Output_SetState(GPIO_Output_TypeDef* output, GPIO_Output_State state);

/**
 * @brief 获取 GPIO 输出状态
 * @param output: GPIO 输出结构体指针
 * @retval GPIO 输出状态
 */
GPIO_Output_State GPIO_Output_GetState(GPIO_Output_TypeDef* output);

#endif /* GPIO_OUTPUT_H */
