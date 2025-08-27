#include "delay.h"

/** 系统支持的所有定时器配置表 */
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

/** 定时器配置表大小 */
const uint8_t TimerConfigTableSize = sizeof(TimerConfigTable) / sizeof(TimerConfigTable[0]);

/** 当前使用的定时器索引 */
static uint8_t currentTimerIndex = 0;

/**
  * @brief  初始化定时器，用于延时功能
  * @param  timerIndex: 定时器在配置表中的索引
  * @retval 无
  */
void Delay_Init(uint8_t timerIndex)
{
    if (timerIndex >= TimerConfigTableSize) {
        // 索引超出范围，使用默认定时器
        timerIndex = 0;
    }
    
    currentTimerIndex = timerIndex;
    const Delay_TimerConfigTypeDef* config = &TimerConfigTable[timerIndex];
    
    // 使能定时器时钟
    config->CLKCmd(config->TIMx_CLK, ENABLE);

    // 停止定时器
    TIM_Cmd(config->TIMx, DISABLE);

    // 配置定时器为向上计数模式
    TIM_InternalClockConfig(config->TIMx);
    TIM_CounterModeConfig(config->TIMx, TIM_CounterMode_Up);

    // 设置 PSC 为 71，使计数器时钟为 1MHz (72MHz / 72 = 1MHz)
    TIM_PrescalerConfig(config->TIMx, 71, TIM_PSCReloadMode_Immediate);

    // 清除更新标志位
    TIM_ClearFlag(config->TIMx, TIM_FLAG_Update);
}

/**
  * @brief  微秒级延时函数
  * @param  us: 延时的微秒数，范围 0 - 65535
  * @retval 无
  */
void DelayUs(uint16_t us)
{
    if (us == 0) return;

    const Delay_TimerConfigTypeDef* config = &TimerConfigTable[currentTimerIndex];

    // 设置自动重载值
    TIM_SetAutoreload(config->TIMx, us);

    // 清零计数器
    TIM_SetCounter(config->TIMx, 0);

    // 清除更新标志位
    TIM_ClearFlag(config->TIMx, TIM_FLAG_Update);

    // 启动定时器
    TIM_Cmd(config->TIMx, ENABLE);

    // 等待更新标志位被置位
    while (!TIM_GetFlagStatus(config->TIMx, TIM_FLAG_Update));

    // 停止定时器
    TIM_Cmd(config->TIMx, DISABLE);

    // 清除更新标志位
    TIM_ClearFlag(config->TIMx, TIM_FLAG_Update);
}

/**
  * @brief  毫秒级延时函数
  * @param  ms: 延时的毫秒数
  * @retval 无
  */
void DelayMs(uint16_t ms)
{
    for (uint16_t i = 0; i < ms; i++)
    {
        DelayUs(1000);  // 循环调用 1000 微秒延时实现 1 毫秒延时
    }
}

/**
  * @brief  获取当前使用的定时器索引
  * @retval 当前使用的定时器在配置表中的索引
  */
uint8_t Delay_GetCurrentTimerIndex(void)
{
    return currentTimerIndex;
}
