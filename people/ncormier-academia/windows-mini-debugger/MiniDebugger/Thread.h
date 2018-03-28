#pragma once

#define TF_BIT			0x100 

#include "Backtrace.h"
#include "Process.h"

namespace MiniDebugger
{

	class Thread
	{
	public:
		Thread(DWORD, MiniDebugger::Process*);
		bool		Running();
		void		Suspend();
		void		Resume();
		void		Continue();
		bool		PostNullMsg();
		void		SingleStep();
		void		Backtrace(MiniDebugger::Backtrace&);
		void		RecurStackCallTrace(MiniDebugger::Backtrace&, CONTEXT);

	public:
		~Thread(void);

	private:
		DWORD iThread;
		MiniDebugger::Process* myProcess;
		HANDLE hThread;
		bool running;

	};
}
