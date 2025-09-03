#include "board_config.h"

#include "module_config.h"

#include "sensor_app.h"

#include "lcd_app.h"


/*
************************************************************
*	函数名称：	main
*
*	函数功能：	
*
*	入口参数：	无
*
*	返回参数：	0
*
*	说明：
************************************************************
*/

/* 温度阈值（℃） */
#define TEMP_THRESHOLD   30

/* 湿度阈值（%） */
#define HIMI_THRESHOLD   20

/* 光敏阈值 */
#define LIGHT_THRESHOLD  30

/* 烟雾阈值 */
#define SMOKE_THRESHOLD  70

int main(void)
{
	/* 软件定时器 ---------------------------------------------------------------*/
		static uint32_t tick_100ms = 0;
		static uint32_t tick_200ms = 0;
		static uint32_t tick_500ms = 0;
		static uint32_t tick_1s    = 0;
	/* 72 MHz 内核时钟 → 1 kHz SysTick */
    SysTick_Config(SystemCoreClock / 1000);
	// 初始化所有外设
    BOARD_InitAll();
	
	// 初始化所有模块
		MODULE_InitAll();
	
	// 初始化传感器
		Sensor_Init();
	
		UsartPrintf(USART_DEBUG,"开机成功\r\n");
    //Sensor_GetValueAll();
    while (1) {
		uint32_t now = uwTick;
		/* 100 ms：温湿度 + DS18B20 */
		if (now - tick_100ms >= 100) {
				tick_100ms = now;
				Sensor_GetValueDHT11();
		}

		/* 200 ms：ADC1(光敏电阻)/ADC2(MQ-2烟雾) */
		if (now - tick_200ms >= 200) {
				tick_200ms = now;
				Sensor_GetValueADC();
				if(g_env.adc1 <= LIGHT_THRESHOLD)//环境光低于阈值,开灯
				{
					 LED_On(&BOARD_LED1);
				}
				if(g_env.adc2 >= SMOKE_THRESHOLD)//烟雾浓度高于阈值，蜂鸣器响
				{
					Beep_On(&BOARD_BEEP);
					UsartPrintf(USART_DEBUG,"烟雾浓度过高！\n");
				}
		}

		/* 500 ms：开关量 + OLED 刷新 */
		if (now - tick_500ms >= 500) {
				tick_500ms = now;
				g_env.sw[0]=INPUT1_DO;
				g_env.sw[1]=INPUT2_DO;
				switch(g_env.sw[0])//雨滴传感器检测到雨滴，蜂鸣器响
				{
					case 0:Beep_Off(&BOARD_BEEP);UsartPrintf(USART_DEBUG,"雨滴传感器正常工作！\n");break;
					case 1:Beep_On(&BOARD_BEEP);UsartPrintf(USART_DEBUG,"检测到雨滴！\n");break;
					default:UsartPrintf(USART_DEBUG,"雨滴传感器出现故障！\n");break;
				}
				switch(g_env.sw[1])//红外反射器被遮挡,蜂鸣器响
				{
					case 0:Beep_Off(&BOARD_BEEP);UsartPrintf(USART_DEBUG,"红外反射器正常工作！\n");break;
					case 1:Beep_On(&BOARD_BEEP);UsartPrintf(USART_DEBUG,"红外传感器被遮挡！\n");break;
					default:UsartPrintf(USART_DEBUG,"红外反射器出现故障！\n");break;
				}				
				
		}

		/* 1 s：温控风扇 */
		if (now - tick_1s >= 1000) {
				tick_1s = now;
				if(g_env.temp >= TEMP_THRESHOLD)
				{
					 Sensor_GPIO_Output(true);//打开风扇
					 Sensor_TestPWM(false);//关闭窗帘
				}
				else if(g_env.temp < TEMP_THRESHOLD|| g_env.humi > HIMI_THRESHOLD){
					 Sensor_GPIO_Output(false);//关闭风扇
					 Sensor_TestPWM(true);//打开窗帘
				}				
		}	

  }
}
