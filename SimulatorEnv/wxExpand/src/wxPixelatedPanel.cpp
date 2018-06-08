//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <wx/clipbrd.h>
#include "wxPixelatedPanel.h"

//=======================================================================//
//= Event table.													    =//
//=======================================================================//
BEGIN_EVENT_TABLE(wxPixelatedPanel,wxPanel)
	EVT_PAINT		(wxPixelatedPanel::_wxEvent_OnPaint)
END_EVENT_TABLE()

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
wxPixelatedPanel::wxPixelatedPanel(wxWindow *pclsParent, wxWindowID iWinID, const wxPoint& clsPosition):
wxPanel(pclsParent, iWinID, clsPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxNO_BORDER, wxPanelNameStr),
m_clsCDC(this)
{
	m_ppuiDisplayBuffer =			NULL;
	m_pfDrawPoint =					NULL;

	m_uiHorizontalPixelNumber =		0;
	m_uiVerticalPixelNumber =		0;
	m_uiPixelSize =					0;
	m_uiEdgeWidth =					0;
	m_bGridVisible =				false;

	m_pclsEdgeColor =				new wxColor(0x80, 0x80, 0x80, 0x00);
	m_pclsBaseColor =				new wxColor(0xFF, 0xFF, 0xFF, 0x00);
	m_pclsGridColor =				new wxColor(0x80, 0x80, 0x80, 0x00);
}

wxPixelatedPanel::~wxPixelatedPanel()
{
	_freeDisplayBuffer(m_ppuiDisplayBuffer);
	delete m_pclsEdgeColor;
	delete m_pclsBaseColor;
	delete m_pclsGridColor;
}

void wxPixelatedPanel::SetPixelNumber(uint32_t uiHorizontalPixelNumber, uint32_t uiVerticalPixelNumber)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	uint32_t**              ppuiNewDisplayBuffer;
	uint32_t                uiCopiedRowNumber;
	uint32_t                uiCopiedColumnNumber;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	ppuiNewDisplayBuffer =  NULL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((uiHorizontalPixelNumber > 0) && (uiVerticalPixelNumber > 0))
    {
        // Create a new display buffer
        ppuiNewDisplayBuffer = _createNewDisplayBuffer(uiHorizontalPixelNumber, uiVerticalPixelNumber);
        if(NULL != ppuiNewDisplayBuffer)
        {
            uiCopiedRowNumber = m_uiVerticalPixelNumber<uiVerticalPixelNumber?m_uiVerticalPixelNumber:uiVerticalPixelNumber;
            uiCopiedColumnNumber = m_uiHorizontalPixelNumber<uiHorizontalPixelNumber?m_uiHorizontalPixelNumber:uiHorizontalPixelNumber;
            // Copy old buffer content to new buffer;
            for(uint32_t uiIdxV=0; uiIdxV<uiCopiedRowNumber; uiIdxV++)
            {
                memcpy(*(ppuiNewDisplayBuffer+uiIdxV), *(m_ppuiDisplayBuffer+uiIdxV), sizeof(uint32_t)*uiCopiedColumnNumber);
            }
            // Free current buffer.
            _freeDisplayBuffer(m_ppuiDisplayBuffer);
            m_ppuiDisplayBuffer = ppuiNewDisplayBuffer;
            m_uiHorizontalPixelNumber = uiHorizontalPixelNumber;
            m_uiVerticalPixelNumber = uiVerticalPixelNumber;
        }
    }
}

void wxPixelatedPanel::GetPixelNumber(uint32_t* puiHorizontalPixelNumber, uint32_t* puiVerticalPixelNumber)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != puiHorizontalPixelNumber)
	{
		*puiHorizontalPixelNumber = m_uiHorizontalPixelNumber;
	}

	if(NULL != puiVerticalPixelNumber)
	{
		*puiVerticalPixelNumber = m_uiVerticalPixelNumber;
	}
}

void wxPixelatedPanel::SetColors(const wxColor& clsEdgeColor, const wxColor& clsBaseColor, const wxColor& clsGridColor)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != m_pclsEdgeColor)
	{
		m_pclsEdgeColor->SetRGB(clsEdgeColor.GetRGB());
	}
	if(NULL != m_pclsBaseColor)
	{
		m_pclsBaseColor->SetRGB(clsBaseColor.GetRGB());
	}
	if(NULL != m_pclsGridColor)
	{
		m_pclsGridColor->SetRGB(clsGridColor.GetRGB());
	}
}

void wxPixelatedPanel::SetEdgeWidth(uint32_t uiEdgeWidth)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	m_uiEdgeWidth = uiEdgeWidth;
}

void wxPixelatedPanel::SetPixelSize(uint32_t uiPixelSize)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Save the pixel size value.
    m_uiPixelSize = uiPixelSize;

	if(3 > m_uiPixelSize)
    // If pixel size is less then 3, grid is fixed to NOT visible.
	{
        if(0 == m_uiPixelSize)
        // If pixel size is 0, the pixel drawing function will set to invalid.
        {
            m_pfDrawPoint = NULL;
        }
        else if(1 == m_uiPixelSize)
        // If pixel size is 1, the pixel drawing function will set to draw pixel unit by point.
        {
            m_pfDrawPoint = &_drawPointSinglePixel;
        }
        else
        {
            m_pfDrawPoint = &_drawPointMultiplePixel;
        }
	}
	else
	{
		if(true == m_bGridVisible)
		{
			m_pfDrawPoint = &_drawPointMultiplePixelWithGrid;
		}
		else
		{
			m_pfDrawPoint = &_drawPointMultiplePixel;
		}
	}
}

void wxPixelatedPanel::SetGridVisibled(bool bGridVisible)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Save the grid visible value set.
    m_bGridVisible = bGridVisible;

    if(3 <= m_uiPixelSize)
	{
		if(true == m_bGridVisible)
		{
			m_pfDrawPoint = &_drawPointMultiplePixelWithGrid;
		}
		else
		{
			m_pfDrawPoint = &_drawPointMultiplePixel;
		}
	}
}

void wxPixelatedPanel::CleanPanel(void)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	for(uint32_t uiIdxV=0; uiIdxV<m_uiVerticalPixelNumber; uiIdxV++)
    {
        memset(*(m_ppuiDisplayBuffer+uiIdxV), 0x00, sizeof(uint32_t)*m_uiHorizontalPixelNumber);
    }
	RefreshDisplay();
}

void wxPixelatedPanel::SetEdgeColor(const wxColor& clsColor)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    if(NULL != m_pclsEdgeColor)
	{
		*m_pclsEdgeColor = clsColor;
	}
}

wxColor& wxPixelatedPanel::GetEdgeColor(void)
{
    return *m_pclsEdgeColor;
}

void wxPixelatedPanel::SetBaseColor(const wxColor& clsColor)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != m_pclsBaseColor)
	{
		*m_pclsBaseColor = clsColor;
	}
}

wxColor& wxPixelatedPanel::GetBaseColor(void)
{
    return *m_pclsBaseColor;
}

void wxPixelatedPanel::SetGridColor(const wxColor& clsColor)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != m_pclsGridColor)
	{
		*m_pclsGridColor = clsColor;
	}
}

wxColor& wxPixelatedPanel::GetGridColor(void)
{
    return *m_pclsGridColor;
}

void wxPixelatedPanel::OnPaint(void)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	RefreshDisplay();
}


void wxPixelatedPanel::_drawPointSinglePixel(wxMemoryDC& clsCDCObject, uint32_t uiPosX, uint32_t uiPosY, uint32_t uiPixelSize)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	clsCDCObject.DrawPoint(wxPoint(m_uiEdgeWidth+uiPosX, m_uiEdgeWidth+uiPosY));
}

void wxPixelatedPanel::_drawPointMultiplePixel(wxMemoryDC& clsCDCObject, uint32_t uiPosX, uint32_t uiPosY, uint32_t uiPixelSize)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	clsCDCObject.DrawRectangle(wxPoint(m_uiEdgeWidth+uiPosX*uiPixelSize, m_uiEdgeWidth+uiPosY*uiPixelSize), wxSize(uiPixelSize, uiPixelSize));
}

void wxPixelatedPanel::_drawPointMultiplePixelWithGrid(wxMemoryDC& clsCDCObject, uint32_t uiPosX, uint32_t uiPosY, uint32_t uiPixelSize)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	clsCDCObject.DrawRectangle(wxPoint(m_uiEdgeWidth+uiPosX*uiPixelSize+1, m_uiEdgeWidth+uiPosY*uiPixelSize+1), wxSize(uiPixelSize-1, uiPixelSize-1));
}

/*************************************************************************/
/** Function Name:	SetPixelUnitColor                                   **/
/** Purpose:		Set a pixel RGBA color value.                       **/
/** Params:																**/
/**	@ uiPosX[in]:       X-Coordinate of pixel.                          **/
/**	@ uiPosY[in]:       Y-Coordinate of pixel.                          **/
/**	@ clsColor[in]:     Color data object.                              **/
/**	@ bRefreshNow[in]:  Refresh display at once, default to false.      **/
/** Return:			None.                                               **/
/** Notice:			This function only change the pixel color register  **/
/**                 if bRefreshNow is false(default), and when the      **/
/**                 parameter bRefreshNow is true, all pixel blocks of  **/
/**                 LCD screen panel will be repaint. if need to draw   **/
/**                 only one pixel, please use the DrawPixel function   **/
/**                 directly.                                           **/
/*************************************************************************/
void wxPixelatedPanel::SetPixelUnitColor(uint32_t uiPosX, uint32_t uiPosY, const wxColor& clsColor, bool bRefreshNow)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((uiPosX < m_uiHorizontalPixelNumber) && (uiPosY < m_uiVerticalPixelNumber))
	{
		if(NULL != m_ppuiDisplayBuffer)
		{
		    *(*(m_ppuiDisplayBuffer+uiPosY)+uiPosX) = clsColor.GetRGBA();
		}
		if(true == bRefreshNow)
		{
			RefreshDisplay();
		}
	}
}

/*************************************************************************/
/** Function Name:	GetPixelUnitColor                                   **/
/** Purpose:		Get color of a pixel unit.                          **/
/** Params:																**/
/**	@ uiPosX[in]:       X-Coordinate of pixel.                          **/
/**	@ uiPosY[in]:       Y-Coordinate of pixel.                          **/
/** Return:			RGBA color value of the pixel unit.                 **/
/** Notice:			None.                                               **/
/*************************************************************************/
uint32_t wxPixelatedPanel::GetPixelUnitColor(uint32_t uiPosX, uint32_t uiPosY)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	uint32_t		uiReturnValue = 0x00;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((uiPosX < m_uiHorizontalPixelNumber) && (uiPosY < m_uiVerticalPixelNumber))
	{
		if(NULL != m_ppuiDisplayBuffer)
		{
			uiReturnValue = *(*(m_ppuiDisplayBuffer+uiPosY)+uiPosX);
		}
	}

	return uiReturnValue;
}

/*************************************************************************/
/** Function Name:	DrawPixel                                           **/
/** Purpose:		Draw a pixel.                                       **/
/** Params:																**/
/**	@ uiPosX[in]:       X-Coordinate of pixel.                          **/
/**	@ uiPosY[in]:       Y-Coordinate of pixel.                          **/
/**	@ clsColor[in]:     Color data object.                              **/
/** Return:			None.                                               **/
/** Notice:			Draw only one pixel directly.                       **/
/*************************************************************************/
void wxPixelatedPanel::DrawPixel(uint32_t uiPosX, uint32_t uiPosY, wxColor& clsColor)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((uiPosX < m_uiHorizontalPixelNumber) && (uiPosY < m_uiVerticalPixelNumber) && (m_uiPixelSize != 0))
	{
		_setDCColor(clsColor);
		_prepareDC(m_clsCDC);
		if(m_uiPixelSize == 1)
		{
			m_clsCDC.DrawPoint(wxPoint(m_uiEdgeWidth+uiPosX, m_uiEdgeWidth+uiPosY));
		}
		else
		{
			if((true == m_bGridVisible) && (m_uiPixelSize > 2))
			{
				m_clsCDC.DrawRectangle(wxPoint(m_uiEdgeWidth+uiPosX*m_uiPixelSize+1, m_uiEdgeWidth+uiPosY*m_uiPixelSize+1), wxSize(m_uiPixelSize-1, m_uiPixelSize-1));
			}
			else
			{
				m_clsCDC.DrawRectangle(wxPoint(m_uiEdgeWidth+uiPosX*m_uiPixelSize, m_uiEdgeWidth+uiPosY*m_uiPixelSize), wxSize(m_uiPixelSize, m_uiPixelSize));
			}
		}
		SetPixelUnitColor(uiPosX, uiPosY, clsColor);
		_releaseDC(m_clsCDC);

		if(NULL != m_ppuiDisplayBuffer)
        {
            *(*(m_ppuiDisplayBuffer+uiPosY)+uiPosX) = clsColor.GetRGBA();
        }
	}
}

/*************************************************************************/
/** Function Name:	RefreshDisplay                                      **/
/** Purpose:		Repaint LCD screen panel.                           **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			Call this function after the SetPixelColor called,  **/
/**                 all pixels of the LCD panel will be repaint by the  **/
/**                 pixels's RGBA value register array.                 **/
/*************************************************************************/
void wxPixelatedPanel::RefreshDisplay(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	uint32_t				uiPaintSizeWidth, uiPaintSizeHeight;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	uiPaintSizeWidth =	m_uiHorizontalPixelNumber*m_uiPixelSize+m_uiEdgeWidth*2;
	uiPaintSizeHeight =	m_uiVerticalPixelNumber*m_uiPixelSize+m_uiEdgeWidth*2;

	// Set buffer size.
	if((true == m_bGridVisible) && (2 < m_uiPixelSize))
	{
		uiPaintSizeWidth++;
		uiPaintSizeHeight++;
	}
	// Create buffer image and DC object.
	wxBitmap			clsBufferImage(uiPaintSizeWidth, uiPaintSizeHeight);
	wxBufferedDC		clsBufferedDC(&m_clsCDC, clsBufferImage);

	// Draw background.
	_setDCColor(*m_pclsEdgeColor);
	_prepareDC(clsBufferedDC);
	clsBufferedDC.DrawRectangle(wxPoint(0, 0),wxSize(uiPaintSizeWidth, uiPaintSizeHeight));

	// Draw grid line.
	if((true == m_bGridVisible) && (2 < m_uiPixelSize))
	{
		_setDCColor(*m_pclsGridColor);
		_prepareDC(clsBufferedDC);
		clsBufferedDC.DrawRectangle(wxPoint(m_uiEdgeWidth, m_uiEdgeWidth),
							wxSize(	m_uiHorizontalPixelNumber*m_uiPixelSize+1,
									m_uiVerticalPixelNumber*m_uiPixelSize+1));
	}

	if(NULL != m_pfDrawPoint)
	{
		// Refresh each pixel on screen.
		for(uint32_t i_H=0; i_H<m_uiVerticalPixelNumber; i_H++)
		{
			for(uint32_t i_W=0; i_W<m_uiHorizontalPixelNumber; i_W++)
			{
				_setDCColor(wxColor(*(*(m_ppuiDisplayBuffer+i_H)+i_W)));
				_prepareDC(clsBufferedDC);
				(this->*m_pfDrawPoint)(clsBufferedDC, i_W, i_H, m_uiPixelSize);
			}
		}
	}
}

/*************************************************************************/
/** Function Name:	ResizeParent                                        **/
/** Purpose:		Resize parent window to adapt current object size.  **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			Call this function after the SetPixelColor called,  **/
/**                 all pixels of the LCD panel will be repaint by the  **/
/**                 pixels's RGBA value register array.                 **/
/*************************************************************************/
void wxPixelatedPanel::ResizeParent(void)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	uint32_t						uiPaintSizeWidth, uiPaintSizeHeight;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	// Get drawing area size.
	uiPaintSizeWidth =	m_uiHorizontalPixelNumber*m_uiPixelSize+m_uiEdgeWidth*2;
	uiPaintSizeHeight =	m_uiVerticalPixelNumber*m_uiPixelSize+m_uiEdgeWidth*2;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((true == m_bGridVisible) && (2 <m_uiPixelSize))
	{
		GetParent()->SetClientSize(uiPaintSizeWidth+1, uiPaintSizeHeight+1);
	}
	else
	{
		GetParent()->SetClientSize(uiPaintSizeWidth, uiPaintSizeHeight);
	}
}

/*************************************************************************/
/** Function Name:	SaveScreenImageToFile                               **/
/** Purpose:		Save current panel image to a jpeg file.            **/
/** Params:			                                                    **/
/**	@ strFilePath[in]:  Save file path.                                 **/
/** Return:                                                             **/
/** @ true:             Save successfully.                              **/
/** @ false:            Save failed.                                    **/
/** Notice:			To support jpeg format, need add wxWidgets jpeg     **/
/**                 format handler in initialize(OnInit) function of    **/
/**                 the wxApp Object.                                   **/
/**                 wxImage::AddHandler(new wxJPEGHandler)              **/
/*************************************************************************/
bool wxPixelatedPanel::SaveScreenImageToFile(const wxString& strFilePath)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	wxBitmap*               pclsBitMap;
	wxMemoryDC*             pclsMemoryDC;
	bool                    bReturn;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	bReturn =               false;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Create bitmap buffer.
	pclsBitMap = new wxBitmap(GetSize(), wxBITMAP_SCREEN_DEPTH);
	if(NULL != pclsBitMap)
    {
        pclsMemoryDC = new wxMemoryDC(*pclsBitMap);
        if(NULL != pclsMemoryDC)
        {
            bReturn = pclsMemoryDC->Blit(wxPoint(0, 0), GetSize(), &m_clsCDC, wxPoint(0, 0));
            if(true == bReturn)
            {
                bReturn = pclsBitMap->SaveFile(strFilePath, wxBITMAP_TYPE_JPEG);
            }
        }
    }
    //
    delete pclsBitMap;
    delete pclsMemoryDC;

	return bReturn;
}

/*************************************************************************/
/** Function Name:	CopyScreenImageToClipBoard                          **/
/** Purpose:		Copy current screen image to clip board.            **/
/** Params:			None.                                               **/
/** Return:                                                             **/
/** @ true:             Copy successfully.                              **/
/** @ false:            Copy failed.                                    **/
/** Notice:			None.                                               **/
/*************************************************************************/
bool wxPixelatedPanel::CopyScreenImageToClipBoard(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	uint32_t                uiPictureWidth, uiPictureHeight;
	wxBitmap*               pclsDCBufferBitmap;
	wxMemoryDC*             pclsDCBuffer;
	bool                    bReturn;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	uiPictureWidth =        GetSize().GetX();
	uiPictureHeight =       GetSize().GetY();
	pclsDCBufferBitmap =    new wxBitmap(uiPictureWidth, uiPictureHeight, wxBITMAP_SCREEN_DEPTH);
	pclsDCBuffer =          new wxMemoryDC(*pclsDCBufferBitmap);
	bReturn =               true;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if((NULL != pclsDCBufferBitmap) && (NULL != pclsDCBuffer))
    {
        if(true == wxTheClipboard->Open())
        {
            pclsDCBuffer->Blit(0, 0, uiPictureWidth, uiPictureHeight, &m_clsCDC, 0, 0);
            wxTheClipboard->SetData(new wxBitmapDataObject(*pclsDCBufferBitmap));
            wxTheClipboard->Close();
        }
        else
        {
            bReturn = false;
        }
    }

    delete pclsDCBufferBitmap;
    delete pclsDCBuffer;

	return bReturn;
}

void wxPixelatedPanel::_freeDisplayBuffer(uint32_t** ppuiDisplayBuffer)
{
    if(NULL != ppuiDisplayBuffer)
	{
	    for(uint32_t uiIdxV=0; uiIdxV<m_uiVerticalPixelNumber; uiIdxV++)
        {
            free(*(ppuiDisplayBuffer+uiIdxV));
        }
        free(ppuiDisplayBuffer);
	}
}

uint32_t** wxPixelatedPanel::_createNewDisplayBuffer(uint32_t uiHorizontalPixelNumber, uint32_t uiVerticalPixelNumber)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	uint32_t**              ppuiNewDisplayBuffer;
	uint32_t*               puiNewHorizontalPixelBuffer;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	ppuiNewDisplayBuffer =  NULL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/

	if((uiHorizontalPixelNumber > 0) && (uiVerticalPixelNumber > 0))
    {
        ppuiNewDisplayBuffer = (uint32_t**)malloc(sizeof(uint32_t*)*uiVerticalPixelNumber);
        if(NULL != ppuiNewDisplayBuffer)
        {
            memset(ppuiNewDisplayBuffer, 0x00, sizeof(uint32_t*)*uiVerticalPixelNumber);
            for(uint32_t uiIdxV=0; uiIdxV<uiVerticalPixelNumber; uiIdxV++)
            {
                puiNewHorizontalPixelBuffer = (uint32_t*)malloc(sizeof(uint32_t)*uiHorizontalPixelNumber);
                if(NULL != puiNewHorizontalPixelBuffer)
                {
                    memset(puiNewHorizontalPixelBuffer, 0x00, sizeof(uint32_t)*uiHorizontalPixelNumber);
                    *(ppuiNewDisplayBuffer+uiIdxV) = puiNewHorizontalPixelBuffer;
                }
                else
                {
                    _freeDisplayBuffer(ppuiNewDisplayBuffer);
                    ppuiNewDisplayBuffer = NULL;
                    break;
                }
            }
        }
    }

    return ppuiNewDisplayBuffer;
}
