#ifndef DELAY_H
#define DELAY_H

#include "system_config.h"


/** ��ʱ�����ýṹ�� */
typedef struct {
    TIM_TypeDef* TIMx;              // ��ʱ��ָ��
    uint32_t TIMx_CLK;              // ��ʱ��ʱ��
    uint32_t APBxPeriph_CLKCmd;     // ʱ��ʹ�ܺ�������
    void (*CLKCmd)(uint32_t, FunctionalState); // ʱ��ʹ�ܺ���ָ��
} Delay_TimerConfigTypeDef;

/** ϵͳ֧�ֵ����ж�ʱ�����ñ� */
extern const Delay_TimerConfigTypeDef TimerConfigTable[];
extern const uint8_t TimerConfigTableSize;

/** ��ʱ������ʼ���Ͳ��� */
void Delay_Init(uint8_t timerIndex);
void DelayUs(uint16_t us);
void DelayMs(uint16_t ms);

/** ��ȡ��ǰʹ�õĶ�ʱ������ */
uint8_t Delay_GetCurrentTimerIndex(void);

#endif /* DELAY_H */   
