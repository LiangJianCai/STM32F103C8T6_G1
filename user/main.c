#include "board_config.h"

#include "module_config.h"

#include "sensor_app.h"

#include "lcd_app.h"


/*
************************************************************
*	�������ƣ�	main
*
*	�������ܣ�	
*
*	��ڲ�����	��
*
*	���ز�����	0
*
*	˵����
************************************************************
*/

/* �¶���ֵ���棩 */
#define TEMP_THRESHOLD   30

/* ʪ����ֵ��%�� */
#define HIMI_THRESHOLD   20

/* ������ֵ */
#define LIGHT_THRESHOLD  30

/* ������ֵ */
#define SMOKE_THRESHOLD  70

int main(void)
{
	/* �����ʱ�� ---------------------------------------------------------------*/
		static uint32_t tick_100ms = 0;
		static uint32_t tick_200ms = 0;
		static uint32_t tick_500ms = 0;
		static uint32_t tick_1s    = 0;
	/* 72 MHz �ں�ʱ�� �� 1 kHz SysTick */
    SysTick_Config(SystemCoreClock / 1000);
	// ��ʼ����������
    BOARD_InitAll();
	
	// ��ʼ������ģ��
		MODULE_InitAll();
	
	// ��ʼ��������
		Sensor_Init();
	
		UsartPrintf(USART_DEBUG,"�����ɹ�\r\n");
    //Sensor_GetValueAll();
    while (1) {
		uint32_t now = uwTick;
		/* 100 ms����ʪ�� + DS18B20 */
		if (now - tick_100ms >= 100) {
				tick_100ms = now;
				Sensor_GetValueDHT11();
		}

		/* 200 ms��ADC1(��������)/ADC2(MQ-2����) */
		if (now - tick_200ms >= 200) {
				tick_200ms = now;
				Sensor_GetValueADC();
				if(g_env.adc1 <= LIGHT_THRESHOLD)//�����������ֵ,����
				{
					 LED_On(&BOARD_LED1);
				}
				if(g_env.adc2 >= SMOKE_THRESHOLD)//����Ũ�ȸ�����ֵ����������
				{
					Beep_On(&BOARD_BEEP);
					UsartPrintf(USART_DEBUG,"����Ũ�ȹ��ߣ�\n");
				}
		}

		/* 500 ms�������� + OLED ˢ�� */
		if (now - tick_500ms >= 500) {
				tick_500ms = now;
				g_env.sw[0]=INPUT1_DO;
				g_env.sw[1]=INPUT2_DO;
				switch(g_env.sw[0])//��δ�������⵽��Σ���������
				{
					case 0:Beep_Off(&BOARD_BEEP);UsartPrintf(USART_DEBUG,"��δ���������������\n");break;
					case 1:Beep_On(&BOARD_BEEP);UsartPrintf(USART_DEBUG,"��⵽��Σ�\n");break;
					default:UsartPrintf(USART_DEBUG,"��δ��������ֹ��ϣ�\n");break;
				}
				switch(g_env.sw[1])//���ⷴ�������ڵ�,��������
				{
					case 0:Beep_Off(&BOARD_BEEP);UsartPrintf(USART_DEBUG,"���ⷴ��������������\n");break;
					case 1:Beep_On(&BOARD_BEEP);UsartPrintf(USART_DEBUG,"���⴫�������ڵ���\n");break;
					default:UsartPrintf(USART_DEBUG,"���ⷴ�������ֹ��ϣ�\n");break;
				}				
				
		}

		/* 1 s���¿ط��� */
		if (now - tick_1s >= 1000) {
				tick_1s = now;
				if(g_env.temp >= TEMP_THRESHOLD)
				{
					 Sensor_GPIO_Output(true);//�򿪷���
					 Sensor_TestPWM(false);//�رմ���
				}
				else if(g_env.temp < TEMP_THRESHOLD|| g_env.humi > HIMI_THRESHOLD){
					 Sensor_GPIO_Output(false);//�رշ���
					 Sensor_TestPWM(true);//�򿪴���
				}				
		}	

  }
}
