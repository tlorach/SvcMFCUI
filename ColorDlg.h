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
#if !defined(AFX_COLORDLG_H__23D56EBF_CDEC_48BA_B7F5_46373C78F9A4__INCLUDED_)
#define AFX_COLORDLG_H__23D56EBF_CDEC_48BA_B7F5_46373C78F9A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ISvcUI.h"
#include "IWindowImplementation.h"

#ifdef USEPLUGS
#include "plug/plug.h"
using namespace plug;
#endif
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CMyStatic

class CMyButton : public CButton
{
public:
	COLORREF m_c;
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		CDC dc;
		RECT r;
		r.left = lpDrawItemStruct->rcItem.left + 3;
		r.right = lpDrawItemStruct->rcItem.right - 3;
		r.top = lpDrawItemStruct->rcItem.top + 3;
		r.bottom = lpDrawItemStruct->rcItem.bottom - 3;
		dc.Attach(lpDrawItemStruct->hDC);
		dc.FillSolidRect(&(lpDrawItemStruct->rcItem), 0);
		dc.FillSolidRect(&(r), m_c);
	}
};
/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog

class CColorDlg : public IWindowImpl<CColorDlg>, public CDialog, public IControlVector
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
	static const void *NewObjectFunc(size_t lparam1, size_t lparam2); // DECL_NEWOBJECTFUNC(CTabWnd, IWindowLog);
	//@}
	IMPLSMARTREF();
	BEGINQUERYINTERFACE();
		QIMAPITF(IControlVector)	
		QIMAPITF(IWindow)
		QIMAPITF(CBindVariable)
	ENDQUERYINTERFACE();
	CColorDlg(CWnd* pParent = NULL);   // standard constructor

#ifdef USEPLUGS
	plug::Plug m_plugvector;	///< attached to m_vec[4]
#endif
	float		m_vec[4];
	CString		m_tmpstr;
// Dialog Data
	//{{AFX_DATA(CColorDlg)
	enum { IDD = IDD_COLOR };
	CEdit	m_compw;
	CStatic	m_title;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColorDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateCompw();
	afx_msg void OnColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_IWINDOW_IMPL_NOSETTITLE(CColorDlg)
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
	virtual IControlLabel	  *AsControlLabel()   { return NULL; };
	virtual IControlToolbar	 *AsControlToolbar() { return NULL; };
	virtual IControlVector	  *AsControlVector()  { return this; };
	virtual IControlStatic	  *AsControlStatic()  { return NULL; };
	virtual IControlScalar	  *AsControlBoolean() { return NULL; };
	virtual IControlScalar	  *AsControlCheck()   { return NULL; };
	virtual IWindow				*AsControlButton()  { return NULL; };
	virtual IControlVector	  *AsControlColor()   { return this; };
	/// \name From IControlVector
	//@{
	virtual IControlVector* SetIntMode(bool bYes=true);
	virtual IControlVector* SetDimension(int dim);
	virtual IControlVector* GetValuesAsInt(int *x, int *y=NULL, int *z=NULL, int *w=NULL);
	virtual IControlVector* GetValuesAsFloat(float *x, float *y=NULL, float *z=NULL, float *w=NULL);
	virtual IControlVector* GetValuesAsDouble(double *x, double *y=NULL, double *z=NULL, double *w=NULL);
	virtual IControlVector* SetValue(float x, float y, float z, float w);
	virtual IControlVector* SetValue(int n, float val);
	//@}
	void UpdateValue();
	virtual void UpdateControl();
public:
	CMyButton m_colorbutton;
	afx_msg void OnClickedButtonColor();
#ifdef USEPLUGS
  virtual bool plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent); // from PlugContainer
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORDLG_H__23D56EBF_CDEC_48BA_B7F5_46373C78F9A4__INCLUDED_)
