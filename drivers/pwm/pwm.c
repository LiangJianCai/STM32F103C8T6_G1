/**
 * @file pwm.c
 * @brief PWM 驱动库实现文件，提供 PWM 初始化和控制功能
 * @author Your Name
 * @date Date of creation
 */

#include "pwm.h"
#include "led.h"
#include "delay.h"
#include "sensor_app.h"
#include <stdlib.h>

// 角度转换为脉冲宽度
/**
 * @brief 将给定的角度转换为对应的脉冲宽度
 * @param config 指向 PWM_Config 结构体的指针，包含 PWM 配置信息
 * @param angle 要转换的角度，范围 0 - 180 度
 * @return 对应的脉冲宽度
 */
uint16_t AngleToPulseWidth(PWM_Config* config, uint8_t angle)
{
    // 限制角度范围
    if (angle > 180) angle = 180;
    
    // 映射：0-180 度 -> Servo_Min_Pulse - Servo_Max_Pulse
    return config->Servo_Min_Pulse + (config->Servo_Max_Pulse - config->Servo_Min_Pulse) * angle / 180;
}

// 初始化 PWM
void PWM_Init(PWM_Config* config)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    // 使能定时器时钟
    if (config->TIMx == TIM1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    } else if (config->TIMx == TIM2) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    } else if (config->TIMx == TIM3) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    } else if (config->TIMx == TIM4) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    }

    // 使能 GPIO 和 AFIO 时钟
    if (config->GPIOx == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    } else if (config->GPIOx == GPIOB) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    } else if (config->GPIOx == GPIOC) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    }

    // 配置定时器重映射
    if (config->TIM_AFIO_Remap != 0) {
        GPIO_PinRemapConfig(config->TIM_AFIO_Remap, ENABLE);
    }

    // 配置 GPIO 为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = config->GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(config->GPIOx, &GPIO_InitStructure);

    // 初始化定时器
    uint16_t PWM_Period = (1000000 / config->PWM_Frequency) - 1;
    TIM_TimeBaseStructure.TIM_Period = PWM_Period;
    TIM_TimeBaseStructure.TIM_Prescaler = config->TIM_Prescaler - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(config->TIMx, &TIM_TimeBaseStructure);

    // 初始化定时器通道 PWM 模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = config->Servo_Min_Pulse;  // 初始位置，0 度
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    // 特殊处理高级定时器 TIM1
    if (config->TIMx == TIM1) {
        TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    }

    switch (config->TIM_Channel) {
        case 1:
            TIM_OC1Init(config->TIMx, &TIM_OCInitStructure);
            TIM_OC1PreloadConfig(config->TIMx, TIM_OCPreload_Enable);
            break;
        case 2:
            TIM_OC2Init(config->TIMx, &TIM_OCInitStructure);
            TIM_OC2PreloadConfig(config->TIMx, TIM_OCPreload_Enable);
            break;
        case 3:
            TIM_OC3Init(config->TIMx, &TIM_OCInitStructure);
            TIM_OC3PreloadConfig(config->TIMx, TIM_OCPreload_Enable);
            break;
        case 4:
            TIM_OC4Init(config->TIMx, &TIM_OCInitStructure);
            TIM_OC4PreloadConfig(config->TIMx, TIM_OCPreload_Enable);
            break;
    }

    TIM_ARRPreloadConfig(config->TIMx, ENABLE);  // 使能自动重装载寄存器预装载

    // 使能定时器
    TIM_Cmd(config->TIMx, ENABLE);
    
    // 特殊处理高级定时器 TIM1
    if (config->TIMx == TIM1) {
        TIM_CtrlPWMOutputs(config->TIMx, ENABLE);
    }
    
    // 初始化状态为关闭
    config->current_status = 0;
}

// 设置舵机角度
void PWM_SetAngle(PWM_Config* config, uint8_t angle)
{
    uint16_t target_pulse = AngleToPulseWidth(config, angle);
    uint16_t current_pulse;

    switch (config->TIM_Channel) {
        case 1:
            current_pulse = TIM_GetCapture1(config->TIMx);
            break;
        case 2:
            current_pulse = TIM_GetCapture2(config->TIMx);
            break;
        case 3:
            current_pulse = TIM_GetCapture3(config->TIMx);
            break;
        case 4:
            current_pulse = TIM_GetCapture4(config->TIMx);
            break;
    }

    int16_t step = (target_pulse > current_pulse) ? 5 : -5;  // 步进

    // 平滑过渡到目标角度
    while (abs(current_pulse - target_pulse) > 5) {
        current_pulse += step;
        switch (config->TIM_Channel) {
            case 1:
                TIM_SetCompare1(config->TIMx, current_pulse);
                break;
            case 2:
                TIM_SetCompare2(config->TIMx, current_pulse);
                break;
            case 3:
                TIM_SetCompare3(config->TIMx, current_pulse);
                break;
            case 4:
                TIM_SetCompare4(config->TIMx, current_pulse);
                break;
        }
        DelayMs(10);  // 延时控制转动速度
    }
    
    // 精确到达目标角度
    switch (config->TIM_Channel) {
        case 1:
            TIM_SetCompare1(config->TIMx, target_pulse);
            break;
        case 2:
            TIM_SetCompare2(config->TIMx, target_pulse);
            break;
        case 3:
            TIM_SetCompare3(config->TIMx, target_pulse);
            break;
        case 4:
            TIM_SetCompare4(config->TIMx, target_pulse);
            break;
    }
    DelayMs(500);  // 等待舵机稳定
}

// 设置开关状态
void PWM_SetStatus(PWM_Config* config, _Bool status)
{
    if (status == 1) {
        // 打开
        PWM_SetAngle(config, config->Open_Angle);
        config->current_status = 1;
    }
    else {
        // 关闭
        PWM_SetAngle(config, config->Close_Angle);
        config->current_status = 0;
    }
}

// 获取当前开关状态
_Bool PWM_GetStatus(PWM_Config* config)
{
    return config->current_status;
}
