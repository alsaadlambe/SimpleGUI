#ifndef __INCLUDE_USER_ACTION_INTERFACE__
#define __INCLUDE_USER_ACTION_INTERFACE__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <stdint.h>
#include <stdbool.h>

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
void UAIF_OnInitialize(void);
void UAIF_OnKeyPress(bool bShift, bool bCtrl, bool bAlt, uint16_t uiKeyCode);
void UAIF_OnTimerEventProcess(void);
void UAIF_OnRTCUpdateEventProcess(uint16_t uiYear, uint16_t uiMonth, uint16_t uiDay, uint16_t uiHour, uint16_t uiMinute, uint16_t uiSecond);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // __INCLUDE_USER_ACTION_INTERFACE__
