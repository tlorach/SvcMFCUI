/*
	Copyright (c) 2006 Tristan Lorach (lorachnroll@gmail.com)
	All rights reserved.

	Redistribution and use in source and binary forms, with or
	without modification, are permitted provided that the following
	conditions are met:

	 * Redistributions of source code must retain the above
	   copyright notice, this list of conditions and the following
	   disclaimer.

	   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
	   REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
	   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
	   POSSIBILITY OF SUCH DAMAGE. 

*/
#if !defined(AFX_TABWND_H__77AEDE62_5F7D_4EC0_A7FD_3EA09E865DF1__INCLUDED_)
#define AFX_TABWND_H__77AEDE62_5F7D_4EC0_A7FD_3EA09E865DF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabWnd.h : header file
//
#include "ISvcUI.h"
#include "IWindowImplementation.h"
#include <vector>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CTabWnd dialog

class CTabWnd : public IWindowImpl<CTabWnd>, public CDialog, public IWindowContainer //, public ISmartRef
{
// Construction
public:
	CTabWnd(CWnd* pParent = NULL);   // standard constructor

	/// \name Factory stuff
	//@{
	DECL_OBJECTDESC_VARIABLE();
	static const void *NewObjectFunc(size_t lparam1, size_t lparam2); // DECL_NEWOBJECTFUNC(CTabWnd, IDlgLog);
	//@}
	IMPLSMARTREF();
	BEGINQUERYINTERFACE();
		QIMAPITF(IWindowContainer)	
		QIMAPITF(IWindow)
	ENDQUERYINTERFACE();
	virtual ~CTabWnd();

	/*struct TWindows
	{
		TWindows(IWindow *_itf, CWnd *_cwnd) : itf(_itf), cwnd(_cwnd) {}
		SmartPtr<IWindow>	itf;
		CWnd				*cwnd;
	};
	vector<TWindows> m_controls;*/
	vector<SmartPtr<IWindow> > m_controls;
	CWnd		*m_currentWnd;
// Dialog Data
	//{{AFX_DATA(CTabWnd)
	enum { IDD = IDD_TAB };
	CTabCtrl	m_TabCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeTabctrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_IWINDOW_IMPL(CTabWnd)
	virtual IWindowContainer	*AsWindowContainer(){ return this; };
    virtual IWindowFolding	*AsWindowFolding(){ return NULL; };
	virtual IWindowConsole	  *AsWindowConsole()  { return NULL; };
	virtual IWindowHandler	  *AsWindowHandler()  { return NULL; };
	virtual IWindowLog		  *AsWindowLog()	  { return NULL; };
	virtual IWindowSplitter	 *AsWindowSplitter() { return NULL; };
	virtual IProgressBar		*AsProgressBar()	{ return NULL; };
	virtual IControlCombo	   *AsControlCombo()   { return NULL; };
	virtual IControlListBox	 *AsControlListBox() { return NULL; };
	virtual IControlScalar	  *AsControlScalar()  { return NULL; };
	virtual IControlString	  *AsControlString()  { return NULL; };
	virtual IControlLabel	  *AsControlLabel()   { return NULL; };
	virtual IControlToolbar	 *AsControlToolbar() { return NULL; };
	virtual IControlVector	  *AsControlVector()  { return NULL; };
	virtual IControlStatic	  *AsControlStatic()  { return NULL; };
	virtual IControlScalar	  *AsControlBoolean() { return NULL; };
	virtual IControlScalar	  *AsControlCheck()   { return NULL; };
	virtual IWindow				*AsControlButton()  { return NULL; };
	virtual IControlVector	  *AsControlColor()   { return NULL; };
	/// \name From IWindowContainer
	//@{
	virtual int GetNumItems();
	virtual IWindowContainer* AddItem(LPCSTR title, IWindow *lpWnd, bool bUpdateComponents);
	//virtual IWindowContainer* AddItem(LPCSTR title, LPCSTR windowname, bool bUpdateComponents);
	virtual IWindowContainer* RemoveItem(int Itemnum, IWindow **lpWnd=NULL, bool bUpdateComponents=false); 
	virtual IWindowContainer* RemoveItem(IWindow *lpWnd, bool bUpdateComponents);
	virtual IWindowContainer* SelectItem(int Itemnum);
	virtual IWindowContainer* SelectItem(IWindow *pw);
	virtual IWindow*		  GetItemNum(int Itemnum);
	virtual void				Refresh();
	//@}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABWND_H__77AEDE62_5F7D_4EC0_A7FD_3EA09E865DF1__INCLUDED_)
