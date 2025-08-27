#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"

/**
 * @brief OLED对象结构体
 */
typedef struct {
    GPIO_TypeDef* SCL_GPIOx;  /**< SCL引脚所在的GPIO端口 */
    uint16_t SCL_Pin;         /**< SCL引脚编号 */
    GPIO_TypeDef* SDA_GPIOx;  /**< SDA引脚所在的GPIO端口 */
    uint16_t SDA_Pin;         /**< SDA引脚编号 */
} OLED_HandleTypeDef;

/**
 * @brief 初始化OLED
 * @param OLED_Handle 指向OLED对象结构体的指针
 */
void OLED_Init(OLED_HandleTypeDef* OLED_Handle);

/**
 * @brief 清除OLED屏幕
 * @param OLED_Handle 指向OLED对象结构体的指针
 */
void OLED_Clear(OLED_HandleTypeDef* OLED_Handle);

/**
 * @brief 显示一个字符
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Line 行位置，范围：1~4
 * @param Column 列位置，范围：1~16
 * @param Char 要显示的一个字符，范围：ASCII可见字符
 */
void OLED_ShowChar(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, char Char);

/**
 * @brief 显示字符串
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Line 起始行位置，范围：1~4
 * @param Column 起始列位置，范围：1~16
 * @param String 要显示的字符串，范围：ASCII可见字符
 */
void OLED_ShowString(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, char *String);

/**
 * @brief 显示数字（十进制，正数）
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Line 起始行位置，范围：1~4
 * @param Column 起始列位置，范围：1~16
 * @param Number 要显示的数字，范围：0~4294967295
 * @param Length 要显示数字的长度，范围：1~10
 */
void OLED_ShowNum(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

/**
 * @brief 显示数字（十进制，带符号数）
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Line 起始行位置，范围：1~4
 * @param Column 起始列位置，范围：1~16
 * @param Number 要显示的数字，范围：-2147483648~2147483647
 * @param Length 要显示数字的长度，范围：1~10
 */
void OLED_ShowSignedNum(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);

/**
 * @brief 显示数字（十六进制，正数）
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Line 起始行位置，范围：1~4
 * @param Column 起始列位置，范围：1~16
 * @param Number 要显示的数字，范围：0~0xFFFFFFFF
 * @param Length 要显示数字的长度，范围：1~8
 */
void OLED_ShowHexNum(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

/**
 * @brief 显示数字（二进制，正数）
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Line 起始行位置，范围：1~4
 * @param Column 起始列位置，范围：1~16
 * @param Number 要显示的数字，范围：0~1111 1111 1111 1111
 * @param Length 要显示数字的长度，范围：1~16
 */
void OLED_ShowBinNum(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

/**
 * @brief 清除OLED12864从指定行开始及以下的所有内容
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param row 起始行
 */
void OLED_ClearFromSecondLine(OLED_HandleTypeDef* OLED_Handle, uint8_t row);

#endif
