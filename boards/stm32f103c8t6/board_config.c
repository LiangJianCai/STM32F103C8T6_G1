#include "board_config.h"

/**
 * @file    board_config.c
 * @brief   开发板外设配置与初始化
 * @details 本文件实现了开发板上所有外设的配置与初始化函数，
 *          包括LED、蜂鸣器、按键、SPI、USART等模块的初始化。
 */

/************************************* 变量定义 *******************************************************/

/* 1ms计数器 */
volatile uint32_t uwTick = 0;

/* 环境变量 */
Env_t g_env;

/** 
 * @brief 开发板LED对象定义
 * @details 定义了开发板上三个LED的控制结构体
 */
LED_TypeDef BOARD_LED1; /**< 开发板LED1对象 (PB0) */
LED_TypeDef BOARD_LED2; /**< 开发板LED2对象 (PB1) */

/** 
 * @brief 开发板蜂鸣器对象定义
 * @details 定义了开发板上蜂鸣器的控制结构体
 */
Beep_TypeDef BOARD_BEEP; /**< 开发板蜂鸣器对象 (PB8) */

/** 
 * @brief 开发板按键配置数组
 * @details 定义了开发板上4个按键的配置参数，包括GPIO、中断设置等
 */
Key_InitTypeDef BOARD_KEYS[] = {
    {
        RCC_APB2Periph_GPIOA,     /**< GPIO时钟使能：GPIOA */
        GPIOA,                    /**< GPIO端口：GPIOA */
        GPIO_Pin_0,               /**< 引脚：PA0 */
        GPIO_PortSourceGPIOA,     /**< 端口源：GPIOA */
        GPIO_PinSource0,          /**< 引脚源：Pin0 */
        EXTI_Line0,               /**< 外部中断线：Line0 */
        EXTI0_IRQn,               /**< 中断号：EXTI0_IRQn */
        2,                        /**< 抢占优先级：2 */
        1,                        /**< 子优先级：1 */
        EXTI_Trigger_Rising,      /**< 触发方式：上升沿触发 */
        GPIO_Mode_IPD             /**< GPIO模式：下拉输入 */
    },
    {
        RCC_APB2Periph_GPIOB,     /**< GPIO时钟使能：GPIOC */
        GPIOB,                    /**< GPIO端口：GPIOC */
        GPIO_Pin_12,               /**< 引脚：PC5 */
        GPIO_PortSourceGPIOB,     /**< 端口源：GPIOC */
        GPIO_PinSource12,          /**< 引脚源：Pin5 */
        EXTI_Line12,               /**< 外部中断线：Line5 */
        EXTI15_10_IRQn,             /**< 中断号：EXTI9_5_IRQn */
        2,                        /**< 抢占优先级：2 */
        2,                        /**< 子优先级：2 */
        EXTI_Trigger_Falling,     /**< 触发方式：下降沿触发 */
        GPIO_Mode_IPU             /**< GPIO模式：上拉输入 */
    },
    {
        RCC_APB2Periph_GPIOB,     /**< GPIO时钟使能：GPIOC */
        GPIOB,                    /**< GPIO端口：GPIOC */
        GPIO_Pin_13,               /**< 引脚：PC6 */
        GPIO_PortSourceGPIOB,     /**< 端口源：GPIOC */
        GPIO_PinSource13,          /**< 引脚源：Pin6 */
        EXTI_Line13,               /**< 外部中断线：Line6 */
        EXTI15_10_IRQn,             /**< 中断号：EXTI9_5_IRQn */
        2,                        /**< 抢占优先级：2 */
        2,                        /**< 子优先级：2 */
        EXTI_Trigger_Falling,     /**< 触发方式：下降沿触发 */
        GPIO_Mode_IPU             /**< GPIO模式：上拉输入 */
    },
    {
        RCC_APB2Periph_GPIOB,     /**< GPIO时钟使能：GPIOC */
        GPIOB,                    /**< GPIO端口：GPIOC */
        GPIO_Pin_14,               /**< 引脚：PC7 */
        GPIO_PortSourceGPIOB,     /**< 端口源：GPIOC */
        GPIO_PinSource14,          /**< 引脚源：Pin7 */
        EXTI_Line14,               /**< 外部中断线：Line7 */
        EXTI15_10_IRQn,             /**< 中断号：EXTI9_5_IRQn */
        2,                        /**< 抢占优先级：2 */
        2,                        /**< 子优先级：2 */
        EXTI_Trigger_Falling,     /**< 触发方式：下降沿触发 */
        GPIO_Mode_IPU             /**< GPIO模式：上拉输入 */
    },
	{
        RCC_APB2Periph_GPIOB,     /**< GPIO时钟使能：GPIOC */
        GPIOB,                    /**< GPIO端口：GPIOC */
        GPIO_Pin_15,               /**< 引脚：PC7 */
        GPIO_PortSourceGPIOB,     /**< 端口源：GPIOC */
        GPIO_PinSource15,          /**< 引脚源：Pin7 */
        EXTI_Line15,               /**< 外部中断线：Line7 */
        EXTI15_10_IRQn,             /**< 中断号：EXTI9_5_IRQn */
        2,                        /**< 抢占优先级：2 */
        2,                        /**< 子优先级：2 */
        EXTI_Trigger_Falling,     /**< 触发方式：下降沿触发 */
        GPIO_Mode_IPU             /**< GPIO模式：上拉输入 */
    }
};

/** 
 * @brief 按键数量常量
 * @details 计算并存储按键配置数组的元素个数
 */
const uint8_t BOARD_KEYS_NUM = sizeof(BOARD_KEYS) / sizeof(BOARD_KEYS[0]);

/** 
 * @brief 开发板ADC
 * @details ADC初始化
 */
ADC_ConfigTypeDef adc_config = {
	.GPIOx = GPIOA,
	.GPIO_Clock = RCC_APB2Periph_GPIOA,
	.GPIO_Pins = GPIO_Pin_4 | GPIO_Pin_5,
	.Channel_Num = 2,
	.Channels = {ADC_Channel_4, ADC_Channel_5},
	.ADCx = ADC1,
	.ADC_Clock = RCC_APB2Periph_ADC1,
	.ADC_Prescaler = RCC_PCLK2_Div6  // 72MHz/6=12MHz
};

Time_t currentTime;

// 初始化 PWM 配置结构体
PWM_Config window_pwm_config = {
	.GPIOx = GPIOA,
	.GPIO_Pin = GPIO_Pin_7,       // TIM3 CH2
	.TIMx = TIM3,
	.TIM_Channel = 2,
	.TIM_AFIO_Remap = 0,          // 无重映射
	.TIM_Prescaler = 72,
	.PWM_Frequency = 50,
	.Servo_Min_Pulse = 500,
	.Servo_Max_Pulse = 2500,
	.Open_Angle = 90,
	.Close_Angle = 0,
	.current_status = 0  // 初始状态为关闭
};

/************************************* 局部初始化函数 *******************************************************/

/**
 * @brief 初始化开发板LED
 * @details 配置开发板上LED的GPIO引脚，使其可以被控制
 * @return 无
 */
static void BOARD_InitLEDs(void) {
    // 初始化LED1 (PB0)
    LED_Init(&BOARD_LED1, GPIOB, GPIO_Pin_0);
    
    // 初始化LED2 (PB1)
    LED_Init(&BOARD_LED2, GPIOB, GPIO_Pin_1);
}

/**
 * @brief 初始化开发板蜂鸣器
 * @details 配置开发板上蜂鸣器的GPIO引脚，使其可以被控制
 * @return 无
 */
static void BOARD_InitBeep(void) {
    // 初始化蜂鸣器 (PB8)
    Beep_Init(&BOARD_BEEP, GPIOB, GPIO_Pin_8);
}

/**
 * @brief 初始化开发板USART
 * @details 配置开发板上的USART1和USART2串口，设置波特率、引脚等参数
 * @return 无
 */
static void BOARD_InitUSART(void) {
    // 配置USART1初始化参数
    USART_InitParams usart1_params = {
        .USARTx = USART1,                          /**< USART外设：USART1 */
        .RCC_APBxPeriph_USART = RCC_APB2Periph_USART1, /**< USART1时钟使能：APB2总线 */
        .RCC_APBxPeriph_GPIO = RCC_APB2Periph_GPIOA,  /**< GPIO时钟使能：APB2总线 */
        .GPIO_Pin_Tx = GPIO_Pin_9,                 /**< TX引脚：PA9 */
        .GPIO_Pin_Rx = GPIO_Pin_10,                /**< RX引脚：PA10 */
        .NVIC_IRQChannel = USART1_IRQn,            /**< 中断号：USART1_IRQn */
        .NVIC_IRQChannelPreemptionPriority = 0,    /**< 抢占优先级：0 */
        .NVIC_IRQChannelSubPriority = 2            /**< 子优先级：2 */
    };

    // 初始化USART1，波特率为115200
    Usart_Init(&usart1_params, 115200);
}

/**
 * @brief 初始化开发板按键
 * @details 配置开发板上按键的GPIO和中断参数，使其可以触发中断
 * @return 无
 */
static void BOARD_InitKeys(void) {
    Key_Init(BOARD_KEYS, BOARD_KEYS_NUM);
}

/**
 * @brief 初始化延时模块
 * @details 配置系统时钟，选择指定的定时器作为延时基准
 * @param timer 选择的定时器编号 (如 1 表示 TIM2)
 * @return 无
 */
static void BOARD_InitDelay(uint8_t timer) {
    // 初始化延时模块，选择 TIM2 作为延时定时器
    Delay_Init(timer);
}

/**
 * @brief 初始化板载ADC
 * @details 初始化板载ADC，配置GPIO引脚和通道
 * @return 无
 */
static void BOARD_InitADC(void) {
    if (Adc_Init(&adc_config) != 0) {
        // 处理初始化失败的情况
        while(1);  // 这里可以添加错误处理代码
    }
}

/**
 * @brief 初始化 RTC
 * @details 初始化 RTC 模块，使其能够正常工作
 * @return 无
 */
static void BOARD_InitRTC(void) {
    // 初始化 RTC
    RTC_Init();
}

/**
 * @brief 初始化板载PWM
 * @details 初始化板载PWM，配置GPIO引脚和通道
 * @return 无
 */
static void BOARD_InitPWM(void) {
    // 初始化 PWM
    PWM_Init(&window_pwm_config);
}

/************************************* 全局初始化函数 *******************************************************/

/**
 * @brief 初始化开发板上的所有外设
 * @details 调用各个外设的初始化函数，完成整个开发板的初始化工作
 * @return 无
 */
void BOARD_InitAll(void) {
    // 配置中断优先级分组为2 (2位抢占优先级，2位子优先级)
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	
    // 初始化LED
    BOARD_InitLEDs();				
	
    // 初始化蜂鸣器
    BOARD_InitBeep();						
	
    // 初始化USART
    BOARD_InitUSART();		

	// 初始化按键
    BOARD_InitKeys();  

	// 使用 TIM2 作为延时定时器
    BOARD_InitDelay(1);	
	
	// 初始化ADC
		BOARD_InitADC();
	
	// 初始化 RTC
    BOARD_InitRTC();
	
	// 初始化PWM
		BOARD_InitPWM();
}
