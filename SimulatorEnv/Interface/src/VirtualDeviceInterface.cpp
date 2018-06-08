/*************************************************************************/
/** Copyright.															**/
/** FileName: VirtualDeviceInterface.c                                  **/
/** Author: Polarix														**/
/** Version: 1.0.0.0													**/
/** Description: Virtual device operation interface.                    **/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "VirtualDeviceInterface.h"
#include "LCDFrame.h"

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	VDIF_SetPixel                                       **/
/** Purpose:		Set virtual device pixel register data.             **/
/** Params:																**/
/**	@ uiPosX[in]:        Pixel x-coordinate on display panel.           **/
/**	@ uiPosY[in]:        Pixel y-coordinate on display panel.			**/
/**	@ uiPixelValue[out]: Pixel value, 0 for clear, 1 for set.           **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void VDIF_SetPixel(uint32_t uiPosX, uint32_t uiPosY, uint32_t uiPixelValue)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
    LCDFrame*           pclsMainFrameObjectPtr;

    /*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
    pclsMainFrameObjectPtr = LCDFrame::GetInstance();

    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    if(NULL != pclsMainFrameObjectPtr)
    {
        pclsMainFrameObjectPtr->SetLCDPixel(uiPosX, uiPosY, uiPixelValue);
    }
}

/*************************************************************************/
/** Function Name:	VDIF_GetPixel                                       **/
/** Purpose:		Get a pixel value form virtual device register.     **/
/** Params:																**/
/**	@ uiPosX[in]:        Pixel x-coordinate on display panel.           **/
/**	@ uiPosY[in]:        Pixel y-coordinate on display panel.		    **/
/** Return:			Pixel state, 0 for cleared, 1 for set.              **/
/** Notice:			None.                                               **/
/*************************************************************************/
uint32_t VDIF_GetPixel(uint32_t uiPosX, uint32_t uiPosY)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
    LCDFrame*           pclsMainFrameObjectPtr;
    uint32_t            uiPixelValue;

    /*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
    pclsMainFrameObjectPtr = LCDFrame::GetInstance();

    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    if(NULL != pclsMainFrameObjectPtr)
    {
        uiPixelValue = pclsMainFrameObjectPtr->GetLCDPixel(uiPosX, uiPosY);
    }
    else
    {
        uiPixelValue = 0;
    }

    return uiPixelValue;
}

/*************************************************************************/
/** Function Name:	VDIF_RefreshDisplay.                                **/
/** Purpose:		Refresh virtual device display.                     **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void VDIF_RefreshDisplay(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
    LCDFrame*           pclsMainFrameObjectPtr;

    /*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
    pclsMainFrameObjectPtr = LCDFrame::GetInstance();

    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    if(NULL != pclsMainFrameObjectPtr)
    {
        pclsMainFrameObjectPtr->RefreshLCD();
    }
}

/*************************************************************************/
/** Function Name:	VDIF_ClearDisplay.                                  **/
/** Purpose:		Clear screen display.                               **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void VDIF_ClearDisplay(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
    LCDFrame*           pclsMainFrameObjectPtr;

    /*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
    pclsMainFrameObjectPtr = LCDFrame::GetInstance();

    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    if(NULL != pclsMainFrameObjectPtr)
    {
        pclsMainFrameObjectPtr->ClearLCD();
        pclsMainFrameObjectPtr->RefreshLCD();
    }
}

