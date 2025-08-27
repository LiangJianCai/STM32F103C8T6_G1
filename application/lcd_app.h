#ifndef __LCD_APP_H
#define __LCD_APP_H

#include "board_config.h"

#include "module_config.h"

/************************************************
 * @brief �Ա���������
 * @details �ṩ�Ա����̵����ӣ��ɷ��������Ʒ��
 * @note ������ָ�� https://shop475501589.taobao.com/?spm=pc_detail.29232929/evo365560b447259.shop_block.dshopinfo.5dd97dd6JvMuG3
 ************************************************/
/************************************************
 * @brief �����������
 * @details �ṩ������̵����ӣ��ɲ鿴�����Ʒ��
 * @note ������ָ�� https://www.goofish.com/personal?spm=a21ybx.item.itemHeader.1.c17a3da6hy8k28&userId=3890583014
 ************************************************/
/************************************************
 * @brief ���������ռ�����
 * @details �ṩ���������ռ�����ӣ��ɲ鿴������ݡ�
 * @note ������ָ�� https://space.bilibili.com/482024430?spm_id_from=333.788.upinfo.detail.click
 ************************************************/
/************************************************
 * @brief ������Ϣ
 * @details ���������Ϊʤ�ڵ��ӡ�
 ************************************************/
 

void LCD_Display_Init(void);

void LCD_Refresh_Data(void);

void LCD_ShowTime(void);

#endif
