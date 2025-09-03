#ifndef BEEP_H
#define BEEP_H

#include "system_config.h"


// ������״̬ö��
typedef enum {
    BEEP_OFF = 0,
    BEEP_ON = 1
} Beep_State;

// �������ṹ�嶨��
typedef struct {
    GPIO_TypeDef* GPIOx;     // GPIO�˿�
    uint16_t GPIO_Pin;       // GPIO����
    Beep_State state;        // ��������ǰ״̬
} Beep_TypeDef;

// ��������
void Beep_Init(Beep_TypeDef* beep, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Beep_On(Beep_TypeDef* beep);
void Beep_Off(Beep_TypeDef* beep);
void Beep_Toggle(Beep_TypeDef* beep);
Beep_State Beep_GetState(Beep_TypeDef* beep);

#endif /* BEEP_H */  
