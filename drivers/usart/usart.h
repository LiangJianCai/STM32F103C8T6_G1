#ifndef _USART_H_
#define _USART_H_


#include "system_config.h"


// ���崮�ڳ�ʼ�������ṹ��
typedef struct {
    USART_TypeDef *USARTx;          // ����ѡ��
    uint32_t RCC_APBxPeriph_USART;  // ����ʱ��
    uint32_t RCC_APBxPeriph_GPIO;   // GPIOʱ��
    uint16_t GPIO_Pin_Tx;           // ��������
    uint16_t GPIO_Pin_Rx;           // ��������
    uint8_t NVIC_IRQChannel;        // �ж�ͨ��
    uint8_t NVIC_IRQChannelPreemptionPriority; // �����ȼ�
    uint8_t NVIC_IRQChannelSubPriority;        // �����ȼ�
} USART_InitParams;


void Usart_Init(USART_InitParams *params, unsigned int baud);

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);

unsigned short Usart_ReceiveData(USART_TypeDef *USARTx, unsigned char *buffer, unsigned short max_len);

#endif
