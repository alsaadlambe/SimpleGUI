#include "SSD1306_SPI.h"
#include "ASCII_Font8Pix.h"
#include "GPIO.h"


#define     SSD1306_SPI_PIN_IDX_CLK         (0)
#define     SSD1306_SPI_PIN_IDX_DIO         (1)
#define     SSD1306_SPI_PIN_IDX_CS          (2)
#define     SSD1306_SPI_PIN_IDX_DC          (3)

#define     SSD1306_PIN_HIGH(IDX)           GPIO_SetPinHigh(arrstSSD1306_GPIO, IDX)
#define     SSD1306_PIN_LOW(IDX)            GPIO_SetPinLow(arrstSSD1306_GPIO, IDX)

#define     SSD1306_DATA_MODE_CMD()         SSD1306_PIN_LOW(SSD1306_SPI_PIN_IDX_DC)
#define     SSD1306_DATA_MODE_DAT()         SSD1306_PIN_HIGH(SSD1306_SPI_PIN_IDX_DC)

#define     SSD1306_CS_ENABLE()             SSD1306_PIN_LOW(SSD1306_SPI_PIN_IDX_CS)
#define     SSD1306_CS_DISABLE()            SSD1306_PIN_HIGH(SSD1306_SPI_PIN_IDX_CS)

static GPIO_OBJECT_STRUCT	arrstSSD1306_GPIO[] =
{
	{RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_0, GPIO_Mode_Out_PP, GPIO_Speed_2MHz, GPIO_STATE_HIGH},
	{RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_1, GPIO_Mode_Out_PP, GPIO_Speed_2MHz, GPIO_STATE_HIGH},
	{RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP, GPIO_Speed_2MHz, GPIO_STATE_HIGH},
	{RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_3, GPIO_Mode_Out_PP, GPIO_Speed_2MHz, GPIO_STATE_HIGH},
};

static void SSD1306_SPI_WriteByte(uint8_t uiData);;

static const uint8_t arrLogoBitMap[] = {
0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,
0xFF,0x00,0x00,0x00,0x00,0xFE,0xFE,0x86,0x86,0x86,0xFE,0xFE,0x00,0x00,0xFE,0xFE,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xFE,0xFE,0x00,0x00,0x40,0xE0,0x70,0x3F,
0x1F,0x18,0x18,0x18,0x18,0x00,0x0C,0x2C,0x7C,0xEC,0x4C,0x0D,0x0F,0x0E,0x4C,0xEC,0x7C,0x2C,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0x60,0x60,0x60,
0xE0,0xE0,0x60,0x60,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xF0,0x30,0xFF,0xFF,0x30,0xF0,0xF0,0x00,0xC0,0xC6,0xD6,0xFE,0xF6,0xC6,0xFF,0xFF,0xC3,0xF3,
0xFB,0xD3,0xC3,0xC0,0x00,0xF0,0xF0,0x30,0xFF,0xFF,0x30,0xF0,0xF0,0x00,0x4C,0xEC,0xCC,0x0C,0x8C,0xED,0x7F,0x9E,0xEC,0x6C,0x8C,0xCC,0xEC,0x4C,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0xFF,0xFF,0x61,0x61,0x61,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0x18,0x18,0x18,0x78,0xF8,0x98,0x18,0x18,0x9F,0xDF,0xE0,0x40,0x00,0x63,0x63,0x63,
0xFF,0xFF,0x63,0x63,0x00,0x03,0x03,0xFB,0xFB,0x5B,0x5B,0x5B,0x5B,0x5B,0x5B,0x5B,0xFB,0xFB,0x03,0x03,0x00,0x00,0x00,0x00,0xFC,0xFF,0x83,0x01,0x80,0xC0,0xF0,0x3E,
0x0F,0x01,0x00,0x00,0x00,0x81,0xC3,0xFF,0x3E,0x00,0x00,0x00,0x00,0x7F,0x7F,0x60,0xFF,0xFF,0x60,0x7F,0x7F,0x00,0x10,0x30,0x38,0x1C,0x0E,0x87,0x7F,0x7F,0x07,0x0E,
0x1C,0x38,0x30,0x10,0x00,0x3F,0x3F,0x30,0xFF,0xFF,0x30,0x3F,0x3F,0x10,0x38,0x1C,0x0E,0x34,0x39,0x3D,0xF7,0xF3,0x31,0x3C,0x3D,0x05,0x1E,0x18,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x7F,0x7F,0x18,0x18,0x18,0x7F,0x7F,0x00,0x40,0xFF,0xFF,0x70,0x38,0x1C,0x08,0x03,0x0F,0x1E,0x3F,0x73,0xE1,0xC0,0x40,0x00,0x00,0x00,0x20,
0x7F,0x3F,0x1C,0x0E,0x44,0xC0,0xE0,0x73,0x3B,0x1F,0x0F,0x03,0x03,0x7F,0xFF,0xC3,0xC3,0xC3,0xF8,0x78,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x03,0x01,0x00,0x00,
0x0C,0x0C,0x0C,0x06,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x30,0x30,0x3F,0x1F,0x18,0x3E,0x3E,0x60,0x7E,0x3E,0x00,0x7C,0xFD,0xC3,0xC7,0xCE,0xC4,0xF0,
0x70,0x0E,0x7E,0x78,0x18,0x18,0x18,0x18,0x1F,0x1F,0x18,0x1E,0x3E,0x30,0x03,0x03,0x03,0x03,0x03,0x03,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x10,0x10,0x90,0x60,0x00,0xE0,0x10,0x10,0x10,0xE0,0x00,0x00,0x20,0xF0,0x00,
0x00,0x00,0x30,0x10,0x90,0x50,0x30,0x00,0x40,0x20,0x9C,0x90,0x90,0x90,0xF0,0x90,0x90,0x90,0x10,0x00,0xE0,0x90,0x90,0x90,0x20,0x00,0x00,0x00,0xF8,0x48,0x48,0x48,
0x48,0x48,0x48,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x4C,0x48,0x58,0x48,0xD0,0x4C,0x48,0x58,0xC8,0x08,0x00,0x80,0x80,0x80,0x80,0xFC,0x40,0x40,0x40,
0x40,0x40,0x40,0x00,0x00,0xF8,0x40,0x7C,0x40,0xF8,0x48,0xC8,0x48,0x44,0xC4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0A,0x09,0x08,0x08,0x00,0x07,0x08,0x08,0x08,0x07,0x00,0x00,0x08,0x0F,0x08,
0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x04,0x04,0x07,0x04,0x04,0x04,0x3F,0x04,0x04,0x04,0x04,0x00,0x07,0x08,0x08,0x08,0x07,0x00,0x20,0x18,0x07,0x02,0x02,0x02,
0x02,0x22,0x22,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x27,0x15,0x0D,0x05,0x3F,0x05,0x05,0x25,0x25,0x1C,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,
0x20,0x20,0x38,0x00,0x20,0x1F,0x02,0x3E,0x10,0x0F,0x20,0x17,0x08,0x16,0x21,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x41,0x41,0x22,0x1C,0x00,0x7E,0x11,0x11,0x11,0x7E,0x00,0x7F,0x40,0x40,0x40,0x40,0x00,0x00,0x41,0x7F,0x41,0x00,0x00,0x7E,
0x11,0x11,0x11,0x7E,0x00,0x7F,0x04,0x08,0x10,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x40,0x41,0x3F,0x01,0x00,0x00,0x41,0x7F,0x41,0x00,0x00,0x61,0x51,0x49,
0x45,0x43,0x00,0x7F,0x08,0x08,0x08,0x7F,0x00,0x7F,0x49,0x49,0x49,0x41,0x00,0x7F,0x04,0x08,0x10,0x7F,0x00,0x61,0x51,0x49,0x45,0x43,0x00,0x7F,0x08,0x08,0x08,0x7F,
0x00,0x00,0x41,0x7F,0x41,0x00,0x00,0x7F,0x41,0x41,0x22,0x1C,0x00,0x3F,0x40,0x40,0x40,0x3F,0x00,0x00,0x41,0x7F,0x41,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xFF
};

/*****************************************************************************/
/** Function Name:	SSD1306_SPI_WriteByte.                                  **/
/** Purpose:			Simulate SPI communication timing to write a byte	**/
/**					to OLED controler.										**/
/** Resources:		Simulate SPI GPIO.										**/
/** Params:																	**/
/**	@uiData:			Byte will be write.									**/
/** Return:			None.													**/
/** Limitation:		None.													**/
/*****************************************************************************/
void SSD1306_SPI_WriteByte(uint8_t uiData)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	uint16_t                i;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	for(i=0;i<8;i++)
	{
		if((uiData << i) & 0x80)
		{
			SSD1306_PIN_HIGH(SSD1306_SPI_PIN_IDX_DIO);
		}
		else
		{
			SSD1306_PIN_LOW(SSD1306_SPI_PIN_IDX_DIO);
		}
		SSD1306_PIN_LOW(SSD1306_SPI_PIN_IDX_CLK);
		SSD1306_PIN_HIGH(SSD1306_SPI_PIN_IDX_CLK);
	}
}

/*****************************************************************************/
/* Function Name:	SSD1306_WriteData.										**/
/* Purpose:			Write a data byte to OLED coltroler.					**/
/* Resources:		None.													**/
/* Params:																	**/
/*	@uiData:			One byte data will be write.						**/
/* Return:			None.													**/
/* Limitation:		None.													**/
/*****************************************************************************/
void SSD1306_SPI_WriteData(uint8_t uiData)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	SSD1306_DATA_MODE_DAT();
	SSD1306_SPI_WriteByte(uiData);
}

/*****************************************************************************/
/* Function Name:	SSD1306_WriteCommand.									**/
/* Purpose:			Write a command byte to OLED coltroler.					**/
/* Resources:		None.													**/
/* Params:																	**/
/*	@uiData:			One byte command will be write.						**/
/* Return:			None.													**/
/* Limitation:		None.													**/
/*****************************************************************************/
void SSD1306_SPI_WriteCommand(uint8_t uiCommand)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	SSD1306_DATA_MODE_CMD();
	SSD1306_SPI_WriteByte(uiCommand);
}

/*****************************************************************************/
/* Function Name:	OLED_SetPosition.										**/
/* Purpose:			Set current position of page and colomn.				**/
/* Resources:		None.													**/
/* Params:																	**/
/*	@uiColumn:			Position of column.									**/
/*	@uiPage:			Position of page.									**/
/* Return:			None.													**/
/* Limitation:		None.													**/
/*****************************************************************************/
void SSD1306_SPI_SetPosition(uint8_t uiColumn, uint8_t uiPage)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	SSD1306_SPI_WriteCommand(0xB0 | uiPage);
	SSD1306_SPI_WriteCommand(((uiColumn & 0xF0)>>4)|0x10);
	SSD1306_SPI_WriteCommand((uiColumn & 0x0F));
}

/*****************************************************************************/
/* Function Name:	SSD1306_SPI_Fill.												**/
/* Purpose:			Fill screen.											**/
/* Resources:		None.													**/
/* Params:																	**/
/*	@uiColumn:			Position of column.									**/
/* Return:			None.													**/
/* Limitation:		None.													**/
/*****************************************************************************/
void SSD1306_SPI_Fill(uint8_t uiByte)//全屏填充
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	uint8_t uiPage, uiColumn;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	for(uiPage=0; uiPage<8; uiPage++)
	{
		SSD1306_SPI_WriteCommand(0xb0 | uiPage);
		SSD1306_SPI_WriteCommand(0x00);
		SSD1306_SPI_WriteCommand(0x10);
		for(uiColumn=0; uiColumn<128; uiColumn++)
		{
			SSD1306_SPI_WriteData(uiByte);
		}
	}
}

/*****************************************************************************/
/* Function Name:	SSD1306_SPI_Clear.												**/
/* Purpose:			Clear screen display.									**/
/* Resources:		None.													**/
/* Params:			None.													**/
/* Return:			None.													**/
/* Limitation:		None.													**/
/*****************************************************************************/
void SSD1306_SPI_Clear(void)//清屏
{
	SSD1306_SPI_Fill(0x00);
}

/*****************************************************************************/
/** Function Name:	SSD1306_Initialize										**/
/** Purpose:		Run the device initialize sequence.						**/
/** Resources:		None.													**/
/** Params:			None.													**/
/** Return:			None.													**/
/** Limitation:		Initialize sequence must executed after power on 500ms.	**/
/*****************************************************************************/
void SSD1306_SPI_Initialize(void)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Initialize GPIO
	GPIO_Initialize(arrstSSD1306_GPIO, 4);

	SSD1306_CS_ENABLE();

	SSD1306_SPI_WriteCommand(0xae);
	SSD1306_SPI_WriteCommand(0xae);//--turn off oled panel
	SSD1306_SPI_WriteCommand(0x00);//---set low column address
	SSD1306_SPI_WriteCommand(0x10);//---set high column address
	SSD1306_SPI_WriteCommand(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	SSD1306_SPI_WriteCommand(0x81);//--set contrast control register
	SSD1306_SPI_WriteCommand(0xcf); // Set SEG Output Current Brightness
	SSD1306_SPI_WriteCommand(0xa1);//--Set SEG/Column Mapping     0xa0,0xa1
	SSD1306_SPI_WriteCommand(0xc8);//Set COM/Row Scan Direction   0xc0,0xc8
	SSD1306_SPI_WriteCommand(0xa6);//--set normal display
	SSD1306_SPI_WriteCommand(0xa8);//--set multiplex ratio(1 to 64)
	SSD1306_SPI_WriteCommand(0x3f);//--1/64 duty
	SSD1306_SPI_WriteCommand(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	SSD1306_SPI_WriteCommand(0x00);//-not offset
	SSD1306_SPI_WriteCommand(0xd5);//--set display clock divide ratio/oscillator frequency
	SSD1306_SPI_WriteCommand(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	SSD1306_SPI_WriteCommand(0xd9);//--set pre-charge period
	SSD1306_SPI_WriteCommand(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	SSD1306_SPI_WriteCommand(0xda);//--set com pins hardware configuration
	SSD1306_SPI_WriteCommand(0x12);
	SSD1306_SPI_WriteCommand(0xdb);//--set vcomh
	SSD1306_SPI_WriteCommand(0x40);//Set VCOM Deselect Level
	SSD1306_SPI_WriteCommand(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	SSD1306_SPI_WriteCommand(0x02);//
	SSD1306_SPI_WriteCommand(0x8d);//--set Charge Pump enable/disable
	SSD1306_SPI_WriteCommand(0x14);//--set(0x10) disable
	SSD1306_SPI_WriteCommand(0xa4);// Disable Entire Display On (0xa4/0xa5)
	SSD1306_SPI_WriteCommand(0xa6);// Disable Inverse Display On (0xa6/a7)
	SSD1306_SPI_WriteCommand(0xaf);//--turn on oled panel
	SSD1306_SPI_Fill(0x00);
	SSD1306_SPI_SetPosition(0,0);
}

void OLED_BMP(uint32_t uiPosX, uint32_t uiPosY, uint32_t uiWidth, uint32_t uiHeight, uint8_t *pData)
{
	uint16_t	uiColumnIndex, uiPageIndex;
	uint16_t	uiColumnCount, uiPageCount;
	uint32_t	uiDataIndex;

	uiColumnCount = (uint16_t)uiWidth;
	if(uiHeight%8==0)
	{
		uiPageCount=uiHeight/8;
	}
	else
	{
		uiPageCount=uiHeight/8+1;
	}
	uiDataIndex = 0;

	for(uiPageIndex=uiPosY; uiPageIndex<uiPageCount; uiPageIndex++)
	{
		SSD1306_SPI_SetPosition(uiPosX, uiPageIndex);
		for(uiColumnIndex=uiPosX; uiColumnIndex<uiColumnCount; uiColumnIndex++)
		{
			SSD1306_SPI_WriteData(*(pData+uiDataIndex));
			uiDataIndex++;
		}
	}
}

void OLED_ShowLOGO(void)
{
	OLED_BMP(0, 0, 128, 64, (uint8_t*)arrLogoBitMap);
}
