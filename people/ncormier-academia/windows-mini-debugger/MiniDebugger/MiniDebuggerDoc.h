// MiniDebuggerDoc.h : interface of the CMiniDebuggerDoc class
//


#pragma once


class CMiniDebuggerDoc : public CDocument
{
protected: // create from serialization only
	CMiniDebuggerDoc();
	DECLARE_DYNCREATE(CMiniDebuggerDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CMiniDebuggerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


