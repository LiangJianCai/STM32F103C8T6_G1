#include "lcd_app.h"

#include <stdio.h>


/************************************************************************* 宏定义 ********************************************************************/


/************************************************************************* 全局变量 ******************************************************************/

/************************************************************************* 局部变量 ******************************************************************/
// 星期字符串数组
static const char *weekdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

/************************************************************************* 局部函数 **************************************************************/
/*
************************************************************
*	函数名称：	LCD_Display_Info
*
*	函数功能：	显示信息
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		无
************************************************************
*/
void LCD_Display_Info(void)
{
	
}

/************************************************************************* 全局函数 ******************************************************************/
void LCD_ShowTime(void)
{
	// 获取当前 RTC 时间
	RTC_GetCurrentTime(&currentTime);

	OLED_ShowNum(&OLED_Handle, 1, 1, currentTime.year,4);
	OLED_ShowString(&OLED_Handle, 1, 5, "-");
	OLED_ShowNum(&OLED_Handle, 1, 6, currentTime.month,2);
	OLED_ShowString(&OLED_Handle, 1, 8, "-");
	OLED_ShowNum(&OLED_Handle, 1, 9, currentTime.day,2);
	
	OLED_ShowString(&OLED_Handle, 1, 12, (char*)weekdays[currentTime.weekday]);
	
	OLED_ShowNum(&OLED_Handle, 2, 1, currentTime.hour,2);
	OLED_ShowString(&OLED_Handle, 2, 3, ":");
	OLED_ShowNum(&OLED_Handle, 2, 4, currentTime.minute,2);
	OLED_ShowString(&OLED_Handle, 2, 6, ":");
	OLED_ShowNum(&OLED_Handle, 2, 7, currentTime.second,2);
}

// 刷新数据显示
void LCD_Refresh_Data(void)
{
	
}
