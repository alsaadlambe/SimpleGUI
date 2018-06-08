#ifndef __INCLUDE_DEMOACTIONS_H__
#define __INCLUDE_DEMOACTIONS_H__

#include <stdbool.h>

#define		KEY_NONE									    (0x00)
// Key value definition.
#define		KEY_VALUE_TAB									(0x01)
#define		KEY_VALUE_ENTER									(0x02)
#define		KEY_VALUE_ESC									(0x03)
#define		KEY_VALUE_SPACE									(0x04)
#define		KEY_VALUE_LEFT									(0x05)
#define		KEY_VALUE_UP									(0x06)
#define		KEY_VALUE_RIGHT									(0x07)
#define		KEY_VALUE_DOWN									(0x08)

// User option flag value define
#define		KEY_OPTION_CTRL									(0x10)
#define		KEY_OPTION_ALT									(0x20)
#define		KEY_OPTION_SHIFT								(0x40)

void        DemoAction_InitializeUsart1(void);
void        DemoAction_TimerEvent(void);
void        DemoAction_UsartReceiveEvent(uint8_t cbReceiveByte);
void        DemoAction_RTCUpdateEventProcess(void);

#endif /* __INCLUDE_DEMOACTIONS_H__ */
