/**
 * @file    dht11.c
 * @brief   DHT11��ʪ�ȴ���������ʵ���ļ�
 * @details ʵ��DHT11�������ĳ�ʼ�������ݶ�ȡ�ȹ���
 */

#include "dht11.h"
#include "delay.h"


// ʹ��GPIOʱ�ӵĸ�������
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
 * @brief  ��λDHT11������
 * @param  handle DHT11���ָ��
 * @details ���͸�λ�źŵ�DHT11����������������������18ms��Ȼ������20 - 40us
 */
void DHT11_Rst(DHT11_HandleTypeDef* handle) {
    DHT11_SET_IO_OUT(handle);  // ����Ϊ���ģʽ
    DHT11_DQ_LOW(handle);      // ����DQ
    DelayMs(20);               // ��������18ms
    DHT11_DQ_HIGH(handle);     // DQ=1
    DelayUs(30);               // ����20 - 40us
}

/**
 * @brief  ���DHT11����������Ӧ
 * @param  handle DHT11���ָ��
 * @return uint8_t �������1:δ��⵽��Ӧ��0:�ɹ�
 * @details ���͸�λ�źź󣬵ȴ�DHT11����40 - 80us��Ϊ��Ӧ��Ȼ��������40 - 80us
 */
uint8_t DHT11_Check(DHT11_HandleTypeDef* handle) {
    uint8_t retry = 0;
    DHT11_SET_IO_IN(handle);  // ����Ϊ����ģʽ

    // �ȴ�DHT11����40 - 80us
    while (DHT11_DQ_IN(handle) && retry < 100) {
        retry++;
        DelayUs(1);
    }
    if (retry >= 100) return 1;

    retry = 0;
    // �ȴ�DHT11����40 - 80us
    while (!DHT11_DQ_IN(handle) && retry < 100) {
        retry++;
        DelayUs(1);
    }
    if (retry >= 100) return 1;

    return 0;
}

/**
 * @brief  ��DHT11��ȡһλ����
 * @param  handle DHT11���ָ��
 * @return uint8_t ��ȡ��λ���ݣ�0��1
 * @details ͨ���жϸߵ�ƽ����ʱ����ȷ����0����1
 */
uint8_t DHT11_Read_Bit(DHT11_HandleTypeDef* handle) {
    uint8_t retry = 0;

    // �ȴ���Ϊ�͵�ƽ
    while (DHT11_DQ_IN(handle) && retry < 100) {
        retry++;
        DelayUs(1);
    }

    retry = 0;
    // �ȴ���Ϊ�ߵ�ƽ
    while (!DHT11_DQ_IN(handle) && retry < 100) {
        retry++;
        DelayUs(1);
    }

    DelayUs(40);  // �ȴ�40us����ʱ�ߵ�ƽ��ʾ1���͵�ƽ��ʾ0
    if (DHT11_DQ_IN(handle)) return 1;
    else return 0;
}

/**
 * @brief  ��DHT11��ȡһ���ֽڵ�����
 * @param  handle DHT11���ָ��
 * @return uint8_t ��ȡ���ֽ�����
 * @details ���ζ�ȡ8λ���ݣ����һ���ֽ�
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
 * @brief  ��ȡDHT11����������ʪ������
 * @param  handle DHT11���ָ��
 * @param  temp �¶�ֵָ��
 * @param  humi ʪ��ֵָ��
 * @return uint8_t ��ȡ�����0:�ɹ���1:ʧ��
 * @details ��ȡDHT11��40λ���ݣ�����ʪ������8λ + ʪ��С��8λ + �¶�����8λ + �¶�С��8λ + У���8λ
 */
uint8_t DHT11_Read_Data(DHT11_HandleTypeDef* handle, uint8_t *temp, uint8_t *humi) {
    uint8_t buf[5];  // �洢5���ֽ�����
    uint8_t i;

    DHT11_Rst(handle);  // ��λDHT11

    if (DHT11_Check(handle) == 0) {  // ���DHT11��Ӧ
        for (i = 0; i < 5; i++) {    // ��ȡ40λ����
            buf[i] = DHT11_Read_Byte(handle);
        }

        // У������
        uint8_t checksum = (buf[0] + buf[1] + buf[2] + buf[3]) & 0xFF;
        if (checksum == buf[4]) {
            *humi = buf[0];  // ʪ����������
            *temp = buf[2];  // �¶���������
        } else {
            return 1;
        }
    } else {
        return 1;
    }

    return 0;
}

/**
 * @brief  ��ʼ��DHT11������
 * @param  handle DHT11���ָ��
 * @return uint8_t ��ʼ�������0:�ɹ���1:ʧ��
 * @details ����GPIO���ȴ�DHT11��Ӧ
 */
uint8_t DHT11_Init(DHT11_HandleTypeDef* handle) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // ʹ��GPIOʱ��
    DHT11_EnableGPIOClock(handle->GPIOx);

    // ����GPIOΪ�������
    GPIO_InitStructure.GPIO_Pin = handle->GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(handle->GPIOx, &GPIO_InitStructure);

    GPIO_SetBits(handle->GPIOx, handle->GPIO_Pin);  // ��ʼ����

    DHT11_Rst(handle);  // ��λDHT11

    return DHT11_Check(handle);  // �ȴ�DHT11��Ӧ

}
