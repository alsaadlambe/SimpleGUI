/*************************************************************************/
/** Copyright.															**/
/** FileName: HMI_Process.c												**/
/** Author: Polarix														**/
/** Version: 1.0.0.0													**/
/** Description: This file provides ActionMap interface function.		**/
/*************************************************************************/

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "HMI_Engine.h"
#include "SGUI_Common.h"

//=======================================================================//
//= Static variable declaration.									    =//
//=======================================================================//
static HMI_ENGINE_OBJECT*           g_pstActivedEngineObject;

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//
static __INLINE HMI_SCREEN_OBJECT* HMI_GetScreenObjectInEngine(HMI_ENGINE_OBJECT* pstHMIEngineObject, SGUI_INT iScreenID);

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*****************************************************************************/
/** Function Name:	HMI_PrepareEngine.                                      **/
/** Purpose:		Initialize HMI engine object.			            	**/
/** Params:             													**/
/** @pstHMIEngineObject[in]: Engine object pointer. 						**/
/**	@pstScreenObject[in]: Screen object pointer.							**/
/** Return:			HMI_ENGINE_RESULT										**/
/** @HMI_RET_NORMAL:    Add success.	    								**/
/** Notice:			HMI engine object must prepared by this function before **/
/**                 used.                                                   **/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_PrepareEngine(HMI_ENGINE_OBJECT* pstHMIEngineObject)
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
        // Clean HMI engine object memory area.
        SGUI_Common_MemorySet(pstHMIEngineObject, 0x00, sizeof(HMI_ENGINE_OBJECT));

        // Initialize engine object data,
        pstHMIEngineObject->ScreenCount = 0;
        pstHMIEngineObject->State = HMI_ENGINE_STATE_READY;
    }
    return eProcessResult;
}

/*****************************************************************************/
/** Function Name:	HMI_AddScreen                                           **/
/** Purpose:		Add a screen object to a existed HMI engine object.     **/
/** Params:																	**/
/** @pstHMIEngineObject[in]: Engine object pointer. 						**/
/**	@pstScreenObject[in]: Screen object pointer.							**/
/** @bInitializeScreenObject[in]: Initialize or reinitialize screen object.	**/
/** Return:			HMI_ENGINE_RESULT										**/
/** @HMI_RET_NORMAL:    Add success.	    								**/
/**	@HMI_RET_NO_SPACE:  No blank element in engine object for new screen.   **/
/** Notice:			The engine object only record pointer of the screen     **/
/**                 object, so please keep the memory area is usable during **/
/**                 processing.												**/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_AddScreen(HMI_ENGINE_OBJECT* pstHMIEngineObject, HMI_SCREEN_OBJECT* pstScreenObject, SGUI_BOOL bInitializeScreenObject)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_ENGINE_RESULT           eProcessResult;
	SGUI_INT                    iIndex;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eProcessResult =			HMI_RET_NORMAL;
	iIndex =                    0;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstHMIEngineObject) && (NULL != pstScreenObject))
	{
        // Looking for the blank element in array.
        while(iIndex < pstHMIEngineObject->ScreenCount)
        {
            if(NULL == pstHMIEngineObject->Screen)
            {
                break;
            }
            else
            {
                iIndex++;
            }
        }
        if(iIndex < HMI_SCREEN_LIST_MAX)
        {
        	if(SGUI_TRUE == bInitializeScreenObject)
			{
				pstScreenObject->Actions->Initialize();
			}
            // Add screen object pointer to engine.
            pstHMIEngineObject->Screen[iIndex] = pstScreenObject;
            pstHMIEngineObject->ScreenCount++;
        }
        else
        {
            // No blank element in array, screen object add failed.
            eProcessResult = HMI_RET_NO_SPACE;
        }
	}
	return eProcessResult;
}

/*****************************************************************************/
/** Function Name:	HMI_ActiveEngine			    						**/
/** Purpose:		Set a HMI engine object to the activated state.         **/
/** Params:																	**/
/** @pstHMIEngineObject[in]: The pointer of the engine object will active.  **/
/** @iScreenID[in]:     Specif a screen ID, this screen will be show first  **/
/**                     when engine start, for HMI_SCREEN_ID_ANY to keep    **/
/**                     last screen when this engine activated, and if the  **/
/**                     screen not add into the engine object, the error    **/
/**                     HMI_RET_INVALID_DATA will be returned.              **/
/** Return:			HMI_ENGINE_RESULT										**/
/** @HMI_RET_NORMAL:    Engine activated.	    							**/
/** @HMI_RET_ERROR_STATE: Current engine object is in processing.           **/
/** @HMI_RET_INVALID_DATA: Specified screen ID is invalid or not add to     **/
/**                     this engine.                                        **/
/** Notice:			The process event will post to the activated engine.	**/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_ActiveEngine(HMI_ENGINE_OBJECT* pstHMIEngineObject, SGUI_INT iScreenID)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_ENGINE_RESULT           eProcessResult;
	HMI_SCREEN_OBJECT*          pstScreenObject;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eProcessResult =			HMI_RET_NORMAL;
	pstScreenObject =           NULL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != g_pstActivedEngineObject) && (HMI_ENGINE_STATE_INPROCESS == g_pstActivedEngineObject->State))
	{
		// Current engine is already in processing, cannot active others.
		eProcessResult = HMI_RET_ERROR_STATE;
	}
	else
	{
		if(NULL != pstHMIEngineObject)
		{
			if(HMI_SCREEN_ID_ANY != iScreenID)
			{
				pstScreenObject = HMI_GetScreenObjectInEngine(pstHMIEngineObject, iScreenID);
			}
			else
			{
				pstScreenObject = pstHMIEngineObject->Screen[0];
			}
			if(NULL != pstScreenObject)
            {
                pstHMIEngineObject->CurrentScreenObject = pstScreenObject;
            }
            else
			{
				eProcessResult = HMI_RET_INVALID_DATA;
			}
		}
	}

    if(HMI_RET_NORMAL == eProcessResult)
	{
		g_pstActivedEngineObject = pstHMIEngineObject;
	}
    return eProcessResult;
}

/*****************************************************************************/
/** Function Name:	HMI_StartEngine											**/
/** Purpose:		Start HMI engine and show screen display.               **/
/** Params:			None.                       							**/
/** Return:			HMI_ENGINE_RESULT										**/
/** @HMI_RET_NORMAL:    Engine activated.	    							**/
/** @HMI_RET_ERROR_STATE: Current engine object is in processing.           **/
/** @HMI_RET_INVALID_DATA: Specified screen ID is invalid or not add to     **/
/**                     this engine.                                        **/
/** Notice:			The process event will post to the activated engine.	**/
/** Notice:			Screen will only refresh when pstPreProcessData is NULL	**/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_StartEngine(const void* pstParameters)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_ENGINE_RESULT           eProcessResult;
	HMI_SCREEN_OBJECT*          pstStartScreen;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eProcessResult =			HMI_RET_NORMAL;
	pstStartScreen =            NULL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    if(NULL != g_pstActivedEngineObject)
    {
        if(HMI_ENGINE_STATE_READY != g_pstActivedEngineObject->State)
        {
            // Engine not initialized.
            eProcessResult = HMI_RET_ERROR_STATE;
        }
        else
        {
            // Prepare and display screen.
            g_pstActivedEngineObject->State = HMI_ENGINE_STATE_INPROCESS;
            pstStartScreen = g_pstActivedEngineObject->CurrentScreenObject;
            if(NULL != pstStartScreen)
            {
                if(NULL != pstStartScreen->Actions)
                {
                    if(NULL != pstStartScreen->Actions->Prepare)
                    {
                        pstStartScreen->Actions->Prepare(pstParameters);
                    }
                }
            }
            g_pstActivedEngineObject->State = HMI_ENGINE_STATE_READY;
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
/** Function Name:	HMI_ProcessEvent                                        **/
/** Purpose:		Receive posted event and post to screen object.         **/
/** Params:																	**/
/**	@eEventType[in]:	Event type, see also declare of HMI_EVENT_TYPE.		**/
/**	@pstEvent[in]:		Event data, include action type and parameter data.	**/
/** Return:			HMI_ENGINE_RESULT.										**/
/** Notice:			None.													**/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_ProcessEvent(HMI_EVENT_TYPE eEventType, const HMI_EVENT* pstEvent)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_ENGINE_RESULT           eProcessResult;
	HMI_SCREEN_OBJECT*          pstCurrentScreen;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eProcessResult =			HMI_RET_NORMAL;
	pstCurrentScreen =          NULL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    if(NULL != g_pstActivedEngineObject)
    {
        if(HMI_ENGINE_STATE_READY != g_pstActivedEngineObject->State)
        {
            // Engine not initialized.
            eProcessResult = HMI_RET_ERROR_STATE;
        }
        else
        {
            g_pstActivedEngineObject->State = HMI_ENGINE_STATE_INPROCESS;
            pstCurrentScreen = g_pstActivedEngineObject->CurrentScreenObject;
            if(NULL == pstCurrentScreen)
            {
                // Screen ID is invalid or screen object is not in this engine.
                eProcessResult = HMI_RET_INVALID_DATA;
            }
            else
            {
                if(NULL != pstCurrentScreen->Actions)
                {
                    if(NULL != pstCurrentScreen->Actions->ProcessEvent)
                    {
                         eProcessResult = pstCurrentScreen->Actions->ProcessEvent(eEventType, pstEvent);
                    }
                }
            }
            g_pstActivedEngineObject->State = HMI_ENGINE_STATE_READY;
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
/** Function Name:	HMI_PostProcess											**/
/** Purpose:		Process after event process.							**/
/** Params:																	**/
/**	@iActionResult[in]: Event post result.									**/
/** Return:			HMI_ENGINE_RESULT.										**/
/** @HMI_RET_NORMAL:    Post process finished normally.						**/
/** @HMI_RET_INVALID_DATA: Screen object is error, post process function	**/
/**						might be unspecified.								**/
/** Notice:			None.													**/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_PostProcess(SGUI_INT iActionResult)
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
	if(NULL != g_pstActivedEngineObject)
    {
    	if(	(NULL != g_pstActivedEngineObject) &&
			(NULL != g_pstActivedEngineObject->CurrentScreenObject) &&
			(NULL != g_pstActivedEngineObject->CurrentScreenObject->Actions) &&
			(NULL != g_pstActivedEngineObject->CurrentScreenObject->Actions->PostProcess))
		{
			eProcessResult = g_pstActivedEngineObject->CurrentScreenObject->Actions->PostProcess(iActionResult);
		}
		else
		{
			eProcessResult = HMI_RET_INVALID_DATA;
		}
    }
    return eProcessResult;
}

/*****************************************************************************/
/** Function Name:	HMI_Goto                                                **/
/** Purpose:		Turn to a screen with screen index.						**/
/** Params:																	**/
/**	@uiDestScreenID[in]: Screen ID witch will be going to.					**/
/**	@pstParameters[in]:	Screen prepare data pointer.                        **/
/** Return:			None.													**/
/** Notice:			Screen will only refresh when pstPreProcessData is NULL	**/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_Goto(SGUI_INT iDestScreenID, const void* pstParameters)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_ENGINE_RESULT           eProcessResult;
	HMI_SCREEN_OBJECT*          pstCurrentScreen;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eProcessResult =			HMI_RET_NORMAL;
	pstCurrentScreen =          NULL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != g_pstActivedEngineObject)
    {
        if(HMI_ENGINE_STATE_READY != g_pstActivedEngineObject->State)
        {
            // Engine not initialized or in processing.
            eProcessResult = HMI_RET_ERROR_STATE;
        }
        else
        {
            g_pstActivedEngineObject->State = HMI_ENGINE_STATE_INPROCESS;
            pstCurrentScreen = g_pstActivedEngineObject->CurrentScreenObject;
            if(NULL != pstCurrentScreen)
            {
                // Push current screen to history stack.
                g_pstActivedEngineObject->History.Item[g_pstActivedEngineObject->History.TopIndex] = pstCurrentScreen;
                g_pstActivedEngineObject->History.TopIndex++;

                // Set next screen information in engine.
                pstCurrentScreen = HMI_GetScreenObjectInEngine(g_pstActivedEngineObject, iDestScreenID);
                if(NULL != pstCurrentScreen)
                {
                    g_pstActivedEngineObject->CurrentScreenObject = pstCurrentScreen;
                    if(NULL != pstCurrentScreen->Actions)
                    {
                        if(NULL != pstCurrentScreen->Actions->Prepare)
                        {
                            eProcessResult = pstCurrentScreen->Actions->Prepare(pstParameters);
                        }
                    }
                }
                else
                {
                    eProcessResult = HMI_RET_INVALID_DATA;
                }
            }
            g_pstActivedEngineObject->State = HMI_ENGINE_STATE_READY;
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
/** Function Name:	HMI_GoBack.                                             **/
/** Purpose:		Go back to previous screen in actions history.			**/
/** Resources:		Screen data structure and action process function.		**/
/** Params:																	**/
/**	@pstPreProcessData[in]:	Update screen data.									**/
/** Return:			None.													**/
/** Notice:			Screen will only refresh when pstPreProcessData is NULL	**/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_GoBack(const void* pstParameters)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_ENGINE_RESULT           eProcessResult;
	HMI_SCREEN_OBJECT*          pstCurrentScreen;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eProcessResult =			HMI_RET_NORMAL;
	pstCurrentScreen =          NULL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != g_pstActivedEngineObject)
    {
        if(HMI_ENGINE_STATE_READY != g_pstActivedEngineObject->State)
        {
            // Engine not initialized or in processing.
            eProcessResult = HMI_RET_ERROR_STATE;
        }
        else
        {
            g_pstActivedEngineObject->History.TopIndex--;
            pstCurrentScreen = g_pstActivedEngineObject->History.Item[g_pstActivedEngineObject->History.TopIndex];
            g_pstActivedEngineObject->History.Item[g_pstActivedEngineObject->History.TopIndex] = NULL;

            if(NULL != pstCurrentScreen)
            {
                g_pstActivedEngineObject->CurrentScreenObject = pstCurrentScreen;
                if(NULL != pstCurrentScreen->Actions)
                {
                    if(NULL != pstCurrentScreen->Actions->Prepare)
                    {
                        eProcessResult = pstCurrentScreen->Actions->Prepare(pstParameters);
                    }
                }
            }
            else
            {
                eProcessResult = HMI_RET_INVALID_DATA;
            }
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
/** Function Name:	HMI_GetScreenObjectInEngine.                            **/
/** Purpose:		Get a screen object pointer form a existed HMI engine   **/
/**                 object, return NULL if screen not existed.              **/
/** Params:																	**/
/** @pstHMIEngineObject[in]: The pointer of the engine object.              **/
/** @iScreenID[in]:     Screen ID of screen object will be searched in HMI  **/
/**                     engine object.                                      **/
/** Return:			HMI_SCREEN_OBJECT pointer.                              **/
/** @NULL:              Screen object not existed or not add in this HMI    **/
/**                     engine.         	    							**/
/** Notice:			None.                                                   **/
/*****************************************************************************/
HMI_SCREEN_OBJECT* HMI_GetScreenObjectInEngine(HMI_ENGINE_OBJECT* pstHMIEngineObject, SGUI_INT iScreenID)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_SCREEN_OBJECT*          pstScreenObject;
	SGUI_INT                    iIndex;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pstScreenObject =           NULL;
	iIndex =                    0;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    if(NULL != pstHMIEngineObject)
    {
        while(iIndex < pstHMIEngineObject->ScreenCount)
        {
            if(iScreenID == pstHMIEngineObject->Screen[iIndex]->ScreenID)
            {
                pstScreenObject = pstHMIEngineObject->Screen[iIndex];
                break;
            }
            else
            {
                iIndex++;
            }
        }
    }

	return pstScreenObject;
}
