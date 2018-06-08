#ifndef __INCLUDE_GUI_COMMON_H__
#define __INCLUDE_GUI_COMMON_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <stdbool.h>
#include "SGUI_Typedef.h"
#if (_SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_ > 0)
#include "VirtualDeviceInterface.h"
#else
// Insert platform driver head file here.
#endif //_SIMPLE_GUI_ENABLE_SIMULATOR_

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define		SGUI_NUMBER_STR_LENGTH_MAX		(12)
#define		SGUI_ISDIGIT(C)					(((C>='0')&&(C<='9'))?true:false)
#define		SGUI_ISALPHA(C)					((((C>='A')&&(C<='Z'))||((C>='a')&&(C<='z')))?true:false)
#define		SGUI_ISHEXDIGIT(C)				((((C>='A')&&(C<='F'))||((C>='a')&&(C<='f'))||((C>='0')&&(C<='9')))?true:false)
#define		SGUI_ISUPPER(C)					(((C>='A')&&(C<='Z'))?true:false)
#define		SGUI_TOUPPER(C)					(SGUI_ISUPPER(C)?(C):(C-32))
#define		SGUI_SWAP(A, B)					{A=A^B; B=A^B; A=A^B;}

#if   defined ( __CC_ARM )
	#define __ASM			__asm						// asm keyword for ARM Compiler.
	#define __INLINE		__inline					// inline keyword for ARM Compiler.
	#pragma	diag_suppress	870							// Disabled "multibyte character sequence" warning.

#elif defined ( __ICCARM__ )
  #define __ASM           __asm							// < asm keyword for IAR Compiler.
  #define __INLINE        inline						// inline keyword for IAR Compiler. Only available in High optimization mode!

#elif defined ( __GNUC__ )
  #define __ASM            __asm						// asm keyword for GNU Compiler.
  #define __INLINE         inline 						// inline keyword for GNU Compiler.

#elif defined ( __TASKING__ )
  #define __ASM            __asm 						// asm keyword for TASKING Compiler.
  #define __INLINE         inline						// inline keyword for TASKING Compiler.
#endif

//=======================================================================//
//= Public variable declaration.									    =//
//=======================================================================//
//#if (_SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_ > 0)
extern SGUI_CBYTE		SGUI_FONT_H8[];
extern SGUI_CBYTE		SGUI_FONT_H12[];
extern SGUI_CBYTE		SGUI_FONT_H16[];
extern SGUI_CBYTE		SGUI_FONT_H32[];
//#endif

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
#if (_SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_ > 0) && (_SIMPLE_GUI_ENABLE_ICONV_GB2312_ > 0)
// Only used when running in simulated environment and text encode need convert to GB2312.
SGUI_PSZSTR				SGUI_Common_EncodeConvert(SGUI_PCSZSTR szSourceEncode, SGUI_PSZSTR szDestinationEncode, SGUI_PSZSTR szSource);
#define		_SIMPLE_GUI_ENCODE_BUFFER_SIZE	(512)
#define		ENCODE(S)						(SGUI_Common_EncodeConvert("UTF-8", "GB2312", (char *)S))
#else
#define		ENCODE(S)						(S)
#endif

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
SGUI_SIZE               SGUI_Common_IntegerToStringWithDecimalPoint(SGUI_INT iInteger, SGUI_UINT uiDecimalPlaces, SGUI_PSZSTR pszStringBuffer, SGUI_INT iAlignment, SGUI_CHAR cFillCharacter);
SGUI_SIZE               SGUI_Common_IntegerToString(SGUI_INT iInteger, SGUI_PSZSTR pszStringBuffer, SGUI_UINT uiBase, SGUI_INT iAlignment, SGUI_CHAR cFillCharacter);
SGUI_UINT               SGUI_Common_ConvertStringToUnsignedInteger(SGUI_PSZSTR szString, SGUI_CHAR** ppcEndPointer, SGUI_UINT uiBase);
SGUI_INT                SGUI_Common_ConvertStringToInteger(SGUI_PSZSTR szString, SGUI_CHAR** ppcEndPointer, SGUI_UINT uiBase);
#if (_SIMPLE_GUI_ENABLE_DYNAMIC_MEMORY_ > 0)
void*					SGUI_Common_Allocate(SGUI_SIZE uiSize);
void					SGUI_Common_Free(void* pFreePointer);
#endif
void*                   SGUI_Common_MemoryCopy(void* pDest, const void* pSrc, SGUI_UINT uiSize);
void                    SGUI_Common_MemorySet(void* pMemoryPtr, SGUI_BYTE iSetValue, SGUI_UINT uiSize);
SGUI_SIZE               SGUI_Common_StringLength(SGUI_PCSZSTR szString);
SGUI_PSZSTR             SGUI_Common_StringCopy(SGUI_PSZSTR szDest, SGUI_PCSZSTR szSrc);
SGUI_PSZSTR             SGUI_Common_StringLengthCopy(SGUI_PSZSTR szDest, SGUI_PCSZSTR szSrc, SGUI_SIZE uiSize);
void                    SGUI_Common_GetNowTime(SGUI_TIME* pstTime);
void                    SGUI_Common_RefreshScreen(void);
void                    SGUI_Common_ReadFlashROM(SGUI_ROM_ADDRESS uiAddressHead, SGUI_SIZE uiDataLength, SGUI_BYTE* pBuffer);
void                    SGUI_Common_Delay(SGUI_UINT32 uiTimeMs);

#endif // __INCLUDE_GUI_COMMON_H__
