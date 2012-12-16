#if !defined(AFX_PROFILEWND_H__04A57D51_5AEC_4BE9_A198_E097034EAFC0__INCLUDED_)
#define AFX_PROFILEWND_H__04A57D51_5AEC_4BE9_A198_E097034EAFC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IWindowImplementation.h"

/////////////////////////////////////////////////////////////////////////////
// CProfileWnd frame

class CProfileWnd : public IWindowImpl<CProfileWnd>, public CDialog, public IDlgClientProfiling, public ISmartRef
{
public:
	/// \name Factory stuff
	//@{
	DECL_OBJECTDESC_VARIABLE();
	static const void *NewObjectFunc(unsigned long lparam1, unsigned long lparam2); // DECL_NEWOBJECTFUNC(CProfileWnd, IDlgClientProfiling);
	//@}
    IMPLSMARTREF();
    BEGINQUERYINTERFACE();
		QIMAPITF(IDlgClientProfiling)
		QIMAPITF(IWindow)
    ENDQUERYINTERFACE();
	CProfileWnd();           // protected constructor used by dynamic creation
	virtual ~CProfileWnd();

// Attributes
protected:
//	CStatic		m_fps;
//	CListCtrl	m_list;

// Operations
public:

// Dialog Data
	//{{AFX_DATA(CProfileWnd)
	enum { IDD = IDD_PROFILING };
	CListCtrl	m_list;
	CStatic	m_fps;
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfileWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProfileWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_IWINDOW_IMPL(CProfileWnd)
	virtual void Printf(char* fmt, ... );
	virtual void Clear();
	virtual void AddLine(LPCSTR ave2, LPCSTR min, LPCSTR max, LPCSTR name, LPCSTR card);
	virtual void SetFPS(int fps);
	virtual void *GetPlug(int n) { return NULL; }

	int curline;
public:
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILEWND_H__04A57D51_5AEC_4BE9_A198_E097034EAFC0__INCLUDED_)
