#ifndef __DS18B20_H
#define __DS18B20_H 


#include "system_config.h"  

/**
 * @brief DS18B20设备结构体，包含GPIO端口和引脚信息
 */
typedef struct {
    GPIO_TypeDef* GPIOx;    /**< GPIO端口 */
    uint16_t GPIO_Pin;      /**< GPIO引脚 */
} DS18B20_HandleTypeDef;

/**
 * @brief 初始化DS18B20设备
 * @param device 指向DS18B20_HandleTypeDef结构体的指针，包含GPIO端口和引脚信息
 * @return 初始化结果，1表示失败，0表示成功
 */
u8 DS18B20_Init(DS18B20_HandleTypeDef* device);

/**
 * @brief 从DS18B20获取温度值
 * @param device 指向DS18B20_HandleTypeDef结构体的指针，包含GPIO端口和引脚信息
 * @return 温度值，单位为°C，分辨率为0.0625°C
 */
short DS18B20_Get_Temp(DS18B20_HandleTypeDef* device);

/**
 * @brief 开始温度转换
 * @param device 指向DS18B20_HandleTypeDef结构体的指针，包含GPIO端口和引脚信息
 */
void DS18B20_Start(DS18B20_HandleTypeDef* device);

/**
 * @brief 向DS18B20写入一个字节
 * @param device 指向DS18B20_HandleTypeDef结构体的指针，包含GPIO端口和引脚信息
 * @param dat 要写入的字节数据
 */
void DS18B20_Write_Byte(DS18B20_HandleTypeDef* device, u8 dat);

/**
 * @brief 从DS18B20读取一个字节
 * @param device 指向DS18B20_HandleTypeDef结构体的指针，包含GPIO端口和引脚信息
 * @return 读取的字节数据
 */
u8 DS18B20_Read_Byte(DS18B20_HandleTypeDef* device);

/**
 * @brief 从DS18B20读取一位数据
 * @param device 指向DS18B20_HandleTypeDef结构体的指针，包含GPIO端口和引脚信息
 * @return 读取的位数据，1或0
 */
u8 DS18B20_Read_Bit(DS18B20_HandleTypeDef* device);

/**
 * @brief 检查是否检测到DS18B20
 * @param device 指向DS18B20_HandleTypeDef结构体的指针，包含GPIO端口和引脚信息
 * @return 检查结果，1表示未检测到，0表示检测到
 */
u8 DS18B20_Check(DS18B20_HandleTypeDef* device);

/**
 * @brief 复位DS18B20
 * @param device 指向DS18B20_HandleTypeDef结构体的指针，包含GPIO端口和引脚信息
 */
void DS18B20_Rst(DS18B20_HandleTypeDef* device);

#endif
