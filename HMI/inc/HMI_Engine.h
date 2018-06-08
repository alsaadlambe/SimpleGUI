#ifndef __INCLUDE_HMI_ENGINE_H__
#define __INCLUDE_HMI_ENGINE_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Typedef.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
// GoBack history size
#define		HMI_SCREEN_HISTORY_MAX		    				(20)
#define     HMI_SCREEN_LIST_MAX                             (20)
#define     HMI_EVENT_KEY_VALUE_LENGTH_MAX                  (4)
// Parameter post label.
#define		HMI_SCREEN_ID_ANY								(-1)	// This label means parameter will posted to every screen.

#define		HMI_PROCESS_SUCCESSFUL(RESULT)					(RESULT >= 0)

// Start screen definition
#define		HMI_SCREEN_START								(0)

//=======================================================================//
//= Data type definition.											    =//
//=======================================================================//
// HMI process result
typedef enum
{
    // Abnormal.
    HMI_RET_ERROR =                 -1,
    HMI_RET_INVALID_DATA =          -2,
    HMI_RET_NO_SPACE =              -3,
    HMI_RET_MEMORY_ERR =            -4,
    HMI_RET_UNDEFINED_ACTION =      -5,
    HMI_RET_ERROR_STATE =           -6,
    // Normal.
    HMI_RET_NORMAL =                0,
    HMI_RET_INITIALIZE,
    HMI_RET_CONFIRM,
    HMI_RET_CANCEL,
    HMI_RET_FOLLOWUP,
    HMI_RET_FALLBACK,
    HMI_RET_INPROCESSING,
    HMI_RET_NOACTION,
}HMI_ENGINE_RESULT;

// HMI engine state.
typedef enum
{
    HMI_ENGINE_STATE_UNINIT =       0,
    HMI_ENGINE_STATE_READY,
    HMI_ENGINE_STATE_INPROCESS,
    HMI_ENGINE_STATE_ERROR,
}HMI_ENGINE_STATE;

// HMI event type.
typedef enum
{
    HMI_ENGINE_EVENT_BASE =         0,
    HMI_ENGINE_EVENT_UNKNOW =       HMI_ENGINE_EVENT_BASE,
    HMI_ENGINE_EVENT_ACTION,		// Turn to a new screen.
    HMI_ENGINE_EVENT_DATA,			// Refresh screen display.

    HMI_ENGINE_EVENT_MAX,
}HMI_EVENT_TYPE;

// HMI action type
typedef enum
{
	HMI_ENGINE_ACTION_BASE =		0,
	HMI_ENGINE_ACTION_UNKNOW =		HMI_ENGINE_ACTION_BASE,
	// User defined action type.
	HMI_ENGINE_ACTION_KEY_PRESS,
	HMI_ENGINE_ACTION_ON_TIMER,
	HMI_ENGINE_ACTION_ON_TIMER_RTC,
	// User defined action above.
	HMI_ENGINE_ACTION_MAX
}HMI_EVENT_ACTION;

// HMI event data.
typedef struct
{
    HMI_EVENT_ACTION				Action;
    SGUI_BYTE*						Data;
}HMI_EVENT;

// Screen action interface function pointer structure.
typedef struct
{
    HMI_ENGINE_RESULT               (*Initialize)(void); // Initialize screen data and parameter.
	HMI_ENGINE_RESULT               (*Prepare)(const void* pstParameters); // Do some thing before current screen display.
	HMI_ENGINE_RESULT               (*Repaint)(const void* pstParameters); // Repaint screen if needed.
	HMI_ENGINE_RESULT               (*ProcessEvent)(HMI_EVENT_TYPE eEventType, const HMI_EVENT* pstEvent); // Process event.
	HMI_ENGINE_RESULT               (*PostProcess)(SGUI_INT iActionResult);
	//void*                           (*InstanceData)(void); // Get current screen instance data pointer.
}HMI_SCREEN_ACTION;
// Screen data structure.
typedef struct
{
	SGUI_INT						ScreenID;
	HMI_SCREEN_ACTION*  			Actions;
}HMI_SCREEN_OBJECT;

typedef struct
{
    SGUI_INT                        TopIndex;
    HMI_SCREEN_OBJECT*              Item[HMI_SCREEN_HISTORY_MAX];
}HMI_HISTORY_STACK;

typedef struct
{
    HMI_SCREEN_OBJECT*              Screen[HMI_SCREEN_LIST_MAX];
    SGUI_INT                        ScreenCount;
    HMI_SCREEN_OBJECT*              CurrentScreenObject;
    HMI_HISTORY_STACK               History;
    HMI_ENGINE_STATE                State;
}HMI_ENGINE_OBJECT;

//=======================================================================//
//= Public function declaration.									    =//
//=======================================================================//
HMI_ENGINE_RESULT   HMI_PrepareEngine(HMI_ENGINE_OBJECT* pstHMIEngineObject);
HMI_ENGINE_RESULT   HMI_AddScreen(HMI_ENGINE_OBJECT* pstHMIEngineObject, HMI_SCREEN_OBJECT* pstScreenObject, SGUI_BOOL bInitializeScreenObject);
HMI_ENGINE_RESULT   HMI_ActiveEngine(HMI_ENGINE_OBJECT* pstHMIEngineObject, SGUI_INT iScreenID);
HMI_ENGINE_RESULT   HMI_StartEngine(const void* pstParameters);
HMI_ENGINE_RESULT   HMI_ProcessEvent(HMI_EVENT_TYPE eEventType, const HMI_EVENT* pstEvent);
HMI_ENGINE_RESULT	HMI_PostProcess(SGUI_INT iActionResult);
HMI_ENGINE_RESULT   HMI_Goto(SGUI_INT iDestScreenID, const void* pstParameters);
HMI_ENGINE_RESULT   HMI_GoBack(const void* pstParameters);
#endif // __INCLUDE_HMI_ENGINE_H__
