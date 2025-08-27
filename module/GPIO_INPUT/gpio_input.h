#ifndef GPIO_INPUT_H
#define GPIO_INPUT_H

#include "system_config.h"

// GPIO ����״̬ö��
typedef enum {
    GPIO_INPUT_LOW = 0,
    GPIO_INPUT_HIGH = 1
} GPIO_Input_State;

// GPIO ����ṹ�嶨��
typedef struct {
    GPIO_Config_TypeDef config;    // GPIO ����
    GPIO_Input_State state;        // GPIO ��ǰ״̬
} GPIO_Input_TypeDef;

// ��������
void GPIO_Input_Init(GPIO_Input_TypeDef* input);
GPIO_Input_State GPIO_Input_GetState(GPIO_Input_TypeDef* input);
void GPIO_Input_UpdateState(GPIO_Input_TypeDef* input);

#endif /* GPIO_INPUT_H */
