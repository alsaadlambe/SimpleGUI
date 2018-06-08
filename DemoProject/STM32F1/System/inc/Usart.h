#ifndef __INCLUDE_SYSTEM_USART_H__
#define __INCLUDE_SYSTEM_USART_H__
#include <stdbool.h>
#include "stm32f10x_usart.h"

void    USART_Config(USART_TypeDef* st_USARTx, uint32_t ui_Baudrate, bool b_Enable);
void    USART_SendByte(USART_TypeDef* pstUSARTx, uint16_t uiByte);
uint8_t GetReveivedByte(void);
void    ResetReveivedByte(void);

#endif
