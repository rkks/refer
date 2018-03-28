#pragma once


namespace MiniDebugger
{

	class Backtrace
	{
	public:
		Backtrace(void);
		void	Add(DWORD);
		bool	Get(int, DWORD&);

	public:
		~Backtrace(void);
	private:
		CList<DWORD>*	addrList;
	};

}