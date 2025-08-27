#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

// �������б�Ҫ��STM32��׼��ͷ�ļ�
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// ��������ͷ�ļ�
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

// GPIO ���ýṹ��
typedef struct {
    GPIO_TypeDef* GPIOx;           // GPIO �˿�
    uint16_t GPIO_Pin;             // GPIO ����
    GPIOMode_TypeDef GPIO_Mode;    // GPIO ģʽ
    GPIOSpeed_TypeDef GPIO_Speed;  // GPIO �ٶ�
} GPIO_Config_TypeDef;

// ���û�ж���NULL���ֶ�����
#ifndef NULL
#define NULL ((void*)0)
#endif

#endif /* SYSTEM_CONFIG_H */    
