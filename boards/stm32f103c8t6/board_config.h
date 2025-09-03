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


#define USART_DEBUG		USART1		//调试打印所使用的串口组

#define RX_BUFFER_SIZE 64

// 按键中断处理函数声明
#define    KEYUP_IRQHandler    		EXTI0_IRQHandler
#define    KEY012_IRQHandler   		EXTI15_10_IRQHandler

extern volatile uint32_t uwTick;
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

/* 全局环境数据结构 ---------------------------------------------------------*/
typedef struct {
    uint16_t adc1;   // 0-100 %
    uint16_t adc2;
    uint8_t  temp;   // DHT11
    uint8_t  humi;
    int16_t  ds18_temp_x10;   // DS18B20*10，省浮点
    uint8_t  sw[3];
} Env_t;

extern Env_t g_env;
#endif /* BOARD_CONFIG_H */
