// MiniDebuggerDoc.cpp : implementation of the CMiniDebuggerDoc class
//

#include "stdafx.h"
#include "MiniDebugger.h"

#include "MiniDebuggerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMiniDebuggerDoc

IMPLEMENT_DYNCREATE(CMiniDebuggerDoc, CDocument)

BEGIN_MESSAGE_MAP(CMiniDebuggerDoc, CDocument)
END_MESSAGE_MAP()

// CMiniDebuggerDoc construction/destruction

CMiniDebuggerDoc::CMiniDebuggerDoc()
{
	// TODO: add one-time construction code here
}

CMiniDebuggerDoc::~CMiniDebuggerDoc()
{
}

BOOL CMiniDebuggerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMiniDebuggerDoc serialization

void CMiniDebuggerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CMiniDebuggerDoc diagnostics

#ifdef _DEBUG
void CMiniDebuggerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMiniDebuggerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMiniDebuggerDoc commands
