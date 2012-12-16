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
#if !defined(AFX_LOGWND_H__A89933A1_B6D3_43D2_955A_9F761A99B978__INCLUDED_)
#define AFX_LOGWND_H__A89933A1_B6D3_43D2_955A_9F761A99B978__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IWindowImplementation.h"

#include "ISvcUI.h"
#include "IWindowImplementation.h"

#ifdef USEPLUGS
#include "plug/plug.h"
using namespace plug;
#endif
/////////////////////////////////////////////////////////////////////////////
// CLogWnd frame

class CLogWnd : public IWindowImpl<CLogWnd>, public CDialog, public IWindowLog
#ifdef USEPLUGS
    , public PlugContainer
#endif
{
public:
	/// \name Factory stuff
	//@{
	DECL_OBJECTDESC_VARIABLE();
	static const void *NewObjectFunc(size_t lparam1, size_t lparam2); // DECL_NEWOBJECTFUNC(CLogWnd, IWindowLog);
	//@}
	IMPLSMARTREF();
	BEGINQUERYINTERFACE();
		QIMAPITF(IWindowLog)	
		QIMAPITF(IWindow)
	ENDQUERYINTERFACE();
	CLogWnd();		   // protected constructor used by dynamic creation
	virtual ~CLogWnd();

	CImageList m_imglist;
	//CCJListCtrl m_ListLog;
#ifdef USEPLUGS
	plug::Plug m_pluglogstr;	///< attached to lastlogstr
#endif
	String m_lastlogstr;
// Attributes
public:
	void BuildColumns(int nCols, int * nWidth, int * iCol);

// Operations
public:

// Dialog Data
	//{{AFX_DATA(CLogWnd)
	enum { IDD = IDD_LOG };
	CListCtrl	m_ListLog;
	//}}AFX_DATA
    int numCols;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLvnItemActivateListlogging(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnHdnItemclickListlogging(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnColumnclickListlogging(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	//DECLARE_IWINDOW_IMPL(CLogWnd)
    DECLARE_IWINDOW_IMPL_NOSETTITLE(CLogWnd)
	virtual IWindow* SetTitle(LPCSTR title);
	virtual IWindow* GetTitle(char * title, int maxlen);

	virtual IWindowContainer	*AsWindowContainer(){ return NULL; };
    virtual IWindowFolding	*AsWindowFolding(){ return NULL; };
	virtual IWindowConsole	  *AsWindowConsole()  { return NULL; };
	virtual IWindowHandler	  *AsWindowHandler()  { return NULL; };
	virtual IWindowLog		  *AsWindowLog()	  { return this; };
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

	virtual IWindowLog* Clear();
	virtual IWindowLog* AddMessage(LPCSTR fmt, ...);
	virtual IWindowLog* AddMessage(int level, LPCSTR fmt, ...);
    virtual int         GetNumItems();
    virtual IWindowLog* AddColumns(int n, int* psz, LPSTR *pstr);
    virtual IWindowLog* DelColumns(int n);
    virtual int         GetNumColumns();
    virtual IWindowLog* SetColumSize(int c, int sz);

#ifdef USEPLUGS
	virtual bool plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent); // from PlugContainer
#endif
    afx_msg void OnNMClickListlogging(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnItemchangedListlogging(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGWND_H__A89933A1_B6D3_43D2_955A_9F761A99B978__INCLUDED_)
