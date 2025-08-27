#include "lcd_app.h"

#include <stdio.h>


/************************************************************************* �궨�� ********************************************************************/


/************************************************************************* ȫ�ֱ��� ******************************************************************/

/************************************************************************* �ֲ����� ******************************************************************/
// �����ַ�������
static const char *weekdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

/************************************************************************* �ֲ����� **************************************************************/
/*
************************************************************
*	�������ƣ�	LCD_Display_Info
*
*	�������ܣ�	��ʾ��Ϣ
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��
************************************************************
*/
void LCD_Display_Info(void)
{
	
}

/************************************************************************* ȫ�ֺ��� ******************************************************************/
void LCD_ShowTime(void)
{
	// ��ȡ��ǰ RTC ʱ��
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

// ˢ��������ʾ
void LCD_Refresh_Data(void)
{
	
}
