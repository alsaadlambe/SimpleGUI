#include "wx\wx.h"
#include "Application.h"
#include "Common.h"

IMPLEMENT_APP(Application);

bool Application::OnInit(void)
{
    /*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
    bool                        bReturn;

    /*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	bReturn =                   true;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    // Register image media handle.
    wxInitAllImageHandlers();
    // Read virtual device parameter.
    SetDefaultParameterData(&g_stParameters);
    // Create main frame instance.
    LCDFrame::Initialize((wxFrame*)NULL);

	m_pclsMainFrame = LCDFrame::GetInstance();

	if(NULL != m_pclsMainFrame)
    {
        SetTopWindow(m_pclsMainFrame);
        m_pclsMainFrame->Show();
    }
    else
    {
        // Create instance failed.
        bReturn = false;
    }
    return bReturn;
}


int Application::OnExit()
{
    return 0;
}
