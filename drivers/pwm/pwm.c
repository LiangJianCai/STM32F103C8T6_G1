/**
 * @file pwm.c
 * @brief PWM ������ʵ���ļ����ṩ PWM ��ʼ���Ϳ��ƹ���
 * @author Your Name
 * @date Date of creation
 */

#include "pwm.h"
#include "led.h"
#include "delay.h"
#include "sensor_app.h"
#include <stdlib.h>

// �Ƕ�ת��Ϊ������
/**
 * @brief �������ĽǶ�ת��Ϊ��Ӧ��������
 * @param config ָ�� PWM_Config �ṹ���ָ�룬���� PWM ������Ϣ
 * @param angle Ҫת���ĽǶȣ���Χ 0 - 180 ��
 * @return ��Ӧ��������
 */
uint16_t AngleToPulseWidth(PWM_Config* config, uint8_t angle)
{
    // ���ƽǶȷ�Χ
    if (angle > 180) angle = 180;
    
    // ӳ�䣺0-180 �� -> Servo_Min_Pulse - Servo_Max_Pulse
    return config->Servo_Min_Pulse + (config->Servo_Max_Pulse - config->Servo_Min_Pulse) * angle / 180;
}

// ��ʼ�� PWM
void PWM_Init(PWM_Config* config)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    // ʹ�ܶ�ʱ��ʱ��
    if (config->TIMx == TIM1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    } else if (config->TIMx == TIM2) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    } else if (config->TIMx == TIM3) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    } else if (config->TIMx == TIM4) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    }

    // ʹ�� GPIO �� AFIO ʱ��
    if (config->GPIOx == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    } else if (config->GPIOx == GPIOB) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    } else if (config->GPIOx == GPIOC) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    }

    // ���ö�ʱ����ӳ��
    if (config->TIM_AFIO_Remap != 0) {
        GPIO_PinRemapConfig(config->TIM_AFIO_Remap, ENABLE);
    }

    // ���� GPIO Ϊ�����������
    GPIO_InitStructure.GPIO_Pin = config->GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(config->GPIOx, &GPIO_InitStructure);

    // ��ʼ����ʱ��
    uint16_t PWM_Period = (1000000 / config->PWM_Frequency) - 1;
    TIM_TimeBaseStructure.TIM_Period = PWM_Period;
    TIM_TimeBaseStructure.TIM_Prescaler = config->TIM_Prescaler - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(config->TIMx, &TIM_TimeBaseStructure);

    // ��ʼ����ʱ��ͨ�� PWM ģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = config->Servo_Min_Pulse;  // ��ʼλ�ã�0 ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    // ���⴦��߼���ʱ�� TIM1
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

    TIM_ARRPreloadConfig(config->TIMx, ENABLE);  // ʹ���Զ���װ�ؼĴ���Ԥװ��

    // ʹ�ܶ�ʱ��
    TIM_Cmd(config->TIMx, ENABLE);
    
    // ���⴦��߼���ʱ�� TIM1
    if (config->TIMx == TIM1) {
        TIM_CtrlPWMOutputs(config->TIMx, ENABLE);
    }
    
    // ��ʼ��״̬Ϊ�ر�
    config->current_status = 0;
}

// ���ö���Ƕ�
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

    int16_t step = (target_pulse > current_pulse) ? 5 : -5;  // ����

    // ƽ�����ɵ�Ŀ��Ƕ�
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
        DelayMs(10);  // ��ʱ����ת���ٶ�
    }
    
    // ��ȷ����Ŀ��Ƕ�
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
    DelayMs(500);  // �ȴ�����ȶ�
}

// ���ÿ���״̬
void PWM_SetStatus(PWM_Config* config, _Bool status)
{
    if (status == 1) {
        // ��
        PWM_SetAngle(config, config->Open_Angle);
        config->current_status = 1;
    }
    else {
        // �ر�
        PWM_SetAngle(config, config->Close_Angle);
        config->current_status = 0;
    }
}

// ��ȡ��ǰ����״̬
_Bool PWM_GetStatus(PWM_Config* config)
{
    return config->current_status;
}
