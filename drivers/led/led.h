#ifndef LED_H
#define LED_H

#include "system_config.h"


// LED״̬ö��
typedef enum {
    LED_OFF = 0,
    LED_ON = 1
} LED_State;

// LED�ṹ�嶨��
typedef struct {
    GPIO_TypeDef* GPIOx;       // GPIO�˿�
    uint16_t GPIO_Pin;         // GPIO����
    LED_State state;           // LED��ǰ״̬
} LED_TypeDef;

// ��������
void LED_Init(LED_TypeDef* led, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void LED_On(LED_TypeDef* led);
void LED_Off(LED_TypeDef* led);
void LED_Toggle(LED_TypeDef* led);
LED_State LED_GetState(LED_TypeDef* led);

#endif /* LED_H */
