/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_Basic.c												**/
/** Author: XuYulin														**/
/** Version: 1.0.0.0													**/
/** Description: Simple GUI basic drawing operating interface.					**/
/*************************************************************************/

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Basic.h"

//=======================================================================//
//= Static variable declaration.									    =//
//=======================================================================//
//   Here is the 4*6 pixel font data definition, only number characters
// and plug(+), subtract(-), multiply(*), divide(/), brackets and space.
SGUI_CBYTE SGUI_BASIC_FONT_H6[] = {
0x1F, 0x11, 0x1F, 0x00,	//0
0x00, 0x1F, 0x00, 0x00,	//1
0x1D, 0x15, 0x17, 0x00,	//2
0x15, 0x15, 0x1F, 0x00,	//3
0x07, 0x04, 0x1F, 0x00,	//4
0x17, 0x15, 0x1D, 0x00,	//5
0x1F, 0x15, 0x1D, 0x00,	//6
0x19, 0x05, 0x03, 0x00,	//7
0x1F, 0x15, 0x1F, 0x00,	//8
0x17, 0x15, 0x1F, 0x00,	//9
0x00, 0x10, 0x00, 0x00,	//.
0x04, 0x0E, 0x04, 0x00,	//+
0x04, 0x04, 0x04, 0x00,	//-
0x0A, 0x04, 0x0A, 0x00,	//**
0x18, 0x04, 0x03, 0x00, ///
0x00, 0x0E, 0x11, 0x00, //(
0x11, 0x0E, 0x00, 0x00, //)
0x00, 0x00, 0x00, 0x00, //space
};

//   Here is the 6*8 pixel font data definition, only visible ASCII
// characters it's also used as basic font, when the extended font data
// lost, these data can used for display some error message or runtime
// information.
SGUI_CBYTE SGUI_BASIC_FONT_H8[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// space index 0
0x00, 0x00, 0x5F, 0x00, 0x00, 0x00,	// !
0x00, 0x07, 0x00, 0x07, 0x00, 0x00,	// "
0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00,	// #
0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00,	// $
0x23, 0x13, 0x08, 0x64, 0x62, 0x00,	// %
0x36, 0x49, 0x55, 0x22, 0x50, 0x00,	// &
0x00, 0x05, 0x03, 0x00, 0x00, 0x00,	// '
0x00, 0x1C, 0x22, 0x41, 0x00, 0x00,	// (
0x00, 0x41, 0x22, 0x1C, 0x00, 0x00,	// )
0x08, 0x2A, 0x1C, 0x2A, 0x08, 0x00,	// *
0x08, 0x08, 0x3E, 0x08, 0x08, 0x00,	// +
0x00, 0x50, 0x30, 0x00, 0x00, 0x00,	// ,
0x08, 0x08, 0x08, 0x08, 0x08, 0x00,	// -
0x00, 0x30, 0x30, 0x00, 0x00, 0x00,	// .
0x20, 0x10, 0x08, 0x04, 0x02, 0x00,	// /
0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00,	// 0
0x00, 0x42, 0x7F, 0x40, 0x00, 0x00,	// 1
0x42, 0x61, 0x51, 0x49, 0x46, 0x00,	// 2
0x21, 0x41, 0x45, 0x4B, 0x31, 0x00,	// 3
0x18, 0x14, 0x12, 0x7F, 0x10, 0x00,	// 4
0x27, 0x45, 0x45, 0x45, 0x39, 0x00,	// 5
0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00,	// 6
0x01, 0x71, 0x09, 0x05, 0x03, 0x00,	// 7
0x36, 0x49, 0x49, 0x49, 0x36, 0x00,	// 8
0x06, 0x49, 0x49, 0x29, 0x1E, 0x00,	// 9
0x00, 0x36, 0x36, 0x00, 0x00, 0x00,	// :
0x00, 0x56, 0x36, 0x00, 0x00, 0x00,	// ;
0x00, 0x08, 0x14, 0x22, 0x41, 0x00,	// <
0x14, 0x14, 0x14, 0x14, 0x14, 0x00,	// =
0x41, 0x22, 0x14, 0x08, 0x00, 0x00,	// >
0x02, 0x01, 0x51, 0x09, 0x06, 0x00,	// ?
0x32, 0x49, 0x79, 0x41, 0x3E, 0x00,	// @
0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00,	// A
0x7F, 0x49, 0x49, 0x49, 0x36, 0x00,	// B
0x3E, 0x41, 0x41, 0x41, 0x22, 0x00,	// C
0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00,	// D
0x7F, 0x49, 0x49, 0x49, 0x41, 0x00,	// E
0x7F, 0x09, 0x09, 0x01, 0x01, 0x00,	// F
0x3E, 0x41, 0x41, 0x51, 0x32, 0x00,	// G
0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00,	// H
0x00, 0x41, 0x7F, 0x41, 0x00, 0x00,	// I
0x20, 0x40, 0x41, 0x3F, 0x01, 0x00,	// J
0x7F, 0x08, 0x14, 0x22, 0x41, 0x00,	// K
0x7F, 0x40, 0x40, 0x40, 0x40, 0x00,	// L
0x7F, 0x02, 0x04, 0x02, 0x7F, 0x00,	// M
0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00,	// N
0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00,	// O
0x7F, 0x09, 0x09, 0x09, 0x06, 0x00,	// P
0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00,	// Q
0x7F, 0x09, 0x19, 0x29, 0x46, 0x00,	// R
0x46, 0x49, 0x49, 0x49, 0x31, 0x00,	// S
0x01, 0x01, 0x7F, 0x01, 0x01, 0x00,	// T
0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00,	// U
0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00,	// V
0x7F, 0x20, 0x18, 0x20, 0x7F, 0x00,	// W
0x63, 0x14, 0x08, 0x14, 0x63, 0x00,	// X
0x03, 0x04, 0x78, 0x04, 0x03, 0x00,	// Y
0x61, 0x51, 0x49, 0x45, 0x43, 0x00,	// Z
0x00, 0x00, 0x7F, 0x41, 0x41, 0x00,	// [
0x02, 0x04, 0x08, 0x10, 0x20, 0x00,	// "\"
0x41, 0x41, 0x7F, 0x00, 0x00, 0x00,	// ]
0x04, 0x02, 0x01, 0x02, 0x04, 0x00,	// ^
0x40, 0x40, 0x40, 0x40, 0x40, 0x00,	// _
0x00, 0x01, 0x02, 0x04, 0x00, 0x00,	// `
0x20, 0x54, 0x54, 0x54, 0x78, 0x00,	// a
0x7F, 0x48, 0x44, 0x44, 0x38, 0x00,	// b
0x38, 0x44, 0x44, 0x44, 0x20, 0x00,	// c
0x38, 0x44, 0x44, 0x48, 0x7F, 0x00,	// d
0x38, 0x54, 0x54, 0x54, 0x18, 0x00,	// e
0x08, 0x7E, 0x09, 0x01, 0x02, 0x00,	// f
0x08, 0x14, 0x54, 0x54, 0x3C, 0x00,	// g
0x7F, 0x08, 0x04, 0x04, 0x78, 0x00,	// h
0x00, 0x44, 0x7D, 0x40, 0x00, 0x00,	// i
0x20, 0x40, 0x44, 0x3D, 0x00, 0x00,	// j
0x00, 0x7F, 0x10, 0x28, 0x44, 0x00,	// k
0x00, 0x41, 0x7F, 0x40, 0x00, 0x00,	// l
0x7C, 0x04, 0x18, 0x04, 0x78, 0x00,	// m
0x7C, 0x08, 0x04, 0x04, 0x78, 0x00,	// n
0x38, 0x44, 0x44, 0x44, 0x38, 0x00,	// o
0x7C, 0x14, 0x14, 0x14, 0x08, 0x00,	// p
0x08, 0x14, 0x14, 0x18, 0x7C, 0x00,	// q
0x7C, 0x08, 0x04, 0x04, 0x08, 0x00,	// r
0x48, 0x54, 0x54, 0x54, 0x20, 0x00,	// s
0x04, 0x3F, 0x44, 0x40, 0x20, 0x00,	// t
0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00,	// u
0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00,	// v
0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00,	// w
0x44, 0x28, 0x10, 0x28, 0x44, 0x00,	// x
0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00,	// y
0x44, 0x64, 0x54, 0x4C, 0x44, 0x00,	// z
0x00, 0x08, 0x36, 0x41, 0x00, 0x00,	// {
0x00, 0x00, 0x7F, 0x00, 0x00, 0x00,	// |
0x00, 0x41, 0x36, 0x08, 0x00, 0x00,	// }
0x00, 0x01, 0x02, 0x00, 0x00, 0x00	// ` index 94
};

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_Basic_DrawPoint								**/
/** Purpose:		Set a pixel color or draw a point.					**/
/** Params:																**/
/**	@uiCoordinateX[in]:	X coordinate of point by pixels.				**/
/**	@uiCoordinateY[in]:	Y coordinate of point by pixels.				**/
/**	@eColor[in]:		Point color, GUI_COLOR_BKGCLR means clear pix, 	**/
/**						GUI_COLOR_FRGCLR means set pix.					**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_Basic_DrawPoint(SGUI_UINT uiCoordinateX, SGUI_UINT uiCoordinateY, SGUI_COLOR eColor)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((uiCoordinateX < LCD_SIZE_WIDTH) && (uiCoordinateY < LCD_SIZE_HEIGHT))
	{
		if(SGUI_COLOR_FRGCLR == eColor)
		{
#if (_SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_ > 0)
			VDIF_SetPixel(uiCoordinateX, uiCoordinateY, 1);
#else
			// Call draw pix interface here.
			OLED_SetPixel(uiCoordinateX, uiCoordinateY, OLED_COLOR_FRG);
#endif //_SIMPLE_GUI_ENABLE_SIMULATOR_
		}
		else if(SGUI_COLOR_BKGCLR == eColor)
		{
#if (_SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_ > 0)
			VDIF_SetPixel(uiCoordinateX, uiCoordinateY, 0);
#else
			// Call draw pix interface here.
			OLED_SetPixel(uiCoordinateX, uiCoordinateY, OLED_COLOR_BKG);
#endif //_SIMPLE_GUI_ENABLE_SIMULATOR_
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Basic_GetPoint									**/
/** Purpose:		Get a pixel color .									**/
/** Params:																**/
/**	@uiCoordinateX[in]:	X coordinate of point by pixels.				**/
/**	@uiCoordinateY[in]:	Y coordinate of point by pixels.				**/
/** Return:			SGUI_COLOR type enumerated for point color.			**/
/** Notice:			None.												**/
/*************************************************************************/
SGUI_COLOR SGUI_Basic_GetPoint(SGUI_UINT uiCoordinateX, SGUI_UINT uiCoordinateY)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_COLOR					eColor;
	SGUI_UINT					uiPixValue;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eColor =					SGUI_COLOR_BKGCLR;
	uiPixValue =				0;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((uiCoordinateX < LCD_SIZE_WIDTH) && (uiCoordinateY < LCD_SIZE_HEIGHT))
	{
#if (_SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_ > 0)
		uiPixValue = VDIF_GetPixel(uiCoordinateX, uiCoordinateY);
#else
		// Call read pix interface here.
		uiPixValue = OLED_GetPixel(uiCoordinateX, uiCoordinateY);
#endif //_SIMPLE_GUI_ENABLE_SIMULATOR_
		if(0 == uiPixValue)
		{
			eColor = SGUI_COLOR_BKGCLR;
		}
		else
		{
			eColor = SGUI_COLOR_FRGCLR;
		}
	}

	return eColor;
}

/*************************************************************************/
/** Function Name:	SGUI_Basic_ClearScreen								**/
/** Purpose:		Clean LCD screen display.							**/
/** Params:			None.												**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_Basic_ClearScreen(void)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
#if (_SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_ > 0)
	VDIF_ClearDisplay();
#else
	// Call clear screen function here;
	OLED_ClearDisplay();
#endif //_SIMPLE_GUI_ENABLE_SIMULATOR_
}

/*************************************************************************/
/** Function Name:	SGUI_Basic_DrawLine									**/
/** Purpose:		Draw a line by the Bresenham algorithm.				**/
/** Params:																**/
/**	@uiStartX[in]:		X coordinate of start point of line.			**/
/**	@uiStartY[in]:		Y coordinate of start point of line.			**/
/**	@uiEndX[in]:		X coordinate of end point of line.				**/
/**	@uiEndY[in]:		Y coordinate of end point of line.				**/
/**	@eColor[in]:		Line color.										**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_Basic_DrawLine(SGUI_INT uiStartX, SGUI_INT uiStartY, SGUI_INT uiEndX, SGUI_INT uiEndY, SGUI_COLOR eColor)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_INT					iDx, iDy;
	SGUI_INT					iIncX, iIncY;
	SGUI_INT					iErrX = 0, iErrY = 0;
	SGUI_INT					i, uiDs;
	SGUI_INT					uiCurrentPosX, uiCurrentPosY;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	iErrX = 0;
	iErrY = 0;
    iDx = uiEndX - uiStartX;
    iDy = uiEndY - uiStartY;
	uiCurrentPosX = uiStartX;
	uiCurrentPosY = uiStartY;

	if(iDx > 0)
	{
		iIncX = 1;
	}
	else
	{
		if(iDx == 0)
		{
			iIncX = 0;
		}
		else
		{
			iIncX = -1;
			iDx = -iDx;
		}
	}

	if(iDy > 0)
	{
		iIncY = 1;
	}
	else
	{
		if(iDy == 0)
		{
			iIncY = 0;
		}
		else
		{
			iIncY = -1;
			iDy = -iDy;
		}
	}

	if(iDx > iDy)
	{
		uiDs = iDx;
	}
	else
	{
		uiDs = iDy;
	}

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	for(i = 0; i <= uiDs+1; i++)
	{
		SGUI_Basic_DrawPoint(uiCurrentPosX,uiCurrentPosY, eColor);
		iErrX += iDx;
		if(iErrX > uiDs)
		{
			iErrX -= uiDs;
			uiCurrentPosX += iIncX;
		}
		iErrY += iDy;
		if(iErrY > uiDs)
		{
			iErrY -= uiDs;
			uiCurrentPosY += iIncY;
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Basic_DrawRectangle							**/
/** Purpose:		Draw a rectangle on screen. 						**/
/** Params:																**/
/**	@uiStartX[in]:		X coordinate of the upper-left corner.			**/
/**	@uiStartY[in]:		Y coordinate of the upper-left corner.			**/
/**	@uiWidth[in]: .		Width of rectangle.								**/
/**	@uiHeight[in]:		Height of rectangle.							**/
/**	@eEdgeColor[in]:	Edge color.										**/
/**	@eFillColor[in]:	Fill color.										**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_Basic_DrawRectangle(SGUI_UINT uiStartX, SGUI_UINT uiStartY, SGUI_UINT uiWidth, SGUI_UINT uiHeight, SGUI_COLOR eEdgeColor, SGUI_COLOR eFillColor)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_UINT					uiColumnIndex;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((uiWidth > 0) && (uiHeight > 0))
	{
		if((uiWidth == 1) && (uiHeight == 1))
		{
			SGUI_Basic_DrawPoint(uiStartX, uiStartY, eEdgeColor);
		}
		else if(uiWidth == 1)
		{
			SGUI_Basic_DrawLine(uiStartX, uiStartY, uiStartX, uiStartY+uiHeight-1, eEdgeColor);
		}
		else if(uiHeight == 1)
		{
			SGUI_Basic_DrawLine(uiStartX, uiStartY, uiStartX+uiWidth-1, uiStartY, eEdgeColor);
		}
		else
		{
			// Draw edge.
			// Check and set changed page and column index is in edge display action.
			SGUI_Basic_DrawLine(uiStartX, uiStartY, uiStartX, uiStartY+uiHeight-1, eEdgeColor);
			SGUI_Basic_DrawLine(uiStartX+uiWidth-1, uiStartY, uiStartX+uiWidth-1, uiStartY+uiHeight-1, eEdgeColor);
			SGUI_Basic_DrawLine(uiStartX, uiStartY, uiStartX+uiWidth-1, uiStartY, eEdgeColor);
			SGUI_Basic_DrawLine(uiStartX, uiStartY+uiHeight-1, uiStartX+uiWidth-1, uiStartY+uiHeight-1, eEdgeColor);
			// Fill area.
			if((eFillColor != SGUI_COLOR_TRANS) && (uiWidth > 2) && (uiHeight > 2))
			{
				for(uiColumnIndex=(uiStartX+1); uiColumnIndex<(uiStartX+uiWidth-1); uiColumnIndex++)
				{
					SGUI_Basic_DrawLine(uiColumnIndex, uiStartY+1, uiColumnIndex, uiStartY+uiHeight-2, eFillColor);
				}
			}
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Basic_DrawCircle								**/
/** Purpose:		Draw a circle by center coordinate and radius.		**/
/** Params:																**/
/**	@uiCx[in]:			Circle center X coordinate.						**/
/**	@uiCy[in]:			Circle center Y coordinate.						**/
/**	@uiRadius[in]:		Circle radius.									**/
/**	@eEdgeColor[in]:	Edge color.										**/
/**	@eFillColor[in]:	Fill color.										**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_Basic_DrawCircle(SGUI_UINT uiCx, SGUI_UINT uiCy, SGUI_UINT uiRadius, SGUI_COLOR eEdgeColor, SGUI_COLOR eFillColor)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_UINT					uiPosXOffset, uiPosYOffset;
	SGUI_UINT					uiPosXOffset_Old, uiPosYOffset_Old;
	SGUI_INT					iXChange, iYChange, iRadiusError;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	uiPosXOffset				= uiRadius;
	uiPosYOffset				= 0;
	uiPosXOffset_Old			= 0xFFFF;
	uiPosYOffset_Old			= 0xFFFF;
	iXChange					= 1 - 2 * uiRadius;
	iYChange					= 1;
	iRadiusError				= 0;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(uiRadius < 1)
	{
		SGUI_Basic_DrawPoint(uiCx, uiCy, eEdgeColor);
	}
	else
	{
		while(uiPosXOffset >= uiPosYOffset)
		{
			if((uiPosXOffset_Old != uiPosXOffset) || (uiPosYOffset_Old != uiPosYOffset) )
			{
				// Fill the circle
				if((uiRadius > 1) && (eFillColor != SGUI_COLOR_TRANS) && (uiPosXOffset_Old != uiPosXOffset))
				{

					SGUI_Basic_DrawLine(uiCx-uiPosXOffset, uiCy-uiPosYOffset+1, uiCx-uiPosXOffset, uiCy+uiPosYOffset-1, eFillColor);
					SGUI_Basic_DrawLine(uiCx+uiPosXOffset, uiCy-uiPosYOffset+1, uiCx+uiPosXOffset, uiCy+uiPosYOffset-1, eFillColor);
					uiPosXOffset_Old = uiPosXOffset;
				}
				SGUI_Basic_DrawLine(uiCx-uiPosYOffset, uiCy-uiPosXOffset+1, uiCx-uiPosYOffset, uiCy+uiPosXOffset-1, eFillColor);
				SGUI_Basic_DrawLine(uiCx+uiPosYOffset, uiCy-uiPosXOffset+1, uiCx+uiPosYOffset, uiCy+uiPosXOffset-1, eFillColor);
				uiPosYOffset_Old = uiPosYOffset;

				// Draw edge.
				SGUI_Basic_DrawPoint(uiCx+uiPosXOffset, uiCy+uiPosYOffset, eEdgeColor);
				SGUI_Basic_DrawPoint(uiCx-uiPosXOffset, uiCy+uiPosYOffset, eEdgeColor);
				SGUI_Basic_DrawPoint(uiCx-uiPosXOffset, uiCy-uiPosYOffset, eEdgeColor);
				SGUI_Basic_DrawPoint(uiCx+uiPosXOffset, uiCy-uiPosYOffset, eEdgeColor);
				SGUI_Basic_DrawPoint(uiCx+uiPosYOffset, uiCy+uiPosXOffset, eEdgeColor);
				SGUI_Basic_DrawPoint(uiCx-uiPosYOffset, uiCy+uiPosXOffset, eEdgeColor);
				SGUI_Basic_DrawPoint(uiCx-uiPosYOffset, uiCy-uiPosXOffset, eEdgeColor);
				SGUI_Basic_DrawPoint(uiCx+uiPosYOffset, uiCy-uiPosXOffset, eEdgeColor);
			}
			uiPosYOffset++;
			iRadiusError += iYChange;
			iYChange += 2;
			if ((2 * iRadiusError + iXChange) > 0)
			{
				uiPosXOffset--;
				iRadiusError += iXChange;
				iXChange += 2;
			}
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Basic_ReverseBlockColor						**/
/** Purpose:		Reverse all pixel color in a rectangle area.		**/
/** Params:																**/
/**	@uiStartX[in]:		X coordinate of the upper-left corner.			**/
/**	@uiStartY[in]:		Y coordinate of the upper-left corner.			**/
/**	@uiWidth[in]: .		Width of rectangle.								**/
/**	@uiHeight[in]:		Height of rectangle.							**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_Basic_ReverseBlockColor(SGUI_UINT uiStartX, SGUI_UINT uiStartY, SGUI_UINT uiWidth, SGUI_UINT uiHeight)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_UINT					i_W, i_H;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	for(i_W=0; i_W<uiWidth; i_W++)
	{
        for(i_H=0; i_H<uiHeight; i_H++)
		{
			if(SGUI_Basic_GetPoint(uiStartX+i_W, uiStartY+i_H) == SGUI_COLOR_FRGCLR)
			{
                SGUI_Basic_DrawPoint(uiStartX+i_W, uiStartY+i_H, SGUI_COLOR_BKGCLR);
			}
			else
			{
				SGUI_Basic_DrawPoint(uiStartX+i_W, uiStartY+i_H, SGUI_COLOR_FRGCLR);
			}
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Basic_DrawBitMap								**/
/** Purpose:		Draw a rectangular area bit map on LCD screen.		**/
/** Params:																**/
/**	@pstDisplayArea[in]:Display area position and size.					**/
/**	@pstDataArea[in]:	Data area size and display offset.				**/
/**	@pDataBuffer[in]:	Bit map data buffer.							**/
/**	@eDrawMode[in]		Bit map display mode(normal or reverse color).	**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_Basic_DrawBitMap(SGUI_RECT_AREA* pstDisplayArea, SGUI_RECT_AREA* pstDataArea, SGUI_BYTE* pDataBuffer, SGUI_DRAW_MODE eDrawMode)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_INT					iDrawPixX, iDrawPixY;
	SGUI_INT					iBmpPixX, iBmpPixY;
	SGUI_UINT					uiDrawnWidthIndex, uiDrawnHeightIndex;
	SGUI_UINT					uiPixIndex;
	SGUI_BYTE*					pData;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	uiDrawnWidthIndex			= 0;
	uiDrawnHeightIndex			= 0;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Only draw in visible area of screen.
	if(	(RECTANGLE_X_START(*pstDisplayArea) < LCD_SIZE_WIDTH) && (RECTANGLE_Y_START(*pstDisplayArea) < LCD_SIZE_HEIGHT) &&
		(RECTANGLE_X_END(*pstDisplayArea) > 0) && (RECTANGLE_Y_END(*pstDisplayArea) > 0))
	{
		// Recalculate display area and data area.
		if(RECTANGLE_X_START(*pstDisplayArea) < 0)
		{
			RECTANGLE_X_START(*pstDataArea) += RECTANGLE_X_START(*pstDisplayArea);
			RECTANGLE_WIDTH(*pstDisplayArea) += RECTANGLE_X_START(*pstDisplayArea);
			RECTANGLE_X_START(*pstDisplayArea) = 0;
		}
		if(RECTANGLE_Y_START(*pstDisplayArea) < 0)
		{
			RECTANGLE_Y_START(*pstDataArea) += RECTANGLE_Y_START(*pstDisplayArea);
			RECTANGLE_HEIGHT(*pstDisplayArea) += RECTANGLE_Y_START(*pstDisplayArea);
			RECTANGLE_Y_START(*pstDisplayArea) = 0;
		}
		// Only process drawing when valid display data existed
		if((RECTANGLE_VALID_WIDTH(*pstDataArea) > 0) && (RECTANGLE_VALID_HEIGHT(*pstDataArea) > 0))
		{
			// Set loop start parameter of x coordinate
			iDrawPixX = RECTANGLE_X_START(*pstDisplayArea);
			iBmpPixX = 0;
			if(RECTANGLE_X_START(*pstDataArea) > 0)
			{
				iDrawPixX += RECTANGLE_X_START(*pstDataArea);
			}
			else
			{
				iBmpPixX -= RECTANGLE_X_START(*pstDataArea);
			}
			uiDrawnWidthIndex = iBmpPixX;
			// Loop for x coordinate;
			while((uiDrawnWidthIndex<RECTANGLE_WIDTH(*pstDataArea)) && (iDrawPixX<=RECTANGLE_X_END(*pstDisplayArea)) && (iDrawPixX<LCD_SIZE_WIDTH))
			{
				// Redirect to data array for column.
				pData = pDataBuffer + iBmpPixX;
				// Set loop start parameter of y coordinate
				iDrawPixY = RECTANGLE_Y_START(*pstDisplayArea);
				iBmpPixY = 0;
				if(RECTANGLE_Y_START(*pstDataArea) > 0)
				{
					iDrawPixY += RECTANGLE_Y_START(*pstDataArea);
				}
				else
				{
					iBmpPixY -= RECTANGLE_Y_START(*pstDataArea);
				}
				uiDrawnHeightIndex = iBmpPixY;
				uiPixIndex = iBmpPixY % 8;
				pData += (iBmpPixY / 8) * RECTANGLE_WIDTH(*pstDataArea);
				// Loop for y coordinate;
				while((uiDrawnHeightIndex<RECTANGLE_HEIGHT(*pstDataArea)) && (iDrawPixY<=RECTANGLE_Y_END(*pstDisplayArea)) && (iDrawPixY<LCD_SIZE_HEIGHT))
				{
					if(uiPixIndex == 8)
					{
						uiPixIndex = 0;
						pData += RECTANGLE_WIDTH(*pstDataArea);
					}
					if(GET_PAGE_BIT(*pData, uiPixIndex) != eDrawMode)
					{
						SGUI_Basic_DrawPoint(iDrawPixX, iDrawPixY, SGUI_COLOR_FRGCLR);
					}
					else
					{
						SGUI_Basic_DrawPoint(iDrawPixX, iDrawPixY, SGUI_COLOR_BKGCLR);
					}
					uiDrawnHeightIndex ++;
					uiPixIndex ++;
					iDrawPixY ++;
					iBmpPixY ++;
				}
				uiDrawnWidthIndex ++;
				iDrawPixX ++;
				iBmpPixX ++;
			}
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_Basic_RefreshDisplay							**/
/** Purpose:		Sync display buffer to screen.                      **/
/** Params:			None.                                               **/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_Basic_RefreshDisplay(void)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
#if (_SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_ > 0)
	VDIF_RefreshDisplay();
#else
	// Call clear screen function here;
	OLED_RefreshScreen();
#endif //_SIMPLE_GUI_ENABLE_SIMULATOR_
}
