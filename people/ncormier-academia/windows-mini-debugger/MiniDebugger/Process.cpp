#include "StdAfx.h"
#include "Process.h"
#include "DebuggerException.h"

namespace MiniDebugger
{

	Process::Process(DWORD iProcess)
	{
		this->iProcess = iProcess;
		this->hProcess = ::OpenProcess(PROCESS_VM_READ, FALSE, this->iProcess);
		if (this->hProcess == NULL)
		{
			CString str;
			str.Format("%s, error code (%d)", "/!\\ Error OpenProcess /!\\", GetLastError());
			throw new DebuggerException(str);
		}
	}

	Process::~Process(void)
	{
		if (this->hProcess)
		{
			if (!CloseHandle(hProcess))
			{
				CString str;
				str.Format("%s, error code (%d)", "/!\\ Error CloseHandle /!\\", GetLastError());
				throw new DebuggerException(str);
			}
		}
	}

	// ------------------------------------------------------
	// Process info.
	// ------------------------------------------------------
	//
	DWORD Process::GetId()
	{
		return (this->iProcess);
	}
	//
	// ------------------------------------------------------
	// ------------------------------------------------------

	// ------------------------------------------------------
	// Read from process.
	// ------------------------------------------------------
	//
	SIZE_T Process::Read(LPCVOID addr, LPVOID laddr, SIZE_T size)
	{
		SIZE_T readed;

		if (!::ReadProcessMemory(this->hProcess, (LPCVOID) addr, laddr, size, &readed))
		{
			CString str;
			str.Format("%s, error code (%d)", "/!\\ Error ReadProcessMemory /!\\", GetLastError());
			throw new DebuggerException(str);
		}
		return (readed);
	}
	//
	// ------------------------------------------------------
	// ------------------------------------------------------

}