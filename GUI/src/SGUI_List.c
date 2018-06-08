/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_List.c												**/
/** Author: XuYulin														**/
/** Version: 1.0.0.0													**/
/** Description: Draw list interface.									**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"
#include "SGUI_Text.h"
#include "SGUI_ScrollBar.h"
#include "SGUI_List.h"
#include "SGUI_Frame.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
// User settings
#define						LIST_EDGE_MULTIPLE					(1)
#define						LIST_ITEM_TEXT_BLANK_EDGEX			(2)
#define						LIST_ITEM_TEXT_BLANK_EDGEY			(1)
#define						LIST_ITEM_PARAMETER_SPLIT_WIDTH		(4)
#define						LIST_ITEM_PARAMETER_TEXT_LENGTH_MAX	(16)
#define						LIST_SCROLLBAR_WIDTH				(5)
// Automatic calculation
#if(LIST_EDGE_MULTIPLE == 0)
	#define 				LIST_EDGE_SIZE						(0)
#else
	#define					LIST_EDGE_SIZE						(LIST_EDGE_MULTIPLE*2-1)
#endif
#define						LIST_TITLE_HEIGHT(FONT)             (g_stFontSize[FONT].Height+2)
#define						LIST_SCROLLBAR_POSX					(LCD_SIZE_WIDTH-LIST_EDGE_SIZE-LIST_SCROLLBAR_WIDTH)
#define						LIST_ITEM_RECT_POSX					(LIST_EDGE_SIZE+2)
#define						LIST_ITEM_RECT_WIDTH				(LCD_SIZE_WIDTH-(LIST_ITEM_RECT_POSX*2)-LIST_SCROLLBAR_WIDTH)
#define						LIST_ITEM_TEXT_AREA_WIDTH			(LIST_ITEM_RECT_WIDTH-(LIST_ITEM_TEXT_BLANK_EDGEX*2))
#define						LIST_ITEM_PARAMETER_AREA_WIDTH		((LIST_ITEM_TEXT_AREA_WIDTH/2)-LIST_ITEM_PARAMETER_SPLIT_WIDTH)
#define						LIST_ITEM_PARAMETER_STARTX			(LIST_ITEM_RECT_POSX+LIST_ITEM_TEXT_BLANK_EDGEX+(LIST_ITEM_TEXT_AREA_WIDTH-LIST_ITEM_PARAMETER_AREA_WIDTH))

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//
static __INLINE void		SGUI_LIST_RefreshListItems(SGUI_LIST_STRUCT* pstList);
static __INLINE void		SGUI_LIST_DrawItem(SGUI_LIST_STRUCT* pstList, SGUI_SIZE uiIndex);

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_LIST_InitializeListData						**/
/** Purpose:		Initialize a list control data.						**/
/** Params:																**/
/**	@pstList[in]:		Pointer of list data will be initialized.		**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_LIST_InitializeListData(SGUI_LIST_STRUCT* pstList)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_LIST_CONTROL*			pstListControl;
	SGUI_LIST_DATA*				pstListData;
	SGUI_LIST_SUBELEMENT*		pstSubElement;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstList)
	{
		// Initialize member object pointer.
		pstListControl = &(pstList->ControlVariable);
		pstListData = &(pstList->Data);
		pstSubElement = &(pstList->SubElement);
        // Initialize visible area control parameter.
		pstListControl->ListTitleHeight = LIST_TITLE_HEIGHT(pstList->FontSize); // List title height, Include border.
		pstListControl->PageStartIndex = 0;
		pstListControl->SelectIndex = 0;
		pstListControl->ItemPosYOffSet = 0;
		pstListControl->FirstVisibleItemPosY = LIST_EDGE_SIZE;
		pstListControl->ListItemHeight = g_stFontSize[pstList->FontSize].Height + (LIST_ITEM_TEXT_BLANK_EDGEY*2);
		pstListControl->VisibleItemsAreaHeight = LCD_SIZE_HEIGHT-LIST_EDGE_SIZE*2;
		if(NULL != pstList->Data.Title)
		{
			pstListControl->FirstVisibleItemPosY += LIST_TITLE_HEIGHT(pstList->FontSize) + 1;
			pstListControl->VisibleItemsAreaHeight -= LIST_TITLE_HEIGHT(pstList->FontSize) + 1;
		}
		pstListControl->VisibleIntegralItemsNum = pstListControl->VisibleItemsAreaHeight/(pstListControl->ListItemHeight);
		if(pstListControl->VisibleIntegralItemsNum > pstList->Data.Count)
		{
			pstListControl->VisibleIntegralItemsNum = pstList->Data.Count;
		}
		pstListControl->VisibleItemsNum = (pstListControl->VisibleItemsAreaHeight-1)/pstListControl->ListItemHeight+1;

		// Initialize scroll bar.
		pstSubElement->ScrollBar.Parameter.eDirection = SGUI_SCROLLBAR_VERTICAL;
		pstSubElement->ScrollBar.Parameter.PosX = LIST_SCROLLBAR_POSX;
		pstSubElement->ScrollBar.Parameter.PosY = pstListControl->FirstVisibleItemPosY;
		pstSubElement->ScrollBar.Parameter.Width = LIST_SCROLLBAR_WIDTH;
		pstSubElement->ScrollBar.Parameter.Height = pstListControl->VisibleItemsAreaHeight;
		pstSubElement->ScrollBar.Data.Index = 0;
		pstSubElement->ScrollBar.Parameter.MaxIndex = (pstListData->Count>pstListControl->VisibleIntegralItemsNum)?(pstListData->Count-pstListControl->VisibleIntegralItemsNum-1):0;
	}
}

/*************************************************************************/
/** Function Name:	SGUI_LIST_RefreshListItems							**/
/** Purpose:		Refresh all visible list item display.				**/
/** Params:																**/
/**	@pstList[in]:		Pointer of list data will be refreshed.			**/
/** Return:			None.												**/
/** Notice:			This only refresh visible items and scrollbar.		**/
/*************************************************************************/
void SGUI_LIST_RefreshListItems(SGUI_LIST_STRUCT* pstList)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_LIST_CONTROL*			pstListControl;
	SGUI_LIST_SUBELEMENT*		pstSubElement;
	SGUI_LIST_DATA*				pstListData;
	SGUI_SIZE					uiCurrentItemIndex;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	if(NULL != pstList)
	{
		pstListControl = &(pstList->ControlVariable);
		pstListData = &(pstList->Data);
		pstSubElement = &(pstList->SubElement);

		uiCurrentItemIndex = pstListControl->PageStartIndex;
	}

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstList)
	{
		if(pstList->Data.Count > 0)
		{
#if (_SIMPLE_GUI_ENABLE_DYNAMIC_MEMORY_ > 0)
			// Recalculate scroll bar parameter.
			pstListControl->VisibleIntegralItemsNum = pstListControl->VisibleItemsAreaHeight/(pstListControl->ListItemHeight);
			if(pstListControl->VisibleIntegralItemsNum > pstList->Data.Count)
			{
				pstListControl->VisibleIntegralItemsNum = pstList->Data.Count;
			}
			pstSubElement->ScrollBar.Parameter.MaxIndex = (pstListData->Count>pstListControl->VisibleIntegralItemsNum)?(pstListData->Count-pstListControl->VisibleIntegralItemsNum-1):0;
#endif
			// Clear list item display area.
			SGUI_Basic_DrawRectangle(LIST_ITEM_RECT_POSX, pstListControl->FirstVisibleItemPosY, LIST_ITEM_RECT_WIDTH, pstListControl->VisibleItemsAreaHeight, SGUI_COLOR_BKGCLR, SGUI_COLOR_BKGCLR);
			// Refresh scroll bar
			pstSubElement->ScrollBar.Data.Index = pstListControl->PageStartIndex;
			SGUI_ScrollBar_RefreshScrollBar(&(pstSubElement->ScrollBar));
			// Draw all visible items.
			while((uiCurrentItemIndex < pstListControl->PageStartIndex+pstListControl->VisibleItemsNum) && (uiCurrentItemIndex < pstListData->Count))
			{
				SGUI_LIST_DrawItem(pstList, uiCurrentItemIndex++);
			}
			// High light focused items.
			SGUI_Basic_ReverseBlockColor(LIST_ITEM_RECT_POSX, pstListControl->FirstVisibleItemPosY+(pstListControl->SelectIndex-pstListControl->PageStartIndex)*pstListControl->ListItemHeight-pstListControl->ItemPosYOffSet,
										LIST_ITEM_RECT_WIDTH, pstListControl->ListItemHeight);
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_LIST_RefreshList								**/
/** Purpose:		Refresh list display.								**/
/** Params:																**/
/**	@pstList[in]:		Pointer of list data will be refreshed.			**/
/** Return:			None.												**/
/** Notice:			This function will refresh all list display on		**/
/**					screen, include edge, items, title and scrollbar.	**/
/*************************************************************************/
void SGUI_LIST_RefreshList(SGUI_LIST_STRUCT* pstList)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_BOX_FRAME_STRUCT		stFrameData;
	SGUI_LIST_DATA				*pstListData;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstList)
	{
		pstListData = &(pstList->Data);
		// Draw frame
		stFrameData.Parameter.EdgeLayers = LIST_EDGE_MULTIPLE;
		stFrameData.Parameter.FontSize = pstList->FontSize;
		stFrameData.Data.Title = pstListData->Title;
		SGUI_Frame_DrawFullScreenFrame(&stFrameData);
		// Draw list items
		SGUI_LIST_RefreshListItems(pstList);
	}
}

/*************************************************************************/
/** Function Name:	SGUI_LIST_SelectUpItem								**/
/** Purpose:		Select previous list item if existed.				**/
/** Params:																**/
/**	@pstList[in]:		Pointer of list will be selected.				**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_LIST_SelectUpItem(SGUI_LIST_STRUCT* pstList)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_LIST_CONTROL			*pstListControl;
	SGUI_LIST_DATA				*pstListData;
	SGUI_UINT16					uiReverseBlockPosY;

    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstList)
	{
		pstListControl = &(pstList->ControlVariable);
		pstListData = &(pstList->Data);
        if((pstListControl->SelectIndex > 0) && (pstListData->Count > 1))
        {
            //Focused item and select item is visible and Integral;
            if((pstListControl->SelectIndex>pstListControl->PageStartIndex+1) && (pstListControl->VisibleIntegralItemsNum>1))
            {
                // Unfocused current item.
                uiReverseBlockPosY = pstListControl->FirstVisibleItemPosY+(pstListControl->SelectIndex-pstListControl->PageStartIndex)*pstListControl->ListItemHeight-pstListControl->ItemPosYOffSet;
                SGUI_Basic_ReverseBlockColor(LIST_ITEM_RECT_POSX, uiReverseBlockPosY, LIST_ITEM_RECT_WIDTH, pstListControl->ListItemHeight);
                pstListControl->SelectIndex--;
                // Focused previous item.
                uiReverseBlockPosY = pstListControl->FirstVisibleItemPosY+(pstListControl->SelectIndex-pstListControl->PageStartIndex)*pstListControl->ListItemHeight-pstListControl->ItemPosYOffSet;
                SGUI_Basic_ReverseBlockColor(LIST_ITEM_RECT_POSX, uiReverseBlockPosY, LIST_ITEM_RECT_WIDTH, pstListControl->ListItemHeight);
            }
            else
            {
                pstListControl->SelectIndex--;
                if(pstListControl->PageStartIndex == pstListControl->SelectIndex)
                {
                    pstListControl->ItemPosYOffSet = 0;
                }
                if(pstListControl->SelectIndex < pstListControl->PageStartIndex)
                {
                    pstListControl->PageStartIndex--;
                }
                SGUI_LIST_RefreshListItems(pstList);
            }
        }
	}
}

/*************************************************************************/
/** Function Name:	SGUI_LIST_SelectDownItem							**/
/** Purpose:		Select next list item if existed.					**/
/** Params:																**/
/**	@pstList[in]:		Pointer of list will be selected.				**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_LIST_SelectDownItem(SGUI_LIST_STRUCT* pstList)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_LIST_CONTROL			*pstListControl;
	SGUI_LIST_DATA				*pstListData;
	SGUI_UINT16					uiReverseBlockPosY;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstList)
	{
		pstListControl = &(pstList->ControlVariable);
		pstListData = &(pstList->Data);
        if((pstListControl->SelectIndex < pstListData->Count-1) && (pstListData->Count > 1))
        {
            //Focused item and select item is visible and Integral;
            if((pstListControl->VisibleIntegralItemsNum>1) && ((pstListControl->SelectIndex)<(pstListControl->PageStartIndex+pstListControl->VisibleIntegralItemsNum-1)))
            {
                // Unfocused current item.
                uiReverseBlockPosY = pstListControl->FirstVisibleItemPosY+(pstListControl->SelectIndex-pstListControl->PageStartIndex)*pstListControl->ListItemHeight-pstListControl->ItemPosYOffSet;
                SGUI_Basic_ReverseBlockColor(LIST_ITEM_RECT_POSX, uiReverseBlockPosY, LIST_ITEM_RECT_WIDTH, pstListControl->ListItemHeight);
                pstListControl->SelectIndex++;
                // Focused previous item.
                uiReverseBlockPosY = pstListControl->FirstVisibleItemPosY+(pstListControl->SelectIndex-pstListControl->PageStartIndex)*pstListControl->ListItemHeight-pstListControl->ItemPosYOffSet;
                SGUI_Basic_ReverseBlockColor(LIST_ITEM_RECT_POSX, uiReverseBlockPosY, LIST_ITEM_RECT_WIDTH, pstListControl->ListItemHeight);
            }
            else
            {
                pstListControl->SelectIndex++;
                if((pstListControl->SelectIndex-pstListControl->PageStartIndex+1) == pstListControl->VisibleItemsNum)
                {
                    pstListControl->ItemPosYOffSet = pstListControl->ListItemHeight-pstListControl->VisibleItemsAreaHeight%pstListControl->ListItemHeight;
                }
                if((pstListControl->SelectIndex-pstListControl->PageStartIndex+1) > pstListControl->VisibleItemsNum)
                {
                    pstListControl->PageStartIndex++;
                }
                SGUI_LIST_RefreshListItems(pstList);
            }
        }
	}
}

/*************************************************************************/
/** Function Name:	SGUI_LIST_DrawItem									**/
/** Purpose:		Draw a visible list item.							**/
/** Params:																**/
/**	@pstList[in]:		Pointer of list will be selected.				**/
/**	@uiIndex[in]:		List item index will be draw.					**/
/** Return:			None.												**/
/** Notice:			This function only draw visible items, it's			**/
/**					depending on the first visible item index and count	**/
/**					of visible items.									**/
/*************************************************************************/
void SGUI_LIST_DrawItem(SGUI_LIST_STRUCT* pstList, SGUI_SIZE uiIndex)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_RECT_AREA				stItemTextDisplayArea;
	SGUI_RECT_AREA				stItemTextDataArea;
	SGUI_LIST_CONTROL*			pstListControl;
	SGUI_LIST_ITEM*				pstListItemPointer;
	SGUI_CHAR					szParameterStringBuffer[LIST_ITEM_PARAMETER_TEXT_LENGTH_MAX];
	SGUI_SIZE					uiParameterTextWidth;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	if(NULL != pstList)
	{
		pstListControl = &(pstList->ControlVariable);
		pstListItemPointer = SGUI_LIST_GetListItemPtr(pstList, uiIndex);
		szParameterStringBuffer[0] = '\0';
	}

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstList)
	{
		if((uiIndex >= pstListControl->PageStartIndex) && (uiIndex < pstListControl->PageStartIndex+pstListControl->VisibleItemsNum))
		{
			// Prepare draw list item text.
			stItemTextDataArea.PosX = 0;
			stItemTextDataArea.PosY = LIST_ITEM_TEXT_BLANK_EDGEY;
			stItemTextDisplayArea.PosX = LIST_ITEM_RECT_POSX + LIST_ITEM_TEXT_BLANK_EDGEX;
			stItemTextDisplayArea.Width = LIST_ITEM_TEXT_AREA_WIDTH;

			if(uiIndex == pstListControl->PageStartIndex)
			{
				stItemTextDataArea.PosY -= pstListControl->ItemPosYOffSet;
				stItemTextDisplayArea.PosY = pstListControl->FirstVisibleItemPosY;
				stItemTextDisplayArea.Height = pstListControl->ListItemHeight-pstListControl->ItemPosYOffSet;
			}
			else if(uiIndex == pstListControl->PageStartIndex+pstListControl->VisibleItemsNum-1)
			{
				stItemTextDisplayArea.PosY = pstListControl->FirstVisibleItemPosY+pstListControl->ListItemHeight*(uiIndex-pstListControl->PageStartIndex)-pstListControl->ItemPosYOffSet;

				if((0==pstListControl->ItemPosYOffSet) && (0 != pstListControl->VisibleItemsAreaHeight%pstListControl->ListItemHeight))
				{
					stItemTextDisplayArea.Height = (pstListControl->VisibleItemsAreaHeight%pstListControl->ListItemHeight);
				}
				else
				{
					stItemTextDisplayArea.Height = pstListControl->ListItemHeight;
				}
				//stItemTextDisplayArea.Height = (0==pstListControl->ItemPosYOffSet)?(pstListControl->VisibleItemsAreaHeight%pstListControl->ListItemHeight):(pstListControl->ListItemHeight);
			}
			else
			{
				stItemTextDisplayArea.PosY = pstListControl->FirstVisibleItemPosY+pstListControl->ListItemHeight*(uiIndex-pstListControl->PageStartIndex)-pstListControl->ItemPosYOffSet;
				stItemTextDisplayArea.Height = pstListControl->ListItemHeight;
			}

			// Draw list item text.
			SGUI_Text_DrawSingleLineText(	pstListItemPointer->Text, pstList->FontSize,
											&stItemTextDisplayArea, &stItemTextDataArea, SGUI_DRAW_NORMAL);

			// Prepare draw parameter text.
			stItemTextDataArea.PosX = LIST_ITEM_PARAMETER_SPLIT_WIDTH+1;
			stItemTextDisplayArea.PosX = LIST_ITEM_PARAMETER_STARTX;
			stItemTextDisplayArea.Width = LIST_ITEM_PARAMETER_AREA_WIDTH;
			if(LIST_ITEM_NORMAL != pstListItemPointer->Type)
			{
				if(pstListItemPointer->Valid.Value > pstListItemPointer->Valid.Max)
				{
					pstListItemPointer->Valid.Value = pstListItemPointer->Valid.Max;
				}
				else if(pstListItemPointer->Valid.Value < pstListItemPointer->Valid.Min)
				{
					pstListItemPointer->Valid.Value = pstListItemPointer->Valid.Min;
				}
				if(pstListItemPointer->Decimal.Value > pstListItemPointer->Decimal.Max)
				{
					pstListItemPointer->Decimal.Value = pstListItemPointer->Decimal.Max;
				}
				else if(pstListItemPointer->Decimal.Value < pstListItemPointer->Decimal.Min)
				{
					pstListItemPointer->Decimal.Value = pstListItemPointer->Decimal.Min;
				}

				switch(pstListItemPointer->Type)
				{
					case LIST_ITEM_ENUM:
					{
						SGUI_Common_StringLengthCopy(szParameterStringBuffer, pstListItemPointer->EnumerationValues[pstListItemPointer->Valid.Value], LIST_ITEM_PARAMETER_TEXT_LENGTH_MAX-1);
						szParameterStringBuffer[LIST_ITEM_PARAMETER_TEXT_LENGTH_MAX-1] = '\0';
						uiParameterTextWidth = SGUI_Text_GetTextGraphicsWidth(szParameterStringBuffer, pstList->FontSize);
						if(uiParameterTextWidth < LIST_ITEM_PARAMETER_AREA_WIDTH)
						{
							stItemTextDataArea.PosX += (LIST_ITEM_PARAMETER_AREA_WIDTH-LIST_ITEM_PARAMETER_SPLIT_WIDTH)-uiParameterTextWidth;
						}
						break;
					}
					case LIST_ITEM_DIGIT:
					{
						SGUI_Common_IntegerToStringWithDecimalPoint(pstListItemPointer->Valid.Value, pstListItemPointer->Decimal.Value, szParameterStringBuffer, 0, ' ');
						uiParameterTextWidth = SGUI_Text_GetTextGraphicsWidth(szParameterStringBuffer, pstList->FontSize);
						if(uiParameterTextWidth < LIST_ITEM_PARAMETER_AREA_WIDTH)
						{
							stItemTextDataArea.PosX += (LIST_ITEM_PARAMETER_AREA_WIDTH-LIST_ITEM_PARAMETER_SPLIT_WIDTH)-uiParameterTextWidth;
						}
						break;
					}
					default:
					{
						// Undefined list item type, no any process.
						break;
					}
				}
				// Draw parameter text.
				SGUI_Text_DrawSingleLineText(szParameterStringBuffer, pstList->FontSize, &stItemTextDisplayArea, &stItemTextDataArea, SGUI_DRAW_NORMAL);
			}
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_LIST_SetListItemValue							**/
/** Purpose:		Set list parameter value if supported.				**/
/** Params:																**/
/**	@pstList[in]:		Pointer of list will be selected.				**/
/**	@iItemIndex[in]:	Index of list item index will be changed.		**/
/**	@iSetValid[in]:		Valid value will be set.						**/
/**	@iSetDecimal[in]:	Decimal value will be set.						**/
/** Return:			None.												**/
/** Notice:			The bind value will update if existed.				**/
/*************************************************************************/
void SGUI_LIST_SetListItemValue(SGUI_LIST_STRUCT* pstList, SGUI_INDEX iItemIndex, int32_t iSetValid, int32_t iSetDecimal)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_LIST_ITEM				*pstSelectedListItem;
	SGUI_LIST_CONTROL			*pstListControl;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pstSelectedListItem		= NULL;
	if(NULL != pstList)
	{
		if(iItemIndex < pstList->Data.Count)
		{
			pstSelectedListItem = SGUI_LIST_GetListItemPtr(pstList, iItemIndex);
		}
		pstListControl = &(pstList->ControlVariable);
	}

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstSelectedListItem)
	{
		// Set valid.
		if(iSetValid > pstSelectedListItem->Valid.Max)
		{
			pstSelectedListItem->Valid.Value = pstSelectedListItem->Valid.Max;
		}
		else if(iSetValid < pstSelectedListItem->Valid.Min)
		{
			pstSelectedListItem->Valid.Value = pstSelectedListItem->Valid.Min;
		}
		else
		{
			pstSelectedListItem->Valid.Value = iSetValid;
		}
		// Sync bind if existed.
		if(NULL != pstSelectedListItem->Valid.Bind)
		{
			*(pstSelectedListItem->Valid.Bind) = pstSelectedListItem->Valid.Value;
		}
		// Set decimal.
		if(iSetDecimal > pstSelectedListItem->Decimal.Max)
		{
			pstSelectedListItem->Decimal.Value = pstSelectedListItem->Decimal.Max;
		}
		else if(iSetDecimal < pstSelectedListItem->Decimal.Min)
		{
			pstSelectedListItem->Decimal.Value = pstSelectedListItem->Decimal.Min;
		}
		else
		{
			pstSelectedListItem->Decimal.Value = iSetDecimal;
		}
		// Sync bind if existed.
		if(NULL != pstSelectedListItem->Decimal.Bind)
		{
			*(pstSelectedListItem->Decimal.Bind) = pstSelectedListItem->Decimal.Value;
		}
		// Clear list item area.
		SGUI_Basic_DrawRectangle(LIST_ITEM_RECT_POSX, pstListControl->FirstVisibleItemPosY+(pstListControl->SelectIndex-pstListControl->PageStartIndex)*pstListControl->ListItemHeight-pstListControl->ItemPosYOffSet,
								LIST_ITEM_RECT_WIDTH, pstListControl->ListItemHeight, SGUI_COLOR_BKGCLR, SGUI_COLOR_BKGCLR);
		// refresh list item
		SGUI_LIST_DrawItem(pstList, iItemIndex);
		// High light selected item.
		SGUI_Basic_ReverseBlockColor(LIST_ITEM_RECT_POSX, pstListControl->FirstVisibleItemPosY+(pstListControl->SelectIndex-pstListControl->PageStartIndex)*pstListControl->ListItemHeight-pstListControl->ItemPosYOffSet,
		 							LIST_ITEM_RECT_WIDTH, pstListControl->ListItemHeight);
	}
}

/*************************************************************************/
/** Function Name:	SGUI_LIST_GetListItemValue							**/
/** Purpose:		Get list parameter value if supported.				**/
/** Params:																**/
/**	@pstList[in]:		Pointer of list will be read.					**/
/**	@iItemIndex[in]:	Index of list item index will be read.			**/
/**	@piValid[out]:		Valid value will be read.						**/
/**	@piDecimal[out]:	Decimal value will be read.						**/
/** Return:			None.												**/
/** Notice:			None.												**/
/*************************************************************************/
void SGUI_LIST_GetListItemValue(SGUI_LIST_STRUCT* pstList, SGUI_INDEX iItemIndex, int32_t* piValid, int32_t* piDecimal)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_LIST_ITEM				*pstSelectedListItem;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pstSelectedListItem		= NULL;
	if(NULL != pstList)
	{
		if(iItemIndex < pstList->Data.Count)
		{
			pstSelectedListItem = SGUI_LIST_GetListItemPtr(pstList, iItemIndex);
		}
	}

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstSelectedListItem)
	{
		// Read valid value.
		if(NULL != piValid)
		{
			*piValid = pstSelectedListItem->Valid.Value;
		}

		// Read decimal value.
		if(NULL != piDecimal)
		{
			*piDecimal = pstSelectedListItem->Decimal.Value;
		}
	}
}

/*************************************************************************/
/** Function Name:	SGUI_LIST_BindData									**/
/** Purpose:		Bind a 32-bit integer variable by pointer.			**/
/** Params:																**/
/**	@pstList[in]:		Pointer of list will be read.					**/
/**	@iItemIndex[in]:	Index of list item index will be read.			**/
/**	@piValid[in]:		Valid variable pointer.							**/
/**	@piDecimal[in]:	Decimal variable pointer.							**/
/** Return:			None.												**/
/** Notice:			Variable pointer and decimal variable can be NULL.	**/
/*************************************************************************/
void SGUI_LIST_BindData(SGUI_LIST_STRUCT* pstList, SGUI_INDEX iItemIndex, SGUI_INT32* piValid, SGUI_INT32* piDecimal)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_LIST_ITEM				*pstSelectedListItem;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pstSelectedListItem =		NULL;
	if(NULL != pstList)
	{
		pstSelectedListItem =	SGUI_LIST_GetListItemPtr(pstList, iItemIndex);
	}

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstSelectedListItem)
	{
		pstSelectedListItem->Valid.Bind = piValid;
		pstSelectedListItem->Decimal.Bind = piDecimal;
	}
}

/*************************************************************************/
/** Function Name:	SGUI_LIST_GetListItemPtr							**/
/** Purpose:		Get list item data pointer by index.				**/
/** Params:																**/
/**	@pstList[in]:		Pointer of list will be read.					**/
/**	@iItemIndex[in]:	Index of list item index will be read.			**/
/** Return:			Selected list item data structure pointer.			**/
/** Notice:			Variable pointer and decimal variable can be NULL.	**/
/*************************************************************************/
SGUI_LIST_ITEM* SGUI_LIST_GetListItemPtr(SGUI_LIST_STRUCT* pstList, SGUI_INDEX iItemIndex)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_LIST_ITEM				*pstSelectedListItem;
#if (_SIMPLE_GUI_ENABLE_DYNAMIC_MEMORY_ > 0)
	SGUI_SIZE					uiItemCounter;
#endif

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pstSelectedListItem =		NULL;
#if (_SIMPLE_GUI_ENABLE_DYNAMIC_MEMORY_ > 0)
	uiItemCounter =				0;
#endif

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstList)
	{
		if(iItemIndex < pstList->Data.Count)
		{
#if (_SIMPLE_GUI_ENABLE_DYNAMIC_MEMORY_ > 0)
			pstSelectedListItem = pstList->Data.Items;
			if(NULL != pstSelectedListItem)
			{
				while((pstList->Data.Count > 0) && (NULL != pstSelectedListItem->NextItem) && (uiItemCounter < iItemIndex))
				{
					pstSelectedListItem = pstSelectedListItem->NextItem;
					uiItemCounter++;
				}
			}
#else
			pstSelectedListItem = pstList->Data.Items+iItemIndex;
#endif
		}
	}
	return pstSelectedListItem;
}

#if (_SIMPLE_GUI_ENABLE_DYNAMIC_MEMORY_ > 0)
/*************************************************************************/
/** Function Name:	SGUI_LIST_InsertItem								**/
/** Purpose:		Insert a new item to list.							**/
/** Params:																**/
/**	@pstList[in]:		Pointer of list will be read.					**/
/**	@pstNewItem[in]:	New item data pointer.							**/
/**	@iItemIndex[in]:	Index of new list item.							**/
/** Return:																**/
/** @TRUE				Insert success.									**/
/**	@FALSE				Insert failed.									**/
/** Notice:			This function only supported when the dynamic list	**/
/**					item enabled, and new item index cannot greater		**/
/**					then list item count.								**/
/*************************************************************************/
SGUI_BOOL SGUI_LIST_InsertItem(SGUI_LIST_STRUCT* pstList, SGUI_LIST_ITEM* pstNewItem, SGUI_INDEX iNewItemIndex)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_LIST_ITEM				*pstListItemDataSource;
	SGUI_LIST_ITEM				*pstNewListItem, *pstPosItem;
	SGUI_LIST_DATA				*pstListData;
	SGUI_BOOL					bResult;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pstListItemDataSource =		pstNewItem;
	pstNewListItem =			NULL;
	pstPosItem =				NULL;
	bResult =					SGUI_TRUE;
	pstListData =				&(pstList->Data);

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(iNewItemIndex > pstList->Data.Count)
	{
		bResult = SGUI_FALSE;
	}
	else
	{
		pstNewListItem = (SGUI_LIST_ITEM*)SGUI_Common_Allocate(sizeof(SGUI_LIST_ITEM));
		if(NULL != pstNewListItem)
		{
			SGUI_Common_MemoryCopy(pstNewListItem, pstListItemDataSource, sizeof(SGUI_LIST_ITEM));

			if(0 == pstList->Data.Count)
			{
				pstNewListItem->NextItem = NULL;
				pstNewListItem->PrevItem = NULL;
				pstListData->Items = pstNewListItem;
			}
			else if(iNewItemIndex == pstList->Data.Count)
			{
				pstPosItem = SGUI_LIST_GetListItemPtr(pstList, iNewItemIndex-1);
				pstPosItem->NextItem = pstNewListItem;
				pstNewListItem->PrevItem = pstPosItem;
				pstNewListItem->NextItem = NULL;
			}
			else //if((uiNewItemIndex>0)&&(uiNewItemIndex<pstListData->Count))
			{
				pstPosItem = SGUI_LIST_GetListItemPtr(pstList, iNewItemIndex);
				pstNewListItem->PrevItem = pstPosItem->PrevItem;
				pstNewListItem->NextItem = pstPosItem;
				if(NULL != pstPosItem->PrevItem)
				{
					pstPosItem->PrevItem->NextItem = pstNewListItem;
				}
				else
				{
					// Insert to list head, repointer the list head pointer.
					pstListData->Items = pstNewListItem;
				}
				pstPosItem->PrevItem = pstNewListItem;
			}
			pstList->Data.Count++;
		}
	}
	return bResult;
}

/*************************************************************************/
/** Function Name:	SGUI_LIST_RemoveItem								**/
/** Purpose:		Remove a item from list.							**/
/** Params:																**/
/**	@pstList[in]:		Pointer of list will be read.					**/
/**	@uiItemIndex[in]:	Index of list item will be removed.				**/
/** Return:																**/
/** @TRUE				Remove success.									**/
/**	@FALSE				Remove failed.									**/
/** Notice:			This function only supported when the dynamic list  **/
/**					item enabled, and index of removed item cannot		**/
/**					greater then list item count.						**/
/*************************************************************************/
SGUI_BOOL SGUI_LIST_RemoveItem(SGUI_LIST_STRUCT* pstList, SGUI_INDEX iItemIndex)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_LIST_ITEM				*pstRemoveListItem;
	SGUI_BOOL					bResult;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pstRemoveListItem =			SGUI_LIST_GetListItemPtr(pstList, iItemIndex);
	bResult =					SGUI_TRUE;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstRemoveListItem)
	{
		if(NULL != pstRemoveListItem->PrevItem)
		{
			pstRemoveListItem->PrevItem->NextItem = pstRemoveListItem->NextItem;
		}
		if(NULL != pstRemoveListItem->NextItem)
		{
			pstRemoveListItem->NextItem->PrevItem = pstRemoveListItem->PrevItem;
		}
		if(0 == iItemIndex)
		{
			pstList->Data.Items = pstRemoveListItem->NextItem;
		}
		SGUI_Common_Free(pstRemoveListItem);
		pstList->Data.Count--;
	}
	return bResult;
}
#endif
