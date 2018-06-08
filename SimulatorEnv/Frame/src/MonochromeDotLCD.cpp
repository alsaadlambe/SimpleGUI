/*************************************************************************/
/** Copyright.															**/
/** FileName: LCDScreenPanel.cpp											    **/
/** Author: Polarix														**/
/** Version: 1.7.0.0													**/
/** Description: Main frame class define.                               **/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "MonochromeDotLCD.h"

//=======================================================================//
//= Event table.													    =//
//=======================================================================//
BEGIN_EVENT_TABLE(MonochromeDotLCD,wxPixelatedPanel)
	EVT_SET_FOCUS       (MonochromeDotLCD::wxEvent_OnSetFocus)
END_EVENT_TABLE()

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
MonochromeDotLCD::MonochromeDotLCD(wxWindow *pclsParent, wxWindowID iWinID, const wxPoint& clsPosition):
wxPixelatedPanel(pclsParent, iWinID, clsPosition)
{
	m_pclsPanelColor = new wxColor(0x00, 0x00, 0x00, 0x00);
	m_pclsPixelColor = new wxColor(0x00, 0x00, 0x00, 0x00);
}

MonochromeDotLCD::~MonochromeDotLCD(void)
{
	delete m_pclsPanelColor;
	delete m_pclsPixelColor;
}

void MonochromeDotLCD::SetParameter(PixelPanelParameter* pstPanelParameter)
{
	if(NULL != pstPanelParameter)
	{
	    wxPixelatedPanel::SetEdgeWidth(pstPanelParameter->EdgeWidth);
	    wxPixelatedPanel::SetPixelSize(pstPanelParameter->PixelSize);
	    wxPixelatedPanel::SetGridVisibled(pstPanelParameter->EnableGrid);
		wxPixelatedPanel::SetPixelNumber(pstPanelParameter->HorizontalPixelNumber, pstPanelParameter->VerticalPixelNumber);
		m_uiColumnCount = pstPanelParameter->HorizontalPixelNumber;
		m_uiPageCount = pstPanelParameter->VerticalPixelNumber/8;

		m_pclsPanelColor->SetRGBA(pstPanelParameter->PanelColor.RGBA);
		m_pclsPixelColor->SetRGBA(pstPanelParameter->PixelColor.RGBA);
		wxPixelatedPanel::SetColors(	wxColor(pstPanelParameter->EdgeColor.RGBA),
										wxColor(pstPanelParameter->PanelColor.RGBA),
										wxColor(pstPanelParameter->GridColor.RGBA));
	}
	CleanScreen();
}

void MonochromeDotLCD::SetPixel(uint32_t uiPosX, uint32_t uiPosY, LCD_PIXEL_COLOR_T ePixelValue)
{
    if(LCD_PIXEL_COLOR_L == ePixelValue)
    {
        wxPixelatedPanel::SetPixelUnitColor(uiPosX, uiPosY, *m_pclsPanelColor);
    }
    else
    {
        wxPixelatedPanel::SetPixelUnitColor(uiPosX, uiPosY, *m_pclsPixelColor);
    }
}

MonochromeDotLCD::LCD_PIXEL_COLOR_T MonochromeDotLCD::GetPixel(uint32_t uiPosX, uint32_t uiPosY)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
    MonochromeDotLCD::LCD_PIXEL_COLOR_T eReturn;
    uint32_t                    uiPixelColor;

    /*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
    uiPixelColor = GetPixelUnitColor(uiPosX, uiPosY);

    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    if(uiPixelColor == m_pclsPixelColor->GetRGBA())
    {
        eReturn = MonochromeDotLCD::LCD_PIXEL_COLOR_H;
    }
    else
    {
        eReturn = MonochromeDotLCD::LCD_PIXEL_COLOR_L;
    }

    return eReturn;
}

void MonochromeDotLCD::DrawPixel(uint32_t uiPosX, uint32_t uiPosY, LCD_PIXEL_COLOR_T ePixelValue)
{
    if(LCD_PIXEL_COLOR_L == ePixelValue)
    {
        wxPixelatedPanel::DrawPixel(uiPosX, uiPosY, *m_pclsPanelColor);
    }
    else
    {
        wxPixelatedPanel::DrawPixel(uiPosX, uiPosY, *m_pclsPixelColor);
    }
}

void MonochromeDotLCD::OnSetFocus(wxFocusEvent& event)
{
	event.ResumePropagation(1);
	event.Skip();
}

void MonochromeDotLCD::CleanScreen(void)
{
	uint32_t uiHorizontalPixelNumber, uiVerticalPixelNumber;

	GetPixelNumber(&uiHorizontalPixelNumber, &uiVerticalPixelNumber);
	for(uint32_t i_V=0; i_V<uiVerticalPixelNumber; i_V++)
	{
		for(uint32_t i_H=0; i_H<uiHorizontalPixelNumber; i_H++)
		{
			SetPixelUnitColor(i_H, i_V, *m_pclsPanelColor);
		}
	}
}
