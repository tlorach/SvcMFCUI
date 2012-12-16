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
#if !defined(AFX_LABEL_H__23D56EBF_CDEC_48BA_B7F5_46373C78F9A4__INCLUDED_)
#define AFX_LABEL_H__23D56EBF_CDEC_48BA_B7F5_46373C78F9A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Label.h : header file
//
#include "ISvcUI.h"
#include "IWindowImplementation.h"

#ifdef USEPLUGS
#include "plug/plug.h"
using namespace plug;
#endif
/////////////////////////////////////////////////////////////////////////////
// CLabel dialog

class CLabel : public IWindowImpl<CLabel>, public CDialog, public IControlLabel
#ifdef USEPLUGS
    , public PlugContainer
#endif
    , public CBindVariable//, public ISmartRef
{
// Construction
public:
	/// \name Factory stuff
	//@{
	DECL_OBJECTDESC_VARIABLE();
	static const void *NewObjectFunc(size_t lparam1, size_t lparam2); // DECL_NEWOBJECTFUNC(CTabWnd, IDlgLog);
	//@}
	IMPLSMARTREF();
	BEGINQUERYINTERFACE();
		QIMAPITF(IControlLabel)	
		QIMAPITF(IWindow)
		QIMAPITF(CBindVariable)
	ENDQUERYINTERFACE();
	CLabel(CWnd* pParent = NULL);   // standard constructor

#ifdef USEPLUGS
	plug::Plug  m_plugstring;	///< attached to m_string
#endif
	String		m_string;

	//{{AFX_DATA(CLabel)
	enum { IDD = IDC_EDIT2 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLabel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
protected:
    CEdit m_edit;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLabel)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_IWINDOW_IMPL_NOSETTITLE(CLabel)
	virtual IWindow* SetTitle(LPCSTR title);
	virtual IWindow* GetTitle(char * title, int maxlen);
	virtual IWindowContainer	*AsWindowContainer(){ return NULL; };
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
	virtual IControlLabel	  *AsControlLabel()   { return this; };
	virtual IControlToolbar	 *AsControlToolbar() { return NULL; };
	virtual IControlVector	  *AsControlVector()  { return NULL; };
	virtual IControlStatic	  *AsControlStatic()  { return NULL; };
	virtual IControlScalar	  *AsControlBoolean() { return NULL; };
	virtual IControlScalar	  *AsControlCheck()   { return NULL; };
	virtual IWindow				*AsControlButton()  { return NULL; };
	virtual IControlVector	  *AsControlColor()   { return NULL; };
	/// \name From IControlLabel
	//@{
    virtual IControlLabel* SetString(LPCSTR fmt, ...);
	//@}

#ifdef USEPLUGS
	virtual bool plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent); // from PlugContainer
#endif
	virtual void UpdateControl() {}; // for Binding a variable...TODO!
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LABEL_H__23D56EBF_CDEC_48BA_B7F5_46373C78F9A4__INCLUDED_)
