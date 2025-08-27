#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "led.h"
#include "beep.h"
#include "usart.h"
#include "key.h"
#include "delay.h"
#include "adc.h"
#include "rtc.h"
#include "pwm.h"

/************************************************
 淘宝店铺：https://shop475501589.taobao.com/?spm=pc_detail.29232929/evo365560b447259.shop_block.dshopinfo.5dd97dd6JvMuG3
 咸鱼店铺：https://www.goofish.com/personal?spm=a21ybx.item.itemHeader.1.c17a3da6hy8k28&userId=3890583014
 哔哩哔哩：https://space.bilibili.com/482024430?spm_id_from=333.788.upinfo.detail.click
 作者：胜磊电子
************************************************/

#define USART_DEBUG		USART1		//调试打印所使用的串口组

#define RX_BUFFER_SIZE 64

// 按键中断处理函数声明
#define    KEYUP_IRQHandler    		EXTI0_IRQHandler
#define    KEY012_IRQHandler   		EXTI15_10_IRQHandler


// 导出LED对象供外部使用
extern LED_TypeDef BOARD_LED1;
extern LED_TypeDef BOARD_LED2;
extern LED_TypeDef BOARD_LED3;

// 导出蜂鸣器对象供外部使用
extern Beep_TypeDef BOARD_BEEP;

// 按键配置 - 可在board_config.c中修改
extern Key_InitTypeDef BOARD_KEYS[];
extern const uint8_t BOARD_KEYS_NUM;

extern Time_t currentTime;

// 导出PWM对象供外部使用
extern PWM_Config window_pwm_config;

// 板子初始化函数
void BOARD_InitAll(void);


#endif /* BOARD_CONFIG_H */
