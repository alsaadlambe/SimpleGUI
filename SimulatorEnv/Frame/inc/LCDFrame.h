#ifndef __INCLUDE_CLASS_LCD_FRAME_H__
#define __INCLUDE_CLASS_LCD_FRAME_H__
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/colour.h>
#include <wx/string.h>
#include <wx/statusbr.h>
#include <wx/toolbar.h>
#include <wx/clipbrd.h>
#include <wx/filefn.h>
#include <wx/timer.h>

#include "MonochromeDotLCD.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define FRAME_TITLE						(_T("SimpleGUI Simulator"))

#define wxID_MAIN						1000
#define wxID_TOOLBAR					2000
#define wxID_TOOLBAR_QUICKSHOTS         2100
#define wxID_TOOLBAR_COPY				2101
#define wxID_TOOLBAR_SCREENSHOTS_FOLDER	2102
#define wxID_TOOLBAR_ABOUT				2103
#define wxID_TOOLBAR_EXIT				2104
#define wxID_STATUSBAR					3000
#define wxID_PANEL						4000
#define WXID_MILLISECOND_TIMER          5001
#define WXID_RTC_TIMER                  5002

#define SCREENSHOTS_FOLDER_T			"ScreenShots"
#define SCREENSHOTS_FILE_NAME_T			"LCD_%04u%02u%02u_%02u%02u-%u.bmp"
#define SCREENSHOTS_FILE_FULLNAME_T		wxString::Format("%s\\%s", SCREENSHOTS_FOLDER_T, SCREENSHOTS_FILE_NAME_T)
#define N_YEAR							wxDateTime::Now().GetYear()
#define N_MONTH							wxDateTime::Now().GetMonth()
#define N_DAY							wxDateTime::Now().GetDay()
#define N_HOUR							wxDateTime::Now().GetHour()
#define N_MINUTE						wxDateTime::Now().GetMinute()
#define SCREENSHOTS_FOLDER				_T(SCREENSHOTS_FOLDER_T)
#define SCREENSHOTS_FILE_NAME(IDX)		wxString::Format(SCREENSHOTS_FILE_NAME_T, N_YEAR, N_MONTH, N_DAY, N_HOUR, N_MINUTE, IDX)
#define SCREENSHOTS_FILE_FULLNAME(IDX)	wxString::Format(SCREENSHOTS_FILE_FULLNAME_T, N_YEAR, N_MONTH, N_DAY, N_HOUR, N_MINUTE, IDX)

//=======================================================================//
//= Class declare.                                                      =//
//=======================================================================//
class LCDFrame : public wxFrame
{
	private:
	    // Controlers
		wxStatusBar*            m_pclsCtrlStatusBar;
		wxToolBar*              m_pclsCtrlToolBar;
		MonochromeDotLCD*         m_pclsCtrlPaintPanel;
		wxTimer*                m_pclsMilliSecondTimer;
		wxTimer*                m_pclsRTCTimer;
		static LCDFrame*        m_pclsInstance;

		// Members.
		void				    _setStatusText(const wxString& strString);
		void				    _wxEvent_OnUpdateUI(wxUpdateUIEvent& clsEventObject)    {OnUpdateUI(clsEventObject);}
		void				    _wxEvent_OnSetFocus(wxFocusEvent& clsEventObject)       {SetFocus();}
		void				    _wxEvent_OnClose(wxCloseEvent& clsEventObject)          {OnClose();}
		void				    _wxEvent_OnKeyDown(wxKeyEvent& clsEventObject)          {OnKeyDown(clsEventObject);}
		void				    _wxEvent_OnPaint(wxPaintEvent& clsEventObject)          {OnPaint(clsEventObject);clsEventObject.Skip();}
		void				    _wxEvent_OnScreenshots(wxCommandEvent& clsEventObject)  {Screenshots();}
		void				    _wxEvent_OnToolCopy(wxCommandEvent& clsEventObject)     {Copy();}
		void				    _wxEvent_OnOpenScreenshotsFolder(wxCommandEvent &clsEventObject){OpenScreenshotsFolder();}
		void				    _wxEvent_OnAbout(wxCommandEvent& clsEventObject)        {;}
		void				    _wxEvent_OnExit(wxCommandEvent& clsEventObject)         {OnClose();}
		void				    _wxEvent_OnTimerEvent(wxTimerEvent& event)              {OnTimerEvent(event);}
		void				    _wxEvent_OnRTCUpdate(wxTimerEvent& event)               {OnRTCUpdate(event);}
		void                    _createToolbar(void);
		void                    _initializeTimer(void);
		void                    _startTimer(int32_t iMilliSecondTimerInterval);

	protected:
        virtual void		    OnUpdateUI(wxUpdateUIEvent& clsEventObject);
		virtual void		    OnClose(void);
		virtual void		    OnKeyDown(wxKeyEvent& clsEventObject);
		virtual void		    OnPaint(wxPaintEvent &clsEventObject);
		virtual void		    Screenshots(void);
		virtual void		    Copy(void);
		virtual void		    OpenScreenshotsFolder(void);
		virtual void            OnTimerEvent(wxTimerEvent& event);
		virtual void            OnRTCUpdate(wxTimerEvent& event);
	public:
                                LCDFrame(   wxWindow* pclsParent,
                                            wxWindowID iID =                wxID_MAIN,
                                            const wxString& strTitle =      FRAME_TITLE);

                                ~LCDFrame();
        static void             Initialize( wxWindow* pclsParent,
                                            wxWindowID iID =                wxID_MAIN,
                                            const wxString& strTitle =      FRAME_TITLE);
        static LCDFrame*        GetInstance(void);
        void                    SetLCDPixel(uint32_t uiPosX, uint32_t uiPosY, uint32_t uiPixelValue);
        uint32_t                GetLCDPixel(uint32_t uiPosX, uint32_t uiPosY);
        void                    RefreshLCD(void);
        void                    ClearLCD(void);
		DECLARE_EVENT_TABLE();

};

#endif //__INCLUDE_CLASS_LCD_FRAME_H__
