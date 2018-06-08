/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_Graph.c												**/
/** Author: Polarix														**/
/** Version: 1.0.0.0													**/
/** Description: Graph adjustment UI interface.							**/
/*************************************************************************/

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Text.h"
#include "SGUI_RealtimeGraph.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define	GUI_GRAPH_SCROLLBAR_WIDTH					(3)
#define GUI_GRAPH_GRAPH_AREA_WIDTH					(LCD_SIZE_WIDTH-GUI_GRAPH_SCROLLBAR_WIDTH-1)
#define GUI_GRAPH_GRAPH_AREA_HEIGHT					(LCD_SIZE_HEIGHT- GUI_GRAPH_SCROLLBAR_WIDTH-1)

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//
static SGUI_INT SGUI_RealtimeGraph_GetValuePointYCoordinate(SGUI_RTGRAPH* pstRTGraph, SGUI_INT iValue);

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_RealtimeGraph_Initialize						**/
/** Purpose:		Initialize a graph control data.					**/
/** Params:																**/
/**	@pstRTGraph[in]:	Graph map data.									**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_RealtimeGraph_Initialize(SGUI_RTGRAPH* pstRTGraph)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_RTGRAPH_DATA*			pstData;
	SGUI_RTGRAPH_CONTROL*		pstControl;
	SGUI_INT					iValueIndex;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstRTGraph)
	{
		pstData = pstRTGraph->Data;
		pstControl = pstRTGraph->Control;
		// Initialize graph controller.
		if((NULL != pstControl) && (NULL != pstData))
		{
			// yAxisMax must be greater then yAxisMin;
			if(pstControl->yAxisMax < pstControl->yAxisMin)
			{
				SGUI_SWAP(pstControl->yAxisMax, pstControl->yAxisMin);
			}
			// X-axis step in pixel must be greater then 2.
			if(pstControl->xAxisStepPixel < 2)
			{
				pstControl->xAxisStepPixel = 2;
			}

			pstControl->ValueArea = pstControl->yAxisMax - pstControl->yAxisMin;

			// Initialize graph data.
            //SGUI_Common_MemorySet(pstData, 0x00, sizeof(SGUI_RTGRAPH_DATA));
            // Zero point value must NOT greater then yAxisMax and NOT less then yAxisMin.
			if(pstData->ZeroPointValue > pstControl->yAxisMax)
			{
				pstData->ZeroPointValue = pstControl->yAxisMax;
			}
			if(pstData->ZeroPointValue < pstControl->yAxisMin)
			{
				pstData->ZeroPointValue = pstControl->yAxisMin;
			}
            // Calculate the number of value points that can be used.
			pstData->ValueCount = (LCD_SIZE_WIDTH-2)/pstControl->xAxisStepPixel;
            for(iValueIndex=0; iValueIndex<pstData->ValueCount; iValueIndex++)
			{
				pstData->ValueArray[iValueIndex] = pstData->ZeroPointValue;
				pstData->LimitedValueArray[iValueIndex] = pstData->ZeroPointValue;
				pstData->PointYCoordinateArray[iValueIndex] = SGUI_RealtimeGraph_GetValuePointYCoordinate(pstRTGraph, pstData->LimitedValueArray[iValueIndex]);
			}
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Graph_Refresh									**/
/** Purpose:		Refresh graph map display.							**/
/** Resources:		Graph data.											**/
/** Params:																**/
/**	@pstRTGraph[in]:		Graph map data used refreshed.				**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_RealtimeGraph_Refresh(SGUI_RTGRAPH* pstRTGraph, SGUI_PCSZSTR szTopText, SGUI_PCSZSTR szBottomText)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_RTGRAPH_DATA*			pstData;
	SGUI_RTGRAPH_CONTROL*		pstControl;
	SGUI_INT					iValueIndex;
	SGUI_INT					iPixelCoordinateStartX, iPixelCoordinateEndX;
	SGUI_INT					iBaseLineCoordinateY;
	SGUI_RECT_AREA				stTextDataArea, stTextDispArea;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Draw frame
	SGUI_Basic_DrawRectangle(0, 0, LCD_SIZE_WIDTH, LCD_SIZE_HEIGHT, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
	SGUI_Basic_DrawLine(1, 9, LCD_SIZE_WIDTH-2, 9, SGUI_COLOR_FRGCLR);
	SGUI_Basic_DrawLine(1, LCD_SIZE_HEIGHT-9, LCD_SIZE_WIDTH-2, LCD_SIZE_HEIGHT-9, SGUI_COLOR_FRGCLR);

	if(NULL != pstRTGraph)
	{
		pstData = pstRTGraph->Data;
		pstControl = pstRTGraph->Control;
		if((NULL != pstControl) && (NULL != pstData))
		{
			if(SGUI_TRUE == pstControl->EnableBaseline)
			{
				iBaseLineCoordinateY = SGUI_RealtimeGraph_GetValuePointYCoordinate(pstRTGraph, pstData->ZeroPointValue);
				SGUI_Basic_DrawLine(1, iBaseLineCoordinateY, LCD_SIZE_WIDTH-2, iBaseLineCoordinateY, SGUI_COLOR_FRGCLR);
			}

			if(pstData->ValueCount > 1)
			{
				for(iValueIndex=1; iValueIndex<pstData->ValueCount; iValueIndex++)
				{
					iPixelCoordinateStartX = 1 + (iValueIndex-1) * pstControl->xAxisStepPixel;
					iPixelCoordinateEndX = 1 + iValueIndex * pstControl->xAxisStepPixel;
					SGUI_Basic_DrawLine(iPixelCoordinateStartX, pstData->PointYCoordinateArray[iValueIndex-1],
											iPixelCoordinateEndX, pstData->PointYCoordinateArray[iValueIndex], SGUI_COLOR_FRGCLR);
				}
			}
			//SGUI_Common_IntegerToString(pstData->ValueArray[pstData->ValueCount-1], szValueBuffer, 10, 10, ' ');
            stTextDataArea.PosX = 0; stTextDataArea.PosY = 0;
            stTextDispArea.PosX = 1; stTextDispArea.PosY = 1;
            stTextDispArea.Width = LCD_SIZE_WIDTH-2, stTextDispArea.Height = 7;
            SGUI_Text_DrawSingleLineText(szTopText, SGUI_FONT_SIZE_H8, &stTextDispArea, &stTextDataArea, SGUI_DRAW_NORMAL);
            stTextDispArea.PosY = LCD_SIZE_HEIGHT-8;
            SGUI_Text_DrawSingleLineText(szBottomText, SGUI_FONT_SIZE_H8, &stTextDispArea, &stTextDataArea, SGUI_DRAW_NORMAL);
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_RealtimeGraph_AppendValue						**/
/** Purpose:		Append a new value to graph.						**/
/** Params:																**/
/**	@pstRTGraph[in]:	Real-time graph object pointer.					**/
/**	@iNewValue[in]:		New value will be appended.						**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_RealtimeGraph_AppendValue(SGUI_RTGRAPH* pstRTGraph, SGUI_INT iNewValue)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_RTGRAPH_DATA*			pstData;
	SGUI_RTGRAPH_CONTROL*		pstControl;
	SGUI_INT					iValueIndex;
	SGUI_INT					iLimitedValue;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstRTGraph)
	{
		pstData = pstRTGraph->Data;
		pstControl = pstRTGraph->Control;
		if((NULL != pstControl) && (NULL != pstData))
		{
			// Push value.
            for(iValueIndex=0; iValueIndex<pstData->ValueCount-1; iValueIndex++)
			{
				pstData->ValueArray[iValueIndex] = pstData->ValueArray[iValueIndex+1];
				pstData->LimitedValueArray[iValueIndex] = pstData->LimitedValueArray[iValueIndex+1];
				pstData->PointYCoordinateArray[iValueIndex] = pstData->PointYCoordinateArray[iValueIndex+1];
			}
			// Append new value.
			iValueIndex = pstData->ValueCount-1;
			pstData->ValueArray[iValueIndex] = iNewValue;
			iLimitedValue = iNewValue;
			if(iLimitedValue > pstControl->yAxisMax)
			{
				iLimitedValue = pstControl->yAxisMax;
			}
			if(iLimitedValue < pstControl->yAxisMin)
			{
				iLimitedValue = pstControl->yAxisMin;
			}
			pstData->LimitedValueArray[iValueIndex] = iLimitedValue;
			pstData->PointYCoordinateArray[iValueIndex] = SGUI_RealtimeGraph_GetValuePointYCoordinate(pstRTGraph, iLimitedValue);
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_RealtimeGraph_GetValuePointYCoordinate			**/
/** Purpose:		Convert data point to a drawing point in visible 	**/
/**					graph area.											**/
/** Params:																**/
/**	@pstRTGraph[in]:	Real-time graph object pointer.					**/
/**	@iValue[in]:		Real value.										**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
SGUI_INT SGUI_RealtimeGraph_GetValuePointYCoordinate(SGUI_RTGRAPH* pstRTGraph, SGUI_INT iValue)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_RTGRAPH_DATA*			pstData;
	SGUI_RTGRAPH_CONTROL*		pstControl;
	SGUI_INT					iDisplayValuePointAreaHeight;
	SGUI_INT					iAbsoluteValue;
	SGUI_INT					iValuePointCoordinate;
	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	iDisplayValuePointAreaHeight = LCD_SIZE_HEIGHT-9*2;
	iValuePointCoordinate =     -1;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstRTGraph)
	{
		pstData = pstRTGraph->Data;
		pstControl = pstRTGraph->Control;
		if((NULL != pstControl) && (NULL != pstData))
		{
			if(iValue > pstControl->yAxisMax)
			{
				iValue = pstControl->yAxisMax;
			}
			if(iValue < pstControl->yAxisMin)
			{
				iValue = pstControl->yAxisMin;
			}
			iAbsoluteValue = iValue - pstControl->yAxisMin;

			iValuePointCoordinate = iAbsoluteValue*iDisplayValuePointAreaHeight/pstControl->ValueArea;

			iValuePointCoordinate = iDisplayValuePointAreaHeight-iValuePointCoordinate;

			iValuePointCoordinate = iValuePointCoordinate +9;
		}
	}

	return iValuePointCoordinate;
}
