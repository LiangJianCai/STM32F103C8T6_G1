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
*	�������ƣ�	USART1_IRQHandler
*
*	�������ܣ�	����1�շ��ж�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void USART1_IRQHandler(void)
{
	static unsigned char rx_buffer[RX_BUFFER_SIZE];  // ���ջ�����
	static unsigned short rx_len = 0;                // �ѽ������ݳ���
	
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  // �����жϴ���
    {
        // ��ȡ���յ����ַ�
        unsigned char data = (unsigned char)USART_ReceiveData(USART1);
        
        // ������ԣ����������յ����ַ����ͻ�ȥ
        USART_SendData(USART1, data);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // �ȴ��������
        
        if (rx_len < RX_BUFFER_SIZE - 1)  // ���⻺�������
        {
            rx_buffer[rx_len++] = data;  // ������
            
            // �����ԡ��س�����'\r'����Ϊ���������־
            if (data == '\r')
            {
                // ��ӻ��з��Ա�����ʾ����
                USART_SendData(USART1, '\n');
                while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
                
                rx_buffer[rx_len] = '\0';  // ����ַ���������
                // ������Ե��ô����������ӡ���ݣ�
                UsartPrintf(USART1, "Received: %s\r\n", rx_buffer);
                rx_len = 0;  // ���û�������׼���´ν���
            }
        }
        else
        {
            rx_len = 0;  // ��������������
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);  // ����жϱ�־
    }
}


/*
************************************************************
*	�������ƣ�	USART2_IRQHandler
*
*	�������ܣ�	����2�շ��ж�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //�����ж�
	{
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
}

/*
************************************************************
*	�������ƣ�	KEYUP_IRQHandler
*
*	�������ܣ�	WK_UP �����жϴ�����
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void KEYUP_IRQHandler(void)
{
	if(Key_Read(BOARD_KEYS[0].GPIO_PORT,BOARD_KEYS[0].GPIO_PIN)==1)		//WK_UP���� �л�ϵͳģʽ
	{
		while(Key_Read(BOARD_KEYS[0].GPIO_PORT,BOARD_KEYS[0].GPIO_PIN)==1);
		
		UsartPrintf(USART_DEBUG, "KEYUP_IRQHandler \r\n");
	}
	EXTI_ClearITPendingBit(BOARD_KEYS[0].EXTI_LINE);
}

/*
************************************************************
*	�������ƣ�	KEY012_IRQHandler
*
*	�������ܣ�	KEY0��KEY1��KEY2 �����жϴ�����
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void KEY012_IRQHandler(void)
{
	/*
	* ������KEY0
	* ���ܣ�
	*	�Զ�ģʽ���л�������
	*	�ֶ�ģʽ����
	*	��ʱģʽ���л�������
	*/
	if(Key_Read(BOARD_KEYS[1].GPIO_PORT,BOARD_KEYS[1].GPIO_PIN)==0)
	{
		while(Key_Read(BOARD_KEYS[1].GPIO_PORT,BOARD_KEYS[1].GPIO_PIN)==0);
		
		UsartPrintf(USART_DEBUG, "KEY0_IRQHandler \r\n");

		EXTI_ClearITPendingBit(BOARD_KEYS[1].EXTI_LINE);
	}
	
	/*
	* ������KEY1
	* ���ܣ�
	*	�Զ�ģʽ����
	*	�ֶ�ģʽ����
	*	��ʱģʽ����
	*/
	if(Key_Read(BOARD_KEYS[2].GPIO_PORT,BOARD_KEYS[2].GPIO_PIN)==0)
	{
		while(Key_Read(BOARD_KEYS[2].GPIO_PORT,BOARD_KEYS[2].GPIO_PIN)==0);
		
		UsartPrintf(USART_DEBUG, "KEY1_IRQHandler \r\n");
		
		EXTI_ClearITPendingBit(BOARD_KEYS[2].EXTI_LINE);
	}
	
	/*
	* ������KEY2
	* ���ܣ�
	*	�Զ�ģʽ����
	*	�ֶ�ģʽ����
	*	��ʱģʽ����
	*/
	if(Key_Read(BOARD_KEYS[3].GPIO_PORT,BOARD_KEYS[3].GPIO_PIN)==0)
	{
		while(Key_Read(BOARD_KEYS[3].GPIO_PORT,BOARD_KEYS[3].GPIO_PIN)==0);
		
		UsartPrintf(USART_DEBUG, "KEY2_IRQHandler \r\n");

		EXTI_ClearITPendingBit(BOARD_KEYS[3].EXTI_LINE);
	}
	
	/*
	* ������KEY3
	* ���ܣ�
	*	�Զ�ģʽ����
	*	�ֶ�ģʽ����
	*	��ʱģʽ����
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


