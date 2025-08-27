#include "OLED_Font.h"
#include "OLED.h"

/**
 * @brief 写入SCL引脚电平
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param x 电平值
 */
static void OLED_W_SCL(OLED_HandleTypeDef* OLED_Handle, uint8_t x) {
    GPIO_WriteBit(OLED_Handle->SCL_GPIOx, OLED_Handle->SCL_Pin, (BitAction)(x));
}

/**
 * @brief 写入SDA引脚电平
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param x 电平值
 */
static void OLED_W_SDA(OLED_HandleTypeDef* OLED_Handle, uint8_t x) {
    GPIO_WriteBit(OLED_Handle->SDA_GPIOx, OLED_Handle->SDA_Pin, (BitAction)(x));
}

/**
 * @brief 引脚初始化
 * @param OLED_Handle 指向OLED对象结构体的指针
 */
static void OLED_I2C_Init(OLED_HandleTypeDef* OLED_Handle) {
    GPIO_InitTypeDef GPIO_InitStructure;
    if (OLED_Handle->SCL_GPIOx == GPIOB || OLED_Handle->SDA_GPIOx == GPIOB) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    }
    if (OLED_Handle->SCL_GPIOx == GPIOA || OLED_Handle->SDA_GPIOx == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    }
    // 初始化SCL引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = OLED_Handle->SCL_Pin;
    GPIO_Init(OLED_Handle->SCL_GPIOx, &GPIO_InitStructure);
    // 初始化SDA引脚
    GPIO_InitStructure.GPIO_Pin = OLED_Handle->SDA_Pin;
    GPIO_Init(OLED_Handle->SDA_GPIOx, &GPIO_InitStructure);

    OLED_W_SCL(OLED_Handle, 1);
    OLED_W_SDA(OLED_Handle, 1);
}

/**
 * @brief I2C开始
 * @param OLED_Handle 指向OLED对象结构体的指针
 */
static void OLED_I2C_Start(OLED_HandleTypeDef* OLED_Handle) {
    OLED_W_SDA(OLED_Handle, 1);
    OLED_W_SCL(OLED_Handle, 1);
    OLED_W_SDA(OLED_Handle, 0);
    OLED_W_SCL(OLED_Handle, 0);
}

/**
 * @brief I2C停止
 * @param OLED_Handle 指向OLED对象结构体的指针
 */
static void OLED_I2C_Stop(OLED_HandleTypeDef* OLED_Handle) {
    OLED_W_SDA(OLED_Handle, 0);
    OLED_W_SCL(OLED_Handle, 1);
    OLED_W_SDA(OLED_Handle, 1);
}

/**
 * @brief I2C发送一个字节
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Byte 要发送的一个字节
 */
static void OLED_I2C_SendByte(OLED_HandleTypeDef* OLED_Handle, uint8_t Byte) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        OLED_W_SDA(OLED_Handle, Byte & (0x80 >> i));
        OLED_W_SCL(OLED_Handle, 1);
        OLED_W_SCL(OLED_Handle, 0);
    }
    OLED_W_SCL(OLED_Handle, 1);  // 额外的一个时钟，不处理应答信号
    OLED_W_SCL(OLED_Handle, 0);
}

/**
 * @brief OLED写命令
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Command 要写入的命令
 */
static void OLED_WriteCommand(OLED_HandleTypeDef* OLED_Handle, uint8_t Command) {
    OLED_I2C_Start(OLED_Handle);
    OLED_I2C_SendByte(OLED_Handle, 0x78);  // 从机地址
    OLED_I2C_SendByte(OLED_Handle, 0x00);  // 写命令
    OLED_I2C_SendByte(OLED_Handle, Command);
    OLED_I2C_Stop(OLED_Handle);
}

/**
 * @brief OLED写数据
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Data 要写入的数据
 */
static void OLED_WriteData(OLED_HandleTypeDef* OLED_Handle, uint8_t Data) {
    OLED_I2C_Start(OLED_Handle);
    OLED_I2C_SendByte(OLED_Handle, 0x78);  // 从机地址
    OLED_I2C_SendByte(OLED_Handle, 0x40);  // 写数据
    OLED_I2C_SendByte(OLED_Handle, Data);
    OLED_I2C_Stop(OLED_Handle);
}

/**
 * @brief OLED设置光标位置
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Y 以左上角为原点，向下方向的坐标，范围：0~7
 * @param X 以左上角为原点，向右方向的坐标，范围：0~127
 */
static void OLED_SetCursor(OLED_HandleTypeDef* OLED_Handle, uint8_t Y, uint8_t X) {
    OLED_WriteCommand(OLED_Handle, 0xB0 | Y);  // 设置Y位置
    OLED_WriteCommand(OLED_Handle, 0x10 | ((X & 0xF0) >> 4));  // 设置X位置高4位
    OLED_WriteCommand(OLED_Handle, 0x00 | (X & 0x0F));  // 设置X位置低4位
}

/**
 * @brief 初始化OLED
 * @param OLED_Handle 指向OLED对象结构体的指针
 */
void OLED_Init(OLED_HandleTypeDef* OLED_Handle) {
    uint32_t i, j;
    for (i = 0; i < 1000; i++) {  // 上电延时
        for (j = 0; j < 1000; j++);
    }
    OLED_I2C_Init(OLED_Handle);  // 端口初始化
    OLED_WriteCommand(OLED_Handle, 0xAE);  // 关闭显示
    OLED_WriteCommand(OLED_Handle, 0xD5);  // 设置显示时钟分频比/振荡器频率
    OLED_WriteCommand(OLED_Handle, 0x80);
    OLED_WriteCommand(OLED_Handle, 0xA8);  // 设置多路复用率
    OLED_WriteCommand(OLED_Handle, 0x3F);
    OLED_WriteCommand(OLED_Handle, 0xD3);  // 设置显示偏移
    OLED_WriteCommand(OLED_Handle, 0x00);
    OLED_WriteCommand(OLED_Handle, 0x40);  // 设置显示开始行
    OLED_WriteCommand(OLED_Handle, 0xA1);  // 设置左右方向，0xA1正常 0xA0左右反置
    OLED_WriteCommand(OLED_Handle, 0xC8);  // 设置上下方向，0xC8正常 0xC0上下反置
    OLED_WriteCommand(OLED_Handle, 0xDA);  // 设置COM引脚硬件配置
    OLED_WriteCommand(OLED_Handle, 0x12);
    OLED_WriteCommand(OLED_Handle, 0x81);  // 设置对比度控制
    OLED_WriteCommand(OLED_Handle, 0xCF);
    OLED_WriteCommand(OLED_Handle, 0xD9);  // 设置预充电周期
    OLED_WriteCommand(OLED_Handle, 0xF1);
    OLED_WriteCommand(OLED_Handle, 0xDB);  // 设置VCOMH取消选择级别
    OLED_WriteCommand(OLED_Handle, 0x30);
    OLED_WriteCommand(OLED_Handle, 0xA4);  // 设置整个显示打开/关闭
    OLED_WriteCommand(OLED_Handle, 0xA6);  // 设置正常/倒转显示
    OLED_WriteCommand(OLED_Handle, 0x8D);  // 设置充电泵
    OLED_WriteCommand(OLED_Handle, 0x14);
    OLED_WriteCommand(OLED_Handle, 0xAF);  // 开启显示
    OLED_Clear(OLED_Handle);  // OLED清屏
}

/**
 * @brief 清除OLED屏幕
 * @param OLED_Handle 指向OLED对象结构体的指针
 */
void OLED_Clear(OLED_HandleTypeDef* OLED_Handle) {
    uint8_t i, j;
    for (j = 0; j < 8; j++) {
        OLED_SetCursor(OLED_Handle, j, 0);
        for (i = 0; i < 128; i++) {
            OLED_WriteData(OLED_Handle, 0x00);
        }
    }
}

/**
 * @brief 显示一个字符
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Line 行位置，范围：1~4
 * @param Column 列位置，范围：1~16
 * @param Char 要显示的一个字符，范围：ASCII可见字符
 */
void OLED_ShowChar(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, char Char) {
    uint8_t i;
    OLED_SetCursor(OLED_Handle, (Line - 1) * 2, (Column - 1) * 8);  // 设置光标位置在上半部分
    for (i = 0; i < 8; i++) {
        OLED_WriteData(OLED_Handle, OLED_F8x16[Char - ' '][i]);  // 显示上半部分内容
    }
    OLED_SetCursor(OLED_Handle, (Line - 1) * 2 + 1, (Column - 1) * 8);  // 设置光标位置在下半部分
    for (i = 0; i < 8; i++) {
        OLED_WriteData(OLED_Handle, OLED_F8x16[Char - ' '][i + 8]);  // 显示下半部分内容
    }
}

/**
 * @brief 显示字符串
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Line 起始行位置，范围：1~4
 * @param Column 起始列位置，范围：1~16
 * @param String 要显示的字符串，范围：ASCII可见字符
 */
void OLED_ShowString(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, char *String) {
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++) {
        OLED_ShowChar(OLED_Handle, Line, Column + i, String[i]);
    }
}

/**
 * @brief OLED次方函数
 * @param X 底数
 * @param Y 指数
 * @return 返回值等于X的Y次方
 */
static uint32_t OLED_Pow(uint32_t X, uint32_t Y) {
    uint32_t Result = 1;
    while (Y--) {
        Result *= X;
    }
    return Result;
}

/**
 * @brief 显示数字（十进制，正数）
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Line 起始行位置，范围：1~4
 * @param Column 起始列位置，范围：1~16
 * @param Number 要显示的数字，范围：0~4294967295
 * @param Length 要显示数字的长度，范围：1~10
 */
void OLED_ShowNum(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length) {
    uint8_t i;
    for (i = 0; i < Length; i++) {
        OLED_ShowChar(OLED_Handle, Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
 * @brief 显示数字（十进制，带符号数）
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Line 起始行位置，范围：1~4
 * @param Column 起始列位置，范围：1~16
 * @param Number 要显示的数字，范围：-2147483648~2147483647
 * @param Length 要显示数字的长度，范围：1~10
 */
void OLED_ShowSignedNum(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length) {
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0) {
        OLED_ShowChar(OLED_Handle, Line, Column, '+');
        Number1 = Number;
    } else {
        OLED_ShowChar(OLED_Handle, Line, Column, '-');
        Number1 = -Number;
    }
    for (i = 0; i < Length; i++) {
        OLED_ShowChar(OLED_Handle, Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
 * @brief 显示数字（十六进制，正数）
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Line 起始行位置，范围：1~4
 * @param Column 起始列位置，范围：1~16
 * @param Number 要显示的数字，范围：0~0xFFFFFFFF
 * @param Length 要显示数字的长度，范围：1~8
 */
void OLED_ShowHexNum(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length) {
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++) {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10) {
            OLED_ShowChar(OLED_Handle, Line, Column + i, SingleNumber + '0');
        } else {
            OLED_ShowChar(OLED_Handle, Line, Column + i, SingleNumber - 10 + 'A');
        }
    }
}

/**
 * @brief 显示数字（二进制，正数）
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param Line 起始行位置，范围：1~4
 * @param Column 起始列位置，范围：1~16
 * @param Number 要显示的数字，范围：0~1111 1111 1111 1111
 * @param Length 要显示数字的长度，范围：1~16
 */
void OLED_ShowBinNum(OLED_HandleTypeDef* OLED_Handle, uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length) {
    uint8_t i;
    for (i = 0; i < Length; i++) {
        OLED_ShowChar(OLED_Handle, Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
    }
}

/**
 * @brief 清除OLED12864从指定行开始及以下的所有内容
 * @param OLED_Handle 指向OLED对象结构体的指针
 * @param row 起始行
 */
void OLED_ClearFromSecondLine(OLED_HandleTypeDef* OLED_Handle, uint8_t row) {
    uint8_t i, j;
    for (j = row; j < 8; j++) {
        OLED_SetCursor(OLED_Handle, j, 0);
        for (i = 0; i < 128; i++) {
            OLED_WriteData(OLED_Handle, 0x00);
        }
    }
}
