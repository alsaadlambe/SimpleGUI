/*************************************************************************/
/** Copyright.															**/
/** FileName: VirtualDeviceInterface.c                                  **/
/** Author: Polarix														**/
/** Version: 1.0.0.0													**/
/** Description: Virtual device operation interface.                    **/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "UserActionInterface.h"
#include "VirtualDeviceInterface.h"
#include "DemoProc.h"
#include <stdlib.h>

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
void UAIF_OnInitialize(void)
{
    InitializeEngine();
}

/*************************************************************************/
/** Function Name:	USR_ACT_OnKeyPress									**/
/** Purpose:		Recive and process key press event.					**/
/** Resources:		None.												**/
/** Params:																**/
/** @ bShift:           Shift key is pressed.                           **/
/** @ bCtrl:            Ctrl key is pressed.                            **/
/** @ bAlt:             Alt key is pressed.                             **/
/** @ uiKeyCode:        Key code.										**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void UAIF_OnKeyPress(bool bShift, bool bCtrl, bool bAlt, uint16_t uiKeyCode)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_UINT16				arruiPressedKey[HMI_EVENT_KEY_VALUE_LENGTH_MAX];
	HMI_EVENT				stEvent;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	arruiPressedKey[0] = KEY_OPTION_NONE;
	stEvent.Action = HMI_ENGINE_ACTION_KEY_PRESS;
	if(true == bShift)
	{
		arruiPressedKey[0] |= KEY_OPTION_SHIFT;
	}
	if(true == bCtrl)
	{
		arruiPressedKey[0] |= KEY_OPTION_CTRL;
	}
	if(true == bAlt)
	{
		arruiPressedKey[0] |= KEY_OPTION_ALT;
	}
	arruiPressedKey[1] = uiKeyCode;
	stEvent.Data = (SGUI_BYTE*)arruiPressedKey;
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Call demo process.
	EventProcess(HMI_ENGINE_EVENT_ACTION, &stEvent);
	VDIF_RefreshDisplay();
}

void UAIF_OnTimerEventProcess(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_EVENT				stEvent;
	SGUI_INT				iRandomNumber;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	stEvent.Action =		HMI_ENGINE_ACTION_ON_TIMER;
	stEvent.Data =			NULL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	iRandomNumber = rand();

	iRandomNumber = (iRandomNumber % 200)-100;
	stEvent.Data = (SGUI_BYTE*)(&iRandomNumber);
	// Post timer event.
	EventProcess(HMI_ENGINE_EVENT_ACTION, &stEvent);
	VDIF_RefreshDisplay();
}

void UAIF_OnRTCUpdateEventProcess(uint16_t uiYear, uint16_t uiMonth, uint16_t uiDay, uint16_t uiHour, uint16_t uiMinute, uint16_t uiSecond)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_TIME				stRTCTime;
	HMI_EVENT				stEvent;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	stRTCTime.Year =		uiYear;
	stRTCTime.Month =		uiMonth;
	stRTCTime.Day =			uiDay;
	stRTCTime.Hour =		uiHour;
	stRTCTime.Minute =		uiMinute;
	stRTCTime.Second =		uiSecond;
	stEvent.Action =		HMI_ENGINE_ACTION_ON_TIMER_RTC;
	stEvent.Data = 			(SGUI_BYTE*)(&stRTCTime);

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Post RTC update message to a screen.
	EventProcess(HMI_ENGINE_EVENT_DATA, &stEvent);
	VDIF_RefreshDisplay();
}
