#ifndef __LCD_APP_H
#define __LCD_APP_H

#include "board_config.h"

#include "module_config.h"

/************************************************
 * @brief 淘宝店铺链接
 * @details 提供淘宝店铺的链接，可访问相关商品。
 * @note 此链接指向 https://shop475501589.taobao.com/?spm=pc_detail.29232929/evo365560b447259.shop_block.dshopinfo.5dd97dd6JvMuG3
 ************************************************/
/************************************************
 * @brief 咸鱼店铺链接
 * @details 提供咸鱼店铺的链接，可查看相关物品。
 * @note 此链接指向 https://www.goofish.com/personal?spm=a21ybx.item.itemHeader.1.c17a3da6hy8k28&userId=3890583014
 ************************************************/
/************************************************
 * @brief 哔哩哔哩空间链接
 * @details 提供哔哩哔哩空间的链接，可查看相关内容。
 * @note 此链接指向 https://space.bilibili.com/482024430?spm_id_from=333.788.upinfo.detail.click
 ************************************************/
/************************************************
 * @brief 作者信息
 * @details 代码的作者为胜磊电子。
 ************************************************/
 

void LCD_Display_Init(void);

void LCD_Refresh_Data(void);

void LCD_ShowTime(void);

#endif
