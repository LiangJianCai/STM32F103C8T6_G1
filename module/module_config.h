#ifndef MODULE_CONFIG_H
#define MODULE_CONFIG_H

#include "delay.h"
#include "OLED.h"
#include "gpio_input.h"
#include "gpio_output.h"
#include "dht11.h"
#include "ds18b20.h"


/**
 * @var OLED_Handle
 * @brief 导出 OLED 对象供外部使用
 * @details 外部可以通过该对象访问 OLED 模块的相关配置信息。
 */
extern OLED_HandleTypeDef OLED_Handle;

/**
 * @var MODULE_GPIO_INPUT1
 * @brief 导出第一个 GPIO 输入对象供外部使用
 * @details 外部可以通过该对象访问第一个 GPIO 输入模块的相关配置信息。
 */
extern GPIO_Input_TypeDef MODULE_GPIO_INPUT1;
/**
 * @var MODULE_GPIO_INPUT2
 * @brief 导出第二个 GPIO 输入对象供外部使用
 * @details 外部可以通过该对象访问第二个 GPIO 输入模块的相关配置信息。
 */
extern GPIO_Input_TypeDef MODULE_GPIO_INPUT2;
/**
 * @var MODULE_GPIO_INPUT3
 * @brief 导出第三个 GPIO 输入对象供外部使用
 * @details 外部可以通过该对象访问第三个 GPIO 输入模块的相关配置信息。
 */
extern GPIO_Input_TypeDef MODULE_GPIO_INPUT3;

/**
 * @var MODULE_GPIO_OUTPUT1
 * @brief 导出第一个 GPIO 输出对象供外部使用
 * @details 外部可以通过该对象访问第一个 GPIO 输出模块的相关配置信息。
 */
extern GPIO_Output_TypeDef MODULE_GPIO_OUTPUT1;

/**
 * @var MODULE_DHT11
 * @brief 导出 DHT11 对象供外部使用
 * @details 外部可以通过该对象访问 DHT11 温湿度传感器的相关配置信息。
 */
extern DHT11_HandleTypeDef MODULE_DHT11;

/**
 * @var ds18b20_device
 * @brief 导出 DS18B20 对象供外部使用
 * @details 外部可以通过该对象访问 DS18B20 温度传感器的相关配置信息。
 */
extern DS18B20_HandleTypeDef MODULE_DS18B20;

/**
 * @brief 模块初始化函数
 * @details 调用该函数可以初始化所有模块。
 * @return 无
 */
void MODULE_InitAll(void);

#endif /* MODULE_CONFIG_H */
