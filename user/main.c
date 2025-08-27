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
int main(void)
{
	// ��ʼ����������
    BOARD_InitAll();
	
	// ��ʼ������ģ��
		MODULE_InitAll();
	
	// ��ʼ��������
		Sensor_Init();
	
		UsartPrintf(USART_DEBUG,"�����ɹ�\r\n");
    
    while (1) {
		// �õ�����������
		Sensor_GetValueAll();
		
		// ��˸LED1
    LED_Toggle(&BOARD_LED1);
		
		DelayMs(250);
    }
}
