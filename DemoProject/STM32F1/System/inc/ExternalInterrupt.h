#ifndef _INCLUDE_SYSTEM_EXTERNAL_INTERRUPT_H_
#define _INCLUDE_SYSTEM_EXTERNAL_INTERRUPT_H_
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "GPIO.h"

// Data type define.
typedef struct
{
	uint8_t				SourcePort;
	uint8_t				SourcePin;
	uint16_t			EXTILine;
	EXTIMode_TypeDef	Mode;
	EXTITrigger_TypeDef	Trigger;
}EXTI_STRUCT;

void		ExInt_Initialize(void);

#endif //_INCLUDE_SYSTEM_EXTERNAL_INTERRUPT_H_
