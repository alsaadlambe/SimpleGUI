#ifndef __INCLUDE_GUI_REAL_GRAPH_H__
#define __INCLUDE_GUI_REAL_GRAPH_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef struct
{
	SGUI_INT				yAxisMax;
	SGUI_INT				yAxisMin;
	SGUI_BOOL				EnableBaseline;
	SGUI_INT				xAxisStepPixel;
	SGUI_INT				ValueArea;
}SGUI_RTGRAPH_CONTROL;

typedef struct
{
	SGUI_INT				ValueArray[LCD_SIZE_WIDTH];
	SGUI_INT				LimitedValueArray[LCD_SIZE_WIDTH];
	SGUI_INT				PointYCoordinateArray[LCD_SIZE_WIDTH];
	SGUI_INT				ZeroPointValue;
	SGUI_INT				ValueCount;
}SGUI_RTGRAPH_DATA;

typedef struct
{
	SGUI_RTGRAPH_DATA*		Data;
	SGUI_RTGRAPH_CONTROL*	Control;
}SGUI_RTGRAPH;

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
void			SGUI_RealtimeGraph_Initialize(SGUI_RTGRAPH* pstRTGraph);
void			SGUI_RealtimeGraph_Refresh(SGUI_RTGRAPH* pstRTGraph, SGUI_PCSZSTR szTopText, SGUI_PCSZSTR szBottomText);
void			SGUI_RealtimeGraph_AppendValue(SGUI_RTGRAPH* pstRTGraph, SGUI_INT iNewValue);
#endif // __INCLUDE_GUI_REAL_GRAPH_H__
