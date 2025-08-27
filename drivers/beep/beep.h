#ifndef BEEP_H
#define BEEP_H

#include "system_config.h"

/************************************************
 �Ա����̣�https://shop475501589.taobao.com/?spm=pc_detail.29232929/evo365560b447259.shop_block.dshopinfo.5dd97dd6JvMuG3
 ������̣�https://www.goofish.com/personal?spm=a21ybx.item.itemHeader.1.c17a3da6hy8k28&userId=3890583014
 ����������https://space.bilibili.com/482024430?spm_id_from=333.788.upinfo.detail.click
 ���ߣ�ʤ�ڵ���
************************************************/

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
