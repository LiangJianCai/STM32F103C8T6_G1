/**
 * @file    dht11.c
 * @brief   DHT11温湿度传感器驱动实现文件
 * @details 实现DHT11传感器的初始化、数据读取等功能
 */

#include "dht11.h"
#include "delay.h"


// 使能GPIO时钟的辅助函数
static void DHT11_EnableGPIOClock(GPIO_TypeDef* GPIOx) {
    if (GPIOx == GPIOA)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if (GPIOx == GPIOB)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if (GPIOx == GPIOC)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else if (GPIOx == GPIOD)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    else if (GPIOx == GPIOE)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    else if (GPIOx == GPIOF)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    else if (GPIOx == GPIOG)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
}

/**
 * @brief  复位DHT11传感器
 * @param  handle DHT11句柄指针
 * @details 发送复位信号到DHT11传感器，拉低数据线至少18ms，然后拉高20 - 40us
 */
void DHT11_Rst(DHT11_HandleTypeDef* handle) {
    DHT11_SET_IO_OUT(handle);  // 设置为输出模式
    DHT11_DQ_LOW(handle);      // 拉低DQ
    DelayMs(20);               // 拉低至少18ms
    DHT11_DQ_HIGH(handle);     // DQ=1
    DelayUs(30);               // 拉高20 - 40us
}

/**
 * @brief  检测DHT11传感器的响应
 * @param  handle DHT11句柄指针
 * @return uint8_t 检测结果，1:未检测到响应，0:成功
 * @details 发送复位信号后，等待DHT11拉低40 - 80us作为响应，然后再拉高40 - 80us
 */
uint8_t DHT11_Check(DHT11_HandleTypeDef* handle) {
    uint8_t retry = 0;
    DHT11_SET_IO_IN(handle);  // 设置为输入模式

    // 等待DHT11拉低40 - 80us
    while (DHT11_DQ_IN(handle) && retry < 100) {
        retry++;
        DelayUs(1);
    }
    if (retry >= 100) return 1;

    retry = 0;
    // 等待DHT11拉高40 - 80us
    while (!DHT11_DQ_IN(handle) && retry < 100) {
        retry++;
        DelayUs(1);
    }
    if (retry >= 100) return 1;

    return 0;
}

/**
 * @brief  从DHT11读取一位数据
 * @param  handle DHT11句柄指针
 * @return uint8_t 读取的位数据，0或1
 * @details 通过判断高电平持续时间来确定是0还是1
 */
uint8_t DHT11_Read_Bit(DHT11_HandleTypeDef* handle) {
    uint8_t retry = 0;

    // 等待变为低电平
    while (DHT11_DQ_IN(handle) && retry < 100) {
        retry++;
        DelayUs(1);
    }

    retry = 0;
    // 等待变为高电平
    while (!DHT11_DQ_IN(handle) && retry < 100) {
        retry++;
        DelayUs(1);
    }

    DelayUs(40);  // 等待40us，此时高电平表示1，低电平表示0
    if (DHT11_DQ_IN(handle)) return 1;
    else return 0;
}

/**
 * @brief  从DHT11读取一个字节的数据
 * @param  handle DHT11句柄指针
 * @return uint8_t 读取的字节数据
 * @details 依次读取8位数据，组成一个字节
 */
uint8_t DHT11_Read_Byte(DHT11_HandleTypeDef* handle) {
    uint8_t i, dat;
    dat = 0;

    for (i = 0; i < 8; i++) {
        dat <<= 1;
        dat |= DHT11_Read_Bit(handle);
    }

    return dat;
}

/**
 * @brief  读取DHT11传感器的温湿度数据
 * @param  handle DHT11句柄指针
 * @param  temp 温度值指针
 * @param  humi 湿度值指针
 * @return uint8_t 读取结果，0:成功，1:失败
 * @details 读取DHT11的40位数据，包括湿度整数8位 + 湿度小数8位 + 温度整数8位 + 温度小数8位 + 校验和8位
 */
uint8_t DHT11_Read_Data(DHT11_HandleTypeDef* handle, uint8_t *temp, uint8_t *humi) {
    uint8_t buf[5];  // 存储5个字节数据
    uint8_t i;

    DHT11_Rst(handle);  // 复位DHT11

    if (DHT11_Check(handle) == 0) {  // 检测DHT11响应
        for (i = 0; i < 5; i++) {    // 读取40位数据
            buf[i] = DHT11_Read_Byte(handle);
        }

        // 校验数据
        uint8_t checksum = (buf[0] + buf[1] + buf[2] + buf[3]) & 0xFF;
        if (checksum == buf[4]) {
            *humi = buf[0];  // 湿度整数部分
            *temp = buf[2];  // 温度整数部分
        } else {
            return 1;
        }
    } else {
        return 1;
    }

    return 0;
}

/**
 * @brief  初始化DHT11传感器
 * @param  handle DHT11句柄指针
 * @return uint8_t 初始化结果，0:成功，1:失败
 * @details 配置GPIO并等待DHT11响应
 */
uint8_t DHT11_Init(DHT11_HandleTypeDef* handle) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIO时钟
    DHT11_EnableGPIOClock(handle->GPIOx);

    // 配置GPIO为推挽输出
    GPIO_InitStructure.GPIO_Pin = handle->GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(handle->GPIOx, &GPIO_InitStructure);

    GPIO_SetBits(handle->GPIOx, handle->GPIO_Pin);  // 初始拉高

    DHT11_Rst(handle);  // 复位DHT11

    return DHT11_Check(handle);  // 等待DHT11响应

}
