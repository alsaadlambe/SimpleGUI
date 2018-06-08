#include "stm32f10x_usart.h"
#include "Usart.h"
#include "DemoActions.h"

static uint8_t          s_cbReceivedByte = KEY_NONE;

/************************************************************************/
/** Function Name:	USART_Config.										**/
/** Purpose:			Configure USART controler.							*/
/** Resources:		None.												*/
/** Params:																*/
/**	@pstUSARTx:			Usart device register pointer.					*/
/** Return:			None.												*/
/** Limitation:		None.												*/
/************************************************************************/
void USART_Config(USART_TypeDef* pstUSARTx, uint32_t ui_Baudrate, bool b_Enable)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate =ui_Baudrate;				// Baudrate.
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		// 8 data byte.
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			// 1 bit stop.
	USART_InitStructure.USART_Parity = USART_Parity_No;				// No parity.
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // No hardware stream control.
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // RX/TX mode.
	USART_Init(pstUSARTx, &USART_InitStructure);					// Configuration USART controler.

	if(b_Enable == true)
	{
		USART_ITConfig(pstUSARTx, USART_IT_RXNE, ENABLE);			// Enable USART interrupt.
	}

	USART_Cmd(pstUSARTx, ENABLE);									// Enable USART controler.
	USART_ClearFlag(pstUSARTx, USART_FLAG_TXE);						// Clear transmission complete flag, start transmission.
}

void USART_SendByte(USART_TypeDef* pstUSARTx, uint16_t uiByte)
{
    USART_ClearFlag(pstUSARTx,USART_FLAG_TC);
    // Send data.
    USART_SendData(pstUSARTx, uiByte);
    // Wait for send complete.
    //while(USART_GetFlagStatus(pstUSARTx, USART_FLAG_TXE) == RESET);
    while(USART_GetFlagStatus(pstUSARTx, USART_FLAG_TC) != SET);
}

/* USART-1 receive interrupt function. */
void USART1_IRQHandler(void)
{
	uint8_t cbReceiveByte;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
		cbReceiveByte = USART_ReceiveData(USART1);
		s_cbReceivedByte = cbReceiveByte;
    }
}

uint8_t GetReveivedByte(void)
{
    return s_cbReceivedByte;
}

void ResetReveivedByte(void)
{
    s_cbReceivedByte = KEY_NONE;
}
