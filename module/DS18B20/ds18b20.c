#include "ds18b20.h"
#include "delay.h"	

/**
 * @brief ����DS18B20��������Ϊ���ģʽ
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ��
 */
static void DS18B20_IO_OUT(DS18B20_HandleTypeDef* device) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = device->GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(device->GPIOx, &GPIO_InitStructure);
}

/**
 * @brief ����DS18B20��������Ϊ����ģʽ
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ��
 */
static void DS18B20_IO_IN(DS18B20_HandleTypeDef* device) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = device->GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ʹ����������ģʽ
    GPIO_Init(device->GPIOx, &GPIO_InitStructure);
}

/**
 * @brief ��ȡDS18B20�������ŵ�����״̬
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ��
 * @return ����״̬��0��1
 */
static u8 DS18B20_DQ_IN(DS18B20_HandleTypeDef* device) {
    return GPIO_ReadInputDataBit(device->GPIOx, device->GPIO_Pin);
}

/**
 * @brief ����DS18B20�������ŵ����״̬
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ��
 * @param state Ҫ���õ�״̬��0��1
 */
static void DS18B20_DQ_OUT(DS18B20_HandleTypeDef* device, u8 state) {
    GPIO_WriteBit(device->GPIOx, device->GPIO_Pin, (BitAction)state);
}

/**
 * @brief ��λDS18B20���ߣ����͸�λ����
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ��
 */
void DS18B20_Rst(DS18B20_HandleTypeDef* device) {
    DS18B20_IO_OUT(device);
    DS18B20_DQ_OUT(device, 0);
    DelayUs(480); // ��λ�����ȵ���Ϊ480us
    DS18B20_DQ_OUT(device, 1);
    DelayUs(70);  // �ȴ�ʱ�����Ϊ70us
}

/**
 * @brief ���DS18B20�Ĵ�����Ӧ
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ��
 * @return �������1��ʾδ��⵽�豸��0��ʾ��⵽�豸
 */
u8 DS18B20_Check(DS18B20_HandleTypeDef* device) {
    u8 retry = 0;
    DS18B20_IO_IN(device);
    
    // �ȴ�DS18B20��������
    while (DS18B20_DQ_IN(device) && retry < 240) {
        retry++;
        DelayUs(1);
    }
    
    if (retry >= 240) return 1; // ��ʱ��δ��⵽��Ӧ
    else retry = 0;
    
    // �ȴ�DS18B20�ͷ�����
    while (!DS18B20_DQ_IN(device) && retry < 240) {
        retry++;
        DelayUs(1);
    }
    
    if (retry >= 240) return 1; // ��ʱ��δ��⵽�ͷ�
    return 0;
}

/**
 * @brief ��DS18B20��ȡһλ����
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ��
 * @return ��ȡ��λ���ݣ�0��1
 */
u8 DS18B20_Read_Bit(DS18B20_HandleTypeDef* device) {
    u8 data;
    DS18B20_IO_OUT(device);
    DS18B20_DQ_OUT(device, 0);
    DelayUs(2);
    DS18B20_DQ_OUT(device, 1);
    DS18B20_IO_IN(device);
    DelayUs(15); // ��ȡʱ������Ϊ15us
    data = DS18B20_DQ_IN(device);
    DelayUs(45); // ��ɶ�ȡ����
    return data;
}

/**
 * @brief ��DS18B20��ȡһ���ֽ�����
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ��
 * @return ��ȡ���ֽ�����
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
 * @brief ��DS18B20д��һ���ֽ�����
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ��
 * @param dat Ҫд����ֽ�����
 */
void DS18B20_Write_Byte(DS18B20_HandleTypeDef* device, u8 dat) {
    u8 j;
    u8 testb;
    DS18B20_IO_OUT(device);
    
    for (j = 1; j <= 8; j++) {
        testb = dat & 0x01;
        dat = dat >> 1;
        
        if (testb) {
            // д1ʱ��
            DS18B20_DQ_OUT(device, 0);
            DelayUs(2);
            DS18B20_DQ_OUT(device, 1);
            DelayUs(60);
        } else {
            // д0ʱ��
            DS18B20_DQ_OUT(device, 0);
            DelayUs(60);
            DS18B20_DQ_OUT(device, 1);
            DelayUs(2);
        }
    }
}

/**
 * @brief ����DS18B20���¶�ת��
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ��
 */
void DS18B20_Start(DS18B20_HandleTypeDef* device) {
    DS18B20_Rst(device);
    if(DS18B20_Check(device)) return; // ����豸�Ƿ���Ӧ
    DS18B20_Write_Byte(device, 0xcc); // ����ROM����
    DS18B20_Write_Byte(device, 0x44); // �����¶�ת��
}

/**
 * @brief ��ʼ��DS18B20�豸
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ��
 * @return ��ʼ�������1��ʾʧ�ܣ�0��ʾ�ɹ�
 */
u8 DS18B20_Init(DS18B20_HandleTypeDef* device) {
    // ����GPIOʱ��ʹ��
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
        return 1; // ��Ч��GPIO�˿�
    }

    // ��ʼ��GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = device->GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(device->GPIOx, &GPIO_InitStructure);

    GPIO_SetBits(device->GPIOx, device->GPIO_Pin);

    // ��λ�����DS18B20
    DS18B20_Rst(device);
    return DS18B20_Check(device);
}

/**
 * @brief ��DS18B20��ȡ�¶�ֵ
 * @param device ָ��DS18B20_HandleTypeDef�ṹ���ָ��
 * @return �¶�ֵ����λΪ��C���ֱ���Ϊ0.0625��C
 * 
 * @details 
 * �ú�����DS18B20��ȡԭʼ�¶����ݲ�ת��Ϊʵ���¶�ֵ��
 * �¶����ݸ�ʽΪ16λ�з�����������4λΪС�����֣���12λΪ�������֡�
 * ���磬����ֵ256��ʾʵ���¶�16.0��C (256 / 16 = 16.0)��
 */
short DS18B20_Get_Temp(DS18B20_HandleTypeDef* device) {
    u8 TL, TH;
    short tem;
    
    DS18B20_Start(device); // �����¶�ת��
//    DelayMs(750);         // �ȴ�ת����ɣ����750ms��
    
    DS18B20_Rst(device);
    if(DS18B20_Check(device)) return 0; // ����豸��Ӧ
    
    DS18B20_Write_Byte(device, 0xcc); // ����ROM����
    DS18B20_Write_Byte(device, 0xbe); // ��ȡ�¶ȼĴ���
    
    TL = DS18B20_Read_Byte(device); // ��ȡ�¶ȵ�λ
    TH = DS18B20_Read_Byte(device); // ��ȡ�¶ȸ�λ

    // �ϲ��ߵ��ֽ�
    int16_t raw = (TH << 8) | TL;
    
    // �жϷ���λ
    if (raw & 0x8000) {
        // ���¶ȣ������Ʋ���ת��
        raw = -( (~raw + 1) & 0xFFFF );
    }
    
    // Ӧ�÷ֱ���ϵ�� (12λ�ֱ��ʣ�����16)
    tem = raw / 16;

    return tem; // ����ʵ���¶�ֵ
}
