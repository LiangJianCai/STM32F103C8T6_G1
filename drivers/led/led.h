#ifndef LED_H
#define LED_H

#include "system_config.h"

/************************************************
 LED������
 �Ա����̣�https://shop475501589.taobao.com/?spm=pc_detail.29232929/evo365560b447259.shop_block.dshopinfo.5dd97dd6JvMuG3
 ������̣�https://www.goofish.com/personal?spm=a21ybx.item.itemHeader.1.c17a3da6hy8k28&userId=3890583014
 ����������https://space.bilibili.com/482024430?spm_id_from=333.788.upinfo.detail.click
 ���ߣ�ʤ�ڵ���
************************************************/

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
