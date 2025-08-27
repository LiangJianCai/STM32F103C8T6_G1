#ifndef _USART_H_
#define _USART_H_


#include "system_config.h"

/************************************************
 �Ա����̣�https://shop475501589.taobao.com/?spm=pc_detail.29232929/evo365560b447259.shop_block.dshopinfo.5dd97dd6JvMuG3
 ������̣�https://www.goofish.com/personal?spm=a21ybx.item.itemHeader.1.c17a3da6hy8k28&userId=3890583014
 ����������https://space.bilibili.com/482024430?spm_id_from=333.788.upinfo.detail.click
 ���ߣ�ʤ�ڵ���
************************************************/

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
