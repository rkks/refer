// MiniDebugger.h : main header file for the MiniDebugger application
//
#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "Debugger.h"

// CMiniDebuggerApp:
// See MiniDebugger.cpp for the implementation of this class
//

class CMiniDebuggerApp : public CWinApp
{
public:
	CMiniDebuggerApp();
	// Overrides
public:
	virtual BOOL InitInstance();
	// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

};

extern CMiniDebuggerApp theApp;