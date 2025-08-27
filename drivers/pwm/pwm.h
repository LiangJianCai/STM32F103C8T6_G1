/**
 * @file pwm.h
 * @brief PWM ������ͷ�ļ����ṩ PWM ��ʼ���Ϳ��ƹ���
 * @author Your Name
 * @date Date of creation
 */

#ifndef __PWM_H
#define __PWM_H

#include "system_config.h"

// PWM ���ýṹ��
/**
 * @struct PWM_Config
 * @brief PWM ���ýṹ�壬���ڴ洢 PWM ��ص����ò���
 */
typedef struct {
    GPIO_TypeDef* GPIOx;           /**< GPIO �˿� */
    uint16_t GPIO_Pin;             /**< GPIO ���� */
    TIM_TypeDef* TIMx;             /**< ��ʱ�� */
    uint8_t TIM_Channel;           /**< ��ʱ��ͨ�� */
    uint8_t TIM_AFIO_Remap;        /**< ��ʱ����ӳ������ */
    uint16_t TIM_Prescaler;        /**< ��ʱ��Ԥ��Ƶֵ */
    uint16_t PWM_Frequency;        /**< PWM Ƶ�� */
    uint16_t Servo_Min_Pulse;      /**< �����С������ */
    uint16_t Servo_Max_Pulse;      /**< ������������ */
    uint8_t Open_Angle;            /**< �򿪽Ƕ� */
    uint8_t Close_Angle;           /**< �رսǶ� */
    _Bool current_status;          /**< ��ǰ����״̬ */
} PWM_Config;

// ��ʼ�� PWM
/**
 * @brief ��ʼ�� PWM ��ʱ���� GPIO ����
 * @param config ָ�� PWM_Config �ṹ���ָ�룬���� PWM ������Ϣ
 */
void PWM_Init(PWM_Config* config);

// ���ö���Ƕ�
/**
 * @brief ���ݸ����ĽǶ����ö����λ��
 * @param config ָ�� PWM_Config �ṹ���ָ�룬���� PWM ������Ϣ
 * @param angle Ҫ���õĽǶȣ���Χ 0 - 180 ��
 */
void PWM_SetAngle(PWM_Config* config, uint8_t angle);

// ���ÿ���״̬
/**
 * @brief ���ݸ�����״̬�򿪻�ر��豸
 * @param config ָ�� PWM_Config �ṹ���ָ�룬���� PWM ������Ϣ
 * @param status �豸״̬��1 ��ʾ�򿪣�0 ��ʾ�ر�
 */
void PWM_SetStatus(PWM_Config* config, _Bool status);

// ��ȡ��ǰ����״̬
/**
 * @brief ��ȡ��ǰ�豸�Ŀ���״̬
 * @param config ָ�� PWM_Config �ṹ���ָ�룬���� PWM ������Ϣ
 * @return ��ǰ�豸״̬��1 ��ʾ�򿪣�0 ��ʾ�ر�
 */
_Bool PWM_GetStatus(PWM_Config* config);

#endif
