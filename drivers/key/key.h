#ifndef _KEY_H_
#define _KEY_H_

#include "system_config.h"

/************************************************
 �Ա����̣�https://shop475501589.taobao.com/?spm=pc_detail.29232929/evo365560b447259.shop_block.dshopinfo.5dd97dd6JvMuG3
 ������̣�https://www.goofish.com/personal?spm=a21ybx.item.itemHeader.1.c17a3da6hy8k28&userId=3890583014
 ����������https://space.bilibili.com/482024430?spm_id_from=333.788.upinfo.detail.click
 ���ߣ�ʤ�ڵ���
************************************************/

// ������ʼ���ṹ��
typedef struct {
    uint32_t GPIO_CLK;
    GPIO_TypeDef* GPIO_PORT;
    uint16_t GPIO_PIN;
    uint8_t EXTI_PORT;
    uint8_t EXTI_PIN;
    uint32_t EXTI_LINE;
    uint8_t EXTI_IRQn;
	uint8_t NVIC_IRQChannelPreemptionPriority;  // �����ȼ�
    uint8_t NVIC_IRQChannelSubPriority;         // �����ȼ�
    EXTITrigger_TypeDef Trigger;  				// ������ʽ��EXTI_Trigger_Rising �� EXTI_Trigger_Falling
    GPIOMode_TypeDef GPIO_Mode;  				// GPIOģʽ
} Key_InitTypeDef;

// ������ʼ������
void Key_Init(Key_InitTypeDef* keys, uint8_t key_num);

// ��ȡ����״̬
uint8_t Key_Read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
