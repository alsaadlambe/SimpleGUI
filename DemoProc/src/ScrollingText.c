/*************************************************************************/
/** Copyright.															**/
/** FileName: ScrollingText.c											**/
/** Author: Polarix														**/
/** Version: 1.0.0.0													**/
/** Description: HMI demo for text draw interface.						**/
/*************************************************************************/

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "DemoProc.h"
#include "SGUI_Text.h"
#include "SGUI_Frame.h"
#include "HMI_Engine.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define		HMI_TEXT_DEMO_FRAME_EDGE_LAYERS				(2)
		#if(HMI_TEXT_DEMO_FRAME_EDGE_LAYERS == 0)
#define 	HMI_FRAMR_EDGE_WIDTH						(0)
		#else
#define		HMI_TEXT_DEMO_FRAMR_EDGE_WIDTH				(HMI_TEXT_DEMO_FRAME_EDGE_LAYERS*2-1)
		#endif
#define		HMI_TEXT_DEMO_FRAME_TEXT_POSX				(HMI_TEXT_DEMO_FRAMR_EDGE_WIDTH+1)
#define		HMI_TEXT_DEMO_FRAME_TEXT_POSY				(HMI_TEXT_DEMO_FRAMR_EDGE_WIDTH+1)
#define		HMI_TEXT_DEMO_FRAME_TEXT_WIDTH				(LCD_SIZE_WIDTH-(HMI_TEXT_DEMO_FRAMR_EDGE_WIDTH+1)*2)
#define		HMI_TEXT_DEMO_FRAME_TEXT_HEIGHT				(LCD_SIZE_HEIGHT-(HMI_TEXT_DEMO_FRAMR_EDGE_WIDTH+1)*2)

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//
static HMI_ENGINE_RESULT    HMI_DemoScrollingText_Initialize(void);
static HMI_ENGINE_RESULT    HMI_DemoScrollingText_Prepare(const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_DemoScrollingText_RefreshScreen(const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_DemoScrollingText_ProcessEvent(HMI_EVENT_TYPE eEvent, const HMI_EVENT* pstEvent);
static HMI_ENGINE_RESULT    HMI_DemoScrollingText_PostProcess(SGUI_INT iActionResult);

//=======================================================================//
//= Static variable declaration.									    =//
//=======================================================================//
// Demo text.
static SGUI_CCHAR           s_szDemoText[] =            {DEMO_START_NOTICE};

static int32_t				s_iTextOffset;
static SGUI_INT				s_iTextHeight;
static SGUI_RECT_AREA		s_stTextDisplayArea;

static SGUI_BOX_FRAME_STRUCT s_stTextFrame =			{	{HMI_TEXT_DEMO_FRAME_EDGE_LAYERS, SGUI_FONT_SIZE_H12},
															{NULL}
														};
static HMI_SCREEN_ACTION	s_stDemoScrollingTextActions =	{
															HMI_DemoScrollingText_Initialize,
															HMI_DemoScrollingText_Prepare,
															HMI_DemoScrollingText_RefreshScreen,
															HMI_DemoScrollingText_ProcessEvent,
															HMI_DemoScrollingText_PostProcess
														};

//=======================================================================//
//= Global variable declaration.									    =//
//=======================================================================//
HMI_SCREEN_OBJECT       g_stHMIDemo_ScrollingText =		{
														HMI_SCREEN_ID_DEMO_SCROLLING_TEXT,
														&s_stDemoScrollingTextActions};

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*****************************************************************************/
/** Function Name:	HMI_DemoScrollingText_Initialize                        **/
/** Purpose:		Initialize screen data.									**/
/** Resources:		Current screen display or control data.					**/
/** Parameters:		None.													**/
/** Return:			Initialize process result.								**/
/** Limitation:		None.													**/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_DemoScrollingText_Initialize(void)
{
	s_iTextOffset = HMI_TEXT_DEMO_FRAME_TEXT_HEIGHT;
	s_iTextHeight = SGUI_Text_GetMultiLineTextLines(s_szDemoText, (HMI_TEXT_DEMO_FRAME_TEXT_WIDTH/g_stFontSize[SGUI_FONT_SIZE_H12].Width))*g_stFontSize[SGUI_FONT_SIZE_H12].Height;
	s_stTextDisplayArea.PosX = HMI_TEXT_DEMO_FRAME_TEXT_POSX;
	s_stTextDisplayArea.PosY = HMI_TEXT_DEMO_FRAME_TEXT_POSY;
	s_stTextDisplayArea.Width = HMI_TEXT_DEMO_FRAME_TEXT_WIDTH;
	s_stTextDisplayArea.Height = HMI_TEXT_DEMO_FRAME_TEXT_HEIGHT;
	return HMI_RET_NORMAL;
}

/*****************************************************************************/
/** Function Name:	HMI_DemoScrollingText_Prepare                           **/
/** Purpose:		Preprocess after initialize.							**/
/** Resources:		None.													**/
/** Parameters:		None.													**/
/** Return:			Preprocess result.										**/
/** Limitation:		None.													**/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_DemoScrollingText_Prepare(const void* pstParameters)
{
	SGUI_Frame_DrawFullScreenFrame(&s_stTextFrame);
	return HMI_RET_NORMAL;
}

/*****************************************************************************/
/** Function Name:	HMI_DemoText_RefreshScreen								**/
/** Purpose:		Refresh current screen.									**/
/** Resources:		None.													**/
/** Parameters:		None.													**/
/** Return:			Refresh process result.									**/
/** Limitation:		None.													**/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_DemoScrollingText_RefreshScreen(const void* pstParameters)
{
	SGUI_Frame_DrawFullScreenFrame(&s_stTextFrame);
	SGUI_Text_DrawMultipleLinesText(s_szDemoText, SGUI_FONT_SIZE_H12, &s_stTextDisplayArea, s_iTextOffset, SGUI_DRAW_NORMAL);
	return HMI_RET_NORMAL;
}

/*****************************************************************************/
/** Function Name:	HMI_DemoText_OnExternalEvent							**/
/** Purpose:		Process with user actions.								**/
/** Resources:		List data structure and bind data if existed.			**/
/** Parameters:																**/
/** @uiScreenID[in]: 	Matched screen ID.									**/
/** @pstEvent[in]:		User event data pointer.							**/
/** Return:			User event process result.								**/
/** Limitation:		Parameter pointer is a void type, convert to the 		**/
/**					appropriate type before use.							**/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_DemoScrollingText_ProcessEvent(HMI_EVENT_TYPE eEventType, const HMI_EVENT* pstEvent)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_ENGINE_RESULT           eProcessResult;
	SGUI_UINT16*				parrKeyValue;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eProcessResult =			HMI_RET_NORMAL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(HMI_ENGINE_EVENT_ACTION == eEventType)
	{
		if(NULL != pstEvent)
		{
			switch(pstEvent->Action)
			{
				case HMI_ENGINE_ACTION_KEY_PRESS:
				{
					parrKeyValue = (SGUI_UINT16*)pstEvent->Data;
                    if(KEY_VALUE_SPACE == *(parrKeyValue+1))
                    {
                        eProcessResult = HMI_RET_FOLLOWUP;
                    }
                    else
                    {
                        eProcessResult = HMI_RET_NOACTION;
                    }
                    break;
				}
				case HMI_ENGINE_ACTION_ON_TIMER:
				{
                    //SGUI_Frame_DrawFullScreenFrame(&s_stTextFrame);
                    SGUI_Text_DrawMultipleLinesText(s_szDemoText, SGUI_FONT_SIZE_H12, &s_stTextDisplayArea, s_iTextOffset, SGUI_DRAW_NORMAL);
                    if(s_iTextOffset + s_iTextHeight == 0)
                    {
                        s_iTextOffset = HMI_TEXT_DEMO_FRAME_TEXT_HEIGHT;
                    }
                    else
                    {
                        s_iTextOffset--;
                    }
                    eProcessResult = HMI_RET_NOACTION;
                    break;
				}
				default:
                {
                    eProcessResult = HMI_RET_NOACTION;
                    break;
                }
			}
		}
	}
	return eProcessResult;
}

/*****************************************************************************/
/** Function Name:	HMI_DemoList_PostProcess								**/
/** Purpose:		Do something after user action or update screen.		**/
/** Resources:		List data structure and bind data if existed.			**/
/** Parameters:																**/
/** @iActionResult[in]: User action or system action process result.		**/
/** Return:			Post process result.									**/
/** Limitation:		None.													**/
/*****************************************************************************/
HMI_ENGINE_RESULT HMI_DemoScrollingText_PostProcess(SGUI_INT iActionResult)
{
	if(HMI_RET_FOLLOWUP == iActionResult)
	{
		// Go to main list.
		HMI_Goto(HMI_SCREEN_ID_DEMO_LIST, NULL);
	}
	return HMI_RET_NORMAL;
}
