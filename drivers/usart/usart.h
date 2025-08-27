#ifndef _USART_H_
#define _USART_H_


#include "system_config.h"

/************************************************
 淘宝店铺：https://shop475501589.taobao.com/?spm=pc_detail.29232929/evo365560b447259.shop_block.dshopinfo.5dd97dd6JvMuG3
 咸鱼店铺：https://www.goofish.com/personal?spm=a21ybx.item.itemHeader.1.c17a3da6hy8k28&userId=3890583014
 哔哩哔哩：https://space.bilibili.com/482024430?spm_id_from=333.788.upinfo.detail.click
 作者：胜磊电子
************************************************/

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
