// MiniDebuggerView.h : interface of the CMiniDebuggerView class
//


#pragma once

using namespace MiniDebugger;

class CMiniDebuggerView : public CListView
{
protected: // create from serialization only
	CMiniDebuggerView();
	DECLARE_DYNCREATE(CMiniDebuggerView)

// Attributes
public:
	CMiniDebuggerDoc* GetDocument() const;

// Mine
public:
	CList<CString>	_listString;
private:
	Debugger*		_debugger;

// Menu Implementation
public:
	// On Command
	void	OnCommandDebuggerMenuAttachProcess(void);
	void	OnCommandDebuggerMenuFileOpen(void);
	void	OnCommandActionHello(void);
	void	OnCommandActionStop(void);
	void	OnCommandActionStep(void);
	void	OnCommandActionContinue(void);
	void	OnCommandActionBacktrace(void);
	// On Update
	void	OnUpdateActionHello(CCmdUI*);
	void	OnUpdateActionStop(CCmdUI*);
	void	OnUpdateActionStep(CCmdUI*);
	void	OnUpdateActionContinue(CCmdUI*);
	void	OnUpdateActionBacktrace(CCmdUI*);

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CMiniDebuggerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg LRESULT	OnDebugMessage(UINT wParam, LONG lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MiniDebuggerView.cpp
inline CMiniDebuggerDoc* CMiniDebuggerView::GetDocument() const
   { return reinterpret_cast<CMiniDebuggerDoc*>(m_pDocument); }
#endif

