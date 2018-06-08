#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "DemoActions.h"
#include "DemoProc.h"
#include "HMI_Engine.h"
#include "SGUI_Basic.h"
#include "GPIO.h"
#include "Usart.h"
#include "RTC.h"

SGUI_INT    s_GraphDemoData[72] = {
//   5,  10,  15,  20,  25,  30,  35,  40,  45,  50,  55,  60,  65,  70,  75,  80,  85,  90,
     4,   9,  13,  17,  21,  25,  29,  32,  35,  38,  41,  43,  45,  47,  48,  49,  50,  50,

//  95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180,
    50,  49,  48,  47,  45,  43,  41,  38,  35,  32,  29,  25,  21,  17,  13,   9,   4,   0,

// 185, 190, 195, 200, 205, 210, 215, 220, 225, 230, 235, 240, 245, 250, 255, 260, 265, 270,
    -4,  -9, -13, -17, -21, -25, -29, -32, -35, -38, -41, -43, -45, -47, -48, -49, -50, -50,

// 275, 280, 285, 290, 295, 300, 305, 310, 315, 320, 325, 330, 335, 340, 345, 350, 355, 360,
   -50, -49, -48, -47, -45, -43, -41, -38, -35, -32, -29, -25, -21, -17, -13,  -9,  -4,   0
};

GPIO_OBJECT_STRUCT	astUSART1GPIO[] =
{
	{RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_9, GPIO_Mode_AF_PP, GPIO_Speed_2MHz, GPIO_STATE_HIGH},
	{RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_10, GPIO_Mode_IN_FLOATING, GPIO_Speed_2MHz, GPIO_STATE_HIGH},
};


void DemoAction_InitializeUsart1(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure USART1 GPIO */
	GPIO_Initialize(astUSART1GPIO, 2);

    /* Configure Usart1 interrupt. */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);

	/* Configure USART1 controller */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//Enable USART device clock.
	USART_Config(USART1, 115200, true);         //Initialize USART and enable USART interrupt.
}


void DemoAction_TimerEvent(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_EVENT				stEvent;
	static SGUI_INT			iDemoGraphDataIndex = 0;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	stEvent.Action =		HMI_ENGINE_ACTION_ON_TIMER;
	stEvent.Data =			NULL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	stEvent.Data = (SGUI_BYTE*)(&s_GraphDemoData[iDemoGraphDataIndex]);
	iDemoGraphDataIndex++;
	iDemoGraphDataIndex = iDemoGraphDataIndex % 72;
	// Post timer event.
	EventProcess(HMI_ENGINE_EVENT_ACTION, &stEvent);
	SGUI_Basic_RefreshDisplay();
}

void DemoAction_UsartReceiveEvent(uint8_t cbReceiveByte)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_UINT16				arruiPressedKey[HMI_EVENT_KEY_VALUE_LENGTH_MAX];
	HMI_EVENT				stEvent;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	arruiPressedKey[0] = KEY_NONE;
	stEvent.Action = HMI_ENGINE_ACTION_KEY_PRESS;
	arruiPressedKey[0] = cbReceiveByte & 0xF0;
	arruiPressedKey[1] = cbReceiveByte & 0x0F;
	stEvent.Data = (SGUI_BYTE*)arruiPressedKey;
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Call demo process.
	EventProcess(HMI_ENGINE_EVENT_ACTION, &stEvent);
	SGUI_Basic_RefreshDisplay();
}

void DemoAction_RTCUpdateEventProcess(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	SGUI_TIME               stRTCTime;
	HMI_EVENT				stEvent;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	stRTCTime.Year =		g_stCleandar.tm_year;
	stRTCTime.Month =		g_stCleandar.tm_mon;
	stRTCTime.Day =			g_stCleandar.tm_mday;
	stRTCTime.Hour =		g_stCleandar.tm_hour;
	stRTCTime.Minute =		g_stCleandar.tm_min;
	stRTCTime.Second =		g_stCleandar.tm_sec;
	stEvent.Action =		HMI_ENGINE_ACTION_ON_TIMER_RTC;
	stEvent.Data = 			(SGUI_BYTE*)(&stRTCTime);

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Post RTC update message to a screen.
	EventProcess(HMI_ENGINE_EVENT_DATA, &stEvent);
	SGUI_Basic_RefreshDisplay();
}

