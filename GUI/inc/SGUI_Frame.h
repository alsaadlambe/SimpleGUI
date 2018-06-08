#ifndef __INCLUDE_GUI_FRAME_H__
#define __INCLUDE_GUI_FRAME_H__
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
	SGUI_INT					EdgeLayers;
    SGUI_FONT_SIZE				FontSize;
}SGUI_BOX_FRAME_PARAMETER;

typedef struct
{
    SGUI_PCSZSTR					Title;
}SGUI_BOX_FRAME_DATA;

typedef struct
{
	SGUI_BOX_FRAME_PARAMETER	Parameter;
    SGUI_BOX_FRAME_DATA			Data;
}SGUI_BOX_FRAME_STRUCT;

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
void SGUI_Frame_DrawFullScreenFrame(SGUI_BOX_FRAME_STRUCT* pstFrameStyleData);

#endif // __INCLUDE_GUI_FRAME_H__
