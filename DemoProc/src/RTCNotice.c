/*************************************************************************/
/** Copyright.															**/
/** FileName: RTCNotice.c												**/
/** Author: Polarix														**/
/** Version: 1.0.0.0													**/
/** Description: HMI demo for notice box interface and refresh screen.	**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "DemoProc.h"
#include "HMI_Engine.h"
#include "SGUI_Notice.h"
#include "SGUI_Common.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define					NOTICE_RTC_BUFFER_SIZE				(64)

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//
static HMI_ENGINE_RESULT	HMI_DemoRTCNotice_Initialize(void);
static HMI_ENGINE_RESULT	HMI_DemoRTCNotice_Prepare(const void* pstParameters);
static HMI_ENGINE_RESULT	HMI_DemoRTCNotice_RefreshScreen(const void* pstParameters);
static HMI_ENGINE_RESULT	HMI_DemoRTCNotice_ProcessEvent(HMI_EVENT_TYPE eEventType, const HMI_EVENT* pstEvent);
static HMI_ENGINE_RESULT	HMI_DemoRTCNotice_PostProcess(SGUI_INT iActionResult);

//=======================================================================//
//= Static variable declaration.									    =//
//=======================================================================//
static char				s_szRTCNoticeText[NOTICE_RTC_BUFFER_SIZE+1] = {0x00};
HMI_SCREEN_ACTION		s_stDemoRTCNoticeActions =		{	HMI_DemoRTCNotice_Initialize,
															HMI_DemoRTCNotice_Prepare,
															HMI_DemoRTCNotice_RefreshScreen,
															HMI_DemoRTCNotice_ProcessEvent,
															HMI_DemoRTCNotice_PostProcess
														};
//=======================================================================//
//= Global variable declaration.									    =//
//=======================================================================//
HMI_SCREEN_OBJECT       g_stHMIDemo_RTCNotice =			{	HMI_SCREEN_ID_DEMO_RTC_NOTICE,
															&s_stDemoRTCNoticeActions
														};

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
HMI_ENGINE_RESULT HMI_DemoRTCNotice_Initialize(void)
{
	SGUI_Common_MemorySet(s_szRTCNoticeText, 0x00, sizeof(SGUI_CHAR)*(NOTICE_RTC_BUFFER_SIZE+1));
	return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoRTCNotice_Prepare(const void* pstParameters)
{
	HMI_DemoRTCNotice_RefreshScreen(NULL);
	return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoRTCNotice_RefreshScreen(const void* pstParameters)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_TIME				stRTCTime;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	SGUI_Common_GetNowTime(&stRTCTime);
	sprintf(s_szRTCNoticeText, DEMO_RTC_NOTICE_TEXT_FMT,
				stRTCTime.Year, stRTCTime.Month, stRTCTime.Day,
				stRTCTime.Hour, stRTCTime.Minute, stRTCTime.Second);
	SGUI_Notice_RefreshNotice(s_szRTCNoticeText, 0, SGUI_ICON_INFORMATION);
	return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoRTCNotice_ProcessEvent(HMI_EVENT_TYPE eEventType, const HMI_EVENT* pstEvent)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_ENGINE_RESULT           eProcessResult;
	SGUI_UINT16*				parrKeyValue;
	SGUI_TIME*					pstRTCTime;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eProcessResult =			HMI_RET_NORMAL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstEvent)
	{
		if(eEventType == HMI_ENGINE_EVENT_ACTION)
		{
			parrKeyValue = (SGUI_UINT16*)pstEvent->Data;
			if(NULL != parrKeyValue)
			{
				switch(*(parrKeyValue+1))
				{
					case KEY_VALUE_ENTER:
					case KEY_VALUE_ESC:
					{
						eProcessResult = HMI_RET_CANCEL;
						break;
					}
				}
			}
		}
		else if(eEventType == HMI_ENGINE_EVENT_DATA)
		{
			if(HMI_ENGINE_ACTION_ON_TIMER_RTC == pstEvent->Action)
			{
				pstRTCTime = (SGUI_TIME*)pstEvent->Data;
				if(NULL != pstRTCTime)
				{
					sprintf(s_szRTCNoticeText, DEMO_RTC_NOTICE_TEXT_FMT,
					pstRTCTime->Year, pstRTCTime->Month, pstRTCTime->Day,
					pstRTCTime->Hour, pstRTCTime->Minute, pstRTCTime->Second);
					SGUI_Notice_RefreshNotice(s_szRTCNoticeText, 0, SGUI_ICON_INFORMATION);
					eProcessResult = HMI_RET_NOACTION;
				}
			}
		}
	}
	else
	{
		eProcessResult = HMI_RET_NOACTION;
	}

	return eProcessResult;
}

HMI_ENGINE_RESULT HMI_DemoRTCNotice_PostProcess(SGUI_INT iActionResult)
{
	if(HMI_RET_CANCEL == iActionResult)
	{
		HMI_GoBack(NULL);
	}
	return HMI_RET_NORMAL;
}

