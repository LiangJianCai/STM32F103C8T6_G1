#include "module_config.h"

/************************************************** �궨�� *************************************************************/
/**
 * @def MAX_INIT_RETRIES
 * @brief ģ���ʼ����������Դ���
 * @details ������ģ���ʼ��ʧ��ʱ��������Դ���Ϊ 5 �Ρ�
 */
#define MAX_INIT_RETRIES 5

/************************************************** ȫ�ֱ��� ***********************************************************/
/**
 * @var OLED_Handle
 * @brief OLED ģ��ľ��
 * @details ���ڴ洢 OLED ģ������������Ϣ������ SCL �� SDA �� GPIO �˿ں����š�
 */
OLED_HandleTypeDef OLED_Handle = {
    .SCL_GPIOx = GPIOB,
    .SCL_Pin = GPIO_Pin_6,
    .SDA_GPIOx = GPIOB,
    .SDA_Pin = GPIO_Pin_7
};

// ���� 3 �� GPIO ����ṹ��
/**
 * @var MODULE_GPIO_INPUT1
 * @brief ��һ�� GPIO ����ģ��
 * @details �������úʹ洢��һ�� GPIO ����������Ϣ������ GPIO �˿ڡ����š�ģʽ���ٶȣ���ʼ״̬Ϊ�ߵ�ƽ��
 */
GPIO_Input_TypeDef MODULE_GPIO_INPUT1 = {
    .config = {
        .GPIOx = GPIOA,
        .GPIO_Pin = GPIO_Pin_1,
        .GPIO_Mode = GPIO_Mode_IPU,
        .GPIO_Speed = GPIO_Speed_50MHz
    },
    .state = GPIO_INPUT_HIGH
};
/**
 * @var MODULE_GPIO_INPUT2
 * @brief �ڶ��� GPIO ����ģ��
 * @details �������úʹ洢�ڶ��� GPIO ����������Ϣ������ GPIO �˿ڡ����š�ģʽ���ٶȣ���ʼ״̬Ϊ�ߵ�ƽ��
 */
GPIO_Input_TypeDef MODULE_GPIO_INPUT2 = {
    .config = {
        .GPIOx = GPIOA,
        .GPIO_Pin = GPIO_Pin_2,
        .GPIO_Mode = GPIO_Mode_IPU,
        .GPIO_Speed = GPIO_Speed_50MHz
    },
    .state = GPIO_INPUT_HIGH
};
/**
 * @var MODULE_GPIO_INPUT3
 * @brief ������ GPIO ����ģ��
 * @details �������úʹ洢������ GPIO ����������Ϣ������ GPIO �˿ڡ����š�ģʽ���ٶȣ���ʼ״̬Ϊ�ߵ�ƽ��
 */
GPIO_Input_TypeDef MODULE_GPIO_INPUT3 = {
    .config = {
        .GPIOx = GPIOA,
        .GPIO_Pin = GPIO_Pin_3,
        .GPIO_Mode = GPIO_Mode_IPU,
        .GPIO_Speed = GPIO_Speed_50MHz
    },
    .state = GPIO_INPUT_HIGH
};

/**
 * @var MODULE_GPIO_OUTPUT1
 * @brief ��һ�� GPIO ���ģ��
 * @details �������úʹ洢��һ�� GPIO ����������Ϣ������ GPIO �˿ڡ����š�ģʽ���ٶȡ�
 */
GPIO_Output_TypeDef MODULE_GPIO_OUTPUT1 = {
    // ���� GPIO
    .config.GPIOx = GPIOA,
    .config.GPIO_Pin = GPIO_Pin_8,
    .config.GPIO_Mode = GPIO_Mode_Out_PP,
    .config.GPIO_Speed = GPIO_Speed_50MHz
};

/**
 * @var MODULE_DHT11
 * @brief DHT11 ��ʪ�ȴ������ľ��
 * @details ���ڴ洢 DHT11 ��ʪ�ȴ����������������Ϣ������ GPIO �˿ں����š�
 */
DHT11_HandleTypeDef MODULE_DHT11 = {
    .GPIOx = GPIOA,
    .GPIO_Pin = GPIO_Pin_11
};

// ���� DS18B20 �豸���ӵ� GPIO �˿ں�����
/**
 * @var ds18b20_device
 * @brief DS18B20 �¶ȴ��������豸�ṹ��
 * @details ���ڴ洢 DS18B20 �¶ȴ����������������Ϣ������ GPIO �˿ں����š�
 */
DS18B20_HandleTypeDef MODULE_DS18B20 = {
    .GPIOx = GPIOA,
    .GPIO_Pin = GPIO_Pin_12
};

/************************************* �ֲ���ʼ������ *******************************************************/
/**
 * @brief ��ʼ�� OLED ģ��
 * @details �ú������ڳ�ʼ�� OLED ģ�飬����ʾ��ʼ���ɹ���Ϣ��
 * @return ��
 */
static void BOARD_InitOLED(void) {
    OLED_Init(&OLED_Handle);  // OLED ��ʼ��
}

/**
 * @brief ��ʼ�� GPIO ����ģ��
 * @details �ú������ڳ�ʼ�� 3 �� GPIO ����ģ�顣
 * @return ��
 */
static void MODULE_InitGPIOInputs(void) {
    GPIO_Input_Init(&MODULE_GPIO_INPUT1);
    GPIO_Input_Init(&MODULE_GPIO_INPUT2);
    GPIO_Input_Init(&MODULE_GPIO_INPUT3);
}

/**
 * @brief ��ʼ�� GPIO ���ģ��
 * @details �ú������ڳ�ʼ�� 1 �� GPIO ���ģ�顣
 * @return ��
 */
static void MODULE_InitGPIOOutputs(void) {
    GPIO_Output_Init(&MODULE_GPIO_OUTPUT1);
}

/**
 * @brief ��ʼ�� DHT11 ��ʪ�ȴ�����
 * @details �ú������ڳ�ʼ�� DHT11 ��ʪ�ȴ������� GPIO ���ã�����ʼ��ʧ����������ԡ�
 * @note ������Դ����ﵽ MAX_INIT_RETRIES ��ʧ�ܣ�����ʾ������Ϣ����������ѭ����
 * @return ��
 */
static void MODULE_InitDHT11(void) {
    uint8_t retry_count = 0;

    while (DHT11_Init(&MODULE_DHT11) && retry_count < MAX_INIT_RETRIES) {
        OLED_ShowString(&OLED_Handle, 1, 1, "DHT11 Error");

        DelayMs(200);

        OLED_Clear(&OLED_Handle);

        DelayMs(200);

        retry_count++;
    }

    if (retry_count >= MAX_INIT_RETRIES) {
        OLED_ShowString(&OLED_Handle, 1, 1, "DHT11 Init Failed");
        while (1); // ���޸Ĵ˲����߼�
    }
}

/**
 * @brief ��ʼ�� DS18B20 �¶ȴ�����
 * @details �ú������ڳ�ʼ�� DS18B20 �¶ȴ������� GPIO ���ã�����ʼ��ʧ����������ԡ�
 * @note ������Դ����ﵽ MAX_INIT_RETRIES ��ʧ�ܣ�����ʾ������Ϣ����������ѭ����
 * @return ��
 */
static void MODULE_InitDS18B20(void) {
    uint8_t retry_count = 0;

    while (DS18B20_Init(&MODULE_DS18B20) && retry_count < MAX_INIT_RETRIES) {
        OLED_ShowString(&OLED_Handle, 1, 1, "DS18B20 Error!");

        DelayMs(200);

        OLED_Clear(&OLED_Handle);

        DelayMs(200);

        retry_count++;
    }

    if (retry_count >= MAX_INIT_RETRIES) {
        OLED_ShowString(&OLED_Handle, 1, 1, "DS18B20 Init Failed");
        while (1); // ���޸Ĵ˲����߼�
    }
}

/************************************* ȫ�ֳ�ʼ������ *******************************************************/
/**
 * @brief ��ʼ������ģ��
 * @details �ú������ڳ�ʼ������ģ�飬���� OLED ģ�顢GPIO ����ģ�顢GPIO ���ģ�顢DHT11 ��ʪ�ȴ������� DS18B20 �¶ȴ�������
 * @return ��
 */
void MODULE_InitAll(void) {
    BOARD_InitOLED();  // OLED ģ���ʼ��
	
		MODULE_InitGPIOInputs(); // ��ʼ�� GPIO ����
	
		MODULE_InitGPIOOutputs();    // ��ʼ�� GPIO ���
	
		//MODULE_InitDHT11();         // ��ʼ�� DHT11 ��ʪ�ȴ�����
	
		//MODULE_InitDS18B20();         // ��ʼ�� DS18B20 �¶ȴ�����
	
    OLED_ShowString(&OLED_Handle, 1, 1, "MODULE_Init OK!");
}
