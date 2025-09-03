#include "module_config.h"

/************************************************** 宏定义 *************************************************************/
/**
 * @def MAX_INIT_RETRIES
 * @brief 模块初始化的最大重试次数
 * @details 定义了模块初始化失败时的最大重试次数为 5 次。
 */
#define MAX_INIT_RETRIES 5

/************************************************** 全局变量 ***********************************************************/
/**
 * @var OLED_Handle
 * @brief OLED 模块的句柄
 * @details 用于存储 OLED 模块的相关配置信息，包括 SCL 和 SDA 的 GPIO 端口和引脚。
 */
OLED_HandleTypeDef OLED_Handle = {
    .SCL_GPIOx = GPIOB,
    .SCL_Pin = GPIO_Pin_6,
    .SDA_GPIOx = GPIOB,
    .SDA_Pin = GPIO_Pin_7
};

// 定义 3 个 GPIO 输入结构体
/**
 * @var MODULE_GPIO_INPUT1
 * @brief 第一个 GPIO 输入模块
 * @details 用于配置和存储第一个 GPIO 输入的相关信息，包括 GPIO 端口、引脚、模式和速度，初始状态为高电平。
 */
GPIO_Input_TypeDef MODULE_GPIO_INPUT1 = {
    .config = {
        .GPIOx = GPIOA,
        .GPIO_Pin = GPIO_Pin_1,
        .GPIO_Mode = GPIO_Mode_IPU,
        .GPIO_Speed = GPIO_Speed_50MHz
    },
    .state = GPIO_INPUT_HIGH
};
/**
 * @var MODULE_GPIO_INPUT2
 * @brief 第二个 GPIO 输入模块
 * @details 用于配置和存储第二个 GPIO 输入的相关信息，包括 GPIO 端口、引脚、模式和速度，初始状态为高电平。
 */
GPIO_Input_TypeDef MODULE_GPIO_INPUT2 = {
    .config = {
        .GPIOx = GPIOA,
        .GPIO_Pin = GPIO_Pin_2,
        .GPIO_Mode = GPIO_Mode_IPU,
        .GPIO_Speed = GPIO_Speed_50MHz
    },
    .state = GPIO_INPUT_HIGH
};
/**
 * @var MODULE_GPIO_INPUT3
 * @brief 第三个 GPIO 输入模块
 * @details 用于配置和存储第三个 GPIO 输入的相关信息，包括 GPIO 端口、引脚、模式和速度，初始状态为高电平。
 */
GPIO_Input_TypeDef MODULE_GPIO_INPUT3 = {
    .config = {
        .GPIOx = GPIOA,
        .GPIO_Pin = GPIO_Pin_3,
        .GPIO_Mode = GPIO_Mode_IPU,
        .GPIO_Speed = GPIO_Speed_50MHz
    },
    .state = GPIO_INPUT_HIGH
};

/**
 * @var MODULE_GPIO_OUTPUT1
 * @brief 第一个 GPIO 输出模块
 * @details 用于配置和存储第一个 GPIO 输出的相关信息，包括 GPIO 端口、引脚、模式和速度。
 */
GPIO_Output_TypeDef MODULE_GPIO_OUTPUT1 = {
    // 配置 GPIO
    .config.GPIOx = GPIOA,
    .config.GPIO_Pin = GPIO_Pin_8,
    .config.GPIO_Mode = GPIO_Mode_Out_PP,
    .config.GPIO_Speed = GPIO_Speed_50MHz
};

/**
 * @var MODULE_DHT11
 * @brief DHT11 温湿度传感器的句柄
 * @details 用于存储 DHT11 温湿度传感器的相关配置信息，包括 GPIO 端口和引脚。
 */
DHT11_HandleTypeDef MODULE_DHT11 = {
    .GPIOx = GPIOA,
    .GPIO_Pin = GPIO_Pin_11
};

// 定义 DS18B20 设备连接的 GPIO 端口和引脚
/**
 * @var ds18b20_device
 * @brief DS18B20 温度传感器的设备结构体
 * @details 用于存储 DS18B20 温度传感器的相关配置信息，包括 GPIO 端口和引脚。
 */
DS18B20_HandleTypeDef MODULE_DS18B20 = {
    .GPIOx = GPIOA,
    .GPIO_Pin = GPIO_Pin_12
};

/************************************* 局部初始化函数 *******************************************************/
/**
 * @brief 初始化 OLED 模块
 * @details 该函数用于初始化 OLED 模块，并显示初始化成功信息。
 * @return 无
 */
static void BOARD_InitOLED(void) {
    OLED_Init(&OLED_Handle);  // OLED 初始化
}

/**
 * @brief 初始化 GPIO 输入模块
 * @details 该函数用于初始化 3 个 GPIO 输入模块。
 * @return 无
 */
static void MODULE_InitGPIOInputs(void) {
    GPIO_Input_Init(&MODULE_GPIO_INPUT1);
    GPIO_Input_Init(&MODULE_GPIO_INPUT2);
    GPIO_Input_Init(&MODULE_GPIO_INPUT3);
}

/**
 * @brief 初始化 GPIO 输出模块
 * @details 该函数用于初始化 1 个 GPIO 输出模块。
 * @return 无
 */
static void MODULE_InitGPIOOutputs(void) {
    GPIO_Output_Init(&MODULE_GPIO_OUTPUT1);
}

/**
 * @brief 初始化 DHT11 温湿度传感器
 * @details 该函数用于初始化 DHT11 温湿度传感器的 GPIO 配置，若初始化失败则进行重试。
 * @note 如果重试次数达到 MAX_INIT_RETRIES 仍失败，则显示错误信息并进入无限循环。
 * @return 无
 */
static void MODULE_InitDHT11(void) {
    uint8_t retry_count = 0;

    while (DHT11_Init(&MODULE_DHT11) && retry_count < MAX_INIT_RETRIES) {
        OLED_ShowString(&OLED_Handle, 1, 1, "DHT11 Error");

        DelayMs(200);

        OLED_Clear(&OLED_Handle);

        DelayMs(200);

        retry_count++;
    }

    if (retry_count >= MAX_INIT_RETRIES) {
        OLED_ShowString(&OLED_Handle, 1, 1, "DHT11 Init Failed");
        while (1); // 可修改此部分逻辑
    }
}

/**
 * @brief 初始化 DS18B20 温度传感器
 * @details 该函数用于初始化 DS18B20 温度传感器的 GPIO 配置，若初始化失败则进行重试。
 * @note 如果重试次数达到 MAX_INIT_RETRIES 仍失败，则显示错误信息并进入无限循环。
 * @return 无
 */
static void MODULE_InitDS18B20(void) {
    uint8_t retry_count = 0;

    while (DS18B20_Init(&MODULE_DS18B20) && retry_count < MAX_INIT_RETRIES) {
        OLED_ShowString(&OLED_Handle, 1, 1, "DS18B20 Error!");

        DelayMs(200);

        OLED_Clear(&OLED_Handle);

        DelayMs(200);

        retry_count++;
    }

    if (retry_count >= MAX_INIT_RETRIES) {
        OLED_ShowString(&OLED_Handle, 1, 1, "DS18B20 Init Failed");
        while (1); // 可修改此部分逻辑
    }
}

/************************************* 全局初始化函数 *******************************************************/
/**
 * @brief 初始化所有模块
 * @details 该函数用于初始化所有模块，包括 OLED 模块、GPIO 输入模块、GPIO 输出模块、DHT11 温湿度传感器和 DS18B20 温度传感器。
 * @return 无
 */
void MODULE_InitAll(void) {
    BOARD_InitOLED();  // OLED 模块初始化
	
		MODULE_InitGPIOInputs(); // 初始化 GPIO 输入
	
		MODULE_InitGPIOOutputs();    // 初始化 GPIO 输出
	
		//MODULE_InitDHT11();         // 初始化 DHT11 温湿度传感器
	
		//MODULE_InitDS18B20();         // 初始化 DS18B20 温度传感器
	
    OLED_ShowString(&OLED_Handle, 1, 1, "MODULE_Init OK!");
}
