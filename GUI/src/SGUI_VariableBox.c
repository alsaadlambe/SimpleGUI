/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_VariableBox.c										**/
/** Author: XuYulin														**/
/** Version: 1.0.0.0													**/
/** Description: Show and change variable box.							**/
/*************************************************************************/

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"
#include "SGUI_Text.h"
#include "SGUI_VariableBox.h"

//=======================================================================//
//= Static variable declaration.									    =//
//=======================================================================//
static char				arrTextBuffer[VARBOX_TEXT_BUFFER_SIZE] = {0x00};	//Used when convert a number to text.

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//
void					SGUI_TextVariableBox_UpdateCharacter(SGUI_TEXT_VARBOX_STRUCT* pstTextValue, char cNewCharacters, SGUI_DRAW_MODE eMode);

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	GUI_RefreshVariableBox								**/
/** Purpose:		Display or refresh a integer value edit box.		**/
/** Params:																**/
/**	@pstValue[in]:		Value structure, include max value, min value	**/
/**						and current value.								**/
/**	@eAlignment[in]:	Alignment, might be right, center or left.		**/
/**	@eMode[in]:			Display mode, normal or reveres.				**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_IntegerVariableBox_Refresh(SGUI_INT_VARBOX_STRUCT* pstValue, SGUI_VARBOX_ALIG eAlignment, SGUI_DRAW_MODE eMode)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_RECT_AREA				stTextDisplayArea;
	SGUI_RECT_AREA				stTextDataArea;
	SGUI_SIZE					uiTextLength;
	SGUI_SIZE					uiTextWidth;
	SGUI_COLOR					eBackColor;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eBackColor =				((eMode==SGUI_DRAW_NORMAL)?SGUI_COLOR_BKGCLR:SGUI_COLOR_FRGCLR);
	// Draw edge
	SGUI_Basic_DrawRectangle(pstValue->PosX, pstValue->PosY, pstValue->Width, VARBOX_HEIGHT(pstValue->FontSize), eBackColor, eBackColor);

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/

	if(NULL != pstValue)
	{
		// Check value limited.
		if(pstValue->Value > pstValue->Max)
		{
			pstValue->Value = pstValue->Max;
		}
		if(pstValue->Value < pstValue->Min)
		{
			pstValue->Value = pstValue->Min;
		}
		// Convert number to string
		uiTextLength = SGUI_Common_IntegerToString(pstValue->Value, arrTextBuffer, 10, -1, ' ');
		uiTextWidth = VARBOX_TEXT_WIDTH(pstValue->FontSize, uiTextLength);
		stTextDisplayArea.PosX = pstValue->PosX+1;
		stTextDisplayArea.PosY = pstValue->PosY+1;
		stTextDisplayArea.Width = VARBOX_TEXT_AREA_WIDTH(pstValue->Width);
		stTextDisplayArea.Height = g_stFontSize[pstValue->FontSize].Height;
		switch(eAlignment)
		{
			case SGUI_RIGHT:
			{
				stTextDataArea.PosX = stTextDisplayArea.Width - uiTextWidth;
				break;
			}
			case SGUI_CENTER:
			{
				stTextDataArea.PosX = (stTextDisplayArea.Width - uiTextWidth) / 2;
				break;
			}
			default:
			{
				stTextDataArea.PosX = 0;
			}
		}
		stTextDataArea.PosY = 0;
		SGUI_Text_DrawSingleLineText(arrTextBuffer, pstValue->FontSize, &stTextDisplayArea, &stTextDataArea, eMode);
	}
}

/*************************************************************************/
/** Function Name:	SGUI_TextVariableBox_UpdateCharacter				**/
/** Purpose:		Display or refresh a integer value edit box.		**/
/** Params:																**/
/** @pstTextValue[in]:	Text value edit box pointer.					**/
/** @cNewCharacters[in]: New character of value.						**/
/**	@eMode[in]:			Display mode, normal or reveres.				**/
/** Return:			None.												**/
/** Notice:			Static function, call by others interface.			**/
/*************************************************************************/
void SGUI_TextVariableBox_UpdateCharacter(SGUI_TEXT_VARBOX_STRUCT* pstTextValue, char cNewCharacters, SGUI_DRAW_MODE eMode)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_COLOR					eBackColor;
	SGUI_RECT_AREA				stTextDisplayArea, stTextDataArea;
	SGUI_RECT_AREA				stFocusArea;
	SGUI_UINT16					uiFontWidth, uiFontHeight;
	SGUI_SIZE					uiTextLength, uiFocusIndexMax;
	SGUI_SIZE					uiFillIndex;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eBackColor =				((eMode==SGUI_DRAW_NORMAL)?SGUI_COLOR_BKGCLR:SGUI_COLOR_FRGCLR);
	// Clear background.
	if(NULL != pstTextValue->Value)
	{
		SGUI_Basic_DrawRectangle(pstTextValue->PosX, pstTextValue->PosY, pstTextValue->Width, VARBOX_HEIGHT(pstTextValue->FontSize), eBackColor, eBackColor);
	}
	// Get font graphics size.
	uiFontWidth =				g_stFontSize[pstTextValue->FontSize].Width;
	uiFontHeight =				g_stFontSize[pstTextValue->FontSize].Height;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstTextValue) && (NULL != pstTextValue->Value))
	{
		// Get max text length and graphics width.

		if(pstTextValue->MaxTextLength > VARBOX_TEXT_LENGTH_MAX)
		{
			pstTextValue->MaxTextLength = VARBOX_TEXT_LENGTH_MAX;
		}
		uiFocusIndexMax = pstTextValue->MaxTextLength-1;
		// Ignore too long text string.
		uiTextLength = SGUI_Common_StringLength(pstTextValue->Value);
		if(uiTextLength > pstTextValue->MaxTextLength)
		{
			uiTextLength = pstTextValue->MaxTextLength;
			*(pstTextValue->Value+uiTextLength) = '\0';
            // Point at to last character position if index is more then string length.
			if(pstTextValue->FocusIndex > uiFocusIndexMax)
			{
				pstTextValue->FocusIndex = uiFocusIndexMax;
			}
		}
		// Update character in text string when index is valid.
		if(GUI_TEXT_ISASCII(cNewCharacters))
		{
			if(pstTextValue->FocusIndex < uiTextLength)
			{
				*(pstTextValue->Value+pstTextValue->FocusIndex) = cNewCharacters;
			}
			else
			{
				// Extend string with space if focus index is more then text length.
				uiFillIndex = uiTextLength;
				while(uiFillIndex <= pstTextValue->FocusIndex)
				{
					*(pstTextValue->Value+uiFillIndex) = ' ';
					uiFillIndex++;
				}
				*(pstTextValue->Value+uiFillIndex) = '\0';
			}
		}
		// Set text display area.
		stTextDisplayArea.PosX = pstTextValue->PosX+1;
		stTextDisplayArea.PosY = pstTextValue->PosY+1;
		stTextDisplayArea.Width = VARBOX_TEXT_AREA_WIDTH(pstTextValue->Width);
		stTextDisplayArea.Height = uiFontHeight;
		stTextDataArea.PosX = 0;
		stTextDataArea.PosY = 0;

		// Set focus character area.
		stFocusArea.PosX = stTextDisplayArea.PosX+pstTextValue->FocusIndex*uiFontWidth;
		stFocusArea.PosY = stTextDisplayArea.PosY;
		stFocusArea.Width = uiFontWidth;
		stFocusArea.Height = uiFontHeight;
		if(RECTANGLE_X_END(stFocusArea) > RECTANGLE_X_END(stTextDisplayArea))
		{
			stTextDataArea.PosX = RECTANGLE_X_END(stTextDisplayArea) - RECTANGLE_X_END(stFocusArea);
			stFocusArea.PosX = stFocusArea.PosX + stTextDataArea.PosX;
		}
		// Display text.
		SGUI_Text_DrawSingleLineText(pstTextValue->Value, pstTextValue->FontSize, &stTextDisplayArea, &stTextDataArea, eMode);
		// Focus first character.
        SGUI_Basic_ReverseBlockColor(stFocusArea.PosX, stFocusArea.PosY, stFocusArea.Width, stFocusArea.Height);
	}
}

/*************************************************************************/
/** Function Name:	SGUI_TextVariableBox_Refresh						**/
/** Purpose:		Display or refresh a integer value edit box.		**/
/** Params:																**/
/** @pstTextValue[in]:	Text value edit box pointer.					**/
/**	@eMode[in]:			Display mode, normal or reveres.				**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_TextVariableBox_Refresh(SGUI_TEXT_VARBOX_STRUCT* pstTextValue, SGUI_DRAW_MODE eMode)
{

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstTextValue)
	{
		if(NULL != pstTextValue->Value)
		{
			if(pstTextValue->FocusIndex >= pstTextValue->MaxTextLength)
			{
				pstTextValue->FocusIndex = pstTextValue->MaxTextLength-1;
			}
			SGUI_TextVariableBox_UpdateCharacter(pstTextValue, '\0', eMode);
		}
	}
}

/*************************************************************************/
/** Function Name:	GUI_TextVariableBox_PrevCharacter					**/
/** Purpose:		Select previous character at current index.			**/
/** Params:																**/
/** @pstTextValue[in]:	Text value edit box pointer.					**/
/**	@eMode[in]:			Display mode, normal or reveres.				**/
/**	@uiCharacterSet[in]: Mask of characters set.						**/
/** @eOpt[in]:			Operation.										**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_TextVariableBox_ChangeCharacter(SGUI_TEXT_VARBOX_STRUCT* pstTextValue, SGUI_DRAW_MODE eMode, SGUI_UINT uiCharacterSet, SGUI_TEXT_VARBOX_OPT eOpt)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_CHAR					cCurChar;
	SGUI_BOOL					bUpdated;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	cCurChar					= '\0';
	bUpdated					= SGUI_FALSE;
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstTextValue)
	{
		if(NULL != pstTextValue->Value)
		{
			cCurChar = *(pstTextValue->Value+pstTextValue->FocusIndex);
			while(SGUI_FALSE == bUpdated)
			{
				cCurChar += (SGUI_INT)eOpt;
				if(cCurChar > 0x7E)
				{
					cCurChar = 0x20;
				}
				if(cCurChar < 0x20)
				{
					cCurChar = 0x7E;
				}
				if(GUI_TEXT_ISDIGIT(cCurChar) || ('.' == cCurChar) || ('-' == cCurChar))
				{
					if(uiCharacterSet & SGUI_TEXT_NUMBER)
					{
						bUpdated = SGUI_TRUE;
					}
				}
				if(GUI_TEXT_ISALPHA(cCurChar))
				{
					if(uiCharacterSet & SGUI_TEXT_ALPHA)
					{
						bUpdated = SGUI_TRUE;
					}
				}
				if((!GUI_TEXT_ISDIGIT(cCurChar)) && (!(GUI_TEXT_ISALPHA(cCurChar))))
				{
					if(uiCharacterSet & SGUI_TEXT_PUNCTUATION)
					{
						bUpdated = SGUI_TRUE;
					}
				}
			}
			SGUI_TextVariableBox_UpdateCharacter(pstTextValue, cCurChar, eMode);
		}
	}
}
