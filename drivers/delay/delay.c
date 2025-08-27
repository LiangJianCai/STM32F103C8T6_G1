#include "delay.h"

/** ϵͳ֧�ֵ����ж�ʱ�����ñ� */
const Delay_TimerConfigTypeDef TimerConfigTable[] = {
    {TIM1,   RCC_APB2Periph_TIM1,   RCC_APB2Periph_TIM1,   RCC_APB2PeriphClockCmd},
    {TIM2,   RCC_APB1Periph_TIM2,   RCC_APB1Periph_TIM2,   RCC_APB1PeriphClockCmd},
    {TIM3,   RCC_APB1Periph_TIM3,   RCC_APB1Periph_TIM3,   RCC_APB1PeriphClockCmd},
    {TIM4,   RCC_APB1Periph_TIM4,   RCC_APB1Periph_TIM4,   RCC_APB1PeriphClockCmd},
    {TIM5,   RCC_APB1Periph_TIM5,   RCC_APB1Periph_TIM5,   RCC_APB1PeriphClockCmd},
    {TIM6,   RCC_APB1Periph_TIM6,   RCC_APB1Periph_TIM6,   RCC_APB1PeriphClockCmd},
    {TIM7,   RCC_APB1Periph_TIM7,   RCC_APB1Periph_TIM7,   RCC_APB1PeriphClockCmd},
    {TIM8,   RCC_APB2Periph_TIM8,   RCC_APB2Periph_TIM8,   RCC_APB2PeriphClockCmd},
    {TIM9,   RCC_APB2Periph_TIM9,   RCC_APB2Periph_TIM9,   RCC_APB2PeriphClockCmd},
    {TIM10,  RCC_APB2Periph_TIM10,  RCC_APB2Periph_TIM10,  RCC_APB2PeriphClockCmd},
    {TIM11,  RCC_APB2Periph_TIM11,  RCC_APB2Periph_TIM11,  RCC_APB2PeriphClockCmd},
    {TIM12,  RCC_APB1Periph_TIM12,  RCC_APB1Periph_TIM12,  RCC_APB1PeriphClockCmd},
    {TIM13,  RCC_APB1Periph_TIM13,  RCC_APB1Periph_TIM13,  RCC_APB1PeriphClockCmd},
    {TIM14,  RCC_APB1Periph_TIM14,  RCC_APB1Periph_TIM14,  RCC_APB1PeriphClockCmd},
};

/** ��ʱ�����ñ��С */
const uint8_t TimerConfigTableSize = sizeof(TimerConfigTable) / sizeof(TimerConfigTable[0]);

/** ��ǰʹ�õĶ�ʱ������ */
static uint8_t currentTimerIndex = 0;

/**
  * @brief  ��ʼ����ʱ����������ʱ����
  * @param  timerIndex: ��ʱ�������ñ��е�����
  * @retval ��
  */
void Delay_Init(uint8_t timerIndex)
{
    if (timerIndex >= TimerConfigTableSize) {
        // ����������Χ��ʹ��Ĭ�϶�ʱ��
        timerIndex = 0;
    }
    
    currentTimerIndex = timerIndex;
    const Delay_TimerConfigTypeDef* config = &TimerConfigTable[timerIndex];
    
    // ʹ�ܶ�ʱ��ʱ��
    config->CLKCmd(config->TIMx_CLK, ENABLE);

    // ֹͣ��ʱ��
    TIM_Cmd(config->TIMx, DISABLE);

    // ���ö�ʱ��Ϊ���ϼ���ģʽ
    TIM_InternalClockConfig(config->TIMx);
    TIM_CounterModeConfig(config->TIMx, TIM_CounterMode_Up);

    // ���� PSC Ϊ 71��ʹ������ʱ��Ϊ 1MHz (72MHz / 72 = 1MHz)
    TIM_PrescalerConfig(config->TIMx, 71, TIM_PSCReloadMode_Immediate);

    // ������±�־λ
    TIM_ClearFlag(config->TIMx, TIM_FLAG_Update);
}

/**
  * @brief  ΢�뼶��ʱ����
  * @param  us: ��ʱ��΢��������Χ 0 - 65535
  * @retval ��
  */
void DelayUs(uint16_t us)
{
    if (us == 0) return;

    const Delay_TimerConfigTypeDef* config = &TimerConfigTable[currentTimerIndex];

    // �����Զ�����ֵ
    TIM_SetAutoreload(config->TIMx, us);

    // ���������
    TIM_SetCounter(config->TIMx, 0);

    // ������±�־λ
    TIM_ClearFlag(config->TIMx, TIM_FLAG_Update);

    // ������ʱ��
    TIM_Cmd(config->TIMx, ENABLE);

    // �ȴ����±�־λ����λ
    while (!TIM_GetFlagStatus(config->TIMx, TIM_FLAG_Update));

    // ֹͣ��ʱ��
    TIM_Cmd(config->TIMx, DISABLE);

    // ������±�־λ
    TIM_ClearFlag(config->TIMx, TIM_FLAG_Update);
}

/**
  * @brief  ���뼶��ʱ����
  * @param  ms: ��ʱ�ĺ�����
  * @retval ��
  */
void DelayMs(uint16_t ms)
{
    for (uint16_t i = 0; i < ms; i++)
    {
        DelayUs(1000);  // ѭ������ 1000 ΢����ʱʵ�� 1 ������ʱ
    }
}

/**
  * @brief  ��ȡ��ǰʹ�õĶ�ʱ������
  * @retval ��ǰʹ�õĶ�ʱ�������ñ��е�����
  */
uint8_t Delay_GetCurrentTimerIndex(void)
{
    return currentTimerIndex;
}
