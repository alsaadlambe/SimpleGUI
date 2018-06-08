#ifndef __INCLUDE_DEMO_PROC_H__
#define __INCLUDE_DEMO_PROC_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Typedef.h"
#include "HMI_Engine.h"
#if (_SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_ > 0)
#include "DemoResource_UTF8.h"
#else
#include "DemoResource_GB2312.h"
#endif
#include <stdio.h>

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef enum
{
    HMI_INT_UNKNOW,
    HMI_INT_EXINT,
    HMI_INT_KEY,
    HMI_INT_DATA,
    HMI_INT_TIMER,
    HMI_INT_RTC,
}HMI_INTERRUPT_SOURCE;

typedef struct
{
    HMI_INTERRUPT_SOURCE    Source;
    void*                   Data;
}HMI_INTERRUPT_PARAMETER;

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define		HMI_SCREEN_ID_DEMO_SCROLLING_TEXT			(1001)
#define		HMI_SCREEN_ID_DEMO_LIST						(1002)
#define		HMI_SCREEN_ID_DEMO_TEXT_NOTICE				(1003)
#define		HMI_SCREEN_ID_DEMO_RTC_NOTICE				(1004)
#define		HMI_SCREEN_ID_DEMO_VARIABLE_BOX				(1005)
#define		HMI_SCREEN_ID_DEMO_REAL_TIME_GRAPH			(1006)

//=======================================================================//
//= Public variable declaration.									    =//
//=======================================================================//
extern HMI_ENGINE_OBJECT        g_stDemoEngine;
extern HMI_SCREEN_OBJECT        g_stHMIDemo_ScrollingText;
extern HMI_SCREEN_OBJECT		g_stHMIDemo_List;
extern HMI_SCREEN_OBJECT		g_stHMIDemo_TextNotice;
extern HMI_SCREEN_OBJECT		g_stHMIDemo_RTCNotice;
extern HMI_SCREEN_OBJECT        g_stHMIDemo_VariableBox;
extern HMI_SCREEN_OBJECT		g_stHMI_DemoRealtimeGraph;
//=======================================================================//
//= Function declare.   											    =//
//=======================================================================//
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
void                InitializeEngine(void);
HMI_ENGINE_RESULT	EventProcess(HMI_EVENT_TYPE eEventType, const HMI_EVENT* pstEvent);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // __INCLUDE_DEMO_PROC_H__
