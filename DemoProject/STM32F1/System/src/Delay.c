#include "stm32f10x.h"
#include "Delay.h"

// Parameter check macro
#define IS_DELAY_US(TIME) (TIME < 4294967295)

// Delay timer counter.
static uint32_t guiDelay_us = 0x00000000;

/************************************************************************/
/* Function Name:	DelayUs.											*/
/* Purpose:			Delay times(us).									*/
/* Resources:		Systick timer, NVIC.								*/
/* Params:																*/
/*	@ui_Times:			Delay times.									*/
/* Return:																*/
/*	@SUCCESS			Configuration successed.						*/
/* Limitation:		Parameter is not greater then 4294967295.			*/
/************************************************************************/
void DelayUs(uint32_t ui_Times)
{
	// Check the parameters
	assert_param(INVAILD_DELAY_US(ui_Times));
	guiDelay_us = ui_Times;
	while(guiDelay_us);
}

/************************************************************************/
/* Function Name:	DelayMs.											*/
/* Purpose:			Delay times(ms).									*/
/* Resources:		Systick timer, NVIC.								*/
/* Params:																*/
/*	@ui_Times:			Delay times.									*/
/* Return:																*/
/*	@SUCCESS			Configuration successed.						*/
/* Limitation:		Parameter is not greater then 4294967.				*/
/************************************************************************/
void DelayMs(uint32_t ui_Times)
{
	// Check the parameters
	assert_param(INVAILD_DELAY_MS(ui_Times));
	guiDelay_us = ui_Times * 1000;
	while(guiDelay_us);
}

/************************************************************************/
/* Function Name:	SystickProc.										*/
/* Purpose:			Delay timer process.								*/
/* Resources:		None.												*/
/* Params:			None.												*/
/* Return:			None.												*/
/* Limitation:		Neet called by systick timer interrupt.				*/
/************************************************************************/
void SystickProc(void)
{
	if(guiDelay_us > 0)
	{
		guiDelay_us--;
	}
}
