#ifndef MODULE_CONFIG_H
#define MODULE_CONFIG_H

#include "delay.h"
#include "OLED.h"
#include "gpio_input.h"
#include "gpio_output.h"
#include "dht11.h"
#include "ds18b20.h"

/************************************************
 * @brief 淘宝店铺链接
 * @details 提供淘宝店铺的链接，可访问相关商品。
 * @note 此链接指向 https://shop475501589.taobao.com/?spm=pc_detail.29232929/evo365560b447259.shop_block.dshopinfo.5dd97dd6JvMuG3
 ************************************************/
/************************************************
 * @brief 咸鱼店铺链接
 * @details 提供咸鱼店铺的链接，可查看相关物品。
 * @note 此链接指向 https://www.goofish.com/personal?spm=a21ybx.item.itemHeader.1.c17a3da6hy8k28&userId=3890583014
 ************************************************/
/************************************************
 * @brief 哔哩哔哩空间链接
 * @details 提供哔哩哔哩空间的链接，可查看相关内容。
 * @note 此链接指向 https://space.bilibili.com/482024430?spm_id_from=333.788.upinfo.detail.click
 ************************************************/
/************************************************
 * @brief 作者信息
 * @details 代码的作者为胜磊电子。
 ************************************************/

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
