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
int main(void)
{
	// 初始化所有外设
    BOARD_InitAll();
	
	// 初始化所有模块
		MODULE_InitAll();
	
	// 初始化传感器
		Sensor_Init();
	
		UsartPrintf(USART_DEBUG,"开机成功\r\n");
    
    while (1) {
		// 得到传感器数据
		Sensor_GetValueAll();
		
		// 闪烁LED1
    LED_Toggle(&BOARD_LED1);
		
		DelayMs(250);
    }
}
