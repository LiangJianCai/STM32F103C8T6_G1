#include "usart.h"

/*
************************************************************
*	函数名称：	Usart_Init
*
*	函数功能：	通用串口初始化
*
*	输入参数：	params: 串口初始化参数结构体指针
*               baud: 设置的波特率
*
*	返回参数：	无
*
*	说明：		TX和RX引脚根据具体串口而定
************************************************************
*/
void Usart_Init(USART_InitParams *params, unsigned int baud)
{
    GPIO_InitTypeDef gpio_initstruct;
    USART_InitTypeDef usart_initstruct;
    NVIC_InitTypeDef nvic_initstruct;

    // 使能GPIO和串口时钟
    RCC_APB2PeriphClockCmd(params->RCC_APBxPeriph_GPIO, ENABLE);
    if (params->USARTx == USART1) {
        RCC_APB2PeriphClockCmd(params->RCC_APBxPeriph_USART, ENABLE);
    } else {
        RCC_APB1PeriphClockCmd(params->RCC_APBxPeriph_USART, ENABLE);
    }

    // 配置TX引脚
    gpio_initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio_initstruct.GPIO_Pin = params->GPIO_Pin_Tx;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_initstruct);

    // 配置RX引脚
    gpio_initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio_initstruct.GPIO_Pin = params->GPIO_Pin_Rx;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_initstruct);

    // 配置串口参数
    usart_initstruct.USART_BaudRate = baud;
    usart_initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart_initstruct.USART_Parity = USART_Parity_No;
    usart_initstruct.USART_StopBits = USART_StopBits_1;
    usart_initstruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(params->USARTx, &usart_initstruct);

    // 使能串口
    USART_Cmd(params->USARTx, ENABLE);

    // 使能接收中断
    USART_ITConfig(params->USARTx, USART_IT_RXNE, ENABLE);

    // 配置NVIC
    nvic_initstruct.NVIC_IRQChannel = params->NVIC_IRQChannel;
    nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority = params->NVIC_IRQChannelPreemptionPriority;
    nvic_initstruct.NVIC_IRQChannelSubPriority = params->NVIC_IRQChannelSubPriority;
    NVIC_Init(&nvic_initstruct);
}

/*
************************************************************
*	函数名称：	Usart_SendString
*
*	函数功能：	串口数据发送
*
*	入口参数：	USARTx：串口组
*				str：要发送的数据
*				len：数据长度
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//发送数据
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
	}

}

/*
************************************************************
*	函数名称：	UsartPrintf
*
*	函数功能：	格式化打印
*
*	入口参数：	USARTx：串口组
*				fmt：不定长参
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
{
	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);							//格式化
	va_end(ap);
	
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}

}

/*
************************************************************
*	函数名称：	Usart_ReceiveData
*
*	函数功能：	串口数据接收
*
*	入口参数：	USARTx：串口组
*				buffer：接收缓冲区
*				max_len：缓冲区最大长度
*
*	返回参数：	实际接收到的数据长度
*
*	说明：		该函数会尝试从串口接收最多max_len字节的数据，
*				并存储到buffer中。如果没有数据可用，将立即返回0。
************************************************************
*/
unsigned short Usart_ReceiveData(USART_TypeDef *USARTx, unsigned char *buffer, unsigned short max_len)
{
    unsigned short count = 0;
    
    // 检查是否有数据可接收
    while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == SET && count < max_len)
    {
        // 读取数据并存储到缓冲区
        buffer[count++] = (unsigned char)USART_ReceiveData(USARTx);
    }
    
    return count;
}
