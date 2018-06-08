#ifndef __INCLUDE_VIRTUAL_DEVICE_INTERFACE_H__
#define __INCLUDE_VIRTUAL_DEVICE_INTERFACE_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <stdint.h>

//=======================================================================//
//= User definition.												    =//
//=======================================================================//
// Key value definition.
#define		KEY_VALUE_TAB									(0x0009)
#define		KEY_VALUE_ENTER									(0x000D)
#define		KEY_VALUE_ESC									(0x001B)
#define		KEY_VALUE_SPACE									(0x0020)
#define		KEY_VALUE_LEFT									(0x013A)
#define		KEY_VALUE_UP									(0x013B)
#define		KEY_VALUE_RIGHT									(0x013C)
#define		KEY_VALUE_DOWN									(0x013D)
#define		KEY_VALUE_F1									(0x0154)
#define		KEY_VALUE_F2									(0x0155)
#define		KEY_VALUE_F3									(0x0156)
#define		KEY_VALUE_F4									(0x0157)
#define		KEY_VALUE_F5									(0x0158)
#define		KEY_VALUE_F6									(0x0159)
#define		KEY_VALUE_F7									(0x015A)
#define		KEY_VALUE_F8									(0x015B)
#define		KEY_VALUE_F9									(0x015C)
#define		KEY_VALUE_F10									(0x015D)
#define		KEY_VALUE_F11									(0x015E)
#define		KEY_VALUE_F12									(0x015F)
#define		KEY_VALUE_ENTER_PAD								(0x0172)
// User option flag value define
#define		KEY_OPTION_NONE									(0x0000)
#define		KEY_OPTION_CTRL									(0x0001)
#define		KEY_OPTION_ALT									(0x0002)
#define		KEY_OPTION_SHIFT								(0x0004)

#define		KEY_PRESS_EVENT_VALUE_MAX						(4)

// Parameter
#define     LCD_SIZE_WIDTH					                (128)
#define     LCD_SIZE_HEIGHT					                (64)

#if LCD_SIZE_WIDTH < 16
#error Define width of LCD size must greater then 16.
#endif
#if LCD_SIZE_HEIGHT < 16
#error Define height of LCD size must greater then 16.
#endif

//Common operation
#define SET_PAGE_BIT(PAGE, Bit)		((PAGE) = (PAGE) | (0x01 << (Bit)))
#define CLR_PAGE_BIT(PAGE, Bit)		((PAGE) = (PAGE) & (~(0x01 << (Bit))))
#define GET_PAGE_BIT(PAGE, Bit)		((((PAGE) & (0x01 << (Bit)))>0)?1:0)

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
void        VDIF_SetPixel(uint32_t uiPosX, uint32_t uiPosY, uint32_t uiPixelValue);
uint32_t    VDIF_GetPixel(uint32_t uiPosX, uint32_t uiPosY);
void        VDIF_RefreshDisplay(void);
void        VDIF_ClearDisplay(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif // __INCLUDE_VIRTUAL_DEVICE_INTERFACE_H__
