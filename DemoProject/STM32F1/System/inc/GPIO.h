#ifndef _INCLUDE_SYSTEM_GPIO_H_
#define _INCLUDE_SYSTEM_GPIO_H_
#include "stm32f10x.h"

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-	GPIO端口寄存器操作定义											-*/
/*-	方式一															-*/
/*-	使用示例：														-*/
/*-	#define	KEY		PA_IN(0)										-*/
/*-	#define	LED		PB_OUT(1)										-*/
/*-																	-*/
/*-	或																-*/
/*-																	-*/
/*-	#define LED		BIT_ADDR(GPIOA_ODR_Addr,0)						-*/
/*-	#define	KEY		BIT_ADDR(GPIOA_IDR_Addr,1) 						-*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

//GPIO寄存器基地址定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
//端口输出寄存器地址映射
#define GPIOA_ODR_Addr		(GPIOA_BASE+12) //0x4001080C
#define GPIOB_ODR_Addr		(GPIOB_BASE+12) //0x40010C0C
#define GPIOC_ODR_Addr		(GPIOC_BASE+12) //0x4001100C
#define GPIOD_ODR_Addr		(GPIOD_BASE+12) //0x4001140C
#define GPIOE_ODR_Addr		(GPIOE_BASE+12) //0x4001180C
#define GPIOF_ODR_Addr		(GPIOF_BASE+12) //0x40011A0C
#define GPIOG_ODR_Addr		(GPIOG_BASE+12) //0x40011E0C
//端口输入寄存器地址映射
#define GPIOA_IDR_Addr		(GPIOA_BASE+8) //0x40010808
#define GPIOB_IDR_Addr		(GPIOB_BASE+8) //0x40010C08
#define GPIOC_IDR_Addr		(GPIOC_BASE+8) //0x40011008
#define GPIOD_IDR_Addr		(GPIOD_BASE+8) //0x40011408
#define GPIOE_IDR_Addr		(GPIOE_BASE+8) //0x40011808
#define GPIOF_IDR_Addr		(GPIOF_BASE+8) //0x40011A08
#define GPIOG_IDR_Addr		(GPIOG_BASE+8) //0x40011E08

//GPIO端口操作宏定义
//GPIO A
#define PA_OUT(n)			BIT_ADDR(GPIOA_ODR_Addr,n)  //输出
#define PA_IN(n)			BIT_ADDR(GPIOA_IDR_Addr,n)  //输入
//GPIO B
#define PB_OUT(n)			BIT_ADDR(GPIOB_ODR_Addr,n)  //输出
#define PB_IN(n)			BIT_ADDR(GPIOB_IDR_Addr,n)  //输入
//GPIO C
#define PC_OUT(n)			BIT_ADDR(GPIOC_ODR_Addr,n)  //输出
#define PC_IN(n)			BIT_ADDR(GPIOC_IDR_Addr,n)  //输入
//GPIO D
#define PD_OUT(n)			BIT_ADDR(GPIOD_ODR_Addr,n)  //输出
#define PD_IN(n)			BIT_ADDR(GPIOD_IDR_Addr,n)  //输入
//GPIO E
#define PE_OUT(n)			BIT_ADDR(GPIOE_ODR_Addr,n)  //输出
#define PE_IN(n)			BIT_ADDR(GPIOE_IDR_Addr,n)  //输入
//GPIO F
#define PF_OUT(n)			BIT_ADDR(GPIOF_ODR_Addr,n)  //输出
#define PF_IN(n)			BIT_ADDR(GPIOF_IDR_Addr,n)  //输入
//GPIO G
#define PG_OUT(n)			BIT_ADDR(GPIOG_ODR_Addr,n)  //输出
#define PG_IN(n)			BIT_ADDR(GPIOG_IDR_Addr,n)  //输入


/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-	GPIO端口寄存器操作定义											-*/
/*-	方式二															-*/
/*-	使用示例：														-*/
/*-	#define LED1	BIT_BAND(&GPIOB->ODR,0)							-*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

/****************第二种位操作，效率低，但使用方便，IO 可以非操作（例如： LED = !LED)****************/

#define HWREG(x)  (*((volatile unsigned long *)(x)))
#define HWREGBITW(x, b) HWREG( ((unsigned long)(x) & 0xF0000000) | 0x02000000 | (((unsigned long)(x) & 0x000FFFFF) << 5) | ((b) << 2))

#define BIT_BAND(A,B) HWREGBITW(A,B)

typedef enum
{
    GPIO_STATE_LOW,
    GPIO_STATE_HIGH,
}GPIO_STATE;

// Data type define.
typedef struct
{
	uint32_t			PortRCC;
	GPIO_TypeDef*		Port;
	uint16_t			Pin;
	GPIOMode_TypeDef	Mode;
	GPIOSpeed_TypeDef	Speed;
	GPIO_STATE          InitState;
}GPIO_OBJECT_STRUCT;

// Function declare
void		GPIO_Initialize(const GPIO_OBJECT_STRUCT* pstGPIODataList, uint16_t uiArrayListLength);
GPIO_STATE  GPIO_ReadPin(const GPIO_OBJECT_STRUCT* astPinList, uint8_t uiIndex);
void		GPIO_SetPinHigh(const GPIO_OBJECT_STRUCT* astPinList, uint8_t uiIndex);
void		GPIO_SetPinLow(const GPIO_OBJECT_STRUCT* astPinList, uint8_t uiIndex);

extern GPIO_OBJECT_STRUCT	astGPIOInitData[];

#endif
