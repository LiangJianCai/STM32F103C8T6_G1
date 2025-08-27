#ifndef __DS18B20_H
#define __DS18B20_H 


#include "system_config.h"  

/**
 * @brief DS18B20�豸�ṹ�壬����GPIO�˿ں�������Ϣ
 */
typedef struct {
    GPIO_TypeDef* GPIOx;    /**< GPIO�˿� */
    uint16_t GPIO_Pin;      /**< GPIO���� */
} DS18B20_HandleTypeDef;

/**
 * @brief ��ʼ��DS18B20�豸
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ�룬����GPIO�˿ں�������Ϣ
 * @return ��ʼ�������1��ʾʧ�ܣ�0��ʾ�ɹ�
 */
u8 DS18B20_Init(DS18B20_HandleTypeDef* device);

/**
 * @brief ��DS18B20��ȡ�¶�ֵ
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ�룬����GPIO�˿ں�������Ϣ
 * @return �¶�ֵ����λΪ��C���ֱ���Ϊ0.0625��C
 */
short DS18B20_Get_Temp(DS18B20_HandleTypeDef* device);

/**
 * @brief ��ʼ�¶�ת��
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ�룬����GPIO�˿ں�������Ϣ
 */
void DS18B20_Start(DS18B20_HandleTypeDef* device);

/**
 * @brief ��DS18B20д��һ���ֽ�
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ�룬����GPIO�˿ں�������Ϣ
 * @param dat Ҫд����ֽ�����
 */
void DS18B20_Write_Byte(DS18B20_HandleTypeDef* device, u8 dat);

/**
 * @brief ��DS18B20��ȡһ���ֽ�
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ�룬����GPIO�˿ں�������Ϣ
 * @return ��ȡ���ֽ�����
 */
u8 DS18B20_Read_Byte(DS18B20_HandleTypeDef* device);

/**
 * @brief ��DS18B20��ȡһλ����
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ�룬����GPIO�˿ں�������Ϣ
 * @return ��ȡ��λ���ݣ�1��0
 */
u8 DS18B20_Read_Bit(DS18B20_HandleTypeDef* device);

/**
 * @brief ����Ƿ��⵽DS18B20
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ�룬����GPIO�˿ں�������Ϣ
 * @return �������1��ʾδ��⵽��0��ʾ��⵽
 */
u8 DS18B20_Check(DS18B20_HandleTypeDef* device);

/**
 * @brief ��λDS18B20
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ�룬����GPIO�˿ں�������Ϣ
 */
void DS18B20_Rst(DS18B20_HandleTypeDef* device);

#endif
