/*************************************************************************/
/** Copyright.															**/
/** FileName: HMI_Demo04_Graph.c										**/
/** Author: Polarix														**/
/** Version: 1.0.0.0													**/
/** Description: HMI demo for graph interface.							**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "DemoProc.h"
#include "SGUI_Frame.h"
#include "SGUI_Notice.h"
#include "SGUI_VariableBox.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define						TEXT_VARIABLE_LENGTH				(20)

#define						VARIABLE_BOX_WIDTH					(100)
#define						VARIABLE_BOX_POSX					(10)
#define						VARIABLE_BOX_NUMBER_POSY			(19)
#define						VARIABLE_BOX_TEXT_POSY				(38)
//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//
static HMI_ENGINE_RESULT    HMI_DemoVariableBox_Initialize(void);
static HMI_ENGINE_RESULT	HMI_DemoVariableBox_Prepare(const void* pstParameters);
static HMI_ENGINE_RESULT	HMI_DemoVariableBox_RefreshScreen(const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_DemoVariableBox_ProcessEvent(HMI_EVENT_TYPE eEventType, const HMI_EVENT* pstEvent);
static HMI_ENGINE_RESULT	HMI_DemoVariableBox_PostProcess(SGUI_INT iActionResult);
static void				    HMI_DemoVariableBox_DrawFrame(SGUI_PSZSTR szTitle);

//=======================================================================//
//= Static variable declaration.									    =//
//=======================================================================//
static SGUI_INT_VARBOX_STRUCT	s_stNumberVariableBox =		{	VARIABLE_BOX_POSX+2,
																VARIABLE_BOX_NUMBER_POSY+2,
																VARIABLE_BOX_WIDTH,
																SGUI_FONT_SIZE_H12,
																-50,
																100,
																0
															};
static SGUI_CHAR				s_szTextVariableBuffer[TEXT_VARIABLE_LENGTH+1] = {"ABCDEFG"};
static SGUI_TEXT_VARBOX_STRUCT	s_stTextVariableBox =		{	VARIABLE_BOX_POSX+2,
																VARIABLE_BOX_TEXT_POSY+2,
																VARIABLE_BOX_WIDTH,
																SGUI_FONT_SIZE_H16,
																0,
																TEXT_VARIABLE_LENGTH,
																s_szTextVariableBuffer,
															};
static SGUI_CHAR				s_szDefaultFrameTitle[] =	DEMO_VARIABLE_BOX_TITLE;
static SGUI_PSZSTR				s_szFrameTitle =			s_szDefaultFrameTitle;
static SGUI_INT					s_uiFocusedFlag;
static SGUI_PCSZSTR				s_szHelpNoticeText =		DEMO_VARIABLE_BOX_HELPER;
static SGUI_INT					s_uiAutoConfirmTimer =		5;
HMI_SCREEN_ACTION				s_stDemoVariableBoxActions = {
																HMI_DemoVariableBox_Initialize,
																HMI_DemoVariableBox_Prepare,
																HMI_DemoVariableBox_RefreshScreen,
																HMI_DemoVariableBox_ProcessEvent,
																HMI_DemoVariableBox_PostProcess,
															};

//=======================================================================//
//= Global variable declaration.									    =//
//=======================================================================//
HMI_SCREEN_OBJECT       		g_stHMIDemo_VariableBox =	{	HMI_SCREEN_ID_DEMO_VARIABLE_BOX,
																&s_stDemoVariableBoxActions
															};

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
HMI_ENGINE_RESULT HMI_DemoVariableBox_Initialize(void)
{
	s_uiFocusedFlag = 0;
	return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoVariableBox_Prepare(const void* pstParameters)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Draw frame
	s_szFrameTitle = s_szDefaultFrameTitle;
	HMI_DemoVariableBox_DrawFrame((SGUI_PSZSTR)s_szFrameTitle);
	// Show notice
	SGUI_Notice_RefreshNotice(s_szHelpNoticeText, 0, SGUI_ICON_INFORMATION);
	return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoVariableBox_RefreshScreen(const void* pstParameters)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Draw frame
    HMI_DemoVariableBox_DrawFrame((SGUI_PSZSTR)s_szFrameTitle);
    // Draw number box
    SGUI_Basic_DrawRectangle(VARIABLE_BOX_POSX, VARIABLE_BOX_NUMBER_POSY, VARIABLE_BOX_WIDTH+4, g_stFontSize[s_stNumberVariableBox.FontSize].Height+6, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
    SGUI_IntegerVariableBox_Refresh(&s_stNumberVariableBox, SGUI_CENTER, (0 == s_uiFocusedFlag)?SGUI_DRAW_REVERSE:SGUI_DRAW_NORMAL);
    // Draw text box
    SGUI_Basic_DrawRectangle(VARIABLE_BOX_POSX, VARIABLE_BOX_TEXT_POSY, VARIABLE_BOX_WIDTH+4,  g_stFontSize[s_stTextVariableBox.FontSize].Height+6, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
    SGUI_TextVariableBox_Refresh(&s_stTextVariableBox, (0 == s_uiFocusedFlag)?SGUI_DRAW_NORMAL:SGUI_DRAW_REVERSE);

	return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoVariableBox_ProcessEvent(HMI_EVENT_TYPE eEventType, const HMI_EVENT* pstEvent)
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
	if(s_uiAutoConfirmTimer > 0)
    {
        if(NULL != pstEvent)
        {
            if(eEventType == HMI_ENGINE_EVENT_ACTION)
            {

                if(HMI_ENGINE_ACTION_KEY_PRESS == pstEvent->Action)
                {
                    parrKeyValue = (SGUI_UINT16*)pstEvent->Data;
                    if(NULL != parrKeyValue)
                    {
                        if(KEY_VALUE_SPACE == *(parrKeyValue+1))
                        {
                            // Stop count down when press space.
                            s_uiAutoConfirmTimer = 0;
                        }
                    }
                }
            }
            else if(eEventType == HMI_ENGINE_EVENT_DATA)
            {
                if(HMI_ENGINE_ACTION_ON_TIMER_RTC == pstEvent->Action)
                {
                    //Count down five seconds
                    s_uiAutoConfirmTimer--;
                }
            }
        }

        // Redraw screen if time out.
        if(0 == s_uiAutoConfirmTimer)
        {
            eProcessResult = HMI_DemoVariableBox_RefreshScreen(NULL);
        }

        eProcessResult = HMI_RET_NOACTION;
    }
    else
    {
        if(HMI_ENGINE_ACTION_KEY_PRESS == pstEvent->Action)
        {
            parrKeyValue = (SGUI_UINT16*)pstEvent->Data;
            if(NULL != parrKeyValue)
            {
                switch(*(parrKeyValue+1))
                {
                    case KEY_VALUE_TAB:
                    {
                        s_uiFocusedFlag = ((s_uiFocusedFlag+1)%2);
                        if(0 == s_uiFocusedFlag)
                        {
                            SGUI_IntegerVariableBox_Refresh(&s_stNumberVariableBox, SGUI_CENTER, SGUI_DRAW_REVERSE);
                            SGUI_TextVariableBox_Refresh(&s_stTextVariableBox, SGUI_DRAW_NORMAL);
                        }
                        else
                        {
                            SGUI_IntegerVariableBox_Refresh(&s_stNumberVariableBox, SGUI_CENTER, SGUI_DRAW_NORMAL);
                            SGUI_TextVariableBox_Refresh(&s_stTextVariableBox, SGUI_DRAW_REVERSE);
                        }
                        eProcessResult = HMI_RET_NOACTION;
                        break;
                    }
                    case KEY_VALUE_ESC:
                    {
                        eProcessResult = HMI_RET_CANCEL;
                        break;
                    }
                    case KEY_VALUE_LEFT:
                    {
                        if(1 == s_uiFocusedFlag)
                        {
                            if(s_stTextVariableBox.FocusIndex > 0)
                            {
                                s_stTextVariableBox.FocusIndex--;
                                SGUI_TextVariableBox_ChangeCharacter(&s_stTextVariableBox, SGUI_DRAW_REVERSE, GUI_TEXT_ASCII, SGUI_TXT_VARBOX_OPT_NONE);
                            }
                        }
                        eProcessResult = HMI_RET_NOACTION;
                        break;
                    }
                    case KEY_VALUE_UP:
                    {
                        if(1 == s_uiFocusedFlag)
                        {
                            SGUI_TextVariableBox_ChangeCharacter(&s_stTextVariableBox, SGUI_DRAW_REVERSE, GUI_TEXT_ASCII, SGUI_TXT_VARBOX_OPT_PREV);
                        }
                        else
                        {
                            s_stNumberVariableBox.Value++;
                            SGUI_IntegerVariableBox_Refresh(&s_stNumberVariableBox, SGUI_CENTER, SGUI_DRAW_REVERSE);
                        }
                        eProcessResult = HMI_RET_NOACTION;
                        break;
                    }
                    case KEY_VALUE_RIGHT:
                    {
                        if(1 == s_uiFocusedFlag)
                        {
                            if(s_stTextVariableBox.FocusIndex < (s_stTextVariableBox.MaxTextLength-1))
                            {
                                s_stTextVariableBox.FocusIndex++;
                                SGUI_TextVariableBox_ChangeCharacter(&s_stTextVariableBox, SGUI_DRAW_REVERSE, GUI_TEXT_ASCII, SGUI_TXT_VARBOX_OPT_NONE);
                            }
                        }
                        eProcessResult = HMI_RET_NOACTION;
                        break;
                    }
                    case KEY_VALUE_DOWN:
                    {
                        if(1 == s_uiFocusedFlag)
                        {
                            SGUI_TextVariableBox_ChangeCharacter(&s_stTextVariableBox, SGUI_DRAW_REVERSE, GUI_TEXT_ASCII, SGUI_TXT_VARBOX_OPT_NEXT);
                        }
                        else
                        {
                            s_stNumberVariableBox.Value--;
                            SGUI_IntegerVariableBox_Refresh(&s_stNumberVariableBox, SGUI_CENTER, SGUI_DRAW_REVERSE);
                        }
                        eProcessResult = HMI_RET_NOACTION;
                        break;
                    }
                    case KEY_VALUE_ENTER:
                    {
                        if(1 == s_uiFocusedFlag)
                        {
                            s_szFrameTitle = s_stTextVariableBox.Value;
                            HMI_DemoVariableBox_DrawFrame((SGUI_PSZSTR)s_szFrameTitle);
                            // Draw number box
                            SGUI_Basic_DrawRectangle(VARIABLE_BOX_POSX, VARIABLE_BOX_NUMBER_POSY, VARIABLE_BOX_WIDTH+4, g_stFontSize[s_stNumberVariableBox.FontSize].Height+6, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
                            SGUI_IntegerVariableBox_Refresh(&s_stNumberVariableBox, SGUI_CENTER, SGUI_DRAW_NORMAL);
                            // Draw text box
                            SGUI_Basic_DrawRectangle(VARIABLE_BOX_POSX, VARIABLE_BOX_TEXT_POSY, VARIABLE_BOX_WIDTH+4, g_stFontSize[s_stTextVariableBox.FontSize].Height+6, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
                            SGUI_TextVariableBox_Refresh(&s_stTextVariableBox, SGUI_DRAW_REVERSE);
                        }
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
    }

	return eProcessResult;
}

HMI_ENGINE_RESULT HMI_DemoVariableBox_PostProcess(SGUI_INT iActionResult)
{
	if(HMI_RET_CANCEL == iActionResult)
	{
	    s_uiAutoConfirmTimer = 5;
		HMI_GoBack(NULL);
	}
	return HMI_RET_NORMAL;
}

void HMI_DemoVariableBox_DrawFrame(SGUI_PSZSTR szTitle)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_BOX_FRAME_STRUCT	stFrameData;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	stFrameData.Parameter.EdgeLayers = 2;
	stFrameData.Parameter.FontSize = SGUI_FONT_SIZE_H12;
	stFrameData.Data.Title = szTitle;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Draw frame
	SGUI_Frame_DrawFullScreenFrame(&stFrameData);
}
