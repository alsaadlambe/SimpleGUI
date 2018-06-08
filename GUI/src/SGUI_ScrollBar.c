/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_ScrollBar.c											**/
/** Author: XuYulin														**/
/** Version: 1.0.0.0													**/
/** Description: Create and display a scrollbar on screen.				**/
/*************************************************************************/

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Basic.h"
#include "SGUI_ScrollBar.h"

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_ScrollBar_RefreshScrollBar						**/
/** Purpose:		Display or update a scroll bar.						**/
/** Resources:		Scroll bar data structure.							**/
/** Params:																**/
/** @pstScrollBar[in]:	Scroll bar data pointer.						**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_ScrollBar_RefreshScrollBar(SGUI_SCROLLBAR_STRUCT* pstScrollBar)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_INT					uiScrollBlockPos;
	SGUI_SIZE					uiScrollBlockSize;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	if(SGUI_SCROLLBAR_VERTICAL == pstScrollBar->Parameter.eDirection)
	{
		uiScrollBlockSize	= pstScrollBar->Parameter.Width-2;
	}
	else
	{
		uiScrollBlockSize	= pstScrollBar->Parameter.Height-2;
	}

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(((pstScrollBar->Parameter.Height > 2) && (pstScrollBar->Parameter.Width > 2)) && (pstScrollBar->Parameter.Height != pstScrollBar->Parameter.Width))
	{
		// Check new value must be less then max value.
		if(pstScrollBar->Data.Index > pstScrollBar->Parameter.MaxIndex)
		{
			pstScrollBar->Data.Index = pstScrollBar->Parameter.MaxIndex;
		}

		if(SGUI_SCROLLBAR_VERTICAL == pstScrollBar->Parameter.eDirection)
		{
			// Draw scroll bar edge.
			SGUI_Basic_DrawRectangle(pstScrollBar->Parameter.PosX, pstScrollBar->Parameter.PosY,
									pstScrollBar->Parameter.Width, pstScrollBar->Parameter.Height,
									SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
			// Value lower limit is 0, scroll blocks must be greater then 0.
			if(pstScrollBar->Parameter.MaxIndex > 0)
			{
				uiScrollBlockPos = pstScrollBar->Parameter.PosY+1+((pstScrollBar->Parameter.Height-uiScrollBlockSize-2)*pstScrollBar->Data.Index/pstScrollBar->Parameter.MaxIndex);
				// Redraw process block
				SGUI_Basic_DrawRectangle(pstScrollBar->Parameter.PosX+1, uiScrollBlockPos,
										uiScrollBlockSize, uiScrollBlockSize, SGUI_COLOR_FRGCLR, SGUI_COLOR_FRGCLR);
			}
			else
			{
				SGUI_Basic_DrawRectangle(pstScrollBar->Parameter.PosX+1, pstScrollBar->Parameter.PosY+1,
										uiScrollBlockSize, uiScrollBlockSize, SGUI_COLOR_FRGCLR, SGUI_COLOR_FRGCLR);
			}
		}
		else // Horizontal
		{
			// Draw scroll bar edge.
			SGUI_Basic_DrawRectangle(pstScrollBar->Parameter.PosX, pstScrollBar->Parameter.PosY,
									pstScrollBar->Parameter.Width, pstScrollBar->Parameter.Height,
									SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
			// Value lower limit is 0, scroll blocks must be greater then 0.
			if(pstScrollBar->Parameter.MaxIndex > 0)
			{
				uiScrollBlockPos = pstScrollBar->Parameter.PosX+1+((pstScrollBar->Parameter.Width-uiScrollBlockSize-2)*pstScrollBar->Data.Index/pstScrollBar->Parameter.MaxIndex);
				// Redraw process block
				SGUI_Basic_DrawRectangle(uiScrollBlockPos, pstScrollBar->Parameter.PosY+1,
										uiScrollBlockSize, uiScrollBlockSize, SGUI_COLOR_FRGCLR, SGUI_COLOR_FRGCLR);
			}
			else
			{
				SGUI_Basic_DrawRectangle(pstScrollBar->Parameter.PosX+1, pstScrollBar->Parameter.PosY+1,
										uiScrollBlockSize, uiScrollBlockSize, SGUI_COLOR_FRGCLR, SGUI_COLOR_FRGCLR);
			}
		}
	}
}
