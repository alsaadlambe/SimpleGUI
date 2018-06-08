#ifndef __INCLUDE_GUI_SCROLLBAR__
#define __INCLUDE_GUI_SCROLLBAR__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef enum
{
	SGUI_SCROLLBAR_VERTICAL = 0,						//Vertical scroll bar.
	SGUI_SCROLLBAR_HORIZONTAL,						//Horizontal scroll bar.
}SGUI_SCROLLBAR_DIRECTION;

typedef struct
{
	SGUI_INT					PosX;
	SGUI_INT					PosY;
	SGUI_SIZE					Width;
	SGUI_SIZE					Height;
	SGUI_UINT32					MaxIndex;
	SGUI_SCROLLBAR_DIRECTION	eDirection;
}SGUI_SCROLLBAR_PARAMETER;

typedef struct
{
	SGUI_UINT32					Index;
}SGUI_SCROLLBAR_DATA;

typedef struct
{
	SGUI_SCROLLBAR_PARAMETER	Parameter;
	SGUI_SCROLLBAR_DATA			Data;
}SGUI_SCROLLBAR_STRUCT;

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//												--
void	SGUI_ScrollBar_RefreshScrollBar(SGUI_SCROLLBAR_STRUCT* pstScrollBar);

#endif // __INCLUDE_GUI_SCROLLBAR_STRUCT__
