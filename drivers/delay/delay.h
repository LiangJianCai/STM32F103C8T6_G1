#ifndef DELAY_H
#define DELAY_H

#include "system_config.h"

/************************************************
 �Ա����̣�https://shop475501589.taobao.com/?spm=pc_detail.29232929/evo365560b447259.shop_block.dshopinfo.5dd97dd6JvMuG3
 ������̣�https://www.goofish.com/personal?spm=a21ybx.item.itemHeader.1.c17a3da6hy8k28&userId=3890583014
 ����������https://space.bilibili.com/482024430?spm_id_from=333.788.upinfo.detail.click
 ���ߣ�ʤ�ڵ���
************************************************/

/** ��ʱ�����ýṹ�� */
typedef struct {
    TIM_TypeDef* TIMx;              // ��ʱ��ָ��
    uint32_t TIMx_CLK;              // ��ʱ��ʱ��
    uint32_t APBxPeriph_CLKCmd;     // ʱ��ʹ�ܺ�������
    void (*CLKCmd)(uint32_t, FunctionalState); // ʱ��ʹ�ܺ���ָ��
} Delay_TimerConfigTypeDef;

/** ϵͳ֧�ֵ����ж�ʱ�����ñ� */
extern const Delay_TimerConfigTypeDef TimerConfigTable[];
extern const uint8_t TimerConfigTableSize;

/** ��ʱ������ʼ���Ͳ��� */
void Delay_Init(uint8_t timerIndex);
void DelayUs(uint16_t us);
void DelayMs(uint16_t ms);

/** ��ȡ��ǰʹ�õĶ�ʱ������ */
uint8_t Delay_GetCurrentTimerIndex(void);

#endif /* DELAY_H */   
