/**
 * @file    dht11.h
 * @brief   DHT11��ʪ�ȴ���������ͷ�ļ�
 * @details �ṩDHT11�������ĳ�ʼ�������ݶ�ȡ�ȹ��ܽӿ�
 */

#ifndef __DHT11_H
#define __DHT11_H 

#include "system_config.h"

/**
 * @struct  DHT11_HandleTypeDef
 * @brief   DHT11����������ṹ��
 * @details ����DHT11���������ӵ�GPIO�˿ں�������Ϣ
 */
typedef struct {
    GPIO_TypeDef* GPIOx;  /**< GPIO�˿ڣ���GPIOA��GPIOB�� */
    uint16_t GPIO_Pin;    /**< GPIO���ţ���GPIO_Pin_5 */
} DHT11_HandleTypeDef;

/**
 * @def     DHT11_SET_IO_IN(handle)
 * @brief   ����DHT11������Ϊ����ģʽ
 * @param   handle DHT11���ָ��
 * @details ����ָ��GPIO����Ϊ��������ģʽ��֧��Pin0~Pin15
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
 * @brief   ����DHT11������Ϊ���ģʽ
 * @param   handle DHT11���ָ��
 * @details ����ָ��GPIO����Ϊ�������ģʽ��֧��Pin0~Pin15
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
 * @brief   DHT11����������Ĵ���
 * @param   handle DHT11���ָ��
 * @return  ��ӦGPIO�˿ڵ�����Ĵ���
 */
#define DHT11_DQ_OUT(handle) (handle->GPIOx->ODR)

/**
 * @def     DHT11_DQ_IN(handle)
 * @brief   ��ȡDHT11������״̬
 * @param   handle DHT11���ָ��
 * @return  ������״̬��1:�ߵ�ƽ��0:�͵�ƽ
 */
#define DHT11_DQ_IN(handle) ((handle->GPIOx->IDR & handle->GPIO_Pin) != 0)

/**
 * @def     DHT11_DQ_LOW(handle)
 * @brief   ����DHT11������Ϊ�͵�ƽ
 * @param   handle DHT11���ָ��
 */
#define DHT11_DQ_LOW(handle) (handle->GPIOx->ODR &= ~handle->GPIO_Pin)

/**
 * @def     DHT11_DQ_HIGH(handle)
 * @brief   ����DHT11������Ϊ�ߵ�ƽ
 * @param   handle DHT11���ָ��
 */
#define DHT11_DQ_HIGH(handle) (handle->GPIOx->ODR |= handle->GPIO_Pin)
/**
 * @brief  ��ʼ��DHT11������
 * @param  handle DHT11���ָ��
 * @return uint8_t ��ʼ�������0:�ɹ���1:ʧ��
 * @details ����GPIO���ȴ�DHT11��Ӧ
 */
uint8_t DHT11_Init(DHT11_HandleTypeDef* handle);

/**
 * @brief  ��ȡDHT11����������ʪ������
 * @param  handle DHT11���ָ��
 * @param  temp �¶�ֵָ�루��Χ:0~50��C��
 * @param  humi ʪ��ֵָ�루��Χ:20%~90%��
 * @return uint8_t ��ȡ�����0:�ɹ���1:ʧ��
 * @details ��DHT11��ȡһ����ʪ������
 */
uint8_t DHT11_Read_Data(DHT11_HandleTypeDef* handle, uint8_t *temp, uint8_t *humi);

/**
 * @brief  ��DHT11��ȡһ���ֽڵ�����
 * @param  handle DHT11���ָ��
 * @return uint8_t ��ȡ���ֽ�����
 * @details ��DHT11�������϶�ȡ8λ����
 */
uint8_t DHT11_Read_Byte(DHT11_HandleTypeDef* handle);

/**
 * @brief  ��DHT11��ȡһλ����
 * @param  handle DHT11���ָ��
 * @return uint8_t ��ȡ��λ���ݣ�0��1
 * @details ��DHT11�������϶�ȡ1λ����
 */
uint8_t DHT11_Read_Bit(DHT11_HandleTypeDef* handle);

/**
 * @brief  ���DHT11����������Ӧ
 * @param  handle DHT11���ָ��
 * @return uint8_t �������0:�ɹ���1:ʧ��
 * @details ���͸�λ�źź󣬵ȴ�DHT11��Ӧ�ź�
 */
uint8_t DHT11_Check(DHT11_HandleTypeDef* handle);

/**
 * @brief  ��λDHT11������
 * @param  handle DHT11���ָ��
 * @details ���͸�λ�źŵ�DHT11������
 */
void DHT11_Rst(DHT11_HandleTypeDef* handle);

#endif
