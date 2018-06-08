#ifndef _INCLUDE_CLASS_WXEX_PIXEL_PANEL_H__
#define _INCLUDE_CLASS_WXEX_PIXEL_PANEL_H__

#include <wx/panel.h>
#include <wx/dcbuffer.h>
#include <wx/colour.h>

///////////////////////////////////////////////////////////////////////////////
/// Class LCD_DisplayPanel
///////////////////////////////////////////////////////////////////////////////

class wxPixelatedPanel : public wxPanel
{
private:
	// Colors
	wxColor*			m_pclsEdgeColor;
	wxColor*			m_pclsBaseColor;
	wxColor*			m_pclsGridColor;
	// Display area
	uint32_t			m_uiHorizontalPixelNumber;
	uint32_t			m_uiVerticalPixelNumber;
	// Appearance size
    uint32_t			m_uiPixelSize;
    uint32_t			m_uiEdgeWidth;
    bool				m_bGridVisible;
	// Private data declare.
    // Private data.
	uint32_t**          m_ppuiDisplayBuffer;
	void				(wxPixelatedPanel::*m_pfDrawPoint)(wxMemoryDC& clsCDCObject, uint32_t uiPosX, uint32_t uiPosY, uint32_t uiPixelSize);
    // Private object;
	wxClientDC			m_clsCDC;
	wxPen				m_clsPen;
	wxBrush				m_clsBrush;

private:
    // Event callback function.
	void				_wxEvent_OnPaint(wxPaintEvent &event)		{OnPaint();event.Skip();}

public:
    // Constructor/Destructor
						wxPixelatedPanel(wxWindow *pclsParent, wxWindowID iWinID = wxID_ANY, const wxPoint& clsPosition = wxDefaultPosition);
						~wxPixelatedPanel();
    // Public interface
	void				SetPixelNumber(uint32_t uiHorizontalPixelNumber, uint32_t uiVerticalPixelNumber);
	void				GetPixelNumber(uint32_t* puiHorizontalPixelNumber, uint32_t* puiVerticalPixelNumber);
	void				SetColors(const wxColor& clsEdgeColor, const wxColor& clsBaseColor, const wxColor& clsGridColor);
	void				SetEdgeWidth(uint32_t uiEdgeWidth);
	uint32_t			GetEdgeWidth(void)							{return m_uiEdgeWidth;}
	void				SetPixelSize(uint32_t uiPixelSize);
	uint32_t			GetPixelSize(void)							{return m_uiPixelSize;}
	void				SetGridVisibled(bool bGridVisible);
	bool                GetGridVisibled(void)                       {return m_bGridVisible;}
	void				SetEdgeColor(const wxColor& clsColor);
	wxColor&            GetEdgeColor(void);
	void				SetBaseColor(const wxColor& clsColor);
	wxColor&            GetBaseColor(void);
	void				SetGridColor(const wxColor& clsColor);
	wxColor&            GetGridColor(void);
	void				OnPaint(void);
	void				SetPixelUnitColor(uint32_t uiPosX, uint32_t uiPosY, const wxColor& clsColor, bool bRefreshNow = false);
	void                DrawPixel(uint32_t uiPosX, uint32_t uiPosY, wxColor& clsColor);
	uint32_t			GetPixelUnitColor(uint32_t uiPosX, uint32_t uiPosY);
	void				RefreshDisplay(void);
	void				CleanPanel(void);
	bool				SaveScreenImageToFile(const wxString& strFilePath);
	bool				CopyScreenImageToClipBoard(void);
	void				ResizeParent(void);

private:	// Private process function.
	inline void			_drawPointSinglePixel(wxMemoryDC& clsCDCObject, uint32_t uiPosX, uint32_t uiPosY, uint32_t uiPixelSize);
	inline void			_drawPointMultiplePixel(wxMemoryDC& clsCDCObject, uint32_t uiPosX, uint32_t uiPosY, uint32_t uiPixelSize);
	inline void			_drawPointMultiplePixelWithGrid(wxMemoryDC& clsCDCObject, uint32_t uiPosX, uint32_t uiPosY, uint32_t uiPixelSize);
	inline void			_releaseDC(wxMemoryDC& clsCDCObject)		{clsCDCObject.SetBrush(wxNullBrush); clsCDCObject.SetPen(wxNullPen);}
	inline void			_prepareDC(wxMemoryDC& clsCDCObject)		{clsCDCObject.SetBrush(m_clsBrush); clsCDCObject.SetPen(m_clsPen);}
	inline void			_releaseDC(wxClientDC& clsCDCObject)		{clsCDCObject.SetBrush(wxNullBrush); clsCDCObject.SetPen(wxNullPen);}
	inline void			_prepareDC(wxClientDC& clsCDCObject)		{clsCDCObject.SetBrush(m_clsBrush); clsCDCObject.SetPen(m_clsPen);}
	inline void			_setDCColor(const wxColor& clsColour)		{m_clsPen.SetColour(clsColour);m_clsBrush.SetColour(clsColour);}
	inline void         _freeDisplayBuffer(uint32_t** ppuiDisplayBuffer);
	inline uint32_t**   _createNewDisplayBuffer(uint32_t uiHorizontalPixelNumber, uint32_t uiVerticalPixelNumber);

	DECLARE_EVENT_TABLE();
};

#endif // _INCLUDE_CLASS_WXEX_PIXEL_PANEL_H__
