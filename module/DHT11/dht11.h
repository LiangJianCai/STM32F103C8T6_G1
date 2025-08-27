/**
 * @file    dht11.h
 * @brief   DHT11温湿度传感器驱动头文件
 * @details 提供DHT11传感器的初始化、数据读取等功能接口
 */

#ifndef __DHT11_H
#define __DHT11_H 

#include "system_config.h"

/**
 * @struct  DHT11_HandleTypeDef
 * @brief   DHT11传感器句柄结构体
 * @details 包含DHT11传感器连接的GPIO端口和引脚信息
 */
typedef struct {
    GPIO_TypeDef* GPIOx;  /**< GPIO端口，如GPIOA、GPIOB等 */
    uint16_t GPIO_Pin;    /**< GPIO引脚，如GPIO_Pin_5 */
} DHT11_HandleTypeDef;

/**
 * @def     DHT11_SET_IO_IN(handle)
 * @brief   设置DHT11数据线为输入模式
 * @param   handle DHT11句柄指针
 * @details 配置指定GPIO引脚为浮空输入模式，支持Pin0~Pin15
 */
#define DHT11_SET_IO_IN(handle) { \
    uint8_t pin_pos = 0; \
    while (pin_pos < 16 && !((handle->GPIO_Pin) & (1 << pin_pos))) pin_pos++; \
    if (pin_pos < 8) { \
        handle->GPIOx->CRL &= ~(0x0F << (pin_pos * 4)); \
        handle->GPIOx->CRL |= (8 << (pin_pos * 4)); \
    } else { \
        pin_pos -= 8; \
        handle->GPIOx->CRH &= ~(0x0F << (pin_pos * 4)); \
        handle->GPIOx->CRH |= (8 << (pin_pos * 4)); \
    } \
}

/**
 * @def     DHT11_SET_IO_OUT(handle)
 * @brief   设置DHT11数据线为输出模式
 * @param   handle DHT11句柄指针
 * @details 配置指定GPIO引脚为推挽输出模式，支持Pin0~Pin15
 */
#define DHT11_SET_IO_OUT(handle) { \
    uint8_t pin_pos = 0; \
    while (pin_pos < 16 && !((handle->GPIO_Pin) & (1 << pin_pos))) pin_pos++; \
    if (pin_pos < 8) { \
        handle->GPIOx->CRL &= ~(0x0F << (pin_pos * 4)); \
        handle->GPIOx->CRL |= (3 << (pin_pos * 4)); \
    } else { \
        pin_pos -= 8; \
        handle->GPIOx->CRH &= ~(0x0F << (pin_pos * 4)); \
        handle->GPIOx->CRH |= (3 << (pin_pos * 4)); \
    } \
}

/**
 * @def     DHT11_DQ_OUT(handle)
 * @brief   DHT11数据线输出寄存器
 * @param   handle DHT11句柄指针
 * @return  对应GPIO端口的输出寄存器
 */
#define DHT11_DQ_OUT(handle) (handle->GPIOx->ODR)

/**
 * @def     DHT11_DQ_IN(handle)
 * @brief   读取DHT11数据线状态
 * @param   handle DHT11句柄指针
 * @return  数据线状态，1:高电平，0:低电平
 */
#define DHT11_DQ_IN(handle) ((handle->GPIOx->IDR & handle->GPIO_Pin) != 0)

/**
 * @def     DHT11_DQ_LOW(handle)
 * @brief   设置DHT11数据线为低电平
 * @param   handle DHT11句柄指针
 */
#define DHT11_DQ_LOW(handle) (handle->GPIOx->ODR &= ~handle->GPIO_Pin)

/**
 * @def     DHT11_DQ_HIGH(handle)
 * @brief   设置DHT11数据线为高电平
 * @param   handle DHT11句柄指针
 */
#define DHT11_DQ_HIGH(handle) (handle->GPIOx->ODR |= handle->GPIO_Pin)
/**
 * @brief  初始化DHT11传感器
 * @param  handle DHT11句柄指针
 * @return uint8_t 初始化结果，0:成功，1:失败
 * @details 配置GPIO并等待DHT11响应
 */
uint8_t DHT11_Init(DHT11_HandleTypeDef* handle);

/**
 * @brief  读取DHT11传感器的温湿度数据
 * @param  handle DHT11句柄指针
 * @param  temp 温度值指针（范围:0~50°C）
 * @param  humi 湿度值指针（范围:20%~90%）
 * @return uint8_t 读取结果，0:成功，1:失败
 * @details 从DHT11读取一次温湿度数据
 */
uint8_t DHT11_Read_Data(DHT11_HandleTypeDef* handle, uint8_t *temp, uint8_t *humi);

/**
 * @brief  从DHT11读取一个字节的数据
 * @param  handle DHT11句柄指针
 * @return uint8_t 读取的字节数据
 * @details 从DHT11传感器上读取8位数据
 */
uint8_t DHT11_Read_Byte(DHT11_HandleTypeDef* handle);

/**
 * @brief  从DHT11读取一位数据
 * @param  handle DHT11句柄指针
 * @return uint8_t 读取的位数据，0或1
 * @details 从DHT11传感器上读取1位数据
 */
uint8_t DHT11_Read_Bit(DHT11_HandleTypeDef* handle);

/**
 * @brief  检测DHT11传感器的响应
 * @param  handle DHT11句柄指针
 * @return uint8_t 检测结果，0:成功，1:失败
 * @details 发送复位信号后，等待DHT11响应信号
 */
uint8_t DHT11_Check(DHT11_HandleTypeDef* handle);

/**
 * @brief  复位DHT11传感器
 * @param  handle DHT11句柄指针
 * @details 发送复位信号到DHT11传感器
 */
void DHT11_Rst(DHT11_HandleTypeDef* handle);

#endif
