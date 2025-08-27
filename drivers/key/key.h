#ifndef _KEY_H_
#define _KEY_H_

#include "system_config.h"

/************************************************
 淘宝店铺：https://shop475501589.taobao.com/?spm=pc_detail.29232929/evo365560b447259.shop_block.dshopinfo.5dd97dd6JvMuG3
 咸鱼店铺：https://www.goofish.com/personal?spm=a21ybx.item.itemHeader.1.c17a3da6hy8k28&userId=3890583014
 哔哩哔哩：https://space.bilibili.com/482024430?spm_id_from=333.788.upinfo.detail.click
 作者：胜磊电子
************************************************/

// 按键初始化结构体
typedef struct {
    uint32_t GPIO_CLK;
    GPIO_TypeDef* GPIO_PORT;
    uint16_t GPIO_PIN;
    uint8_t EXTI_PORT;
    uint8_t EXTI_PIN;
    uint32_t EXTI_LINE;
    uint8_t EXTI_IRQn;
	uint8_t NVIC_IRQChannelPreemptionPriority;  // 主优先级
    uint8_t NVIC_IRQChannelSubPriority;         // 子优先级
    EXTITrigger_TypeDef Trigger;  				// 触发方式：EXTI_Trigger_Rising 或 EXTI_Trigger_Falling
    GPIOMode_TypeDef GPIO_Mode;  				// GPIO模式
} Key_InitTypeDef;

// 按键初始化函数
void Key_Init(Key_InitTypeDef* keys, uint8_t key_num);

// 读取按键状态
uint8_t Key_Read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
