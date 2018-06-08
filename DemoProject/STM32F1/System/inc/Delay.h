#ifndef __INCLUDE_DELAY_H__
#define __INCLUDE_DELAY_H__

#include "stm32f10x.h"

#define INVAILD_DELAY_MS(TIME) (TIME < 4294967)
#define INVAILD_DELAY_US(TIME) (TIME < 4294967295)

void DelayUs(uint32_t ui_Times);
void DelayMs(uint32_t ui_Times);
void SystickProc(void);

#endif //__INCLUDE_DELAY_H__
