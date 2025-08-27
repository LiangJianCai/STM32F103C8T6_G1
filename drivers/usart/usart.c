#include "usart.h"

/*
************************************************************
*	�������ƣ�	Usart_Init
*
*	�������ܣ�	ͨ�ô��ڳ�ʼ��
*
*	���������	params: ���ڳ�ʼ�������ṹ��ָ��
*               baud: ���õĲ�����
*
*	���ز�����	��
*
*	˵����		TX��RX���Ÿ��ݾ��崮�ڶ���
************************************************************
*/
void Usart_Init(USART_InitParams *params, unsigned int baud)
{
    GPIO_InitTypeDef gpio_initstruct;
    USART_InitTypeDef usart_initstruct;
    NVIC_InitTypeDef nvic_initstruct;

    // ʹ��GPIO�ʹ���ʱ��
    RCC_APB2PeriphClockCmd(params->RCC_APBxPeriph_GPIO, ENABLE);
    if (params->USARTx == USART1) {
        RCC_APB2PeriphClockCmd(params->RCC_APBxPeriph_USART, ENABLE);
    } else {
        RCC_APB1PeriphClockCmd(params->RCC_APBxPeriph_USART, ENABLE);
    }

    // ����TX����
    gpio_initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio_initstruct.GPIO_Pin = params->GPIO_Pin_Tx;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_initstruct);

    // ����RX����
    gpio_initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio_initstruct.GPIO_Pin = params->GPIO_Pin_Rx;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_initstruct);

    // ���ô��ڲ���
    usart_initstruct.USART_BaudRate = baud;
    usart_initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart_initstruct.USART_Parity = USART_Parity_No;
    usart_initstruct.USART_StopBits = USART_StopBits_1;
    usart_initstruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(params->USARTx, &usart_initstruct);

    // ʹ�ܴ���
    USART_Cmd(params->USARTx, ENABLE);

    // ʹ�ܽ����ж�
    USART_ITConfig(params->USARTx, USART_IT_RXNE, ENABLE);

    // ����NVIC
    nvic_initstruct.NVIC_IRQChannel = params->NVIC_IRQChannel;
    nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority = params->NVIC_IRQChannelPreemptionPriority;
    nvic_initstruct.NVIC_IRQChannelSubPriority = params->NVIC_IRQChannelSubPriority;
    NVIC_Init(&nvic_initstruct);
}

/*
************************************************************
*	�������ƣ�	Usart_SendString
*
*	�������ܣ�	�������ݷ���
*
*	��ڲ�����	USARTx��������
*				str��Ҫ���͵�����
*				len�����ݳ���
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//��������
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//�ȴ��������
	}

}

/*
************************************************************
*	�������ƣ�	UsartPrintf
*
*	�������ܣ�	��ʽ����ӡ
*
*	��ڲ�����	USARTx��������
*				fmt����������
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
{
	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);							//��ʽ��
	va_end(ap);
	
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}

}

/*
************************************************************
*	�������ƣ�	Usart_ReceiveData
*
*	�������ܣ�	�������ݽ���
*
*	��ڲ�����	USARTx��������
*				buffer�����ջ�����
*				max_len����������󳤶�
*
*	���ز�����	ʵ�ʽ��յ������ݳ���
*
*	˵����		�ú����᳢�ԴӴ��ڽ������max_len�ֽڵ����ݣ�
*				���洢��buffer�С����û�����ݿ��ã�����������0��
************************************************************
*/
unsigned short Usart_ReceiveData(USART_TypeDef *USARTx, unsigned char *buffer, unsigned short max_len)
{
    unsigned short count = 0;
    
    // ����Ƿ������ݿɽ���
    while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == SET && count < max_len)
    {
        // ��ȡ���ݲ��洢��������
        buffer[count++] = (unsigned char)USART_ReceiveData(USARTx);
    }
    
    return count;
}
