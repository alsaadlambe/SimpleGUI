#include "Timer.h"
#include "DemoActions.h"

static bool            s_bTimerTriggered = false;

void TIMBase_Int_Init(TIM_TypeDef* pstTimerBase, uint16_t uiReloadValue, uint16_t uiPrescaler)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	/* Configure timer data. */
	TIM_TimeBaseStructure.TIM_Period = uiReloadValue; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = uiPrescaler; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(pstTimerBase, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(pstTimerBase,TIM_IT_Update, ENABLE); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(pstTimerBase, ENABLE);
}

/* Timer-3 interrupt service interface. */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );
		s_bTimerTriggered = true;
    }
}

bool GetTimerTriggered(void)
{
    return s_bTimerTriggered;
}
void ResetTimerTriggered(void)
{
    s_bTimerTriggered = false;
}
