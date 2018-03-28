#include "StdAfx.h"
#include "Backtrace.h"


namespace MiniDebugger
{

	Backtrace::Backtrace(void)
	{
		this->addrList = new CList<DWORD>();
	}

	Backtrace::~Backtrace(void)
	{
		delete(this->addrList);
	}

	void Backtrace::Add(DWORD addr)
	{
		this->addrList->AddTail(addr);
	}

	bool Backtrace::Get(int idx, DWORD& addr)
	{
		POSITION pos = this->addrList->FindIndex(idx);

		if (pos)
		{
			addr = this->addrList->GetAt(pos);
			return (true);
		}
		return (false);
	}
}