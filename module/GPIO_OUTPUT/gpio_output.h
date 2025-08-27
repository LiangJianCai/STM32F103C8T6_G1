#ifndef GPIO_OUTPUT_H
#define GPIO_OUTPUT_H

#include "system_config.h"

// GPIO ���״̬ö��
/**
 * @brief ����GPIO�����״̬
 */
typedef enum {
    GPIO_OUTPUT_LOW = 0,  /**< GPIO����͵�ƽ */
    GPIO_OUTPUT_HIGH = 1  /**< GPIO����ߵ�ƽ */
} GPIO_Output_State;

// GPIO ����ṹ��
/**
 * @brief ����GPIO����������Ϣ
 */
typedef struct {
    GPIO_Config_TypeDef config;    /**< GPIO���� */
    GPIO_Output_State state;       /**< GPIO��ǰ���״̬ */
} GPIO_Output_TypeDef;

// ��������
/**
 * @brief ��ʼ�� GPIO ���
 * @param output: GPIO ����ṹ��ָ��
 * @retval ��
 */
void GPIO_Output_Init(GPIO_Output_TypeDef* output);

/**
 * @brief ���� GPIO ���״̬
 * @param output: GPIO ����ṹ��ָ��
 * @param state: Ҫ���õ����״̬
 * @retval ��
 */
void GPIO_Output_SetState(GPIO_Output_TypeDef* output, GPIO_Output_State state);

/**
 * @brief ��ȡ GPIO ���״̬
 * @param output: GPIO ����ṹ��ָ��
 * @retval GPIO ���״̬
 */
GPIO_Output_State GPIO_Output_GetState(GPIO_Output_TypeDef* output);

#endif /* GPIO_OUTPUT_H */
