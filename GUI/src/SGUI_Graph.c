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
#include "SGUI_Graph.h"
#include "SGUI_Frame.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define	GUI_GRAPH_SCROLLBAR_WIDTH					(3)
#define GUI_GRAPH_GRAPH_AREA_WIDTH					(LCD_SIZE_WIDTH-GUI_GRAPH_SCROLLBAR_WIDTH-1)
#define GUI_GRAPH_GRAPH_AREA_HEIGHT					(LCD_SIZE_HEIGHT- GUI_GRAPH_SCROLLBAR_WIDTH-1)

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//
static void SGUI_Graph_DrawLine(SGUI_GRAPH* pstGraph, SGUI_UINT32 uiStartPointIndex, SGUI_UINT32 uiEndPointIndex);
static void SGUI_Praph_GetPointDrawingCoordinate(SGUI_GRAPH_POINT* pstDataPoint, SGUI_GRAPH_CONTROL* pstControlData, SGUI_GRAPH_POINT* pstDrawingPoint);

//=======================================================================//
//= Function implementation.										    =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_Graph_InitializeGraphData						**/
/** Purpose:		Initialize a graph control data.					**/
/** Resources:		Graph data.											**/
/** Params:																**/
/**	@pstGraph[in]:		Graph map data.									**/
/**	@pstInitializeData[in]: Pointer list data will be initialized.		**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_Graph_InitializeGraphData(SGUI_GRAPH* pstGraph, SGUI_GRAPH_INIT_DATA* pstInitializeData)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstGraph) && (NULL != pstInitializeData))
	{
		// Point data
		if(0 == pstInitializeData->Count)
		{
			pstGraph->Data->Points = NULL;
			pstGraph->Data->Count = 0;
		}
		else
		{
			pstGraph->Data->Points = pstInitializeData->Points;
			pstGraph->Data->Count = pstInitializeData->Count;
		}
		// Point range
		pstGraph->Control->PointRangeX = pstGraph->Control->xAxis.Max-pstGraph->Control->xAxis.Min + 1;
		pstGraph->Control->PointRangeY = pstGraph->Control->yAxis.Max-pstGraph->Control->yAxis.Min + 1;
		// Focused point index.
		if(pstInitializeData->FocusIndex >= pstGraph->Data->Count)
		{
			pstGraph->Control->FocusIndex = pstGraph->Data->Count-1;
		}
		else
		{
			pstGraph->Control->FocusIndex = pstInitializeData->FocusIndex;
		}
		// Axis scrollbar
		pstGraph->SubElement.xScrollBar.Parameter.eDirection = SGUI_SCROLLBAR_HORIZONTAL;
 		pstGraph->SubElement.xScrollBar.Parameter.MaxIndex = pstGraph->Control->PointRangeX;
		pstGraph->SubElement.xScrollBar.Parameter.PosX = GUI_GRAPH_SCROLLBAR_WIDTH;
		pstGraph->SubElement.xScrollBar.Parameter.PosY = 0;
		pstGraph->SubElement.xScrollBar.Parameter.Width = LCD_SIZE_WIDTH - GUI_GRAPH_SCROLLBAR_WIDTH;
		pstGraph->SubElement.xScrollBar.Parameter.Height = GUI_GRAPH_SCROLLBAR_WIDTH;

		pstGraph->SubElement.yScrollBar.Parameter.eDirection = SGUI_SCROLLBAR_VERTICAL;
		pstGraph->SubElement.yScrollBar.Parameter.MaxIndex = pstGraph->Control->PointRangeY;
		pstGraph->SubElement.yScrollBar.Parameter.PosX = 0;
		pstGraph->SubElement.yScrollBar.Parameter.PosY = GUI_GRAPH_SCROLLBAR_WIDTH;
		pstGraph->SubElement.yScrollBar.Parameter.Width = GUI_GRAPH_SCROLLBAR_WIDTH;
		pstGraph->SubElement.yScrollBar.Parameter.Height = LCD_SIZE_HEIGHT - GUI_GRAPH_SCROLLBAR_WIDTH;

		pstGraph->Control->FocusIndex = pstInitializeData->FocusIndex;
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Graph_Refresh									**/
/** Purpose:		Refresh graph map display.							**/
/** Resources:		Graph data.											**/
/** Params:																**/
/**	@pstGraph[in]:		Graph map data used refreshed.					**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_Graph_Refresh(SGUI_GRAPH* pstGraph)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_BOX_FRAME_STRUCT		stFrameData;
	SGUI_UINT32					uiPointIndex;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	stFrameData.Data.Title	=	NULL;
	stFrameData.Parameter.EdgeLayers = 1;
	stFrameData.Parameter.FontSize = SGUI_FONT_SIZE_H6;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	SGUI_Frame_DrawFullScreenFrame(&stFrameData);
	//SGUI_Basic_DrawRectangle(0, 0, LCD_SIZE_WIDTH, LCD_SIZE_HEIGHT, GUI_COLOR_BKGCLR, GUI_COLOR_BKGCLR);
	SGUI_ScrollBar_RefreshScrollBar(&(pstGraph->SubElement.xScrollBar));
	SGUI_ScrollBar_RefreshScrollBar(&(pstGraph->SubElement.yScrollBar));

	if(pstGraph->Data->Count > 1)
	{
		for(uiPointIndex=0; uiPointIndex<(pstGraph->Data->Count-1); uiPointIndex++)
		{
			SGUI_Graph_DrawLine(pstGraph, uiPointIndex, uiPointIndex+1);
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Graph_DrawLine									**/
/** Purpose:		Draw a line with two points in graph map.			**/
/** Resources:		Graph data.											**/
/** Params:																**/
/**	@pstGraph[in]:		Graph map data used refreshed.					**/
/**	@uiStartPointIndex[in]: Line starting point index.					**/
/**	@uiEndPointIndex[in]: Line ending point index.						**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_Graph_DrawLine(SGUI_GRAPH* pstGraph, SGUI_UINT32 uiStartPointIndex, SGUI_UINT32 uiEndPointIndex)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_GRAPH_POINT*			pstStartPoint;
	SGUI_GRAPH_POINT*			pstEndPoint;
	SGUI_GRAPH_POINT			stGStartPoint;
	SGUI_GRAPH_POINT			stGEndPoint;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstGraph)
	{
		// Start point.
		pstStartPoint = pstGraph->Data->Points+uiStartPointIndex;
		SGUI_Praph_GetPointDrawingCoordinate(pstStartPoint, pstGraph->Control, &stGStartPoint);

		// End point.
		pstEndPoint = pstGraph->Data->Points+uiEndPointIndex;
		SGUI_Praph_GetPointDrawingCoordinate(pstEndPoint, pstGraph->Control, &stGEndPoint);

		// End point.
		SGUI_Basic_DrawLine(stGStartPoint.x, stGStartPoint.y, stGEndPoint.x, stGEndPoint.y, SGUI_COLOR_FRGCLR);
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Praph_GetPointDrawingCoordinate					**/
/** Purpose:		Convert data point to a drawing point in visible 	**/
/**					graph area.											**/
/** Resources:		None.												**/
/** Params:																**/
/**	@pstDataPoint[in]:	Data point structure pointer.					**/
/**	@pstDrawingPoint[out]: Drawing point structure pointer.				**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_Praph_GetPointDrawingCoordinate(SGUI_GRAPH_POINT* pstDataPoint, SGUI_GRAPH_CONTROL* pstControlData, SGUI_GRAPH_POINT* pstDrawingPoint)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pstDataPoint) && (NULL != pstDrawingPoint))
	{
		// Offset data;
		pstDrawingPoint->x = (pstDataPoint->x-pstControlData->xAxis.Min);
		pstDrawingPoint->y = (pstControlData->PointRangeY-(pstDataPoint->y-pstControlData->yAxis.Min)-1);

		pstDrawingPoint->x = GUI_GRAPH_SCROLLBAR_WIDTH+pstDrawingPoint->x*GUI_GRAPH_GRAPH_AREA_WIDTH/pstControlData->PointRangeX;
		pstDrawingPoint->y = GUI_GRAPH_SCROLLBAR_WIDTH+pstDrawingPoint->y*GUI_GRAPH_GRAPH_AREA_HEIGHT/pstControlData->PointRangeY;
	}
}
