#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "Initialize.h"

/*****************************************************************************/
/** Function Name:	DebugPort_Initialize								    **/
/** Purpose:			Initialize Debug port GPIO.							**/
/** Resources:		RCC_APB2Periph_AFIO.								    **/
/** Params:																    **/
/**	@DebugPortType:		Debug port type.								    **/
/** Return:			None												    **/
/** Notice:		None												        **/
/*****************************************************************************/
void DebugPort_Initialize(DEBUG_PORT_TYPE eDebugPortType)
{
	switch(eDebugPortType)
	{
		case DEBUG_NONE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
			break;
		}
		case DEBUG_SWD:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
			break;
		}
		default:
		{
			//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE);
			break;
		}
	}
}

/*****************************************************************************/
/** Function Name:	HSEClocks_Initialize.								    **/
/** Purpose:			Configure system clock with HSE.					**/
/** Resources:		None.												    **/
/** Params:																    **/
/**	@uiPLLMul:			multiple of PLL clock source.					    **/
/** Return:																    **/
/**	@SUCCESS			Configuration successed.						    **/
/**  @ERROR				Configuration Failed.							    **/
/** Notice:		None.												        **/
/*****************************************************************************/
ErrorStatus HSEClocks_Initialize(uint32_t uiPLLMul)
{
	/** Parameter check */
	assert_param(IS_RCC_PLL_MUL(RCC_PLLMul));

	/** Initialize data structure */
	RCC_DeInit();
	/** Enabble High speed Extern clock */
	RCC_HSEConfig(RCC_HSE_ON);
	/** Wait for HSE startup */
	if(SUCCESS == RCC_WaitForHSEStartUp())
	{
		/** HSE startup success */

		/** Configuration AHB */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		/** Configuration low speed AHB(APB1) */
		RCC_PCLK1Config(RCC_HCLK_Div2);
		/** Configuration high speed AHB(APB1) */
		RCC_PCLK2Config(RCC_HCLK_Div1);
		/** Setup flash access latency ticks */
		FLASH_SetLatency(FLASH_Latency_2);
		/** Enable prefetch buffer*/
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		/** Configuration PLL source and multiple */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, uiPLLMul);
		/** Enable PLL */
		RCC_PLLCmd(ENABLE);
		/** Wait for PLL startup */
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
			/** PLL startup time out */
		}
		/** Configuration SYSCLK */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		/** Wait for SYSCLK configured PLL */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
			/** SYSCLK config timeout */
		}
		return SUCCESS;
	}
	else
	{
		return ERROR;
	}
}

/*****************************************************************************/
/** Function Name:	NVIC_Initialize.									    **/
/** Purpose:			Configure systick timer.							**/
/** Resources:		Systick timer, NVIC.								    **/
/** Params:																    **/
/**	@uiNVICPriorityGroup:NVIC priority group.							    **/
/** Return:			None.												    **/
/** Notice:		    None.											        **/
/*****************************************************************************/
void NVIC_Initialize(uint32_t uiNVICPriorityGroup)
{
	/** Parameter check */
	assert_param(IS_NVIC_PRIORITY_GROUP(uiNVICPriorityGroup));
	/** Configuration NVIC */
	NVIC_PriorityGroupConfig(uiNVICPriorityGroup);
}

