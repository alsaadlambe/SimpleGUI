#ifndef _INCLUDE_SYSTEM_INITIALIZE_H_
#define _INCLUDE_SYSTEM_INITIALIZE_H_
#include "stm32f10x.h"

typedef enum
{
	DEBUG_NONE = 0,
	DEBUG_SWD,
	DEBUG_JTAG,
}DEBUG_PORT_TYPE;

void		DebugPort_Initialize(DEBUG_PORT_TYPE e_DebugPortType);
ErrorStatus	HSEClocks_Initialize(uint32_t ui_PLLMul);
void		NVIC_Initialize(uint32_t ui_NVICPriorityGroup);


#endif
