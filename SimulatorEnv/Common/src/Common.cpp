#include "Common.h"

PixelPanelParameter			g_stParameters;


void SetDefaultParameterData(PixelPanelParameter* pstParameter)
{
	if(NULL != pstParameter)
	{
		// Appearance
		pstParameter->HorizontalPixelNumber = PARAM_DEFAULT_PIXEL_NUM_H;
		pstParameter->VerticalPixelNumber = PARAM_DEFAULT_PIXEL_NUM_V;
		pstParameter->PixelSize = PARAM_DEFAULT_PIXEL_SIZE;
		pstParameter->EdgeWidth = PARAM_DEFAULT_EDGE_WIDTH;
		pstParameter->EnableGrid = PARAM_DEFAULT_GRID_DISABLE;

		// ScreenColor
		pstParameter->PanelColor.RGBA = LCD_COLOR_OBJ_BKG;
		pstParameter->PixelColor.RGBA = LCD_COLOR_OBJ_PIX;
		pstParameter->EdgeColor.RGBA = LCD_COLOR_OBJ_EDGE;
		pstParameter->EdgeColor.RGBA = LCD_COLOR_OBJ_GRID;
	}
}
