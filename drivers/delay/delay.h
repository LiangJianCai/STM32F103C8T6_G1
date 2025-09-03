#ifndef DELAY_H
#define DELAY_H

#include "system_config.h"


/** 定时器配置结构体 */
typedef struct {
    TIM_TypeDef* TIMx;              // 定时器指针
    uint32_t TIMx_CLK;              // 定时器时钟
    uint32_t APBxPeriph_CLKCmd;     // 时钟使能函数参数
    void (*CLKCmd)(uint32_t, FunctionalState); // 时钟使能函数指针
} Delay_TimerConfigTypeDef;

/** 系统支持的所有定时器配置表 */
extern const Delay_TimerConfigTypeDef TimerConfigTable[];
extern const uint8_t TimerConfigTableSize;

/** 延时函数初始化和操作 */
void Delay_Init(uint8_t timerIndex);
void DelayUs(uint16_t us);
void DelayMs(uint16_t ms);

/** 获取当前使用的定时器索引 */
uint8_t Delay_GetCurrentTimerIndex(void);

#endif /* DELAY_H */   
