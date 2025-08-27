#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"

/**
 * @brief OLED����ṹ��
 */
typedef struct {
    GPIO_TypeDef* SCL_GPIOx;  /**< SCL�������ڵ�GPIO�˿� */
    uint16_t SCL_Pin;         /**< SCL���ű�� */
    GPIO_TypeDef* SDA_GPIOx;  /**< SDA�������ڵ�GPIO�˿� */
    uint16_t SDA_Pin;         /**< SDA���ű�� */
} OLED_HandleTypeDef;

/**
 * @brief ��ʼ��OLED
 * @param OLED_Handle ָ��OLED����ṹ���ָ��
 */
void OLED_Init(OLED_HandleTypeDef* OLED_Handle);

/**
 * @brief ���OLED��Ļ
 * @param OLED_Handle ָ��OLED����ṹ���ָ��
 */
void OLED_Clear(OLED_HandleTypeDef* OLED_Handle);

/**
 * @brief ��ʾһ���ַ�
 * @param OLED_Handle ָ��OLED����ṹ���ָ��
 * @param Line ��λ�ã���Χ��1~4
 * @param Column ��λ�ã���Χ��1~16
 * @param Char Ҫ��ʾ��һ���ַ�����Χ��ASCII�ɼ��ַ�
 */
void OLED_ShowChar(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, char Char);

/**
 * @brief ��ʾ�ַ���
 * @param OLED_Handle ָ��OLED����ṹ���ָ��
 * @param Line ��ʼ��λ�ã���Χ��1~4
 * @param Column ��ʼ��λ�ã���Χ��1~16
 * @param String Ҫ��ʾ���ַ�������Χ��ASCII�ɼ��ַ�
 */
void OLED_ShowString(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, char *String);

/**
 * @brief ��ʾ���֣�ʮ���ƣ�������
 * @param OLED_Handle ָ��OLED����ṹ���ָ��
 * @param Line ��ʼ��λ�ã���Χ��1~4
 * @param Column ��ʼ��λ�ã���Χ��1~16
 * @param Number Ҫ��ʾ�����֣���Χ��0~4294967295
 * @param Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
 */
void OLED_ShowNum(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

/**
 * @brief ��ʾ���֣�ʮ���ƣ�����������
 * @param OLED_Handle ָ��OLED����ṹ���ָ��
 * @param Line ��ʼ��λ�ã���Χ��1~4
 * @param Column ��ʼ��λ�ã���Χ��1~16
 * @param Number Ҫ��ʾ�����֣���Χ��-2147483648~2147483647
 * @param Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
 */
void OLED_ShowSignedNum(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);

/**
 * @brief ��ʾ���֣�ʮ�����ƣ�������
 * @param OLED_Handle ָ��OLED����ṹ���ָ��
 * @param Line ��ʼ��λ�ã���Χ��1~4
 * @param Column ��ʼ��λ�ã���Χ��1~16
 * @param Number Ҫ��ʾ�����֣���Χ��0~0xFFFFFFFF
 * @param Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~8
 */
void OLED_ShowHexNum(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

/**
 * @brief ��ʾ���֣������ƣ�������
 * @param OLED_Handle ָ��OLED����ṹ���ָ��
 * @param Line ��ʼ��λ�ã���Χ��1~4
 * @param Column ��ʼ��λ�ã���Χ��1~16
 * @param Number Ҫ��ʾ�����֣���Χ��0~1111 1111 1111 1111
 * @param Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~16
 */
void OLED_ShowBinNum(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

/**
 * @brief ���OLED12864��ָ���п�ʼ�����µ���������
 * @param OLED_Handle ָ��OLED����ṹ���ָ��
 * @param row ��ʼ��
 */
void OLED_ClearFromSecondLine(OLED_HandleTypeDef* OLED_Handle, uint8_t row);

#endif
