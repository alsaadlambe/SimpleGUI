#ifndef __INCLUDE_SSD1306_SPI_H__
#define __INCLUDE_SSD1306_SPI_H__
//////////////////////////////////////////////////////////////////////////////////
//imodule
//////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stddef.h>

void SSD1306_SPI_Initialize(void);
void SSD1306_SPI_SetPosition(uint8_t uiColumn, uint8_t uiPage);
void SSD1306_SPI_Fill(uint8_t bmp_dat);
void SSD1306_SPI_Clear(void);
void SSD1306_SPI_WriteData(uint8_t uiData);
void SSD1306_SPI_WriteCommand(uint8_t cmd);

#endif //__INCLUDE_SSD1306_SPI_H__
