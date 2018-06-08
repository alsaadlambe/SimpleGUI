/*************************************************************************/
/** Copyright.															**/
/** FileName: LCDFrame.cpp											    **/
/** Author: Polarix														**/
/** Version: 1.7.0.0													**/
/** Description: Main frame class define.                               **/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include <wx/msgdlg.h>
#include "LCDFrame.h"
#include "UserActionInterface.h"

//=======================================================================//
//= Static class member define.			    						    =//
//=======================================================================//
LCDFrame* LCDFrame::m_pclsInstance = NULL;

//=======================================================================//
//= Event table.													    =//
//=======================================================================//
BEGIN_EVENT_TABLE(LCDFrame,wxFrame)
    EVT_UPDATE_UI(wxID_MAIN, LCDFrame::_wxEvent_OnUpdateUI)
	EVT_SET_FOCUS(LCDFrame::_wxEvent_OnSetFocus)
    EVT_CLOSE(LCDFrame::_wxEvent_OnClose)
    EVT_SET_FOCUS(LCDFrame::_wxEvent_OnSetFocus)
    EVT_KEY_DOWN(LCDFrame::_wxEvent_OnKeyDown)
    EVT_PAINT(LCDFrame::_wxEvent_OnPaint)
    EVT_TOOL(wxID_TOOLBAR_QUICKSHOTS, LCDFrame::_wxEvent_OnScreenshots)
    EVT_TOOL(wxID_TOOLBAR_COPY, LCDFrame::_wxEvent_OnToolCopy)
    EVT_TOOL(wxID_TOOLBAR_SCREENSHOTS_FOLDER, LCDFrame::_wxEvent_OnOpenScreenshotsFolder)
    EVT_TOOL(wxID_TOOLBAR_EXIT, LCDFrame::_wxEvent_OnExit)
    EVT_TIMER(WXID_MILLISECOND_TIMER, LCDFrame::_wxEvent_OnTimerEvent)
    EVT_TIMER(WXID_RTC_TIMER , LCDFrame::_wxEvent_OnRTCUpdate)
END_EVENT_TABLE()

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	LCDFrame                                            **/
/** Purpose:		Constructor of main frame object.                   **/
/** Params:																**/
/**	@ pclsParent[in]:   Parent object pointer.                          **/
/**	@ iID[in]:          Frame object ID.                                **/
/**	@ strTitle[in]:     Frame window title.                             **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
LCDFrame::LCDFrame(wxWindow* pclsParent, wxWindowID iID, const wxString& strTitle) :
wxFrame(pclsParent, iID, strTitle, wxDefaultPosition, wxDefaultSize, wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
	SetSizeHints( wxDefaultSize, wxDefaultSize );
	// Set frame icon.
	SetIcon(wxIcon(_T("ID_ICON_MAIN"), wxBITMAP_TYPE_ICO_RESOURCE));
	// Create tools bar and tool button.
    _createToolbar();
    // Create status bar.
    m_pclsCtrlStatusBar = CreateStatusBar(1, wxST_SIZEGRIP, wxID_STATUSBAR);
    // Create LCD screen panel.
	m_pclsCtrlPaintPanel = new MonochromeDotLCD(this, wxID_PANEL);
	m_pclsCtrlPaintPanel->SetParameter(&g_stParameters);
	m_pclsCtrlPaintPanel->ResizeParent();

    // Set frame object position on monitor.
	Centre( wxBOTH );
	// Update frame object UI.
	UpdateWindowUI();

	if(false == wxDirExists(SCREENSHOTS_FOLDER))
	{
        wxMkdir(SCREENSHOTS_FOLDER);
	}
}

/*************************************************************************/
/** Function Name:	~LCDFrame                                           **/
/** Purpose:		Destructor of main frame object.                    **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
LCDFrame::~LCDFrame()
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	delete m_pclsCtrlStatusBar;
	delete m_pclsCtrlToolBar;
	delete m_pclsCtrlPaintPanel;
}

/*************************************************************************/
/** Function Name:	_setStatusText                                      **/
/** Purpose:		Set frame status bar text.                          **/
/** Params:																**/
/**	@ strString[in]:    Set text.                                       **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::_setStatusText(const wxString& strString)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    m_pclsCtrlStatusBar->SetStatusText(strString, 0);
}

/*************************************************************************/
/** Function Name:	_createToolbar                                      **/
/** Purpose:		Create main frame tool bar.                         **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::_createToolbar(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	wxToolBar*					pclsNewToolBar;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	pclsNewToolBar				= NULL;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    // Create tools bar.
    pclsNewToolBar = CreateToolBar(wxTB_HORIZONTAL, wxID_TOOLBAR);
    if(NULL != pclsNewToolBar)
    {
        // Screen shot button.
        pclsNewToolBar->AddTool(wxID_TOOLBAR_QUICKSHOTS, _TRANS_TEXT("Quick Shot"),
                                wxBitmap(_T("ID_TOOL_QUICKSHOTS"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxBitmap(_T("ID_TOOL_QUICKSHOTS"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxITEM_NORMAL,
                                _TRANS_TEXT("Quick Shot"),
                                _TRANS_TEXT("Quick save screen shot to file."));

        pclsNewToolBar->AddTool(wxID_TOOLBAR_COPY, _TRANS_TEXT("Copy Screen Image"),
                                wxBitmap(_T("ID_TOOL_COPYSCREENSHOT"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxBitmap(_T("ID_TOOL_COPYSCREENSHOT"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxITEM_NORMAL,
                                _TRANS_TEXT("Copy Screen Image"),
                                _TRANS_TEXT("Copy screenshots picture to clipboard."));


        pclsNewToolBar->AddTool(wxID_TOOLBAR_SCREENSHOTS_FOLDER, _TRANS_TEXT("Open Screenshots Folder"),
                                wxBitmap(_T("ID_TOOL_OPENSCREENSHOTSFOLDER"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxBitmap(_T("ID_TOOL_OPENSCREENSHOTSFOLDER"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxITEM_NORMAL,
                                _TRANS_TEXT("Open Screenshots Folder"),
                                _TRANS_TEXT("Open screenshots saved folder."));
        // Add a separator.
        pclsNewToolBar->AddSeparator();

        pclsNewToolBar->AddTool(wxID_TOOLBAR_EXIT, _TRANS_TEXT("Exit"),
                                wxBitmap(_T("ID_TOOL_EXIT"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxBitmap(_T("ID_TOOL_EXIT"), wxBITMAP_TYPE_PNG_RESOURCE),
                                wxITEM_NORMAL,
                                _TRANS_TEXT("Exit(Alt+F4)"),
                                _TRANS_TEXT("Exit."));

        pclsNewToolBar->Realize();
    }
	m_pclsCtrlToolBar = pclsNewToolBar;
}

/*************************************************************************/
/** Function Name:	_initializeTimer                                    **/
/** Purpose:		Create and initialize interrupt timer and RTC timer.**/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::_initializeTimer(void)
{
    m_pclsMilliSecondTimer = new wxTimer(this, WXID_MILLISECOND_TIMER);
	m_pclsRTCTimer = new wxTimer(this, WXID_RTC_TIMER);
}

/*************************************************************************/
/** Function Name:	_startTimer                                         **/
/** Purpose:		Create and initialize interrupt timer and RTC timer.**/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::_startTimer(int32_t iMilliSecondTimerInterval)
{
    if(NULL != m_pclsMilliSecondTimer)
    {
        if(0 < iMilliSecondTimerInterval)
        {
           m_pclsMilliSecondTimer->Start(iMilliSecondTimerInterval);
        }
    }

    if(NULL != m_pclsRTCTimer)
    {
        m_pclsRTCTimer->Start(1000);
    }
}


/*************************************************************************/
/** Function Name:	OnKeyDown                                           **/
/** Purpose:		Key press event process.                            **/
/** Params:																**/
/**	@ clsEventObject[in]: Event data object.                            **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::OnKeyDown(wxKeyEvent& clsEventObject)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
    int			                iKeyCode;

    /*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
    iKeyCode = clsEventObject.GetKeyCode();

    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	//wxMessageBox(wxString::Format("Key value: %d.", iKeyCode));
	UAIF_OnKeyPress(clsEventObject.ShiftDown(), clsEventObject.ControlDown(), clsEventObject.AltDown(), iKeyCode);
}

/*************************************************************************/
/** Function Name:	Copy                                                **/
/** Purpose:		Copy screen image to clipboard.                     **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::Copy(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	bool                        bResult;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	bResult = m_pclsCtrlPaintPanel->CopyScreenImageToClipBoard();
	if(true == bResult)
	{
		_setStatusText(_T("Copied to clipboard."));
	}
	else
	{
		_setStatusText(_T("Copy failed."));
	}

}

/*************************************************************************/
/** Function Name:	Screenshots                                         **/
/** Purpose:		Save now time screen image to a bit map file.       **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::Screenshots(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	uint32_t                    uiFileCounter;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	uiFileCounter =             1;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	// Check image file existed and
	while(true == wxFileExists(SCREENSHOTS_FILE_FULLNAME(uiFileCounter)))
	{
		uiFileCounter++;
	}
    // Try to save image file.
	if(true == m_pclsCtrlPaintPanel->SaveScreenImageToFile(SCREENSHOTS_FILE_FULLNAME(uiFileCounter)))
	{
		_setStatusText(wxString::Format("Save screen to %s.", SCREENSHOTS_FILE_NAME(uiFileCounter)));
	}
	else
	{
		_setStatusText(_T("Save screen failed."));
	}
}

/*************************************************************************/
/** Function Name:	OnUpdateUI                                          **/
/** Purpose:		Update UI event process.                            **/
/** Params:																**/
/**	@ clsEventObject[in]: Event data object.                            **/
/** Return:			None.                                               **/
/** Notice:			This function process used initialize pixel panel   **/
/**                 display when UI is ready.                           **/
/*************************************************************************/
void LCDFrame::OnUpdateUI(wxUpdateUIEvent& clsEventObject)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	static bool                 bInitialized = false;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    if(false == bInitialized)
	{
	    // Initialize display.
		m_pclsCtrlPaintPanel->CleanScreen();
		_setStatusText(_T("Initialzied."));
        bInitialized = true;
        _initializeTimer();
        _startTimer(50);

        UAIF_OnInitialize();
        m_pclsCtrlPaintPanel->RefreshDisplay();
	}

}

/*************************************************************************/
/** Function Name:	OnPaint                                             **/
/** Purpose:		Paint or repaint UI event process.                  **/
/** Params:																**/
/**	@ clsEventObject[in]: Event data object.                            **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::OnPaint(wxPaintEvent &clsEventObject)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	m_pclsCtrlPaintPanel->RefreshDisplay();
}

/*************************************************************************/
/** Function Name:	OpenScreenshotsFolder                               **/
/** Purpose:		Open screen shots image file folder.                **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::OpenScreenshotsFolder(void)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    wxExecute(wxString::Format(_T("explorer %s\\%s"), wxGetCwd(), _T(SCREENSHOTS_FOLDER_T)));
}

/*************************************************************************/
/** Function Name:	OnClose                                             **/
/** Purpose:		Called when frame close and object destroyed.       **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::OnClose(void)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    if(NULL != m_pclsMilliSecondTimer)
    {
        m_pclsMilliSecondTimer->Stop();
        delete m_pclsMilliSecondTimer;
    }
    if(NULL != m_pclsRTCTimer)
    {
        m_pclsRTCTimer->Stop();
        delete m_pclsRTCTimer;
    }

    Destroy();
}

/*************************************************************************/
/** Function Name:	RefreshLCD                                          **/
/** Purpose:		Refresh screen display by pixel color data.         **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::RefreshLCD(void)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    m_pclsCtrlPaintPanel->RefreshDisplay();
}

/*************************************************************************/
/** Function Name:	ClearLCD                                            **/
/** Purpose:		Clear screen display.                               **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::ClearLCD(void)
{
    /*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    m_pclsCtrlPaintPanel->CleanScreen();
}

/*************************************************************************/
/** Function Name:	Initialize                                          **/
/** Purpose:		Create a main frame object.                         **/
/** Params:																**/
/**	@ pclsParent[in]:   Parent object pointer.                          **/
/**	@ iID[in]:          Frame object ID.                                **/
/**	@ strTitle[in]:     Frame window title.                             **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void LCDFrame::Initialize(wxWindow* pclsParent, wxWindowID iID, const wxString& strTitle)
{
    m_pclsInstance = new LCDFrame(pclsParent, iID, strTitle);
}

/*************************************************************************/
/** Function Name:	OnThreadEnd                                         **/
/** Purpose:		Thread end event process.                           **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
LCDFrame* LCDFrame::GetInstance(void)
{
    return m_pclsInstance;
}

/*************************************************************************/
/** Function Name:	SetLCDPixel                                         **/
/** Purpose:		Set a pixel value.                                  **/
/** Params:																**/
/**	@ uiPosX[in]:       X-Coordinate of pixel.                          **/
/**	@ uiPosY[in]:       Y-Coordinate of pixel.                          **/
/**	@ strTitle[in]:     Pixel value, 0 for clear, 1 for set.            **/
/** Return:			None.                                               **/
/** Notice:			This function only change the pixel register, To    **/
/**                 update the screen display, need to call RefreshLCD  **/
/**                 function or Or use the DrawPixel function directly. **/
/*************************************************************************/
void LCDFrame::SetLCDPixel(uint32_t uiPosX, uint32_t uiPosY, uint32_t uiPixelValue)
{
    if(0 == uiPixelValue)
    {
        m_pclsCtrlPaintPanel->SetPixel(uiPosX, uiPosY, MonochromeDotLCD::LCD_PIXEL_COLOR_L);
    }
    else
    {
        m_pclsCtrlPaintPanel->SetPixel(uiPosX, uiPosY, MonochromeDotLCD::LCD_PIXEL_COLOR_H);
    }
}

uint32_t LCDFrame::GetLCDPixel(uint32_t uiPosX, uint32_t uiPosY)
{
    uint32_t                uiPixelValue;
    uiPixelValue = (uint32_t)m_pclsCtrlPaintPanel->GetPixel(uiPosX, uiPosY);
    return uiPixelValue;
}

void LCDFrame::OnTimerEvent(wxTimerEvent& event)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	static bool                 bInitialized = false;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(false == bInitialized)
	{
	    // Initialize HMI engine.
        bInitialized = true;
        UAIF_OnInitialize();
        m_pclsCtrlPaintPanel->RefreshDisplay();
	}
	else
    {
        UAIF_OnTimerEventProcess();
    }
}

void LCDFrame::OnRTCUpdate(wxTimerEvent& event)
{
    wxDateTime clsTime = wxDateTime::Now();
	UAIF_OnRTCUpdateEventProcess(   clsTime.GetYear(),
                                    clsTime.GetMonth()+1,
                                    clsTime.GetDay(),
                                    clsTime.GetHour(),
                                    clsTime.GetMinute(),
                                    clsTime.GetSecond());
}

