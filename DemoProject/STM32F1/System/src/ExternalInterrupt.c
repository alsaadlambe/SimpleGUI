#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "GPIO.h"
#include "ExternalInterrupt.h"

#include "stdio.h"

static uint8_t uiFlag = 0;

GPIO_OBJECT_STRUCT astInterruptGPIOs[] =
{
	{RCC_APB2Periph_GPIOF, GPIOF, GPIO_Pin_14, GPIO_Mode_IPU, GPIO_Speed_2MHz},
	{RCC_APB2Periph_GPIOF, GPIOF, GPIO_Pin_15, GPIO_Mode_IPU, GPIO_Speed_2MHz},
};

void	ExtInt_GPIOConfigure(void);
void	ExtInt_NVICConfigure(uint16_t uiChannelPin);
void	ExtInt_SourceConfigure(EXTI_STRUCT* astExtiData, FunctionalState ExtiEnable);

/*****************************************************************************/
/** Function Name:	ExInt_Initialize.									    **/
/** Purpose:			Configure external interrupt.						**/
/** Resources:		None.												    **/
/** Params:			None.												    **/
/** Return:			None.												    **/
/** Notice:		    None.                                                   **/
/*****************************************************************************/
void ExInt_Initialize(void)
{
	// Configure NVIC and interrupt sourcce.
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	// Configure GPIOs.
	ExtInt_GPIOConfigure();

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

	//GPIOF.14
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource14);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line14;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	//GPIOF.15
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource15);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}


void ExtInt_Enable(uint32_t uiExtiLine)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line=uiExtiLine;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
}

void ExtInt_Disable(uint32_t uiExtiLine)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line=uiExtiLine;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
}

/*****************************************************************************/
/** Function Name:	ExtInt_GPIOConfigure.								    **/
/** Purpose:			Configure external interrupt source GPIOs.			**/
/** Resources:		None.												    **/
/** Params:			None.												    **/
/** Return:			None.												    **/
/** Notice:		    None.                                                   **/
/*****************************************************************************/
void ExtInt_GPIOConfigure(void)
{
	// Enable AFIO.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// Configure GPIOs.
	GPIO_Initialize(astInterruptGPIOs, 2);
}

/*****************************************************************************/
/** Function Name:	ExtInt_GPIOConfigure.								    **/
/** Purpose:			Configure external interrupt source GPIOs.			**/
/** Resources:		None.												    **/
/** Params:			None.												    **/
/** Return:			None.												    **/
/** Notice:		    None.                                                   **/
/*****************************************************************************/
void ExtInt_NVICConfigure(uint16_t uiChannelPin)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	// Set external interrupt.
	NVIC_InitStructure.NVIC_IRQChannel = uiChannelPin;
	// Set preemption priority
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	//Set sub priority
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	// Enable interrupt.
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// Update NVIC configuration.
	NVIC_Init(&NVIC_InitStructure);
}

/*****************************************************************************/
/** Function Name:	ExtInt_GPIOConfigure								    **/
/** Purpose:			Configure external interrupt source GPIOs.			**/
/** Resources:		EXTILine and GPIO.									    **/
/** Params:			None.												    **/
/** Return:			None.												    **/
/** Notice:		    None.                                                   **/
/*****************************************************************************/
void ExtInt_SourceConfigure(EXTI_STRUCT* astExtiData, FunctionalState ExtiEnable)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	// Clear interrupt pendin bit.
	EXTI_ClearITPendingBit(astExtiData->EXTILine);
	// Connect interrupt source
	GPIO_EXTILineConfig(astExtiData->SourcePort, astExtiData->SourcePin);
	// Configure interrupt.
	EXTI_InitStructure.EXTI_Line	= astExtiData->EXTILine;
	EXTI_InitStructure.EXTI_Mode	= astExtiData->Mode;
	EXTI_InitStructure.EXTI_Trigger = astExtiData->Trigger;
	EXTI_InitStructure.EXTI_LineCmd	= ExtiEnable;
	// Update external interrupt controler configuration.
	EXTI_Init(&EXTI_InitStructure);
}

/*****************************************************************************/
/** Function Name:	EXTI15_10_IRQHandler								    **/
/** Purpose:			Externnal iunterrupt service function for line 10	**/
/**					to 15.``											    **/
/** Resources:		None.												    **/
/** Params:			None.												    **/
/** Return:			None.												    **/
/** Notice:		    None.                                                   **/
/*****************************************************************************/
void EXTI15_10_IRQHandler(void)
{
	if ( EXTI_GetITStatus(EXTI_Line15) == SET)
	{
		// Add interrupt process here for EXTI line 15.
		PE_OUT(6) = !PE_OUT(6);
		printf("exint 15, flag is %d.\r\n", uiFlag);
		EXTI_ClearITPendingBit(EXTI_Line15);
		if(uiFlag == 0)
		{
			printf("Disable exint 14.\r\n");
			ExtInt_Disable(EXTI_Line14);
		}
		else
		{
			printf("Enable exint 14.\r\n");
			ExtInt_Enable(EXTI_Line14);
		}
		uiFlag ++;
		uiFlag %= 2;
	}
	if ( EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		// Add interrupt process here for EXTI line 14.
		PE_OUT(5) = !PE_OUT(5);
		printf("exint 14.\r\n");
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	if ( EXTI_GetITStatus(EXTI_Line13) == SET)
	{
		// Add interrupt process here for EXTI line 13.
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if ( EXTI_GetITStatus(EXTI_Line12) == SET)
	{
		// Add interrupt process here for EXTI line 12.
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	if ( EXTI_GetITStatus(EXTI_Line11) == SET)
	{
		// Add interrupt process here for EXTI line 11.
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	if ( EXTI_GetITStatus(EXTI_Line10) == SET)
	{
		// Add interrupt process here for EXTI line 10.
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
}

/*****************************************************************************/
/** Function Name:	EXTI9_5_IRQHandler									    **/
/** Purpose:		Externnal iunterrupt service function for line 5 to	9.  **/
/** Resources:		None.												    **/
/** Params:			None.												    **/
/** Return:			None.												    **/
/** Notice:         None.                                                   **/
/*****************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		// Add interrupt process here for EXTI line 9.
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		// Add interrupt process here for EXTI line 8.
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		// Add interrupt process here for EXTI line 7.
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		// Add interrupt process here for EXTI line 6.
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		// Add interrupt process here for EXTI line 5.
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
}

/*****************************************************************************/
/** Function Name:	EXTI4_IRQHandler									    **/
/** Purpose:			Externnal iunterrupt service function for line 4.	**/
/** Resources:		None.												    **/
/** Params:			None.												    **/
/** Return:			None.												    **/
/** Notice:		    None.												    **/
/*****************************************************************************/
void EXTI4_IRQHandler(void)
{
	// Add interrupt process here for EXTI line 4.
	EXTI_ClearITPendingBit(EXTI_Line4);
}

/*****************************************************************************/
/** Function Name:	EXTI3_IRQHandler									    **/
/** Purpose:			Externnal iunterrupt service function for line 3.	**/
/** Resources:		None.												    **/
/** Params:			None.												    **/
/** Return:			None.                                                   **/
/** Notice:		    None.												    **/
/*****************************************************************************/
void EXTI3_IRQHandler(void)
{
	// Add interrupt process here for EXTI line 3.
	EXTI_ClearITPendingBit(EXTI_Line3);
}

/*****************************************************************************/
/** Function Name:	EXTI2_IRQHandler									    **/
/** Purpose:			Externnal iunterrupt service function for line 3.	**/
/** Resources:		None.												    **/
/** Params:			None.												    **/
/** Return:			None.												    **/
/** Notice:		    None.												    **/
/*****************************************************************************/
void EXTI2_IRQHandler(void)
{
	// Add interrupt process here for EXTI line 2.
	EXTI_ClearITPendingBit(EXTI_Line2);
}

/*****************************************************************************/
/** Function Name:	EXTI1_IRQHandler									    **/
/** Purpose:			Externnal iunterrupt service function for line 3.	**/
/** Resources:		None.												    **/
/** Params:			None.												    **/
/** Return:			None.												    **/
/** Notice:		    None.											        **/
/*****************************************************************************/
void EXTI1_IRQHandler(void)
{
	// Add interrupt process here for EXTI line 1.
	EXTI_ClearITPendingBit(EXTI_Line1);
}

/*****************************************************************************/
/** Function Name:	EXTI0_IRQHandler									    **/
/** Purpose:			Externnal iunterrupt service function for line 0.	**/
/** Resources:		None.												    **/
/** Params:			None.												    **/
/** Return:			None.												    **/
/** Notice:		    None.                                                   **/
/*****************************************************************************/
void EXTI0_IRQHandler(void)
{
	// Add interrupt process here for EXTI line 0.
	EXTI_ClearITPendingBit(EXTI_Line0);
}
