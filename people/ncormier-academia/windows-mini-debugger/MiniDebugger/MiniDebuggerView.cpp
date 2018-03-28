// MiniDebuggerView.cpp : implementation of the CMiniDebuggerView class
//

#include "stdafx.h"
#include "MiniDebugger.h"

#include "MiniDebuggerDoc.h"
#include "MiniDebuggerView.h"

#include "Debugger.h"
#include "MsgUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMiniDebuggerView
//

IMPLEMENT_DYNCREATE(CMiniDebuggerView, CListView)

//static UINT WM_MSG_DEBUGGER =  ::RegisterWindowMessage(WM_MSG_DEBUGGER);

static UINT WM_MSG_DEBUGGER = WM_USER + 10;
//static UINT WM_MSG_UI = ::RegisterWindowMessage("WM_MSG_UI");//WM_USER + 20;

BEGIN_MESSAGE_MAP(CMiniDebuggerView, CListView)
	
	// -------------------------------------------------------------------
	// Mine
	//

	//
	// On Command

	ON_COMMAND(ID_FILE_OPEN,				OnCommandDebuggerMenuFileOpen)
	ON_COMMAND(ID_DEBUGGER_ATTACHPROCESS,	OnCommandDebuggerMenuAttachProcess)
	ON_COMMAND(ID_ACTION_HELLO_,			OnCommandActionHello)
	ON_COMMAND(ID_ACTION_STOP,				OnCommandActionStop)
	ON_COMMAND(ID_ACTION_STEP,				OnCommandActionStep)
	ON_COMMAND(ID_ACTION_CONTINUE,			OnCommandActionContinue)
	ON_COMMAND(ID_ACTION_BACKTRACE,			OnCommandActionBacktrace)


	// On Message
	//
	ON_MESSAGE(WM_MSG_DEBUGGER,				OnDebugMessage)

	//
	// On Update
	ON_UPDATE_COMMAND_UI(ID_ACTION_HELLO_,	OnUpdateActionHello)
	ON_UPDATE_COMMAND_UI(ID_ACTION_STOP,	OnUpdateActionStop)
	ON_UPDATE_COMMAND_UI(ID_ACTION_STEP,	OnUpdateActionStep)
	ON_UPDATE_COMMAND_UI(ID_ACTION_CONTINUE,OnUpdateActionContinue)
	ON_UPDATE_COMMAND_UI(ID_ACTION_BACKTRACE,OnUpdateActionBacktrace)


	// ---------------------------------------------------------------
	//

END_MESSAGE_MAP()


// On Events
//

// -------------------------------------------------------------------
// -------------------------------------------------------------------
// Message
//
LRESULT	CMiniDebuggerView::OnDebugMessage(UINT wParam, LONG lParam)
{
	CString* rec = (CString*) lParam;

	CMiniDebuggerDoc* curDoc = this->GetDocument();
	CListCtrl& control = this->GetListCtrl();
	control.InsertItem(control.GetItemCount(), (LPCTSTR) *rec);


	this->PostMessageA(WM_VSCROLL, SB_LINEDOWN, 0);

	return (1);
}

// -------------------------------------------------------------------
// -------------------------------------------------------------------
// Update
//
void CMiniDebuggerView::OnUpdateActionHello(CCmdUI* pCmdUI)
{
	if (this->_debugger)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMiniDebuggerView::OnUpdateActionStop(CCmdUI* pCmdUI)
{
	if (this->_debugger && !this->_debugger->IsPause())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMiniDebuggerView::OnUpdateActionStep(CCmdUI* pCmdUI)
{
	if (this->_debugger && this->_debugger->IsPause())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMiniDebuggerView::OnUpdateActionContinue(CCmdUI* pCmdUI)
{
	if (this->_debugger && this->_debugger->IsPause())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMiniDebuggerView::OnUpdateActionBacktrace(CCmdUI* pCmdUI)
{
	if (this->_debugger && this->_debugger->IsPause())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

// -------------------------------------------------------------------
// -------------------------------------------------------------------
// Command
//
void CMiniDebuggerView::OnCommandActionHello(void)
{
	this->_debugger->PostThreadMessageA(UI_MSG_HELLO, (WPARAM) NULL, 0);
}

void CMiniDebuggerView::OnCommandActionStep(void)
{
	this->_debugger->PostThreadMessageA(UI_MSG_STEP, 0, 0);
}

void CMiniDebuggerView::OnCommandActionStop(void)
{
	this->_debugger->PostThreadMessageA(UI_MSG_STOP, 0, 0);
}

void CMiniDebuggerView::OnCommandActionContinue(void)
{
	this->_debugger->PostThreadMessageA(UI_MSG_CONT, 0, 0);
}

void CMiniDebuggerView::OnCommandActionBacktrace(void)
{
	this->_debugger->PostThreadMessageA(UI_MSG_BACKT, 0, 0);
}


void CMiniDebuggerView::OnCommandDebuggerMenuAttachProcess(void)
{
	if (this->_debugger != NULL)
	{
		AfxMessageBox("Un process est deja en cours de debug");
		return;
	}	
	this->_debugger = new Debugger(this->GetSafeHwnd(), 1048);
	this->_debugger->CreateThread();
}

void CMiniDebuggerView::OnCommandDebuggerMenuFileOpen(void)
{
	if (this->_debugger != NULL)
	{
		AfxMessageBox("Un process est deja en cours de debug");
		return;
	}
	CFileDialog file_dial(TRUE, _T("Executables"), _T("*.exe"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		TEXT("Executables (*.exe)|*.exe|Dymanic Link Libraries (*.dll)|*.dll|All Files (*.*)|*.*||"),
		AfxGetMainWnd()
		);

	if(file_dial.DoModal() == IDOK)
	{
		this->_debugger = new Debugger(this->GetSafeHwnd(), file_dial.GetFileName());
		this->_debugger->CreateThread();
	}
}

// -------------------------------------------------------------------
// -------------------------------------------------------------------
//



// CMiniDebuggerView construction/destruction

CMiniDebuggerView::CMiniDebuggerView()
{
	// TODO: add construction code here
	this->_debugger = NULL;
}

CMiniDebuggerView::~CMiniDebuggerView()
{
}

BOOL CMiniDebuggerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style |= LVS_REPORT;

	return CListView::PreCreateWindow(cs);
}

void CMiniDebuggerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	static CFont font;
	static LOGFONT logfont;

	CListCtrl &ctrl = GetListCtrl();

	logfont.lfHeight = 14;
	logfont.lfQuality = ANTIALIASED_QUALITY;
	logfont.lfPitchAndFamily = FF_MODERN | FIXED_PITCH;
	font.CreateFontIndirect(&logfont);
	ctrl.SetFont(&font);
	ctrl.InsertColumn(0, "Messages");
	ctrl.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
}

// CMiniDebuggerView diagnostics

#ifdef _DEBUG
void CMiniDebuggerView::AssertValid() const
{
	CListView::AssertValid();
}

void CMiniDebuggerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CMiniDebuggerDoc* CMiniDebuggerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMiniDebuggerDoc)));
	return (CMiniDebuggerDoc*)m_pDocument;
}
#endif //_DEBUG
