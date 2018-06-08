#ifndef __INCLUDE_TIMER_H__
#define __INCLUDE_TIMER_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "stm32f10x_tim.h"

void    TIMBase_Int_Init(TIM_TypeDef* pstTimerBase, uint16_t uiReloadValue, uint16_t uiPrescaler);
bool    GetTimerTriggered(void);
void    ResetTimerTriggered(void);

#endif /* __INCLUDE_TIMER_H__ */
