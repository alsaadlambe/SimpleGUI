#ifndef __INCLUDE_GUI_NOTICE_H__
#define __INCLUDE_GUI_NOTICE_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef enum
{
	SGUI_ICON_NONE =			0,
	SGUI_ICON_ERROR,
	SGUI_ICON_INFORMATION,
	SGUI_ICON_QUESTION,
	SGUI_ICON_WARNING,
}SGUI_NOTICE_ICON;

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
SGUI_SIZE SGUI_Notice_RefreshNotice(SGUI_PCSZSTR szNoticeText, SGUI_INT uiTextOffset, SGUI_NOTICE_ICON eIcon);

#endif // __INCLUDE_GUI_NOTICE_H__
