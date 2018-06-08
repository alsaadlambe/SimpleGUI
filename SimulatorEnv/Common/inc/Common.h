#ifndef __INCLUDE_COMMON_H__
#define __INCLUDE_COMMON_H__
#include <stddef.h>
#include <stdint.h>

#define LCD_COLOR_OBJ_PIX						    	(0xFF168363)
#define LCD_COLOR_OBJ_BKG						    	(0xFF00F0D7)
#define LCD_COLOR_OBJ_EDGE						    	(0xFF383834)
#define LCD_COLOR_OBJ_GRID						    	(0xFF383834)

#define PARAM_DEFAULT_COLUMNS						    (192)
#define PARAM_DEFAULT_PAGES					    		(8)
#define PARAM_DEFAULT_PIXEL					    		(3)
#define PARAM_DEFAULT_EDGE					    		(5)
#define PARAM_DEFAULT_GRID					    		(false)

#define _TRANS_TEXT(STR)								_(STR)

#define LCD_COLOR_OBJ_PIX						    	(0xFF168363)
#define LCD_COLOR_OBJ_BKG						    	(0xFF00F0D7)
#define LCD_COLOR_OBJ_EDGE						    	(0xFF383834)
#define LCD_COLOR_OBJ_GRID						    	(0xFF383834)

#define LCD_DEFAULT_CONFIG_FILE					    	("Config.xml")

#define PARAM_DEFAULT_PIXEL_NUM_H                       (128)
#define PARAM_DEFAULT_PIXEL_NUM_V                       (64)
#define PARAM_DEFAULT_PIXEL_SIZE					    (2)
#define PARAM_DEFAULT_EDGE_WIDTH                        (5)
#define PARAM_DEFAULT_GRID_ENABLE                       (true)
#define PARAM_DEFAULT_GRID_DISABLE                      (false)

typedef union
{
	uint8_t					Channel[4];
	uint32_t				RGBA;
}LCDRGBAColor;

typedef struct
{
	size_t					HorizontalPixelNumber;
    size_t					VerticalPixelNumber;
    size_t					PixelSize;
    size_t					EdgeWidth;
    bool					EnableGrid;
	LCDRGBAColor			EdgeColor;
	LCDRGBAColor			PanelColor;
	LCDRGBAColor			PixelColor;
	LCDRGBAColor			GridColor;

}PixelPanelParameter;

extern PixelPanelParameter			g_stParameters;

void SetDefaultParameterData(PixelPanelParameter* pstParameter);

#endif // __INCLUDE_LCD_COMMON_H__
