#ifndef LED_H
#define LED_H

#include "system_config.h"

/************************************************
 LED灯驱动
 淘宝店铺：https://shop475501589.taobao.com/?spm=pc_detail.29232929/evo365560b447259.shop_block.dshopinfo.5dd97dd6JvMuG3
 咸鱼店铺：https://www.goofish.com/personal?spm=a21ybx.item.itemHeader.1.c17a3da6hy8k28&userId=3890583014
 哔哩哔哩：https://space.bilibili.com/482024430?spm_id_from=333.788.upinfo.detail.click
 作者：胜磊电子
************************************************/

// LED状态枚举
typedef enum {
    LED_OFF = 0,
    LED_ON = 1
} LED_State;

// LED结构体定义
typedef struct {
    GPIO_TypeDef* GPIOx;       // GPIO端口
    uint16_t GPIO_Pin;         // GPIO引脚
    LED_State state;           // LED当前状态
} LED_TypeDef;

// 函数声明
void LED_Init(LED_TypeDef* led, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void LED_On(LED_TypeDef* led);
void LED_Off(LED_TypeDef* led);
void LED_Toggle(LED_TypeDef* led);
LED_State LED_GetState(LED_TypeDef* led);

#endif /* LED_H */
