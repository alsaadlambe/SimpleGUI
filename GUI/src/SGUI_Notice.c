/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_Notice.c												**/
/** Author: XuYulin														**/
/** Version: 1.0.0.0													**/
/** Description: Notice box display interface.							**/
/*************************************************************************/

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"
#include "SGUI_Text.h"
#include "SGUI_Notice.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
// User settings
#define NOTICE_FONT_SIZE				(SGUI_FONT_SIZE_H12)
#define NOTICE_ICON_SIZE				(16)
#define NOTICE_BOX_EDGE_DIST			(6)
#define NOTICE_BOX_MARGIN				(2)

// Automatic calculation
#define NOTICE_BOX_WIDTH				(LCD_SIZE_WIDTH-NOTICE_BOX_EDGE_DIST*2)
#define NOTICE_BOX_HEIGHT_MIN			(g_stFontSize[NOTICE_FONT_SIZE].Height*2+NOTICE_BOX_MARGIN*2)
#define NOTICE_BOX_HEIGHT_MAX			(LCD_SIZE_HEIGHT-NOTICE_BOX_EDGE_DIST*2)
#define NOTICE_BOX_HEIGHT(LINES)		(LINES*g_stFontSize[NOTICE_FONT_SIZE].Height+NOTICE_BOX_MARGIN*2)
#define NOTICE_BOX_POSX					(NOTICE_BOX_EDGE_DIST)
#define NOTICE_BOX_POSY(HEIGHT)			((LCD_SIZE_HEIGHT-HEIGHT)/2)
#define NOTICE_TEXT_AREA_WIDTH_NOICON	(NOTICE_BOX_WIDTH-NOTICE_BOX_MARGIN*4)
#define	NOTICE_TEXT_AREA_WIDTH			(NOTICE_TEXT_AREA_WIDTH_NOICON-NOTICE_ICON_SIZE-NOTICE_BOX_MARGIN*2)
#define NOTICE_TEXT_AREA_HEIGHT(LINES)	(NOTICE_BOX_HEIGHT(LINES)-NOTICE_BOX_MARGIN*2)
#define NOTICE_TEXT_AREA_HEIGHT_MIN		(g_stFontSize[NOTICE_FONT_SIZE].Height*2)
#define NOTICE_TEXT_AREA_HEIGHT_MAX		(NOTICE_BOX_HEIGHT_MAX-NOTICE_BOX_MARGIN*2)
#define NOTICE_TEXT_POSX_NOICON			(NOTICE_BOX_POSX+NOTICE_BOX_MARGIN*2)
#define NOTICE_TEXT_POSX				(NOTICE_TEXT_POSX_NOICON+NOTICE_ICON_SIZE+NOTICE_BOX_MARGIN*2)
#define NOTICE_TEXT_POSY(HEIGHT)		(NOTICE_BOX_POSY(HEIGHT)+NOTICE_BOX_MARGIN)
#define	NOTICE_TEXT_LINES_MAX			(NOTICE_TEXT_AREA_WIDTH/g_stFontSize[NOTICE_FONT_SIZE].Width)
#define	NOTICE_TEXT_LINES_MAX_NOICON	(NOTICE_TEXT_AREA_WIDTH_NOICON/g_stFontSize[NOTICE_FONT_SIZE].Width)

//=======================================================================//
//= Static variable declaration.									    =//
//=======================================================================//
#if NOTICE_ICON_SIZE == 16
static const uint8_t ICON_DATA_ERROR[] = {
0xFF,0x01,0x01,0x19,0x39,0x71,0xE1,0xC1,0xC1,0xE1,0x71,0x39,0x19,0x01,0x01,0xFF,
0xFF,0x80,0x80,0x98,0x9C,0x8E,0x87,0x83,0x83,0x87,0x8E,0x9C,0x98,0x80,0x80,0xFF
};

static const uint8_t ICON_DATA_INFORMATION[] = {
0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0xED,0xED,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,
0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0xBF,0xBF,0x80,0x80,0x80,0x80,0x80,0x80,0xFF
};

static const uint8_t ICON_DATA_QUESTION[] = {
0xFF,0x01,0x01,0x01,0x31,0x39,0x1D,0x0D,0x0D,0x9D,0xF9,0xF1,0x01,0x01,0x01,0xFF,
0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0xB6,0xB7,0x83,0x81,0x80,0x80,0x80,0x80,0xFF
};

static const uint8_t ICON_DATA_WARNING[] = {
0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0xFD,0xFD,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,
0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0xB7,0xB7,0x80,0x80,0x80,0x80,0x80,0x80,0xFF
};
#elif NOTICE_ICON_SIZE == 24
static const uint8_t ICON_DATA_ERROR[] = {
0xFF,0x01,0x01,0x01,0x71,0xF1,0xF1,0xE1,0xC1,0x81,0x01,0x01,0x01,0x01,0x81,0xC1,0xE1,0xF1,0xF1,0x71,0x01,0x01,0x01,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x81,0xC3,0xE7,0xFF,0xFF,0x7E,0x7E,0xFF,0xFF,0xE7,0xC3,0x81,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x80,0x80,0x80,0x8E,0x8F,0x8F,0x87,0x83,0x81,0x80,0x80,0x80,0x80,0x81,0x83,0x87,0x8F,0x8F,0x8E,0x80,0x80,0x80,0xFF
};

static const uint8_t ICON_DATA_INFORMATION[] = {
0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x79,0x79,0x79,0x79,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x98,0x9F,0x9F,0x9F,0x9F,0x98,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xFF
};

static const uint8_t ICON_DATA_QUESTION[] = {
0xFF,0x01,0x01,0x01,0x01,0x01,0xC1,0xE1,0xF1,0xF1,0x79,0x79,0x79,0x79,0xF1,0xF1,0xE1,0xC1,0x81,0x01,0x01,0x01,0x01,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x03,0x03,0x03,0x03,0x00,0xC0,0xE0,0xF0,0xF8,0x7C,0x3F,0x1F,0x0F,0x07,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x9E,0x9E,0x9E,0x9E,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xFF
};

static const uint8_t ICON_DATA_WARNING[] = {
0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xF1,0xF9,0xF9,0xF1,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x9E,0x9E,0x9E,0x9E,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xFF
};
#endif // NOTICE_ICON_SIZE

const uint8_t* g_arrNoticeIcon[] = {
	NULL,
	(const uint8_t*)ICON_DATA_ERROR,
	(const uint8_t*)ICON_DATA_INFORMATION,
	(const uint8_t*)ICON_DATA_QUESTION,
	(const uint8_t*)ICON_DATA_WARNING
};

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_Notice_RefreshNotice							**/
/** Purpose:		Show a notice box.									**/
/** Params:																**/
/**	@szNoticeText[in]:	Notice text.									**/
/**	@uiTextOffset[in]:	Text top offset.								**/
/**	@eIcon[in]:			Notice icon index.								**/
/** Return:			Remaining text height display.						**/
/** Notice:			None.												**/
/*************************************************************************/
SGUI_SIZE SGUI_Notice_RefreshNotice(SGUI_PCSZSTR szNoticeText, SGUI_INT uiTextOffset, SGUI_NOTICE_ICON eIcon)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_SIZE					uiLineCount;
	SGUI_SIZE					uiNoticeBoxHeight;
	SGUI_SIZE					uiTextLines;
	SGUI_RECT_AREA				stTextDisplayArea;
	SGUI_RECT_AREA				stIconDisplayArea, stIconDataArea;
	SGUI_PCSZSTR				pszNoticeTextPtr;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Encode string if defined.
	pszNoticeTextPtr = szNoticeText;
	// Get max line of notice text.
	if(SGUI_ICON_NONE != eIcon)
	{
		uiLineCount = SGUI_Text_GetMultiLineTextLines(pszNoticeTextPtr, NOTICE_TEXT_LINES_MAX);
	}
	else
	{
		uiLineCount = SGUI_Text_GetMultiLineTextLines(pszNoticeTextPtr, NOTICE_TEXT_LINES_MAX_NOICON);
	}
	if(uiLineCount < 2)
	{
		uiLineCount = 2;
	}
	uiNoticeBoxHeight = NOTICE_BOX_HEIGHT(uiLineCount);
	if(uiNoticeBoxHeight > NOTICE_BOX_HEIGHT_MAX)
	{
		uiNoticeBoxHeight = NOTICE_BOX_HEIGHT_MAX;
	}
	// Draw edge
    SGUI_Basic_DrawRectangle(NOTICE_BOX_POSX, NOTICE_BOX_POSY(uiNoticeBoxHeight), NOTICE_BOX_WIDTH, uiNoticeBoxHeight, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
    // Draw icon if exists.
    if(SGUI_ICON_NONE != eIcon)
	{
		stIconDisplayArea.PosX = NOTICE_BOX_POSX+NOTICE_BOX_MARGIN;
		stIconDisplayArea.PosY = NOTICE_BOX_POSY(uiNoticeBoxHeight)+NOTICE_BOX_MARGIN;
		stIconDisplayArea.Width = NOTICE_ICON_SIZE;
		stIconDisplayArea.Height = NOTICE_ICON_SIZE;
		stIconDataArea.PosX = 0;
		stIconDataArea.PosY = 0;
		stIconDataArea.Width = NOTICE_ICON_SIZE;
		stIconDataArea.Height = NOTICE_ICON_SIZE;
		SGUI_Basic_DrawBitMap(&stIconDisplayArea, &stIconDataArea, (SGUI_BYTE*)g_arrNoticeIcon[eIcon], SGUI_DRAW_NORMAL);
	}
    // Draw text;
    if(SGUI_ICON_NONE != eIcon)
	{
		stTextDisplayArea.PosX = NOTICE_TEXT_POSX;
		stTextDisplayArea.Width = NOTICE_TEXT_AREA_WIDTH;
	}
	else
	{
		stTextDisplayArea.PosX = NOTICE_TEXT_POSX_NOICON;
		stTextDisplayArea.Width = NOTICE_TEXT_AREA_WIDTH_NOICON;;
	}
	stTextDisplayArea.PosY = NOTICE_TEXT_POSY(uiNoticeBoxHeight);
	stTextDisplayArea.Height = NOTICE_TEXT_AREA_HEIGHT(uiLineCount);
    if(stTextDisplayArea.Height > NOTICE_TEXT_AREA_HEIGHT_MAX)
	{
		stTextDisplayArea.Height = NOTICE_TEXT_AREA_HEIGHT_MAX;
	}

    uiTextLines = SGUI_Text_DrawMultipleLinesText(pszNoticeTextPtr, NOTICE_FONT_SIZE, &stTextDisplayArea, uiTextOffset, SGUI_DRAW_NORMAL);

    return uiTextLines;
}




