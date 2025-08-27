/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.6.0
  * @date    20-September-2021
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2011 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "board_config.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/*
************************************************************
*	函数名称：	USART1_IRQHandler
*
*	函数功能：	串口1收发中断
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void USART1_IRQHandler(void)
{
	static unsigned char rx_buffer[RX_BUFFER_SIZE];  // 接收缓冲区
	static unsigned short rx_len = 0;                // 已接收数据长度
	
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  // 接收中断触发
    {
        // 读取接收到的字符
        unsigned char data = (unsigned char)USART_ReceiveData(USART1);
        
        // 输入回显：立即将接收到的字符发送回去
        USART_SendData(USART1, data);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // 等待发送完成
        
        if (rx_len < RX_BUFFER_SIZE - 1)  // 避免缓冲区溢出
        {
            rx_buffer[rx_len++] = data;  // 存数据
            
            // 假设以“回车”（'\r'）作为输入结束标志
            if (data == '\r')
            {
                // 添加换行符以保持显示美观
                USART_SendData(USART1, '\n');
                while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
                
                rx_buffer[rx_len] = '\0';  // 添加字符串结束符
                // 这里可以调用处理函数（如打印数据）
                UsartPrintf(USART1, "Received: %s\r\n", rx_buffer);
                rx_len = 0;  // 重置缓冲区，准备下次接收
            }
        }
        else
        {
            rx_len = 0;  // 缓冲区满，重置
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);  // 清除中断标志
    }
}


/*
************************************************************
*	函数名称：	USART2_IRQHandler
*
*	函数功能：	串口2收发中断
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收中断
	{
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
}

/*
************************************************************
*	函数名称：	KEYUP_IRQHandler
*
*	函数功能：	WK_UP 按键中断处理函数
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void KEYUP_IRQHandler(void)
{
	if(Key_Read(BOARD_KEYS[0].GPIO_PORT,BOARD_KEYS[0].GPIO_PIN)==1)		//WK_UP按键 切换系统模式
	{
		while(Key_Read(BOARD_KEYS[0].GPIO_PORT,BOARD_KEYS[0].GPIO_PIN)==1);
		
		UsartPrintf(USART_DEBUG, "KEYUP_IRQHandler \r\n");
	}
	EXTI_ClearITPendingBit(BOARD_KEYS[0].EXTI_LINE);
}

/*
************************************************************
*	函数名称：	KEY012_IRQHandler
*
*	函数功能：	KEY0、KEY1、KEY2 按键中断处理函数
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void KEY012_IRQHandler(void)
{
	/*
	* 按键：KEY0
	* 功能：
	*	自动模式：切换设置项
	*	手动模式：无
	*	定时模式：切换设置项
	*/
	if(Key_Read(BOARD_KEYS[1].GPIO_PORT,BOARD_KEYS[1].GPIO_PIN)==0)
	{
		while(Key_Read(BOARD_KEYS[1].GPIO_PORT,BOARD_KEYS[1].GPIO_PIN)==0);
		
		UsartPrintf(USART_DEBUG, "KEY0_IRQHandler \r\n");

		EXTI_ClearITPendingBit(BOARD_KEYS[1].EXTI_LINE);
	}
	
	/*
	* 按键：KEY1
	* 功能：
	*	自动模式：减
	*	手动模式：减
	*	定时模式：减
	*/
	if(Key_Read(BOARD_KEYS[2].GPIO_PORT,BOARD_KEYS[2].GPIO_PIN)==0)
	{
		while(Key_Read(BOARD_KEYS[2].GPIO_PORT,BOARD_KEYS[2].GPIO_PIN)==0);
		
		UsartPrintf(USART_DEBUG, "KEY1_IRQHandler \r\n");
		
		EXTI_ClearITPendingBit(BOARD_KEYS[2].EXTI_LINE);
	}
	
	/*
	* 按键：KEY2
	* 功能：
	*	自动模式：加
	*	手动模式：加
	*	定时模式：加
	*/
	if(Key_Read(BOARD_KEYS[3].GPIO_PORT,BOARD_KEYS[3].GPIO_PIN)==0)
	{
		while(Key_Read(BOARD_KEYS[3].GPIO_PORT,BOARD_KEYS[3].GPIO_PIN)==0);
		
		UsartPrintf(USART_DEBUG, "KEY2_IRQHandler \r\n");

		EXTI_ClearITPendingBit(BOARD_KEYS[3].EXTI_LINE);
	}
	
	/*
	* 按键：KEY3
	* 功能：
	*	自动模式：加
	*	手动模式：加
	*	定时模式：加
	*/
	if(Key_Read(BOARD_KEYS[4].GPIO_PORT,BOARD_KEYS[4].GPIO_PIN)==0)
	{
		while(Key_Read(BOARD_KEYS[4].GPIO_PORT,BOARD_KEYS[4].GPIO_PIN)==0);
		
		UsartPrintf(USART_DEBUG, "KEY3_IRQHandler \r\n");

		EXTI_ClearITPendingBit(BOARD_KEYS[4].EXTI_LINE);
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


