#include <stdarg.h>
#include "uart1.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void uart1_init(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
	/* Configure USART1 Rx (PA10) as input floating                             */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 Tx (PA9) as alternate function push-pull                */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	/* Configure the USART1 */
	USART_Init(USART1, &USART_InitStructure);
	
	/* Enable the USART Transmoit interrupt: this interrupt is generated when the 
	USART1 transmit data register is empty */  
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	
	/* Enable the USART Receive interrupt: this interrupt is generated when the 
	USART1 receive data register is not empty */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	/* Enable USART1 */
	USART_Cmd(USART1, ENABLE);
}
void uart1_send_string(char *str)
{
	while(*str)
	{
		USART_SendData(USART1,*str++);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}
/*
void uart1_printf(char *fmt,...) 
{       
	va_list ap;      
	char string[100];       
	va_start(ap,fmt);    
	vsprintf(string,fmt,ap);      
	uart1_send_string(string);   
	va_end(ap);
}
*/
