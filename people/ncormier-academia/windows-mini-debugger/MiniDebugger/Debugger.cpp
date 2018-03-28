// Debugger.cpp
//

#include "stdafx.h"
#include "MiniDebugger.h"
#include "Debugger.h"
#include "MsgUI.h"

using namespace MiniDebugger;

static UINT WM_MSG_DEBUGGER = WM_USER + 10;
//static UINT WM_MSG_UI = ::RegisterWindowMessage("WM_MSG_UI");//WM_USER + 20;

IMPLEMENT_DYNCREATE(Debugger, CWinThread)

//
// -------------------------------------------------------------------
// -------------------------------------------------------------------
// Ctor, DCtor
//
Debugger::Debugger()
{
	// Oooops
}

Debugger::Debugger(HWND ui, DWORD pid)
{
	this->_pause = false;
	this->_attach = true;
	this->_processId = pid;
	this->_hWin = ui;
}

Debugger::~Debugger()
{
}

Debugger::Debugger(HWND ui, CString& path)
{
	this->_pause = false;
	this->_hWin = ui;
	this->_attach = false;
	this->_path = path;
	this->_processId = -1;
}

//
// -------------------------------------------------------------------
// -------------------------------------------------------------------
// Override CWinThread Methods
//
BOOL Debugger::InitInstance()
{
	return (TRUE);
}

int Debugger::ExitInstance()
{
	if (!this->_attach)
	{

	}
	return (0);
}

int Debugger::Run()
{
	if (!this->_attach)
	{	
		if (!this->CreateSonProcess())
		{
			//erreur
		}
	}
	else
	{
		if (!this->AttachProcess())
		{
			//erreur
		}
	}

	DEBUG_EVENT DebugEvent;
	MSG msg;

	while (true)
	{
		SetLastError(0);
		if(WaitForDebugEvent(&DebugEvent, (DWORD) 100))
		{
			this->ProcessDebugEvents(DebugEvent);
		}
		else
		{
			if( GetLastError() != 0 )
			{
				/* Handle error condition. */
			}
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				switch (msg.message)
				{
				case (UI_MSG_HELLO):
					this->OnHelloMsg(msg.wParam, msg.lParam);
					break;
				case (UI_MSG_STOP):
					this->OnStopMsg(msg.wParam, msg.lParam);
					break;
				case (UI_MSG_STEP):
					this->OnStepMsg(msg.wParam, msg.lParam);
					break;
				case (UI_MSG_CONT):
					this->OnContinueMsg(msg.wParam, msg.lParam);
					break;
				case (UI_MSG_DETACH):
					this->OnDetachMsg(msg.wParam, msg.lParam);
					break;
				case (UI_MSG_KILL):
					this->OnKillMsg(msg.wParam, msg.lParam);
					break;
				case (UI_MSG_BACKT):
					this->OnBacktraceMsg(msg.wParam, msg.lParam);
					break;
				case (UI_MSG_BREAKP):
					this->OnBreakpointMsg(msg.wParam, msg.lParam);
					break;
				}
			}
		}

	}
	return (0);
}

// -------------------------------------------------------------------
// -------------------------------------------------------------------
//
bool Debugger::SendDebugString(CString msg)
{
	CString* toSend = new CString(msg);
	return (::SendMessage(this->_hWin, WM_MSG_DEBUGGER, 0, (LPARAM) toSend));
}

// -----------------------------------------------------
// Debug API.
// ------------------------------------------------------
//

bool Debugger::ProcessDebugEvents(DEBUG_EVENT DebugEv)
{
	CString str;
	str.Format("%d: ", DebugEv.dwThreadId);

	bool suspendThread = false;

	switch (DebugEv.dwDebugEventCode)
	{ 
	case EXCEPTION_DEBUG_EVENT: 
		switch(DebugEv.u.Exception.ExceptionRecord.ExceptionCode)
		{ 
		case EXCEPTION_ACCESS_VIOLATION: 
			str += "exception_access_violation";
			this->SendDebugString("DebugEvent from " + str);
			break;

		case EXCEPTION_BREAKPOINT: 
			str += "exception_breakpoint";
			this->SendDebugString("DebugEvent from " + str);
			break;

		case EXCEPTION_DATATYPE_MISALIGNMENT: 
			str += "exception_datatype_misalignment";
			this->SendDebugString("DebugEvent from " + str);
			break;

		case EXCEPTION_SINGLE_STEP: 
			str += "exception_single_step";
			this->SendDebugString("DebugEvent from " + str);
			//
			// Waiting for User Command
			suspendThread = true;
			break;

		case DBG_CONTROL_C: 
			str += "dbg_control_c";
			this->SendDebugString("DebugEvent from " + str);
			break;

		default:
			str += "Oups";
			this->SendDebugString("DebugEvent from " + str);
			break;
		} 
		break;
	case CREATE_THREAD_DEBUG_EVENT: 
		str += "create_thread_debug_event";
		this->SendDebugString("DebugEvent from " + str);
		// Add thread to list;
		this->_threadList.AddHead(DebugEv.dwThreadId);
		break;

	case CREATE_PROCESS_DEBUG_EVENT: 
		str += "create_process_debug_event";
		this->SendDebugString("DebugEvent from " + str);
		break;

	case EXIT_THREAD_DEBUG_EVENT: 
		str += "exit_thread_debug_event";
		this->SendDebugString("DebugEvent from " + str);
		// Remove thread from list.
		this->_threadList.RemoveAt(this->_threadList.Find(DebugEv.dwThreadId));
		break;

	case EXIT_PROCESS_DEBUG_EVENT: 
		str += "exit_process_debug_event";
		this->SendDebugString("DebugEvent from " + str);
		break;

	case LOAD_DLL_DEBUG_EVENT: 
		str += "load_dll_debug_event";
		this->SendDebugString("DebugEvent from " + str);
		break;

	case UNLOAD_DLL_DEBUG_EVENT: 
		str += "unload_dll_debug_event";
		this->SendDebugString("DebugEvent from " + str);
		break;

	case OUTPUT_DEBUG_STRING_EVENT: 
		str += "output_debug_string_event";
		this->SendDebugString("DebugEvent from " + str);
		break;

	} 

	/*if (this->Continue(DebugEv.dwProcessId, DebugEv.dwThreadId) == FALSE)
	{
		this->SendDebugString("Can't CONTINUE !!");
	}*/

	if (suspendThread)
	{
		//if (!(this->StopThread(DebugEv.dwThreadId)))
		//{
		//	this->SendDebugString("ERROR");//fatal error ?
		//}		
	}

	return (true);
}

bool Debugger::IsPause()
{
	return (this->_pause);
}

bool Debugger::AttachProcess()
{
	memset(&this->_si, 0, sizeof(this->_si));
	memset(&this->_pi, 0, sizeof(this->_pi));
	this->_si.cb = sizeof(this->_si);
	if (::DebugActiveProcess(this->_processId))
		return (true);
	return (false);
}

bool Debugger::CreateSonProcess()
{
	memset(&this->_si, 0, sizeof(this->_si));
	memset(&this->_pi, 0, sizeof(this->_pi));
	this->_si.cb = sizeof(this->_si);
	if(CreateProcess(
		this->_path.GetString(), NULL, 0, 0, FALSE,
		DEBUG_ONLY_THIS_PROCESS /*| DEBUG_PROCESS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP */,
		0, 0, &this->_si, &this->_pi) == FALSE
		)
		return (false);
	this->_processId = this->_pi.dwProcessId;
	this->_threadList.AddHead(this->_pi.dwThreadId);
	return (true);
}

//
// ------------------------------------------------------
// ------------------------------------------------------

// -----------------------------------------------------
// Threads.
// ------------------------------------------------------
//
bool Debugger::ContinueEveryThread()
{
	POSITION pos = this->_threadList.GetHeadPosition();
	for (int i = 0;i < this->_threadList.GetCount(); i++)
	{

		DWORD  iThread = this->_threadList.GetNext(pos);
		
		// No Error Check
		//
		
	}
	return (true);
}


bool Debugger::StopEveryThread()
{
	POSITION pos = this->_threadList.GetHeadPosition();
	for (int i = 0;i < this->_threadList.GetCount(); i++)
	{
		DWORD  iThread = this->_threadList.GetNext(pos);
		
	}
	return (true);
}

bool Debugger::ResumeEveryThread()
{
	POSITION pos = this->_threadList.GetHeadPosition();
	for (int i = 0;i < this->_threadList.GetCount(); i++)
	{

		DWORD  iThread = this->_threadList.GetNext(pos);


	}
	return (true);
}
//
// ------------------------------------------------------
// ------------------------------------------------------

// -----------------------------------------------------
// Send NULL Msg on Every Thread.
// ------------------------------------------------------
//

bool Debugger::PostNullMsgEveryThread()
{
	POSITION pos = this->_threadList.GetHeadPosition();
	for (int i = 0;i < this->_threadList.GetCount(); i++)
	{
		DWORD  iThread = this->_threadList.GetNext(pos);

	}
	return (true);
}
//
// ------------------------------------------------------
// ------------------------------------------------------

// -----------------------------------------------------
// Single-Stepping
// ------------------------------------------------------
//
bool Debugger::SingleStepEveryThread()
{
	POSITION pos = this->_threadList.GetHeadPosition();
	for (int i = 0;i < this->_threadList.GetCount(); i++)
	{

		DWORD  iThread = this->_threadList.GetNext(pos);

		
	}
	return (true);
}

//
// ------------------------------------------------------
// ------------------------------------------------------


// -------------------------------------------------------------------
// -------------------------------------------------------------------
// Message Map
//
BEGIN_MESSAGE_MAP(Debugger, CWinThread)
	//ON_THREAD_MESSAGE(WM_MSG_UI, OnMsgUI)
END_MESSAGE_MAP()


//// -------------------------------------------------------------------
//// -------------------------------------------------------------------
// Debugger message handlers
//
void Debugger::OnHelloMsg(WPARAM wParam, LPARAM lParam)
{
	this->SendDebugString("====> HELLO <====");
	this->StopEveryThread();
}

void Debugger::OnStopMsg(WPARAM wParam, LPARAM lParam)
{
	this->SendDebugString("====> STOP <====");

	if (this->StopEveryThread())
		this->_pause = true;
	else
		;//sendErrorString
}

void Debugger::OnStepMsg(WPARAM wParam, LPARAM lParam)
{
	this->SendDebugString("====> STEP <====");

	if (this->SingleStepEveryThread())
		if (this->ResumeEveryThread())
		{
			this->PostNullMsgEveryThread();
		}
	else
		;//SendErrorString
}

void Debugger::OnContinueMsg(WPARAM wParam, LPARAM lParam)
{
	this->SendDebugString("====> CONTINUE <====");

	if (this->ResumeEveryThread())
	{
		this->_pause = false;
		this->PostNullMsgEveryThread();
	}
	else
		;//sendErrorString
}

void Debugger::OnDetachMsg(WPARAM wParam, LPARAM lParam)
{
	this->SendDebugString("====> DETACH <====");
}

void Debugger::OnKillMsg(WPARAM wParam, LPARAM lParam)
{
	this->SendDebugString("====> KILL <====");
}

void Debugger::OnBacktraceMsg(WPARAM wParam, LPARAM lParam)
{
	this->SendDebugString("====> BACKTRACE <====");

	POSITION pos = this->_threadList.GetHeadPosition();
	for (int i = 0;i < this->_threadList.GetCount(); i++)
	{

		DWORD iThread = this->_threadList.GetNext(pos);

	}
}

void Debugger::OnBreakpointMsg(WPARAM wParam, LPARAM lParam)
{
	this->SendDebugString("====> BREAKPOINT <====");
}