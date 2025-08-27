#include "ds18b20.h"
#include "delay.h"	

/**
 * @brief 设置DS18B20数据引脚为输出模式
 * @param device 指向DS18B20_HandleTypeDef结构体的指针
 */
static void DS18B20_IO_OUT(DS18B20_HandleTypeDef* device) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = device->GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(device->GPIOx, &GPIO_InitStructure);
}

/**
 * @brief 设置DS18B20数据引脚为输入模式
 * @param device 指向DS18B20_HandleTypeDef结构体的指针
 */
static void DS18B20_IO_IN(DS18B20_HandleTypeDef* device) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = device->GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 使用上拉输入模式
    GPIO_Init(device->GPIOx, &GPIO_InitStructure);
}

/**
 * @brief 获取DS18B20数据引脚的输入状态
 * @param device 指向DS18B20_HandleTypeDef结构体的指针
 * @return 引脚状态，0或1
 */
static u8 DS18B20_DQ_IN(DS18B20_HandleTypeDef* device) {
    return GPIO_ReadInputDataBit(device->GPIOx, device->GPIO_Pin);
}

/**
 * @brief 设置DS18B20数据引脚的输出状态
 * @param device 指向DS18B20_HandleTypeDef结构体的指针
 * @param state 要设置的状态，0或1
 */
static void DS18B20_DQ_OUT(DS18B20_HandleTypeDef* device, u8 state) {
    GPIO_WriteBit(device->GPIOx, device->GPIO_Pin, (BitAction)state);
}

/**
 * @brief 复位DS18B20总线，发送复位脉冲
 * @param device 指向DS18B20_HandleTypeDef结构体的指针
 */
void DS18B20_Rst(DS18B20_HandleTypeDef* device) {
    DS18B20_IO_OUT(device);
    DS18B20_DQ_OUT(device, 0);
    DelayUs(480); // 复位脉冲宽度调整为480us
    DS18B20_DQ_OUT(device, 1);
    DelayUs(70);  // 等待时间调整为70us
}

/**
 * @brief 检查DS18B20的存在响应
 * @param device 指向DS18B20_HandleTypeDef结构体的指针
 * @return 检查结果，1表示未检测到设备，0表示检测到设备
 */
u8 DS18B20_Check(DS18B20_HandleTypeDef* device) {
    u8 retry = 0;
    DS18B20_IO_IN(device);
    
    // 等待DS18B20拉低总线
    while (DS18B20_DQ_IN(device) && retry < 240) {
        retry++;
        DelayUs(1);
    }
    
    if (retry >= 240) return 1; // 超时，未检测到响应
    else retry = 0;
    
    // 等待DS18B20释放总线
    while (!DS18B20_DQ_IN(device) && retry < 240) {
        retry++;
        DelayUs(1);
    }
    
    if (retry >= 240) return 1; // 超时，未检测到释放
    return 0;
}

/**
 * @brief 从DS18B20读取一位数据
 * @param device 指向DS18B20_HandleTypeDef结构体的指针
 * @return 读取的位数据，0或1
 */
u8 DS18B20_Read_Bit(DS18B20_HandleTypeDef* device) {
    u8 data;
    DS18B20_IO_OUT(device);
    DS18B20_DQ_OUT(device, 0);
    DelayUs(2);
    DS18B20_DQ_OUT(device, 1);
    DS18B20_IO_IN(device);
    DelayUs(15); // 读取时间点调整为15us
    data = DS18B20_DQ_IN(device);
    DelayUs(45); // 完成读取周期
    return data;
}

/**
 * @brief 从DS18B20读取一个字节数据
 * @param device 指向DS18B20_HandleTypeDef结构体的指针
 * @return 读取的字节数据
 */
u8 DS18B20_Read_Byte(DS18B20_HandleTypeDef* device) {
    u8 i, j, dat;
    dat = 0;
    for (i = 1; i <= 8; i++) {
        j = DS18B20_Read_Bit(device);
        dat = (j << 7) | (dat >> 1);
    }
    return dat;
}

/**
 * @brief 向DS18B20写入一个字节数据
 * @param device 指向DS18B20_HandleTypeDef结构体的指针
 * @param dat 要写入的字节数据
 */
void DS18B20_Write_Byte(DS18B20_HandleTypeDef* device, u8 dat) {
    u8 j;
    u8 testb;
    DS18B20_IO_OUT(device);
    
    for (j = 1; j <= 8; j++) {
        testb = dat & 0x01;
        dat = dat >> 1;
        
        if (testb) {
            // 写1时序
            DS18B20_DQ_OUT(device, 0);
            DelayUs(2);
            DS18B20_DQ_OUT(device, 1);
            DelayUs(60);
        } else {
            // 写0时序
            DS18B20_DQ_OUT(device, 0);
            DelayUs(60);
            DS18B20_DQ_OUT(device, 1);
            DelayUs(2);
        }
    }
}

/**
 * @brief 启动DS18B20的温度转换
 * @param device 指向DS18B20_HandleTypeDef结构体的指针
 */
void DS18B20_Start(DS18B20_HandleTypeDef* device) {
    DS18B20_Rst(device);
    if(DS18B20_Check(device)) return; // 检查设备是否响应
    DS18B20_Write_Byte(device, 0xcc); // 跳过ROM操作
    DS18B20_Write_Byte(device, 0x44); // 启动温度转换
}

/**
 * @brief 初始化DS18B20设备
 * @param device 指向DS18B20_HandleTypeDef结构体的指针
 * @return 初始化结果，1表示失败，0表示成功
 */
u8 DS18B20_Init(DS18B20_HandleTypeDef* device) {
    // 修正GPIO时钟使能
    uint32_t rcc_periph = 0;
    
    if (device->GPIOx == GPIOA) rcc_periph = RCC_APB2Periph_GPIOA;
    else if (device->GPIOx == GPIOB) rcc_periph = RCC_APB2Periph_GPIOB;
    else if (device->GPIOx == GPIOC) rcc_periph = RCC_APB2Periph_GPIOC;
    else if (device->GPIOx == GPIOD) rcc_periph = RCC_APB2Periph_GPIOD;
    else if (device->GPIOx == GPIOE) rcc_periph = RCC_APB2Periph_GPIOE;
    else if (device->GPIOx == GPIOF) rcc_periph = RCC_APB2Periph_GPIOF;
    else if (device->GPIOx == GPIOG) rcc_periph = RCC_APB2Periph_GPIOG;
    
    if (rcc_periph) {
        RCC_APB2PeriphClockCmd(rcc_periph, ENABLE);
    } else {
        return 1; // 无效的GPIO端口
    }

    // 初始化GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = device->GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(device->GPIOx, &GPIO_InitStructure);

    GPIO_SetBits(device->GPIOx, device->GPIO_Pin);

    // 复位并检查DS18B20
    DS18B20_Rst(device);
    return DS18B20_Check(device);
}

/**
 * @brief 从DS18B20获取温度值
 * @param device 指向DS18B20_HandleTypeDef结构体的指针
 * @return 温度值，单位为°C，分辨率为0.0625°C
 * 
 * @details 
 * 该函数从DS18B20读取原始温度数据并转换为实际温度值。
 * 温度数据格式为16位有符号整数，低4位为小数部分，高12位为整数部分。
 * 例如，返回值256表示实际温度16.0°C (256 / 16 = 16.0)。
 */
short DS18B20_Get_Temp(DS18B20_HandleTypeDef* device) {
    u8 TL, TH;
    short tem;
    
    DS18B20_Start(device); // 启动温度转换
//    DelayMs(750);         // 等待转换完成（最大750ms）
    
    DS18B20_Rst(device);
    if(DS18B20_Check(device)) return 0; // 检查设备响应
    
    DS18B20_Write_Byte(device, 0xcc); // 跳过ROM操作
    DS18B20_Write_Byte(device, 0xbe); // 读取温度寄存器
    
    TL = DS18B20_Read_Byte(device); // 读取温度低位
    TH = DS18B20_Read_Byte(device); // 读取温度高位

    // 合并高低字节
    int16_t raw = (TH << 8) | TL;
    
    // 判断符号位
    if (raw & 0x8000) {
        // 负温度，二进制补码转换
        raw = -( (~raw + 1) & 0xFFFF );
    }
    
    // 应用分辨率系数 (12位分辨率，除以16)
    tem = raw / 16;

    return tem; // 返回实际温度值
}
