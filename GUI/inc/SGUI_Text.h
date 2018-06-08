#ifndef _INCLUDE_GUI_FONT_H_
#define _INCLUDE_GUI_FONT_H_
//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define FONT_LIB_ADDR					0X00000
#define FONT_LIB_OFFSET_ASCII			(-32)
#define FONT_LIB_OFFSET_GB2312_SYMBOL	(95)
#define FONT_LIB_OFFSET_GB2312_CHARL1	(659)
// Max font size is 64 pix, maximum of font data size is 64 * 8 = 512 Bytes.
#define FONT_DATA_BUFFER_SIZE			(512)
#define TEXT_NUMBER_STR_LENGTH_MAX		(12)
#define TEXT_PLACEHOLDER_CHARACTER		('*')

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef enum
{
	SGUI_FONT_MODULE_SOURCE_BASIC = 0,
	SGUI_FONT_MODULE_SOURCE_NORMAL,
}SGUI_FONT_MODULE_SOURCE;

typedef struct
{
	const SGUI_UINT16	Size;
	const SGUI_UINT16	Width;
	const SGUI_UINT16	Height;
}SGUI_FONT_SIZE_STRUCT;

typedef enum
{
	SGUI_FONT_SIZE_H6 = 0,
	SGUI_FONT_SIZE_H8,
	SGUI_FONT_SIZE_H12,
	SGUI_FONT_SIZE_H16,
	SGUI_FONT_SIZE_H24,
	SGUI_FONT_SIZE_H32,
	SGUI_FONT_SIZE_MAX,
}SGUI_FONT_SIZE;

//=======================================================================//
//= Public variable declaration.									    =//
//=======================================================================//
extern const SGUI_FONT_SIZE_STRUCT g_stFontSize[];

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
void			SGUI_Text_DrawSingleLineText(SGUI_PCSZSTR szTextBuffer, SGUI_FONT_SIZE eFontSize, SGUI_RECT_AREA* pstDisplayArea, SGUI_RECT_AREA* pstStringDataArea, SGUI_DRAW_MODE eFontMode);
SGUI_SIZE		SGUI_Text_DrawMultipleLinesText(SGUI_PCSZSTR szTextBuffer, SGUI_FONT_SIZE eFontSize, SGUI_RECT_AREA* pstDisplayArea, SGUI_INT iTopOffset, SGUI_DRAW_MODE eFontMode);
SGUI_SIZE		SGUI_Text_GetTextGraphicsWidth(SGUI_PCSZSTR szText, SGUI_FONT_SIZE eFontSize);
SGUI_SIZE		SGUI_Text_GetMultiLineTextLines(SGUI_PCSZSTR szNoticeText, SGUI_SIZE uiHalfWidthCharInLine);

#endif
