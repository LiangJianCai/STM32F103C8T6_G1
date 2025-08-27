/**
 * @file pwm.h
 * @brief PWM 驱动库头文件，提供 PWM 初始化和控制功能
 * @author Your Name
 * @date Date of creation
 */

#ifndef __PWM_H
#define __PWM_H

#include "system_config.h"

// PWM 配置结构体
/**
 * @struct PWM_Config
 * @brief PWM 配置结构体，用于存储 PWM 相关的配置参数
 */
typedef struct {
    GPIO_TypeDef* GPIOx;           /**< GPIO 端口 */
    uint16_t GPIO_Pin;             /**< GPIO 引脚 */
    TIM_TypeDef* TIMx;             /**< 定时器 */
    uint8_t TIM_Channel;           /**< 定时器通道 */
    uint8_t TIM_AFIO_Remap;        /**< 定时器重映射配置 */
    uint16_t TIM_Prescaler;        /**< 定时器预分频值 */
    uint16_t PWM_Frequency;        /**< PWM 频率 */
    uint16_t Servo_Min_Pulse;      /**< 舵机最小脉冲宽度 */
    uint16_t Servo_Max_Pulse;      /**< 舵机最大脉冲宽度 */
    uint8_t Open_Angle;            /**< 打开角度 */
    uint8_t Close_Angle;           /**< 关闭角度 */
    _Bool current_status;          /**< 当前开关状态 */
} PWM_Config;

// 初始化 PWM
/**
 * @brief 初始化 PWM 定时器和 GPIO 引脚
 * @param config 指向 PWM_Config 结构体的指针，包含 PWM 配置信息
 */
void PWM_Init(PWM_Config* config);

// 设置舵机角度
/**
 * @brief 根据给定的角度设置舵机的位置
 * @param config 指向 PWM_Config 结构体的指针，包含 PWM 配置信息
 * @param angle 要设置的角度，范围 0 - 180 度
 */
void PWM_SetAngle(PWM_Config* config, uint8_t angle);

// 设置开关状态
/**
 * @brief 根据给定的状态打开或关闭设备
 * @param config 指向 PWM_Config 结构体的指针，包含 PWM 配置信息
 * @param status 设备状态，1 表示打开，0 表示关闭
 */
void PWM_SetStatus(PWM_Config* config, _Bool status);

// 获取当前开关状态
/**
 * @brief 获取当前设备的开关状态
 * @param config 指向 PWM_Config 结构体的指针，包含 PWM 配置信息
 * @return 当前设备状态，1 表示打开，0 表示关闭
 */
_Bool PWM_GetStatus(PWM_Config* config);

#endif
