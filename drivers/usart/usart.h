#ifndef _USART_H_
#define _USART_H_


#include "system_config.h"


// 定义串口初始化参数结构体
typedef struct {
    USART_TypeDef *USARTx;          // 串口选择
    uint32_t RCC_APBxPeriph_USART;  // 串口时钟
    uint32_t RCC_APBxPeriph_GPIO;   // GPIO时钟
    uint16_t GPIO_Pin_Tx;           // 发送引脚
    uint16_t GPIO_Pin_Rx;           // 接收引脚
    uint8_t NVIC_IRQChannel;        // 中断通道
    uint8_t NVIC_IRQChannelPreemptionPriority; // 主优先级
    uint8_t NVIC_IRQChannelSubPriority;        // 子优先级
} USART_InitParams;


void Usart_Init(USART_InitParams *params, unsigned int baud);

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);

unsigned short Usart_ReceiveData(USART_TypeDef *USARTx, unsigned char *buffer, unsigned short max_len);

#endif
