#pragma once

// CDebugger

#define WM_MSG_UI (WM_USER + 33)

class CDebugger : public CWinThread
{
	DECLARE_DYNCREATE(CDebugger)
protected:
	CDebugger();           // protected constructor used by dynamic creation
	virtual ~CDebugger();

public:
	CDebugger(int);

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int	Run();

protected:
public:
	afx_msg void OnRange(UINT pParam, long lParam);
	DECLARE_MESSAGE_MAP()
};


