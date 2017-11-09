#ifndef __UART1_H__
#define __UART1_H__
//#include "stm32f10x_lib.h"


void uart1_init(void);
void uart1_send_string(char *str);
void uart1_printf(char *fmt,...);
#endif
