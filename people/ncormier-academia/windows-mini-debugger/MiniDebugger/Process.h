#pragma once

#include "ThreadList.h"

namespace MiniDebugger
{

	class Process
	{
	public:
		Process(DWORD);
		DWORD	GetId(void);
		SIZE_T	Read(LPCVOID, LPVOID, SIZE_T);
	public:
		~Process(void);

	private:
		DWORD			iProcess;
		HANDLE			hProcess;
		ThreadList*		threadList;
	};

}