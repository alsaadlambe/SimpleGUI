#ifndef __INCLUDE_RTC_H__
#define __INCLUDE_RTC_H__
#include "stm32f10x.h"
#include "stm32f10x_rtc.h"

#include <time.h>

typedef struct tm	RTC_CALENDAR_STRUCT;

typedef enum
{
	RTC_HOLD		= 0,	// Time is not changed.
	RTC_REFRESHED,
}RTC_STATE;

extern RTC_STATE    g_eRTCRefreshedFlag;
extern RTC_CALENDAR_STRUCT	g_stCleandar;

void        RTC_Initialize(void);
void        RTC_UpdateCalendar(RTC_CALENDAR_STRUCT *pstCalendar);
uint32_t    RTC_GetTimeStamp(void);

#endif //__INCLUDE_RTC_H__


