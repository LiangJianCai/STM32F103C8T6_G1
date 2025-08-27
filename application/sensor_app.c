#include "sensor_app.h"

/****************************************************** 宏定义 *******************************************************/


/****************************************************** 全局变量 *******************************************************/
uint16_t ADCValue1=0;		// ADC1传感器值
uint16_t ADCValue2=0;		// ADC2传感器值

uint64_t nowTimeStamp=1753273342176;	// 当前时间戳

GPIO_Input_State INPUT1_DO;	// 正常：1 异常：0
GPIO_Input_State INPUT2_DO;	// 正常：1 异常：0
GPIO_Input_State INPUT3_DO;	// 正常：1 异常：0

uint8_t temperature = 0;  // 温度值
uint8_t humidity = 0;     // 湿度值

short TEMP_Value=0;		  // 温度

/****************************************************** 局部函数 *******************************************************/
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
	// 更新所有ADC通道的值
	Update_All_Adc_Values();
	
	// 读取并计算电压值（假设参考电压为3.3V）
	for (uint8_t i = 0; i < ADC_ChannelCount; i++) {
		ADCValue1=(u16)((float)ADC_ConvertedValue[0]/4096*100+0.5);
		ADCValue2=(u16)((float)ADC_ConvertedValue[1]/4096*100+0.5);
		
//		UsartPrintf(USART_DEBUG, "ADC1_value %d\r\n", ADCValue1);
//		UsartPrintf(USART_DEBUG, "ADC2_Value %d\r\n", ADCValue2);
		
		// 显示ADC值
		OLED_ShowString(&OLED_Handle,2,1,"ADCValue1");
		OLED_ShowNum(&OLED_Handle,2,12,ADCValue1,3);

		OLED_ShowString(&OLED_Handle,3,1,"ADCValue2");
		OLED_ShowNum(&OLED_Handle,3,12,ADCValue2,3);
	}
}

// 模拟获取时间戳的函数，实际应用中需要根据具体情况实现
static uint64_t get_timestamp(void) {
	if(nowTimeStamp!=0){
		return nowTimeStamp;
	}
    return 0; // 默认返回 0，表示没有新的时间戳
}

static void Sensor_SetTime(void)
{
	// 获取时间戳
    uint64_t timestamp = get_timestamp();
	
	if (timestamp != 0) {
		// 根据时间戳设置 RTC 时间
		if (RTC_SetTimeByTimestamp(timestamp)) {
			// 设置成功
		} else {
			// 设置失败，可能是无效时间戳
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
	// 更新 GPIO 输入状态
	GPIO_Input_UpdateState(&MODULE_GPIO_INPUT1);
	GPIO_Input_UpdateState(&MODULE_GPIO_INPUT2);
	GPIO_Input_UpdateState(&MODULE_GPIO_INPUT3);
	
	// 获取 GPIO 输入状态
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
	
	// 读取DHT11数据
	if (DHT11_Read_Data(&MODULE_DHT11, &temperature, &humidity) == 0)
	{
		// 显示温度
		OLED_ShowString(&OLED_Handle, 1, 1, "Temp:");
		
		OLED_ShowNum(&OLED_Handle,1,8,temperature,3);
		
		OLED_ShowString(&OLED_Handle, 1, 12, "C");
		
		// 显示湿度
		OLED_ShowString(&OLED_Handle, 2, 1, "Humi:");
		
		OLED_ShowNum(&OLED_Handle,2,8,humidity,3);
		
		OLED_ShowString(&OLED_Handle, 2, 12, "%");
	}
	else
	{
		// 读取失败，显示错误信息
		OLED_ShowString(&OLED_Handle, 1, 1, "Read DHT11 Failed!");
	}
}

static void Sensor_GetValueDS18B20(void)
{
//	OLED_Clear(&OLED_Handle);
	
	TEMP_Value = DS18B20_Get_Temp(&MODULE_DS18B20);
	
	// 显示温度
	OLED_ShowString(&OLED_Handle, 3, 1, "Temp:");
	
	OLED_ShowNum(&OLED_Handle,1,8,TEMP_Value,3);
	
	OLED_ShowString(&OLED_Handle, 3, 12, "C");
}

/****************************************************** 全局函数 *******************************************************/
/*
************************************************************
*	函数名称：	Sensor_Init
*
*	函数功能：	初始化传感器值
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		初始化传感器数值
************************************************************
*/
void Sensor_Init(void)
{
	OLED_Clear(&OLED_Handle);
	
	// 设置时间
	Sensor_SetTime();
}

/*
************************************************************
*	函数名称：	Sensor_GetValueAll
*
*	函数功能：	得到传感器值
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		得到传感器数值
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
*	函数名称：	Sensor_TemConout
*
*	函数功能：	得到传感器值
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		根据温度控制风扇
************************************************************
*/
void Sensor_TemConout(void)
{
    if(DS18B20_Get_Temp(&MODULE_DS18B20)>=30)
		{
			 Sensor_GPIO_Output();
		}


}