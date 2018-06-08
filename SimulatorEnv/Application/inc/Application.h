/***************************************************************
 * Name:      LCDSimulatorMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2017-02-25
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef __INCLUDE_CLASS_APPLICATION_H__
#define __INCLUDE_CLASS_APPLICATION_H__

#include <wx/dialog.h>
#include <wx/app.h>

#include "LCDFrame.h"


class Application : public wxApp
{
    private:
        LCDFrame*          m_pclsMainFrame;

    private:
		bool            OnInit(void);
		int             OnExit(void);

};

#endif //__INCLUDE_CLASS_APPLICATION_H__
