#ifndef __INCLUDE_GUI_TYPEDEF_H__
#define __INCLUDE_GUI_TYPEDEF_H__
//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
#include "stdint.h"
#include "stddef.h"

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef char						SGUI_INT8;
typedef	unsigned char				SGUI_UINT8;
typedef int16_t						SGUI_INT16;
typedef uint16_t					SGUI_UINT16;
typedef int32_t						SGUI_INT32;
typedef	uint32_t					SGUI_UINT32;

typedef	int							SGUI_INT;
typedef unsigned int				SGUI_UINT;
typedef unsigned char				SGUI_BYTE;
typedef const unsigned char			SGUI_CBYTE;
typedef size_t						SGUI_SIZE;

typedef char*						SGUI_PSZSTR;
typedef const char*					SGUI_PCSZSTR;
typedef char						SGUI_CHAR;
typedef const char					SGUI_CCHAR;

typedef SGUI_SIZE					SGUI_INDEX;
#define	SGUI_INVALID_INDEX			(-1)
typedef	SGUI_UINT32					SGUI_ROM_ADDRESS;

#define	SGUI_BOOL					SGUI_INT
#define SGUI_FALSE					(0)
#define SGUI_TRUE					(!SGUI_FALSE)

typedef struct _st_rectangle_
{
	SGUI_INT		PosX;
	SGUI_INT		PosY;
	SGUI_INT		Width;
	SGUI_INT		Height;
}SGUI_RECT_AREA;

typedef struct
{
	SGUI_UINT16			Year;
	SGUI_UINT16			Month;
	SGUI_UINT16			Day;
	SGUI_UINT16			Hour;
	SGUI_UINT16			Minute;
	SGUI_UINT16			Second;
}SGUI_TIME;

#define 	RECTANGLE_X_START(ST)			((ST).PosX)
#define 	RECTANGLE_X_END(ST)				(((ST).PosX + (ST).Width - 1))
#define 	RECTANGLE_Y_START(ST)			((ST).PosY)
#define 	RECTANGLE_Y_END(ST)				(((ST).PosY + (ST).Height - 1))
#define 	RECTANGLE_WIDTH(ST)				((ST).Width)
#define 	RECTANGLE_HEIGHT(ST)			((ST).Height)
#define 	RECTANGLE_VALID_WIDTH(ST)		((RECTANGLE_X_START(ST)>0)?RECTANGLE_WIDTH(ST):(RECTANGLE_WIDTH(ST)+RECTANGLE_X_START(ST)))
#define		RECTANGLE_VALID_HEIGHT(ST)		((RECTANGLE_Y_START(ST)>0)?RECTANGLE_HEIGHT(ST):(RECTANGLE_HEIGHT(ST)+RECTANGLE_Y_START(ST)))

#endif // __INCLUDE_GUI_TYPEDEF_H__
