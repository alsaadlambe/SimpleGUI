#ifndef __INCLUDE_CLASS_PIXEL_PANEL_H__
#define __INCLUDE_CLASS_PIXEL_PANEL_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "wxPixelatedPanel.h"
#include "Common.h"
#include <wx/colour.h>

//=======================================================================//
//= Class declare.                                                      =//
//=======================================================================//
class MonochromeDotLCD : public wxPixelatedPanel
{
    private:
        uint32_t			m_uiPageCount;
        uint32_t			m_uiColumnCount;
        wxColor*			m_pclsPanelColor;
        wxColor*			m_pclsPixelColor;

        void                wxEvent_OnSetFocus(wxFocusEvent& event)         {OnSetFocus(event);}

    protected:
        void                OnSetFocus(wxFocusEvent& event);

    public:
        typedef enum
        {
            LCD_PIXEL_COLOR_L = 0,
            LCD_PIXEL_COLOR_H,
        }LCD_PIXEL_COLOR_T;

                            MonochromeDotLCD(wxWindow *pclsParent,
											wxWindowID iWinID = wxID_ANY,
											const wxPoint& clsPosition = wxDefaultPosition);
                            ~MonochromeDotLCD(void);

	void				    SetParameter(PixelPanelParameter* pstPanelParameter);
	void				    SetPixel(uint32_t uiPosX, uint32_t uiPosY, LCD_PIXEL_COLOR_T ePixelValue);
	LCD_PIXEL_COLOR_T       GetPixel(uint32_t uiPosX, uint32_t uiPosY);
	void				    DrawPixel(uint32_t uiPosX, uint32_t uiPosY, LCD_PIXEL_COLOR_T ePixelValue);
    void                    CleanScreen(void);
                            DECLARE_EVENT_TABLE();
};


#endif // __INCLUDE_CLASS_PIXEL_PANEL_H__
