#ifndef __SENSOR_APP_H
#define __SENSOR_APP_H


#include "board_config.h"

#include "module_config.h"

extern GPIO_Input_State INPUT1_DO;	// 正常：1 异常：0
extern GPIO_Input_State INPUT2_DO;	// 正常：1 异常：0
extern GPIO_Input_State INPUT3_DO;	// 正常：1 异常：0


extern uint64_t nowTimeStamp;	// 当前时间戳

void Sensor_Init(void);

void Sensor_SetTime(void);

void Sensor_GetValueAll(void);

void Sensor_GetValueInputGPIO(void);

void Sensor_GetValueADC(void);

void Sensor_GPIO_Output(bool FanStatues);

void Sensor_TestPWM(bool WindowsStates);

void Sensor_GetValueDHT11(void);

#endif	/*__SENSOR_APP_H*/
