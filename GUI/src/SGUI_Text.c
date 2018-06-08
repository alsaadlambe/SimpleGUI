/*************************************************************************/
/** Copyright.															**/
/** FileName: SGUI_Text.c												**/
/** Author: XuYulin														**/
/** Version: 1.0.0.0													**/
/** Description: Text display interface									**/
/*************************************************************************/

//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Text.h"
#include "SGUI_Basic.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

//=======================================================================//
//= Static variable declaration.									    =//
//=======================================================================//

SGUI_BYTE					auiFontDataBuffer[FONT_DATA_BUFFER_SIZE] = {0x00};
const SGUI_FONT_SIZE_STRUCT	g_stFontSize[SGUI_FONT_SIZE_MAX] = {	{6, 4, 6},
																	{8, 6, 8},
																	{12, 6, 12},
																	{16, 8, 16},
																	{24, 12, 24},
																	{32, 16, 32}};

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//
static void                 SGUI_Text_ReadFontData(SGUI_FONT_SIZE eFontSize, SGUI_UINT16 uiCharacterCode, SGUI_BYTE* pFontDataBuffer, SGUI_UINT32 uiFontBufferSize);
static SGUI_SIZE			SGUI_Text_GetCharacterTableIndex(SGUI_UINT16 uiCharacterCode);

//=======================================================================//
//= Function define.										            =//
//=======================================================================//

/*************************************************************************/
/** Function Name:	SGUI_Text_DrawSingleLineText						**/
/** Purpose:		Write a single line text in a fixed area.			**/
/** Params:																**/
/**	@szText[in]:		Text array pointer.								**/
/**	@eFontSize[in]:		Font size.										**/
/**	@pstDisplayArea[in]: Display area size.								**/
/**	@pstTextDataArea[in]: Text area size.								**/
/**	@eFontMode[in]		Character display mode(normal or reverse color).**/
/** Return:			Next character X coordinate in current line.		**/
/** Limitation:		None.												**/
/*************************************************************************/
void SGUI_Text_DrawSingleLineText(SGUI_PCSZSTR szText, SGUI_FONT_SIZE eFontSize, SGUI_RECT_AREA* pstDisplayArea, SGUI_RECT_AREA* pstTextDataArea, SGUI_DRAW_MODE eFontMode)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_PSZSTR					pcTextPointer;										// Text character pointer.
	SGUI_UINT16					uiCodeHighByte, uiCodeLowByte, uiCharacterCode;		// Character byte, might be tow bytes.
	SGUI_UINT16					uiFontWidth, uiFontHeight, uiCharacterWidth;		// Font size and character graphics width.
	SGUI_RECT_AREA				stCharacterDataArea = {0};
	SGUI_COLOR					eBackColor;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	// Initialize variable.
	pcTextPointer =				(SGUI_PSZSTR)ENCODE(szText);
	uiCharacterCode =			0x0000;
	eBackColor =				(eFontMode == SGUI_DRAW_NORMAL)?SGUI_COLOR_BKGCLR:SGUI_COLOR_FRGCLR;
	// Get font graphics size.
	uiFontWidth =				g_stFontSize[eFontSize].Width;
	uiFontHeight =				g_stFontSize[eFontSize].Height;
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((szText != NULL) && (RECTANGLE_X_START(*pstDisplayArea) < LCD_SIZE_WIDTH))
	{
		// Recalculate text display area and data area.
        if(RECTANGLE_X_START(*pstDisplayArea) < 0)
		{
			RECTANGLE_X_START(*pstTextDataArea) += RECTANGLE_X_START(*pstDisplayArea);
			RECTANGLE_WIDTH(*pstDisplayArea) += RECTANGLE_X_START(*pstDisplayArea);
			RECTANGLE_X_START(*pstDisplayArea) = 0;
		}
		if(RECTANGLE_Y_START(*pstDisplayArea) < 0)
		{
			RECTANGLE_Y_START(*pstTextDataArea) += RECTANGLE_Y_START(*pstDisplayArea);
			RECTANGLE_HEIGHT(*pstDisplayArea) += RECTANGLE_Y_START(*pstDisplayArea);
			RECTANGLE_Y_START(*pstDisplayArea) = 0;
		}
		// Clear text area.
        SGUI_Basic_DrawRectangle(RECTANGLE_X_START(*pstDisplayArea), RECTANGLE_Y_START(*pstDisplayArea),
						RECTANGLE_WIDTH(*pstDisplayArea), RECTANGLE_HEIGHT(*pstDisplayArea),
						eBackColor, eBackColor);
		// Initialize drawing area data.
		RECTANGLE_X_START(stCharacterDataArea) = RECTANGLE_X_START(*pstTextDataArea);
		RECTANGLE_Y_START(stCharacterDataArea) = RECTANGLE_Y_START(*pstTextDataArea);
		RECTANGLE_HEIGHT(stCharacterDataArea) = uiFontHeight;
		// Loop for Each char.
		while((*pcTextPointer != '\0') && (RECTANGLE_X_START(stCharacterDataArea) < RECTANGLE_WIDTH(*pstDisplayArea)))
		{
			// Get character.
			uiCodeHighByte = 0x00;
			uiCodeLowByte = 0x00;
			// Process with ASCII code.
			if((SGUI_BYTE)(*pcTextPointer) < 0x7F)
			{
				uiCodeLowByte = (SGUI_BYTE)(*pcTextPointer);
				uiCharacterWidth = uiFontWidth;
				pcTextPointer++;
			}
			// Process with GB2312.
			else if(((SGUI_BYTE)(*pcTextPointer) >= 0xA1) && ((SGUI_BYTE)(*pcTextPointer) <= 0xF7))
			{
				uiCodeHighByte = (SGUI_BYTE)*pcTextPointer++;
				uiCodeLowByte = (SGUI_BYTE)*pcTextPointer++;
				uiCharacterWidth = uiFontWidth << 1;	//uiCharacterWidth = uiFontWidth * 2; for full-width character;
			}
			// Invalid character
			else
			{
				uiCharacterWidth = 0;
				pcTextPointer++;
			}
			uiCharacterCode = uiCodeHighByte;
			uiCharacterCode = uiCharacterCode << 8;
			uiCharacterCode = uiCharacterCode | uiCodeLowByte;

			RECTANGLE_WIDTH(stCharacterDataArea) = uiCharacterWidth;

			if(RECTANGLE_X_END(stCharacterDataArea) >= 0)
			{
				// Read Font data.
				SGUI_Text_ReadFontData(eFontSize, uiCharacterCode, auiFontDataBuffer, 512);
				// Display character.
				SGUI_Basic_DrawBitMap(pstDisplayArea, &stCharacterDataArea, auiFontDataBuffer, (SGUI_DRAW_MODE)eFontMode);
			}
			RECTANGLE_X_START(stCharacterDataArea) += uiCharacterWidth;
		}
	}
}


/*****************************************************************************/
/** Function Name:	GUI_DrawMultipleLinesText								**/
/** Purpose:		Write a mulitiplt line text in a rectangular area.		**/
/** Params:																	**/
/**	@szText[in]:		Text array pointer.									**/
/**	@eFontSize[in]:		Font size.											**/
/**	@pstDisplayArea[in]: Display area size.									**/
/**	@eFontMode[in]:		Character display mode(normal or reverse color).	**/
/** Return:			Next character X coordinate in current line.			**/
/** Notice:			None.													**/
/*****************************************************************************/
SGUI_SIZE SGUI_Text_DrawMultipleLinesText(SGUI_PCSZSTR szText, SGUI_FONT_SIZE eFontSize, SGUI_RECT_AREA* pstDisplayArea, SGUI_INT iTopOffset, SGUI_DRAW_MODE eFontMode)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_PSZSTR					pcTextPointer;
	SGUI_UINT16					uiCodeHighByte, uiCodeLowByte, uiCharacterCode;
	SGUI_UINT16					uiFontWidth, uiFontHeight;
	SGUI_SIZE					uiLines;
	SGUI_RECT_AREA				stCharacterDataArea = {0};
	SGUI_COLOR					eBackColor;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pcTextPointer =				(SGUI_PSZSTR)ENCODE(szText);
	uiCharacterCode =			0x0000;
	uiLines =					0;
	eBackColor =				(eFontMode == SGUI_DRAW_NORMAL)?SGUI_COLOR_BKGCLR:SGUI_COLOR_FRGCLR;
	// Get font graphics size.
	uiFontWidth =				g_stFontSize[eFontSize].Width;
	uiFontHeight =				g_stFontSize[eFontSize].Height;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((szText != NULL) && (RECTANGLE_X_START(*pstDisplayArea) < LCD_SIZE_WIDTH))
	{
		// Recalculate text display area and data area.
		if(RECTANGLE_X_START(*pstDisplayArea) < 0)
		{
			RECTANGLE_X_START(stCharacterDataArea) += RECTANGLE_X_START(*pstDisplayArea);
			RECTANGLE_WIDTH(*pstDisplayArea) += RECTANGLE_X_START(*pstDisplayArea);
			RECTANGLE_X_START(*pstDisplayArea) = 0;
		}
		if(RECTANGLE_Y_START(*pstDisplayArea) < 0)
		{
			RECTANGLE_Y_START(stCharacterDataArea) += RECTANGLE_Y_START(*pstDisplayArea);
			RECTANGLE_HEIGHT(*pstDisplayArea) += RECTANGLE_Y_START(*pstDisplayArea);
			RECTANGLE_Y_START(*pstDisplayArea) = 0;
		}
		// Clear text area.
        SGUI_Basic_DrawRectangle(RECTANGLE_X_START(*pstDisplayArea), RECTANGLE_Y_START(*pstDisplayArea),
						RECTANGLE_WIDTH(*pstDisplayArea), RECTANGLE_HEIGHT(*pstDisplayArea),
						eBackColor, eBackColor);
		// Initialize drawing area data.
		RECTANGLE_X_START(stCharacterDataArea)		= 0;
		RECTANGLE_Y_START(stCharacterDataArea)		= iTopOffset;
		RECTANGLE_HEIGHT(stCharacterDataArea)		= uiFontHeight;
		uiLines										= 1;

		// Loop for each word in display area.
		if(NULL != pcTextPointer)
        {
            while(*pcTextPointer != '\0')
            {
                uiCodeHighByte = 0x00;
                uiCodeLowByte = 0x00;
                // Judge change line symbol.
                if(*pcTextPointer == '\n')
                {
                    if(RECTANGLE_X_START(stCharacterDataArea) == 0)
                    {
                        // Ignore change lines in line start.
                    }
                    else
                    {
                        // Change lines.
                        RECTANGLE_X_START(stCharacterDataArea) = 0;
                        RECTANGLE_Y_START(stCharacterDataArea) += uiFontHeight;
                        uiLines ++;
                    }
                    pcTextPointer++;
                    continue;
                }
                // Process with ASCII code.
                if(((SGUI_BYTE)(*pcTextPointer) < 0x7F) && ((SGUI_BYTE)(*pcTextPointer) >= 0x20))
                {
                    uiCodeLowByte = (SGUI_BYTE)*pcTextPointer++;
                    RECTANGLE_WIDTH(stCharacterDataArea) = uiFontWidth;
                }
                // Process with GB2312.
                else if(((SGUI_BYTE)(*pcTextPointer) >= 0xA1) && ((SGUI_BYTE)(*pcTextPointer) <= 0xF7))
                {
                    uiCodeHighByte = (SGUI_BYTE)*pcTextPointer++;
                    uiCodeLowByte = (SGUI_BYTE)*pcTextPointer++;
                    RECTANGLE_WIDTH(stCharacterDataArea) = uiFontWidth << 1;
                }
                // Invalid character
                else
                {
                    pcTextPointer++;
                    RECTANGLE_WIDTH(stCharacterDataArea) = 0;
                    continue;
                }
                uiCharacterCode = uiCodeHighByte;
                uiCharacterCode = uiCharacterCode << 8;
                uiCharacterCode = uiCharacterCode | uiCodeLowByte;

                // Judge change line
                if(RECTANGLE_X_END(stCharacterDataArea) >= RECTANGLE_WIDTH(*pstDisplayArea))
                {
                    // Change lines.
                    RECTANGLE_X_START(stCharacterDataArea) = 0;
                    RECTANGLE_Y_START(stCharacterDataArea) += uiFontHeight;
                    uiLines ++;
                }
                // Draw characters.
                if((RECTANGLE_Y_END(stCharacterDataArea) >= 0) && (RECTANGLE_Y_START(stCharacterDataArea) < RECTANGLE_HEIGHT(*pstDisplayArea)))
                {
                    // Read Font data.
                    SGUI_Text_ReadFontData(eFontSize, uiCharacterCode, (SGUI_BYTE*)auiFontDataBuffer, 512);
                    SGUI_Basic_DrawBitMap(pstDisplayArea, &stCharacterDataArea, (SGUI_BYTE*)auiFontDataBuffer, eFontMode);
                }
                else
                {
                    // character is not in visible area, ignore.
                }
                RECTANGLE_X_START(stCharacterDataArea) += RECTANGLE_WIDTH(stCharacterDataArea);
            }
        }
	}
	return uiLines;
}

/*****************************************************************************/
/** Function Name:	SGUI_Text_ReadFontData									**/
/** Purpose:		Read font from internal or external flash memory.		**/
/** Params:																	**/
/**	@eFontSize[in]: 	Start address of font data.							**/
/**	@uiCharacterCode[in]: Data buffer pointer of read font data.			**/
/**	@pFontDataBuffer[out]: Read data size.									**/
/**	@uiFontBufferSize[in]: Free space for font buffer, 0 means ignore the	**/
/**						data size judgment.									**/
/** Return:			None.													**/
/** Limitation:		This function need to override when use external flash	**/
/**					ROM data.												**/
/*****************************************************************************/
void SGUI_Text_ReadFontData(SGUI_FONT_SIZE eFontSize, SGUI_UINT16 uiCharacterCode, SGUI_BYTE* pFontDataBuffer, SGUI_UINT32 uiFontBufferSize)
{
	// Need rewrite this function for use each different external font data.
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
//#if(_SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_ > 0)
	SGUI_UINT16					uiCharacterIndex;
	SGUI_UINT					uiFontWidth, uiFontHeight;
	SGUI_UINT32					uiCharacterDataSize;
	SGUI_BYTE*					puiFontDataArrayPointer;
	SGUI_UINT					i;
//#endif
	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
//#if(_SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_ > 0)
	// Get character font data index in font table.
	uiCharacterIndex =			SGUI_Text_GetCharacterTableIndex(uiCharacterCode);
	// Calculates the byte used by each half-width character.
	// Get font graphics size.
	uiFontWidth =				g_stFontSize[eFontSize].Width;
	uiFontHeight =				g_stFontSize[eFontSize].Height;
	uiCharacterDataSize =		(((uiFontHeight-1)/8)+1)*uiFontWidth;
//#endif
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
//#if(_SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_ > 0)
	if((NULL != pFontDataBuffer) && (0 < uiFontBufferSize))
	{
		switch(eFontSize)
		{
			case(SGUI_FONT_SIZE_H12):
			{
				puiFontDataArrayPointer = (SGUI_BYTE*)SGUI_FONT_H12;
				break;
			}
			case(SGUI_FONT_SIZE_H16):
			{
				puiFontDataArrayPointer = (SGUI_BYTE*)SGUI_FONT_H16;
				break;
			}
			/*
			case(FONT_SIZE_H24):
			{
				puiFontDataArrayPointer = (SGUI_BYTE*)FONT_H24;
				break;
			}
			case(FONT_SIZE_H32):
			{
				puiFontDataArrayPointer = (SGUI_BYTE*)SGUI_FONT_H32;
				break;
			}
			*/
			default:
			{
				// In other cases, the basic font is used.
				puiFontDataArrayPointer = (SGUI_BYTE*)BASIC_FONT_DATA;
				uiFontWidth = 6;
				uiFontHeight = 8;
			}
		}
		// Set character font data head pointer.
		puiFontDataArrayPointer = puiFontDataArrayPointer + (uiCharacterDataSize * uiCharacterIndex);
		// Check Buffer size area.
		if(uiCharacterCode > 127)
		{
			// Non-ASCII characters
			// For GB2312, full-width character use 2 bytes encoding.
			uiCharacterDataSize = uiCharacterDataSize * 2;
		}
		// Copy data to buffer.
		for(i=0; ((i<uiCharacterDataSize)&&(i<uiFontBufferSize)); i++)
		{
			*(pFontDataBuffer+i) = *(puiFontDataArrayPointer + i);
		}
	}
//#endif
}

/*****************************************************************************/
/** Function Name:	SGUI_Text_GetCharacterTableIndex						**/
/** Purpose:		Get character index in font mode table.					**/
/** Params:																	**/
/**	@uiCharacterCode[in]: Data buffer pointer of read font data.			**/
/** Return:			Font mode data index in table.							**/
/** Limitation:		This function need to override when use external flash	**/
/**					ROM data, user must redefine the data offset by the		**/
/**					actual data.											**/
/*****************************************************************************/
SGUI_SIZE SGUI_Text_GetCharacterTableIndex(SGUI_UINT16 uiCharacterCode)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_UINT16					uiCharacterCodeHighByte;
	SGUI_UINT16					uiCharacterCodeLowByte;
	SGUI_SIZE					uiFontTableIndex;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	uiCharacterCodeHighByte =	(uiCharacterCode >> 8) & 0x00FF;
	uiCharacterCodeLowByte =	uiCharacterCode & 0x00FF;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// ASCII code.
	if((0 == uiCharacterCodeHighByte) && (uiCharacterCodeLowByte < 128))
	{
		uiFontTableIndex = (uiCharacterCodeLowByte + FONT_LIB_OFFSET_ASCII);
	}
	// GB2312 punctuation
	else if((0xAA > uiCharacterCodeHighByte) && (0xA0 < uiCharacterCodeHighByte))
	{
		uiFontTableIndex = ((((uiCharacterCodeHighByte-0xA1)*94 + (uiCharacterCodeLowByte-0xA1))*2) + FONT_LIB_OFFSET_GB2312_SYMBOL);
	}
	// GB2312 level one character.
	else if((0xF8 > uiCharacterCodeHighByte) && (0xAF < uiCharacterCodeHighByte))
	{
		uiFontTableIndex = ((((uiCharacterCodeHighByte-0xB0)*94 + (uiCharacterCodeLowByte-0xA1))*2) + FONT_LIB_OFFSET_GB2312_CHARL1);
	}
	// Other to return full width space.
	else
	{
        uiFontTableIndex = FONT_LIB_OFFSET_GB2312_SYMBOL;	// Full-size space.
	}

	return uiFontTableIndex;
}

/*****************************************************************************/
/** Function Name:	SGUI_Text_GetTextGraphicsWidth							**/
/** Purpose:		Get text displayed full width.							**/
/** Params:																	**/
/**	@szText[in]: 		Text pointer.										**/
/**	@eFontSize[in]:		Font size, usually means font height.				**/
/** Return:			None.													**/
/** Notice:			Only used with Equal-width characters. 					**/
/*****************************************************************************/
SGUI_SIZE SGUI_Text_GetTextGraphicsWidth(SGUI_PCSZSTR szText, SGUI_FONT_SIZE eFontSize)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
    SGUI_SIZE					uiTextHalfCharLength;
    SGUI_UINT16					uiHalfFontWidth;
    SGUI_SIZE					uiTextGraphicsWidth;
	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	uiTextHalfCharLength =		0;
	uiTextGraphicsWidth =		0;
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != szText)
	{
		uiTextHalfCharLength = SGUI_Common_StringLength(ENCODE(szText));
		uiHalfFontWidth = g_stFontSize[eFontSize].Width;
		uiTextGraphicsWidth = uiHalfFontWidth * uiTextHalfCharLength;
	}
	return uiTextGraphicsWidth;
}


/*****************************************************************************/
/** Function Name:	SGUI_Text_GetMultiLineTextLines							**/
/** Purpose:		Get a string's lines in a fixed width area.				**/
/** Resources:		None.													**/
/** Params:																	**/
/**	@szNoticeText[in]:	Notice text resource.								**/
/**	@uiHalfWidthCharInLine[in]: Max number of half-width character in each	**/
/**						line.												**/
/** Return:			String lines.											**/
/** Notice:			None.													**/
/*****************************************************************************/
SGUI_SIZE SGUI_Text_GetMultiLineTextLines(SGUI_PCSZSTR szNoticeText, SGUI_SIZE uiHalfWidthCharInLine)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_SIZE					uiLineCount, uiLineByteCount;
	SGUI_PCSZSTR				pcCur;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	uiLineByteCount =			0;
	uiLineCount =				1;
	pcCur =						ENCODE(szNoticeText);

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pcCur)
	{
		while('\0' != *pcCur)
		{
			if(*pcCur == '\n')
			{
                if(uiLineByteCount > 0)
				{
					// Change lines.
					uiLineCount ++;
					uiLineByteCount = 0;
				}
				else
				{
					// Ignore change lines in line start.
				}
				pcCur++;
				continue;
			}

			if((uint8_t)(*pcCur) < 0x7F)
			{
				if(uiLineByteCount<uiHalfWidthCharInLine)
				{
					uiLineByteCount++;
				}
				else
				{
					uiLineByteCount = 1;
					uiLineCount++;
				}
				pcCur++;
			}
			// Process with GB2312.
			else if(((uint8_t)(*pcCur) >= 0xA1) && ((uint8_t)(*pcCur) <= 0xF7))
			{
				//GB2312
				if((uiHalfWidthCharInLine-uiLineByteCount)>2)
				{
					uiLineByteCount+=2;
				}
				else
				{
					uiLineByteCount = 2;
					uiLineCount++;
				}
				pcCur+=2;
			}
			// Invalid character
			else
			{
				pcCur++;
			}
		}
	}
	return uiLineCount;
}
