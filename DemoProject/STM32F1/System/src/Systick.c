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
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//����Systickʱ��ԴΪHCLK
    Systick_SetReload(ui_Ticks);							//����Systick��װ��ֵ
    Systick_SetValue(0x000000);							//����Systick��ֵ
    if(b_EnableInterrupt == true)
    {
        NVIC_SetPriority(SysTick_IRQn, 0);			//����Systick�ж����ȼ�
        SystickInt_Enable();							//ʹ��Systick�ж�
    }
    Systick_Enable();									//����Systick
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
