#include "StdAfx.h"
#include "Thread.h"
#include "DebuggerException.h"

namespace MiniDebugger
{

	Thread::Thread(DWORD iThread, MiniDebugger::Process* myProcess)
	{
		this->iThread = iThread;
		this->myProcess = myProcess;
		this->running = true;

		this->hThread = ::OpenThread(THREAD_SUSPEND_RESUME, FALSE, this->iThread);		

		if (this->hThread == NULL)
		{
			CString str;
			str.Format("%d: %s, error code (%d)", this->iThread, "/!\\ Error OpenThread /!\\", GetLastError());
			throw new DebuggerException(str);
			return;
		}
	}

	Thread::~Thread(void)
	{
		if (this->hThread)
		{
			if (!CloseHandle(this->hThread))
			{
				CString str;
				str.Format("%d: %s, error code (%d)", this->iThread, "/!\\ Error CloseHandle /!\\", GetLastError());
				throw new DebuggerException(str);
			}
		}
	}

	// ------------------------------------------------------
	// Threads Control.
	// ------------------------------------------------------
	//
	bool Thread::Running(void)
	{
		return (this->running);
	}
	//
	// ------------------------------------------------------
	// ------------------------------------------------------

	void Thread::Suspend(void)
	{
		BOOL ret = ::SuspendThread(this->hThread);
		if (ret != 0)
		{
			CString str;
			str.Format("%d: %s, error code (%d)", this->iThread, "/!\\ Error SuspendThread /!\\", GetLastError());
			throw new DebuggerException(str);
		}
	}
	//
	// ------------------------------------------------------
	// ------------------------------------------------------

	void Thread::Resume(void)
	{
		if (::ResumeThread(this->hThread) == (DWORD) -1)
		{
			CString str;
			str.Format("%d: %s, error code (%d)", this->iThread, "/!\\ Error ResumeThread /!\\", GetLastError());
			throw new DebuggerException(str);
		}
	}
	//
	// ------------------------------------------------------
	// ------------------------------------------------------

	void Thread::Continue(void)
	{
		DWORD dwContinueStatus = DBG_CONTINUE;
		if (ContinueDebugEvent(this->myProcess->GetId(), this->iThread, dwContinueStatus) == TRUE)
		{
			return;
		}
		CString str;
		str.Format("%d: %s, error code (%d)", this->iThread, "/!\\ Error ContinueDebugEvent /!\\", GetLastError());
		throw new DebuggerException(str);
	}
	//
	// ------------------------------------------------------
	// ------------------------------------------------------

	// ------------------------------------------------------
	// Send NULL Msg on Every Thread.
	// ------------------------------------------------------
	//
	bool Thread::PostNullMsg(void)
	{
		if (::PostThreadMessage(this->iThread, WM_NULL, NULL, NULL) == FALSE)
			return (false);
		return (true);
	}
	//
	// ------------------------------------------------------
	// ------------------------------------------------------

	// ------------------------------------------------------
	// Single-Stepping
	// ------------------------------------------------------
	//
	void Thread::SingleStep(void)
	{
		CONTEXT currentContext;
		currentContext.ContextFlags = CONTEXT_FULL;

		if(!::GetThreadContext(this->hThread, &currentContext))
		{
			CString str;
			str.Format("%d: %s, error code (%d)", this->iThread, "/!\\ Error GetThreadContext /!\\", GetLastError());
			throw new DebuggerException(str);
		}
		// Set the trap flag
		//
		currentContext.EFlags |= TF_BIT ;
		currentContext.ContextFlags = CONTEXT_FULL;

		if(!::SetThreadContext(this->hThread, &currentContext))
		{
			CString str;
			str.Format("%d: %s, error code (%d)", this->iThread, "/!\\ Error SetThreadContext /!\\", GetLastError());
			throw new DebuggerException(str);
		}
	}
	//
	// ------------------------------------------------------
	// ------------------------------------------------------

	// ------------------------------------------------------
	// Backtrace
	// ------------------------------------------------------
	//
	void Thread::Backtrace(MiniDebugger::Backtrace& ret)
	{
		CONTEXT currentContext;
		currentContext.ContextFlags = CONTEXT_FULL;

		if(!::GetThreadContext(hThread, &currentContext))
		{
			CString str;
			str.Format("%d: %s, error code (%d)", iThread, "/!\\ Error GetThreadContext /!\\", GetLastError());
			throw new DebuggerException(str);
		}

		// Backtrace.
		//
		ret.Add(currentContext.Eip);
		this->RecurStackCallTrace(ret, currentContext);
	}

	void Thread::RecurStackCallTrace(MiniDebugger::Backtrace& btrace, CONTEXT currentContext)
	{
		DWORD eip, ebp, ret;
		eip = currentContext.Eip;
		ebp = currentContext.Ebp;

		DWORD cur = currentContext.Ebp;

		this->myProcess->Read((LPCVOID) cur, &ret, sizeof(DWORD));

		currentContext.Ebp = ret;
		if (ret <= ebp)
			return;

		this->myProcess->Read((LPCVOID) (cur + sizeof(DWORD)), &ret, sizeof(DWORD));
	
		if (ret == 0)
			return;

		btrace.Add(ret);

		RecurStackCallTrace(btrace, currentContext);
	}
	//
	// ------------------------------------------------------
	// ------------------------------------------------------


}