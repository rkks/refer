#include "StdAfx.h"
#include "DebuggerException.h"

namespace MiniDebugger
{
	DebuggerException::DebuggerException(CString errStr)
	{
		this->errString = errStr;
	}

	CString DebuggerException::ToString(void)
	{
		return (this->errString);
	}


	DebuggerException::~DebuggerException(void)
	{
	}
}