#include "sensor_app.h"

/****************************************************** �궨�� *******************************************************/


/****************************************************** ȫ�ֱ��� *******************************************************/
uint16_t ADCValue1=0;		// ADC1������ֵ
uint16_t ADCValue2=0;		// ADC2������ֵ

uint64_t nowTimeStamp=1753273342176;	// ��ǰʱ���

GPIO_Input_State INPUT1_DO;	// ������1 �쳣��0
GPIO_Input_State INPUT2_DO;	// ������1 �쳣��0
GPIO_Input_State INPUT3_DO;	// ������1 �쳣��0

uint8_t temperature = 0;  // �¶�ֵ
uint8_t humidity = 0;     // ʪ��ֵ

short TEMP_Value=0;		  // �¶�

/****************************************************** �ֲ����� *******************************************************/
static void Sensor_GetSystemClock(void)
{
	RCC_ClocksTypeDef rcc_clocks;
	
	RCC_GetClocksFreq(&rcc_clocks);
	
	OLED_Clear(&OLED_Handle);
	
	OLED_ShowNum(&OLED_Handle,1,1,rcc_clocks.SYSCLK_Frequency,8);
	OLED_ShowNum(&OLED_Handle,2,1,rcc_clocks.HCLK_Frequency,8);
	OLED_ShowNum(&OLED_Handle,3,1,rcc_clocks.PCLK1_Frequency,8);
	OLED_ShowNum(&OLED_Handle,4,1,rcc_clocks.PCLK2_Frequency,8);
//	OLED_ShowNum(&OLED_Handle,1,9,rcc_clocks.ADCCLK_Frequency,8);
}

static void Sensor_GetValueADC(void)
{
	// ��������ADCͨ����ֵ
	Update_All_Adc_Values();
	
	// ��ȡ�������ѹֵ������ο���ѹΪ3.3V��
	for (uint8_t i = 0; i < ADC_ChannelCount; i++) {
		ADCValue1=(u16)((float)ADC_ConvertedValue[0]/4096*100+0.5);
		ADCValue2=(u16)((float)ADC_ConvertedValue[1]/4096*100+0.5);
		
//		UsartPrintf(USART_DEBUG, "ADC1_value %d\r\n", ADCValue1);
//		UsartPrintf(USART_DEBUG, "ADC2_Value %d\r\n", ADCValue2);
		
		// ��ʾADCֵ
		OLED_ShowString(&OLED_Handle,2,1,"ADCValue1");
		OLED_ShowNum(&OLED_Handle,2,12,ADCValue1,3);

		OLED_ShowString(&OLED_Handle,3,1,"ADCValue2");
		OLED_ShowNum(&OLED_Handle,3,12,ADCValue2,3);
	}
}

// ģ���ȡʱ����ĺ�����ʵ��Ӧ������Ҫ���ݾ������ʵ��
static uint64_t get_timestamp(void) {
	if(nowTimeStamp!=0){
		return nowTimeStamp;
	}
    return 0; // Ĭ�Ϸ��� 0����ʾû���µ�ʱ���
}

static void Sensor_SetTime(void)
{
	// ��ȡʱ���
    uint64_t timestamp = get_timestamp();
	
	if (timestamp != 0) {
		// ����ʱ������� RTC ʱ��
		if (RTC_SetTimeByTimestamp(timestamp)) {
			// ���óɹ�
		} else {
			// ����ʧ�ܣ���������Чʱ���
		}
	}
}

static void Sensor_TestPWM(void)
{
	static _Bool WindowsStates=false;
	
	WindowsStates=!WindowsStates;

	PWM_SetStatus(&window_pwm_config, WindowsStates);
	
}

static void Sensor_GetValueInputGPIO(void)
{
	// ���� GPIO ����״̬
	GPIO_Input_UpdateState(&MODULE_GPIO_INPUT1);
	GPIO_Input_UpdateState(&MODULE_GPIO_INPUT2);
	GPIO_Input_UpdateState(&MODULE_GPIO_INPUT3);
	
	// ��ȡ GPIO ����״̬
	INPUT1_DO = GPIO_Input_GetState(&MODULE_GPIO_INPUT1);
	INPUT2_DO = GPIO_Input_GetState(&MODULE_GPIO_INPUT2);
	INPUT3_DO = GPIO_Input_GetState(&MODULE_GPIO_INPUT3);
	
	OLED_ShowString(&OLED_Handle,1,1,"INPUT1_DO:");
	OLED_ShowNum(&OLED_Handle,1,14,INPUT1_DO,1);
	
	OLED_ShowString(&OLED_Handle,2,1,"INPUT2_DO:");
	OLED_ShowNum(&OLED_Handle,2,14,INPUT2_DO,1);
	
	OLED_ShowString(&OLED_Handle,3,1,"INPUT3_DO:");
	OLED_ShowNum(&OLED_Handle,3,14,INPUT3_DO,1);
}

static void Sensor_GPIO_Output(void)
{
	static _Bool FanStatues=false;
	
	FanStatues=!FanStatues;
	
	if(FanStatues){
		GPIO_Output_SetState(&MODULE_GPIO_OUTPUT1, GPIO_OUTPUT_HIGH);
	}
	else{
		GPIO_Output_SetState(&MODULE_GPIO_OUTPUT1, GPIO_OUTPUT_LOW);
	}
	
}

static void Sensor_GetValueDHT11(void)
{
//	OLED_Clear(&OLED_Handle);
	
	// ��ȡDHT11����
	if (DHT11_Read_Data(&MODULE_DHT11, &temperature, &humidity) == 0)
	{
		// ��ʾ�¶�
		OLED_ShowString(&OLED_Handle, 1, 1, "Temp:");
		
		OLED_ShowNum(&OLED_Handle,1,8,temperature,3);
		
		OLED_ShowString(&OLED_Handle, 1, 12, "C");
		
		// ��ʾʪ��
		OLED_ShowString(&OLED_Handle, 2, 1, "Humi:");
		
		OLED_ShowNum(&OLED_Handle,2,8,humidity,3);
		
		OLED_ShowString(&OLED_Handle, 2, 12, "%");
	}
	else
	{
		// ��ȡʧ�ܣ���ʾ������Ϣ
		OLED_ShowString(&OLED_Handle, 1, 1, "Read DHT11 Failed!");
	}
}

static void Sensor_GetValueDS18B20(void)
{
//	OLED_Clear(&OLED_Handle);
	
	TEMP_Value = DS18B20_Get_Temp(&MODULE_DS18B20);
	
	// ��ʾ�¶�
	OLED_ShowString(&OLED_Handle, 3, 1, "Temp:");
	
	OLED_ShowNum(&OLED_Handle,1,8,TEMP_Value,3);
	
	OLED_ShowString(&OLED_Handle, 3, 12, "C");
}

/****************************************************** ȫ�ֺ��� *******************************************************/
/*
************************************************************
*	�������ƣ�	Sensor_Init
*
*	�������ܣ�	��ʼ��������ֵ
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��ʼ����������ֵ
************************************************************
*/
void Sensor_Init(void)
{
	OLED_Clear(&OLED_Handle);
	
	// ����ʱ��
	Sensor_SetTime();
}

/*
************************************************************
*	�������ƣ�	Sensor_GetValueAll
*
*	�������ܣ�	�õ�������ֵ
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		�õ���������ֵ
************************************************************
*/
void Sensor_GetValueAll(void)
{
//	Sensor_GetSystemClock();
	
//	Sensor_GetValueADC();
	
//	Sensor_TestPWM();
	
//	Sensor_GetValueInputGPIO();
	
//	Sensor_GPIO_Output();
	
//	Sensor_GetValueDHT11();
	
		Sensor_GetValueDS18B20();
}
/*
************************************************************
*	�������ƣ�	Sensor_TemConout
*
*	�������ܣ�	�õ�������ֵ
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		�����¶ȿ��Ʒ���
************************************************************
*/
void Sensor_TemConout(void)
{
    if(DS18B20_Get_Temp(&MODULE_DS18B20)>=30)
		{
			 Sensor_GPIO_Output();
		}


}