#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "GPIO.h"

// Global GPIO initialize data.
GPIO_OBJECT_STRUCT	astGPIOInitData[] =
{
	{RCC_APB2Periph_GPIOE, GPIOE, GPIO_Pin_5, GPIO_Mode_Out_PP, GPIO_Speed_2MHz, GPIO_STATE_LOW},
	{RCC_APB2Periph_GPIOE, GPIOE, GPIO_Pin_6, GPIO_Mode_Out_PP, GPIO_Speed_2MHz, GPIO_STATE_HIGH},
};

/*****************************************************************************/
/** Function Name:	GlobalGPIO_Initialize								    **/
/** Purpose:			Initialize global GPIO port.						**/
/** Resources:		Unspecified.										    **/
/** Params:			None												    **/
/** Return:			None												    **/
/** Notice:		None												        **/
/*****************************************************************************/
void GPIO_Initialize(const GPIO_OBJECT_STRUCT* pstGPIODataList, uint16_t uiArrayListLength)
{
	uint8_t i;
	// Initialize data structure.
	GPIO_InitTypeDef GPIO_InitStructure;
	// Loop for initialize each GPIO in array list.
	for(i=0; i<uiArrayListLength; i++)
	{
		// Enable RCC.
		RCC_APB2PeriphClockCmd(pstGPIODataList[i].PortRCC, ENABLE);
		// Enable GPIO.
		GPIO_InitStructure.GPIO_Pin =	pstGPIODataList[i].Pin;
		GPIO_InitStructure.GPIO_Mode =	pstGPIODataList[i].Mode;
		GPIO_InitStructure.GPIO_Speed =	pstGPIODataList[i].Speed;
		GPIO_Init(pstGPIODataList[i].Port, &GPIO_InitStructure);
		if(GPIO_STATE_HIGH == pstGPIODataList[i].InitState)
        {
            GPIO_SetBits(pstGPIODataList[i].Port, pstGPIODataList[i].Pin);
        }
        else
        {
            GPIO_ResetBits(pstGPIODataList[i].Port, pstGPIODataList[i].Pin);
        }
	}
}

/*****************************************************************************/
/** Function Name:	GPIO_ReadPin.										    **/
/** Purpose:			Reads a GPIO whitch is in the input state.			**/
/** Resources:		GPIO control and GPIO device RCC.					    **/
/** Params:																    **/
/**	@uiIndex:			GPIO data index.								    **/
/** Return:			GPIO pin value(0 or 1).								    **/
/** Notice:		None.												        **/
/*****************************************************************************/
GPIO_STATE GPIO_ReadPin(const GPIO_OBJECT_STRUCT* astPinList, uint8_t uiIndex)
{
	// GPIO_Mode_AIN
	// GPIO_Mode_IN_FLOATING
	// GPIO_Mode_IPD
	// GPIO_Mode_IPU
	// GPIO_Mode_Out_OD
	// GPIO_Mode_Out_PP
	// GPIO_Mode_AF_OD
	// GPIO_Mode_AF_PP
	if(	(astPinList+uiIndex)->Mode == GPIO_Mode_Out_PP ||
		(astPinList+uiIndex)->Mode == GPIO_Mode_Out_OD)
	{
		return (GPIO_STATE)GPIO_ReadOutputDataBit((astPinList+uiIndex)->Port, (astPinList+uiIndex)->Pin);
	}
	else if((astPinList+uiIndex)->Mode == GPIO_Mode_IN_FLOATING ||
			(astPinList+uiIndex)->Mode == GPIO_Mode_IPD ||
			(astPinList+uiIndex)->Mode == GPIO_Mode_IPU)
	{
		return (GPIO_STATE)GPIO_ReadInputDataBit((astPinList+uiIndex)->Port, (astPinList+uiIndex)->Pin);
	}
	else
	{
		return GPIO_STATE_HIGH;
	}
}

/*****************************************************************************/
/** Function Name:	GPIO_SetPinHigh.									    **/
/** Purpose:			Set a GPIO to high.									**/
/** Resources:		GPIO control and GPIO device RCC.					    **/
/** Params:																    **/
/**	@uiIndex:			GPIO data index.								    **/
/** Return:			None.												    **/
/** Notice:		    None.												    **/
/*****************************************************************************/
void GPIO_SetPinHigh(const GPIO_OBJECT_STRUCT* astPinList, uint8_t uiIndex)
{
	GPIO_SetBits((astPinList+uiIndex)->Port, (astPinList+uiIndex)->Pin);
}

/*****************************************************************************/
/** Function Name:	TM7707_SetPinLow.									    **/
/** Purpose:			Set a GPIO to low.									**/
/** Resources:		GPIO control and GPIO device RCC.					    **/
/** Params:																    **/
/**	@uiIndex:			GPIO data index.								    **/
/** Return:			None.												    **/
/** Notice:		None.												        **/
/*****************************************************************************/
void GPIO_SetPinLow(const GPIO_OBJECT_STRUCT* astPinList, uint8_t uiIndex)
{
	GPIO_ResetBits((astPinList+uiIndex)->Port, (astPinList+uiIndex)->Pin);
}

