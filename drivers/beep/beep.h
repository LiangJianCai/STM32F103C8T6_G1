#ifndef BEEP_H
#define BEEP_H

#include "system_config.h"

/************************************************
 淘宝店铺：https://shop475501589.taobao.com/?spm=pc_detail.29232929/evo365560b447259.shop_block.dshopinfo.5dd97dd6JvMuG3
 咸鱼店铺：https://www.goofish.com/personal?spm=a21ybx.item.itemHeader.1.c17a3da6hy8k28&userId=3890583014
 哔哩哔哩：https://space.bilibili.com/482024430?spm_id_from=333.788.upinfo.detail.click
 作者：胜磊电子
************************************************/

// 蜂鸣器状态枚举
typedef enum {
    BEEP_OFF = 0,
    BEEP_ON = 1
} Beep_State;

// 蜂鸣器结构体定义
typedef struct {
    GPIO_TypeDef* GPIOx;     // GPIO端口
    uint16_t GPIO_Pin;       // GPIO引脚
    Beep_State state;        // 蜂鸣器当前状态
} Beep_TypeDef;

// 函数声明
void Beep_Init(Beep_TypeDef* beep, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Beep_On(Beep_TypeDef* beep);
void Beep_Off(Beep_TypeDef* beep);
void Beep_Toggle(Beep_TypeDef* beep);
Beep_State Beep_GetState(Beep_TypeDef* beep);

#endif /* BEEP_H */  
