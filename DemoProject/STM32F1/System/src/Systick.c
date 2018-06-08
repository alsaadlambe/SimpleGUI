#include "Systick.h"
#include "Delay.h"

/************************************************************************/
/* Function Name:	Systick_Initialize.									*/
/* Purpose:			Configure systick timer.							*/
/* Resources:		Systick timer, NVIC.								*/
/* Params:																*/
/*	@nTicks:			Systick timer reload value.						*/
/*	@bEnableInterrupt:	Enable systick timer interrupt.					*/
/* Return:																*/
/*	@SUCCESS - Configuration successed.									*/
/* Limitation:		None.												*/
/************************************************************************/
ErrorStatus Systick_Initialize(uint32_t ui_Ticks, bool b_EnableInterrupt)
{
    /* Parameter check */
    assert_param(INVAILD_TICKS(nTicks));
    /* Configuration systick timer */
    //SysTick_Config(nTicks);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//设置Systick时钟源为HCLK
    Systick_SetReload(ui_Ticks);							//设置Systick重装载值
    Systick_SetValue(0x000000);							//设置Systick初值
    if(b_EnableInterrupt == true)
    {
        NVIC_SetPriority(SysTick_IRQn, 0);			//配置Systick中断优先级
        SystickInt_Enable();							//使能Systick中断
    }
    Systick_Enable();									//启动Systick
    return SUCCESS;
}

/************************************************************************/
/* Function Name:	SysTick_Handler.									*/
/* Purpose:			Systick interrupt service function.					*/
/* Resources:		Systick timer, NVIC.								*/
/* Params:			None.												*/
/* Return:			None.												*/
/* Limitation:		None.												*/
/************************************************************************/
void SysTick_Handler(void)
{
    /*  */
    SystickProc();
}
