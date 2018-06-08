/*************************************************************************/
/** Copyright.															**/
/** FileName: DemoProc.c												**/
/** Author: Polarix														**/
/** Version: 1.0.0.0													**/
/** Description: User operation interface.								**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "DemoProc.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//

//=======================================================================//
//= Static function declare											    =//
//=======================================================================//
static HMI_ENGINE_RESULT   InitializeEngine_Internal(HMI_ENGINE_OBJECT* pstHMIEngineObject);

//=======================================================================//
//= Static variable declaration.									    =//
//=======================================================================//
HMI_ENGINE_OBJECT       g_stDemoEngine;
//=======================================================================//
//= Function define.										            =//
//=======================================================================//

/*****************************************************************************/
/** Function Name:	SimpleGUI_DemoProcess   								**/
/** Purpose:		Simple GUI HMI engine and interface demo process.       **/
/** Parameters:		None.													**/
/** Return:			None.													**/
/** Notice:			This function demonstrates how to use the interface and **/
/**                 HMI engine of Simple GUI.                               **/
/*****************************************************************************/
void InitializeEngine(void)
{
	InitializeEngine_Internal(&g_stDemoEngine);
}

/*****************************************************************************/
/** Function Name:	InitializeEngine										**/
/** Purpose:		Initialize HMI demo engine.								**/
/** Parameters:																**/
/** @ pstHMIEngineObject[in]: HMI engine object pointer.					**/
/** Return:			HMI_ENGINE_RESULT.										**/
/** Notice:			This function must be called when initialize.			**/
/*****************************************************************************/
HMI_ENGINE_RESULT InitializeEngine_Internal(HMI_ENGINE_OBJECT* pstHMIEngineObject)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_ENGINE_RESULT           eProcessResult;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eProcessResult =			HMI_RET_NORMAL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    if(NULL != pstHMIEngineObject)
    {
        if(HMI_PROCESS_SUCCESSFUL(eProcessResult))
        {
            eProcessResult = HMI_PrepareEngine(pstHMIEngineObject);
        }

        if(HMI_PROCESS_SUCCESSFUL(eProcessResult))
        {
            eProcessResult = HMI_AddScreen(pstHMIEngineObject, &g_stHMIDemo_ScrollingText, SGUI_TRUE);
        }
        if(HMI_PROCESS_SUCCESSFUL(eProcessResult))
        {
            eProcessResult = HMI_AddScreen(pstHMIEngineObject, &g_stHMIDemo_List, SGUI_TRUE);
        }
        if(HMI_PROCESS_SUCCESSFUL(eProcessResult))
        {
            eProcessResult = HMI_AddScreen(pstHMIEngineObject, &g_stHMIDemo_TextNotice, SGUI_TRUE);
        }
        if(HMI_PROCESS_SUCCESSFUL(eProcessResult))
        {
            eProcessResult = HMI_AddScreen(pstHMIEngineObject, &g_stHMIDemo_RTCNotice, SGUI_TRUE);
        }
        if(HMI_PROCESS_SUCCESSFUL(eProcessResult))
        {
            eProcessResult = HMI_AddScreen(pstHMIEngineObject, &g_stHMIDemo_VariableBox, SGUI_TRUE);
        }
        if(HMI_PROCESS_SUCCESSFUL(eProcessResult))
        {
            eProcessResult = HMI_AddScreen(pstHMIEngineObject, &g_stHMI_DemoRealtimeGraph, SGUI_TRUE);
        }

        if(HMI_PROCESS_SUCCESSFUL(eProcessResult))
        {
            eProcessResult = HMI_ActiveEngine(pstHMIEngineObject, HMI_SCREEN_ID_DEMO_SCROLLING_TEXT);
        }

        if(HMI_PROCESS_SUCCESSFUL(eProcessResult))
        {
            eProcessResult = HMI_StartEngine(NULL);
        }
    }
    else
    {
        // No activated HMI engine object.
        eProcessResult = HMI_RET_INVALID_DATA;
    }

	return eProcessResult;
}

/*****************************************************************************/
/** Function Name:	EventProcess											**/
/** Purpose:		Process posted event.									**/
/** Parameters:																**/
/** @ eEventType[in]:   HMI event type.         						    **/
/** @ pstEvent[in]:     HMI event data.                                     **/
/** Return:			HMI_ENGINE_RESULT.										**/
/** Notice:			This function must be called when initialize.			**/
/*****************************************************************************/
HMI_ENGINE_RESULT EventProcess(HMI_EVENT_TYPE eEventType, const HMI_EVENT* pstEvent)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_ENGINE_RESULT           eProcessResult;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eProcessResult =			HMI_RET_NORMAL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/

	eProcessResult = HMI_ProcessEvent(eEventType, pstEvent);
	if(HMI_PROCESS_SUCCESSFUL(eProcessResult))
	{
		HMI_PostProcess(eProcessResult);
	}

	return eProcessResult;
}
