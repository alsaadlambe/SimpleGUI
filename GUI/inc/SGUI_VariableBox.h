#ifndef __INCLUDE_GUI_VARIABLEBOX_H__
#define __INCLUDE_GUI_VARIABLEBOX_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"
#include "SGUI_Text.h"

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef struct
{
	SGUI_SIZE		PosX;
	SGUI_SIZE		PosY;
	SGUI_SIZE		Width;
	SGUI_FONT_SIZE	FontSize;
	SGUI_INT		Min;
	SGUI_INT		Max;
	SGUI_INT		Value;
}SGUI_INT_VARBOX_STRUCT;

typedef struct
{
	SGUI_SIZE		PosX;
	SGUI_SIZE		PosY;
	SGUI_SIZE		Width;
	SGUI_FONT_SIZE	FontSize;
	SGUI_SIZE		FocusIndex;
	SGUI_SIZE		MaxTextLength;
	SGUI_PSZSTR		Value;
}SGUI_TEXT_VARBOX_STRUCT;

typedef enum
{
	SGUI_RIGHT = 0,
	SGUI_CENTER,
	SGUI_LEFT,
}SGUI_VARBOX_ALIG;

enum
{
	SGUI_TEXT_NUMBER		= 0x0100,
	SGUI_TEXT_ALPHA			= 0x0200,
	SGUI_TEXT_PUNCTUATION	= 0x0400,
	SGUI_TEXT_DATE			= 0x0800,
	SGUI_TEXT_TIME			= 0x1000,
};

typedef enum
{
    SGUI_TXT_VARBOX_OPT_PREV = -1,
    SGUI_TXT_VARBOX_OPT_NONE = 0,
    SGUI_TXT_VARBOX_OPT_NEXT = 1,
}SGUI_TEXT_VARBOX_OPT;

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define VARBOX_WIDTH(FONT_SIZE, L)		(L*(g_stFontSize[FONT_SIZE].Width)-4)
#define VARBOX_HEIGHT(FONT_SIZE)		(g_stFontSize[FONT_SIZE].Height+2)
#define VARBOX_TEXT_AREA_WIDTH(W)		((W>2)?(W-2):0)
#define VARBOX_TEXT_AREA_HEIGHT(FONT_SIZE) (g_stFontSize[FONT_SIZE].Height)

#define VARBOX_TEXT_LENGTH_MAX			(32)
#define VARBOX_TEXT_BUFFER_SIZE			(VARBOX_TEXT_LENGTH_MAX+1)
#define VARBOX_TEXT_WIDTH(FONT_SIZE, L)	(L*(g_stFontSize[FONT_SIZE].Width))

#define GUI_TEXT_ASCII					(SGUI_TEXT_NUMBER|SGUI_TEXT_ALPHA|SGUI_TEXT_PUNCTUATION)

#define GUI_TEXT_ISDIGIT(C)				((C>='0' && C<='9')?true:false)
#define GUI_TEXT_ISUPPER(C)				((C>='A' && C<='Z')?true:false)
#define GUI_TEXT_ISLOWER(C)				((C>='a' && C<='z')?true:false)
#define GUI_TEXT_ISALPHA(C)				(GUI_TEXT_ISUPPER(C) || GUI_TEXT_ISLOWER(C))
#define GUI_TEXT_ISASCII(C)				((C>=0x20 && C<=0x7E)?true:false)

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
void			SGUI_IntegerVariableBox_Refresh(SGUI_INT_VARBOX_STRUCT* pstValue, SGUI_VARBOX_ALIG eAlignment, SGUI_DRAW_MODE eMode);
void			SGUI_TextVariableBox_Refresh(SGUI_TEXT_VARBOX_STRUCT* pstTextValue, SGUI_DRAW_MODE eMode);
void			SGUI_TextVariableBox_ChangeCharacter(SGUI_TEXT_VARBOX_STRUCT* pstTextValue, SGUI_DRAW_MODE eMode, SGUI_UINT uiCharacterSet, SGUI_TEXT_VARBOX_OPT eOpt);



#endif // __INCLUDE_GUI_VARIABLEBOX_H__
