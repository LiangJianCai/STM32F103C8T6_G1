#include "board_config.h"

/**
 * @file    board_config.c
 * @brief   �����������������ʼ��
 * @details ���ļ�ʵ���˿�����������������������ʼ��������
 *          ����LED����������������SPI��USART��ģ��ĳ�ʼ����
 */

/************************************* �������� *******************************************************/

/* 1ms������ */
volatile uint32_t uwTick = 0;

/* �������� */
Env_t g_env;

/** 
 * @brief ������LED������
 * @details �����˿�����������LED�Ŀ��ƽṹ��
 */
LED_TypeDef BOARD_LED1; /**< ������LED1���� (PB0) */
LED_TypeDef BOARD_LED2; /**< ������LED2���� (PB1) */

/** 
 * @brief �����������������
 * @details �����˿������Ϸ������Ŀ��ƽṹ��
 */
Beep_TypeDef BOARD_BEEP; /**< ��������������� (PB8) */

/** 
 * @brief �����尴����������
 * @details �����˿�������4�����������ò���������GPIO���ж����õ�
 */
Key_InitTypeDef BOARD_KEYS[] = {
    {
        RCC_APB2Periph_GPIOA,     /**< GPIOʱ��ʹ�ܣ�GPIOA */
        GPIOA,                    /**< GPIO�˿ڣ�GPIOA */
        GPIO_Pin_0,               /**< ���ţ�PA0 */
        GPIO_PortSourceGPIOA,     /**< �˿�Դ��GPIOA */
        GPIO_PinSource0,          /**< ����Դ��Pin0 */
        EXTI_Line0,               /**< �ⲿ�ж��ߣ�Line0 */
        EXTI0_IRQn,               /**< �жϺţ�EXTI0_IRQn */
        2,                        /**< ��ռ���ȼ���2 */
        1,                        /**< �����ȼ���1 */
        EXTI_Trigger_Rising,      /**< ������ʽ�������ش��� */
        GPIO_Mode_IPD             /**< GPIOģʽ���������� */
    },
    {
        RCC_APB2Periph_GPIOB,     /**< GPIOʱ��ʹ�ܣ�GPIOC */
        GPIOB,                    /**< GPIO�˿ڣ�GPIOC */
        GPIO_Pin_12,               /**< ���ţ�PC5 */
        GPIO_PortSourceGPIOB,     /**< �˿�Դ��GPIOC */
        GPIO_PinSource12,          /**< ����Դ��Pin5 */
        EXTI_Line12,               /**< �ⲿ�ж��ߣ�Line5 */
        EXTI15_10_IRQn,             /**< �жϺţ�EXTI9_5_IRQn */
        2,                        /**< ��ռ���ȼ���2 */
        2,                        /**< �����ȼ���2 */
        EXTI_Trigger_Falling,     /**< ������ʽ���½��ش��� */
        GPIO_Mode_IPU             /**< GPIOģʽ���������� */
    },
    {
        RCC_APB2Periph_GPIOB,     /**< GPIOʱ��ʹ�ܣ�GPIOC */
        GPIOB,                    /**< GPIO�˿ڣ�GPIOC */
        GPIO_Pin_13,               /**< ���ţ�PC6 */
        GPIO_PortSourceGPIOB,     /**< �˿�Դ��GPIOC */
        GPIO_PinSource13,          /**< ����Դ��Pin6 */
        EXTI_Line13,               /**< �ⲿ�ж��ߣ�Line6 */
        EXTI15_10_IRQn,             /**< �жϺţ�EXTI9_5_IRQn */
        2,                        /**< ��ռ���ȼ���2 */
        2,                        /**< �����ȼ���2 */
        EXTI_Trigger_Falling,     /**< ������ʽ���½��ش��� */
        GPIO_Mode_IPU             /**< GPIOģʽ���������� */
    },
    {
        RCC_APB2Periph_GPIOB,     /**< GPIOʱ��ʹ�ܣ�GPIOC */
        GPIOB,                    /**< GPIO�˿ڣ�GPIOC */
        GPIO_Pin_14,               /**< ���ţ�PC7 */
        GPIO_PortSourceGPIOB,     /**< �˿�Դ��GPIOC */
        GPIO_PinSource14,          /**< ����Դ��Pin7 */
        EXTI_Line14,               /**< �ⲿ�ж��ߣ�Line7 */
        EXTI15_10_IRQn,             /**< �жϺţ�EXTI9_5_IRQn */
        2,                        /**< ��ռ���ȼ���2 */
        2,                        /**< �����ȼ���2 */
        EXTI_Trigger_Falling,     /**< ������ʽ���½��ش��� */
        GPIO_Mode_IPU             /**< GPIOģʽ���������� */
    },
	{
        RCC_APB2Periph_GPIOB,     /**< GPIOʱ��ʹ�ܣ�GPIOC */
        GPIOB,                    /**< GPIO�˿ڣ�GPIOC */
        GPIO_Pin_15,               /**< ���ţ�PC7 */
        GPIO_PortSourceGPIOB,     /**< �˿�Դ��GPIOC */
        GPIO_PinSource15,          /**< ����Դ��Pin7 */
        EXTI_Line15,               /**< �ⲿ�ж��ߣ�Line7 */
        EXTI15_10_IRQn,             /**< �жϺţ�EXTI9_5_IRQn */
        2,                        /**< ��ռ���ȼ���2 */
        2,                        /**< �����ȼ���2 */
        EXTI_Trigger_Falling,     /**< ������ʽ���½��ش��� */
        GPIO_Mode_IPU             /**< GPIOģʽ���������� */
    }
};

/** 
 * @brief ������������
 * @details ���㲢�洢�������������Ԫ�ظ���
 */
const uint8_t BOARD_KEYS_NUM = sizeof(BOARD_KEYS) / sizeof(BOARD_KEYS[0]);

/** 
 * @brief ������ADC
 * @details ADC��ʼ��
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

// ��ʼ�� PWM ���ýṹ��
PWM_Config window_pwm_config = {
	.GPIOx = GPIOA,
	.GPIO_Pin = GPIO_Pin_7,       // TIM3 CH2
	.TIMx = TIM3,
	.TIM_Channel = 2,
	.TIM_AFIO_Remap = 0,          // ����ӳ��
	.TIM_Prescaler = 72,
	.PWM_Frequency = 50,
	.Servo_Min_Pulse = 500,
	.Servo_Max_Pulse = 2500,
	.Open_Angle = 90,
	.Close_Angle = 0,
	.current_status = 0  // ��ʼ״̬Ϊ�ر�
};

/************************************* �ֲ���ʼ������ *******************************************************/

/**
 * @brief ��ʼ��������LED
 * @details ���ÿ�������LED��GPIO���ţ�ʹ����Ա�����
 * @return ��
 */
static void BOARD_InitLEDs(void) {
    // ��ʼ��LED1 (PB0)
    LED_Init(&BOARD_LED1, GPIOB, GPIO_Pin_0);
    
    // ��ʼ��LED2 (PB1)
    LED_Init(&BOARD_LED2, GPIOB, GPIO_Pin_1);
}

/**
 * @brief ��ʼ�������������
 * @details ���ÿ������Ϸ�������GPIO���ţ�ʹ����Ա�����
 * @return ��
 */
static void BOARD_InitBeep(void) {
    // ��ʼ�������� (PB8)
    Beep_Init(&BOARD_BEEP, GPIOB, GPIO_Pin_8);
}

/**
 * @brief ��ʼ��������USART
 * @details ���ÿ������ϵ�USART1��USART2���ڣ����ò����ʡ����ŵȲ���
 * @return ��
 */
static void BOARD_InitUSART(void) {
    // ����USART1��ʼ������
    USART_InitParams usart1_params = {
        .USARTx = USART1,                          /**< USART���裺USART1 */
        .RCC_APBxPeriph_USART = RCC_APB2Periph_USART1, /**< USART1ʱ��ʹ�ܣ�APB2���� */
        .RCC_APBxPeriph_GPIO = RCC_APB2Periph_GPIOA,  /**< GPIOʱ��ʹ�ܣ�APB2���� */
        .GPIO_Pin_Tx = GPIO_Pin_9,                 /**< TX���ţ�PA9 */
        .GPIO_Pin_Rx = GPIO_Pin_10,                /**< RX���ţ�PA10 */
        .NVIC_IRQChannel = USART1_IRQn,            /**< �жϺţ�USART1_IRQn */
        .NVIC_IRQChannelPreemptionPriority = 0,    /**< ��ռ���ȼ���0 */
        .NVIC_IRQChannelSubPriority = 2            /**< �����ȼ���2 */
    };

    // ��ʼ��USART1��������Ϊ115200
    Usart_Init(&usart1_params, 115200);
}

/**
 * @brief ��ʼ�������尴��
 * @details ���ÿ������ϰ�����GPIO���жϲ�����ʹ����Դ����ж�
 * @return ��
 */
static void BOARD_InitKeys(void) {
    Key_Init(BOARD_KEYS, BOARD_KEYS_NUM);
}

/**
 * @brief ��ʼ����ʱģ��
 * @details ����ϵͳʱ�ӣ�ѡ��ָ���Ķ�ʱ����Ϊ��ʱ��׼
 * @param timer ѡ��Ķ�ʱ����� (�� 1 ��ʾ TIM2)
 * @return ��
 */
static void BOARD_InitDelay(uint8_t timer) {
    // ��ʼ����ʱģ�飬ѡ�� TIM2 ��Ϊ��ʱ��ʱ��
    Delay_Init(timer);
}

/**
 * @brief ��ʼ������ADC
 * @details ��ʼ������ADC������GPIO���ź�ͨ��
 * @return ��
 */
static void BOARD_InitADC(void) {
    if (Adc_Init(&adc_config) != 0) {
        // �����ʼ��ʧ�ܵ����
        while(1);  // ���������Ӵ��������
    }
}

/**
 * @brief ��ʼ�� RTC
 * @details ��ʼ�� RTC ģ�飬ʹ���ܹ���������
 * @return ��
 */
static void BOARD_InitRTC(void) {
    // ��ʼ�� RTC
    RTC_Init();
}

/**
 * @brief ��ʼ������PWM
 * @details ��ʼ������PWM������GPIO���ź�ͨ��
 * @return ��
 */
static void BOARD_InitPWM(void) {
    // ��ʼ�� PWM
    PWM_Init(&window_pwm_config);
}

/************************************* ȫ�ֳ�ʼ������ *******************************************************/

/**
 * @brief ��ʼ���������ϵ���������
 * @details ���ø�������ĳ�ʼ���������������������ĳ�ʼ������
 * @return ��
 */
void BOARD_InitAll(void) {
    // �����ж����ȼ�����Ϊ2 (2λ��ռ���ȼ���2λ�����ȼ�)
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	
    // ��ʼ��LED
    BOARD_InitLEDs();				
	
    // ��ʼ��������
    BOARD_InitBeep();						
	
    // ��ʼ��USART
    BOARD_InitUSART();		

	// ��ʼ������
    BOARD_InitKeys();  

	// ʹ�� TIM2 ��Ϊ��ʱ��ʱ��
    BOARD_InitDelay(1);	
	
	// ��ʼ��ADC
		BOARD_InitADC();
	
	// ��ʼ�� RTC
    BOARD_InitRTC();
	
	// ��ʼ��PWM
		BOARD_InitPWM();
}
