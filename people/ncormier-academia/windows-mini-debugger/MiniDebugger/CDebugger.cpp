// CDebugger.cpp : implementation file
//

#include "stdafx.h"
#include "MiniDebugger.h"
#include "CDebugger.h"


// CDebugger

IMPLEMENT_DYNCREATE(CDebugger, CWinThread)

CDebugger::CDebugger()
{
}

CDebugger::CDebugger(int i)
{
}

CDebugger::~CDebugger()
{
}

BOOL CDebugger::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CDebugger::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

int	CDebugger::Run()
{
	//if (!this->_attach)
	//{	
	//	if (!this->CreateSonProcess())
	//	{
	//		//erreur
	//	}
	//}
	//this->Loop();
	return (1);
}

BEGIN_MESSAGE_MAP(CDebugger, CWinThread)
	ON_THREAD_MESSAGE(WM_MSG_UI, OnRange)
END_MESSAGE_MAP()


// CDebugger message handlers

void CDebugger::OnRange(UINT pParam, long lParam)
{
	
}