#ifndef __INCLUDE_SGUI_LIST_H__
#define __INCLUDE_SGUI_LIST_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"
#include "SGUI_Text.h"
#include "SGUI_ScrollBar.h"

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
typedef enum
{
	LIST_ITEM_NORMAL =			0,
	LIST_ITEM_ENUM,
	LIST_ITEM_DIGIT,
	LIST_ITEM_DATE,
	LIST_ITEM_TIME
}SGUI_LIST_ITEM_TYPE;

typedef struct
{
	SGUI_INT32					Value;
	SGUI_INT32					Min;
	SGUI_INT32					Max;
	SGUI_INT32					*Bind;
}SGUI_LIST_PARAMETER_VALUE;

typedef struct _st_sgui_list_item_
{
	SGUI_SIZE					Sign;
	SGUI_PCSZSTR				Text;
	SGUI_LIST_ITEM_TYPE			Type;
	SGUI_LIST_PARAMETER_VALUE	Valid;
	SGUI_LIST_PARAMETER_VALUE	Decimal;
    SGUI_PCSZSTR*				EnumerationValues;
    SGUI_SIZE					EnumerationCount;
#if (_SIMPLE_GUI_ENABLE_DYNAMIC_MEMORY_ > 0)
	struct _st_sgui_list_item_	*PrevItem;
    struct _st_sgui_list_item_	*NextItem;
#endif
}SGUI_LIST_ITEM;

typedef struct
{
	SGUI_UINT16					ListTitleHeight;				// Read only
	SGUI_UINT16					VisibleItemsAreaHeight;			// Read only
	SGUI_UINT16					ListItemHeight;					// Read only
	SGUI_UINT16					FirstVisibleItemPosY;			// Read only
	SGUI_UINT16					VisibleIntegralItemsNum;		// Read only
	SGUI_UINT16					VisibleItemsNum;				// Read only
	SGUI_SIZE					SelectIndex;					// Control
	SGUI_SIZE					PageStartIndex;					// Control
	SGUI_UINT16					ItemPosYOffSet;					// Control
}SGUI_LIST_CONTROL;

typedef struct
{
	SGUI_PCSZSTR				Title;
	SGUI_SIZE					Count;
	SGUI_LIST_ITEM*				Items;
}SGUI_LIST_DATA;

typedef struct
{
	SGUI_SCROLLBAR_STRUCT		ScrollBar;
}SGUI_LIST_SUBELEMENT;

typedef struct
{
	SGUI_LIST_DATA				Data;
	SGUI_LIST_CONTROL			ControlVariable;
	SGUI_LIST_SUBELEMENT		SubElement;
	SGUI_FONT_SIZE	            FontSize;
}SGUI_LIST_STRUCT;

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
void			SGUI_LIST_InitializeListData(SGUI_LIST_STRUCT* pstList);
void			SGUI_LIST_RefreshList(SGUI_LIST_STRUCT* pstList);
void			SGUI_LIST_SelectUpItem(SGUI_LIST_STRUCT* pstList);
void			SGUI_LIST_SelectDownItem(SGUI_LIST_STRUCT* pstList);
void			SGUI_LIST_SetListItemValue(SGUI_LIST_STRUCT* pstList, SGUI_INDEX iItemIndex, SGUI_INT32 iSetValid, SGUI_INT32 iSetDecimal);
void			SGUI_LIST_GetListItemValue(SGUI_LIST_STRUCT* pstList, SGUI_INDEX iItemIndex, SGUI_INT32 *piValid, SGUI_INT32 *piDecimal);
SGUI_LIST_ITEM*	SGUI_LIST_GetListItemPtr(SGUI_LIST_STRUCT* pstList, SGUI_INDEX iItemIndex);
#if (_SIMPLE_GUI_ENABLE_DYNAMIC_MEMORY_ > 0)
SGUI_BOOL		SGUI_LIST_InsertItem(SGUI_LIST_STRUCT* pstList, SGUI_LIST_ITEM* pstNewItem, SGUI_INDEX iNewItemIndex);
SGUI_BOOL		SGUI_LIST_RemoveItem(SGUI_LIST_STRUCT* pstList, SGUI_INDEX iItemIndex);
#endif

#endif // __INCLUDE_SGUI_LIST_H__
