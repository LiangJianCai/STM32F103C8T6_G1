#ifndef DELAY_H
#define DELAY_H

#include "system_config.h"

/************************************************
 淘宝店铺：https://shop475501589.taobao.com/?spm=pc_detail.29232929/evo365560b447259.shop_block.dshopinfo.5dd97dd6JvMuG3
 咸鱼店铺：https://www.goofish.com/personal?spm=a21ybx.item.itemHeader.1.c17a3da6hy8k28&userId=3890583014
 哔哩哔哩：https://space.bilibili.com/482024430?spm_id_from=333.788.upinfo.detail.click
 作者：胜磊电子
************************************************/

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
