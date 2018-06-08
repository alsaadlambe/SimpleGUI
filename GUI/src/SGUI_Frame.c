/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_Frame.c												**/
/** Author: Polarix														**/
/** Version: 1.0.0.0													**/
/** Description: Draw frames on screen.									**/
/*************************************************************************/

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Frame.h"

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_Frame_DrawFullScreenFrame						**/
/** Purpose:		Draw a frame on screen, just like a window frame.	**/
/** Params:																**/
/**	@pstFrameData[in]:	Frame data structure pointer.					**/
/** Return:			None.												**/
/** Notice:			none.												**/
/*************************************************************************/
void SGUI_Frame_DrawFullScreenFrame(SGUI_BOX_FRAME_STRUCT* pstFrameData)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_SIZE					uiEdgeLayerIndex;
	SGUI_UINT					uiEdgeRectanglePosX, uiEdgeRectanglePosY;
	SGUI_UINT					uiEdgeRectangleWidth, uiEdgeRectangleHeight;
	SGUI_SIZE					uiTitleLineWidth;
	SGUI_RECT_AREA				stTitleTextDisplayArea;
	SGUI_RECT_AREA				stTitleTextDataArea = {0, 0, 0, 0};

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	if(NULL != pstFrameData)
	{
		uiEdgeRectanglePosX		= 0;
		uiEdgeRectanglePosY		= 0;
		uiEdgeRectangleWidth	= LCD_SIZE_WIDTH;
		uiEdgeRectangleHeight	= LCD_SIZE_HEIGHT;
		uiTitleLineWidth		= g_stFontSize[pstFrameData->Parameter.FontSize].Height+2;
	}

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstFrameData)
	{
        // Draw frame edge
		for(uiEdgeLayerIndex = 0; uiEdgeLayerIndex<pstFrameData->Parameter.EdgeLayers; uiEdgeLayerIndex++)
		{
			SGUI_Basic_DrawRectangle(	uiEdgeRectanglePosX, uiEdgeRectanglePosY,
										uiEdgeRectangleWidth, uiEdgeRectangleHeight,
										SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
			uiEdgeRectanglePosX += 2;
			uiEdgeRectanglePosY += 2;
			uiEdgeRectangleWidth -= 4;
			uiEdgeRectangleHeight -= 4;
		}

		// Draw Title if exist.
		if(NULL != pstFrameData->Data.Title)
		{
			if(pstFrameData->Parameter.EdgeLayers > 0)
			{
				SGUI_Basic_DrawLine((pstFrameData->Parameter.EdgeLayers*2-1), (pstFrameData->Parameter.EdgeLayers*2-1+uiTitleLineWidth),
									LCD_SIZE_WIDTH-pstFrameData->Parameter.EdgeLayers*2, (pstFrameData->Parameter.EdgeLayers*2-1+uiTitleLineWidth),
									SGUI_COLOR_FRGCLR);
				stTitleTextDisplayArea.PosX		= pstFrameData->Parameter.EdgeLayers*2;
				stTitleTextDisplayArea.PosY		= pstFrameData->Parameter.EdgeLayers*2;
				stTitleTextDisplayArea.Width	= LCD_SIZE_WIDTH-pstFrameData->Parameter.EdgeLayers*4;
				stTitleTextDisplayArea.Height	= g_stFontSize[pstFrameData->Parameter.FontSize].Height;
			}
			else
			{
				SGUI_Basic_DrawLine(	0, (pstFrameData->Parameter.EdgeLayers*2+uiTitleLineWidth),
									LCD_SIZE_WIDTH-1, (pstFrameData->Parameter.EdgeLayers*2+uiTitleLineWidth),
									SGUI_COLOR_FRGCLR);
				stTitleTextDisplayArea.PosX		= 1;
				stTitleTextDisplayArea.PosY		= 1;
				stTitleTextDisplayArea.Width	= LCD_SIZE_WIDTH-2;
				stTitleTextDisplayArea.Height	= g_stFontSize[pstFrameData->Parameter.FontSize].Height;
			}
			SGUI_Text_DrawSingleLineText(	pstFrameData->Data.Title, pstFrameData->Parameter.FontSize,
											&stTitleTextDisplayArea, &stTitleTextDataArea, SGUI_DRAW_NORMAL);
		}
	}
}
