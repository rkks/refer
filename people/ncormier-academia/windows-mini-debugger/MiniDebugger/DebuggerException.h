#pragma once

namespace MiniDebugger
{

	class DebuggerException
	{
	public:
		DebuggerException(CString);
		CString	ToString();
	public:
		~DebuggerException(void);
	private:
		CString	errString;
	};

}