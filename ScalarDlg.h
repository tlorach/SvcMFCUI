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
#if !defined(AFX_SCALARDLG_H__23D56EBF_CDEC_48BA_B7F5_46373C78F9A4__INCLUDED_)
#define AFX_SCALARDLG_H__23D56EBF_CDEC_48BA_B7F5_46373C78F9A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScalarDlg.h : header file
//
#include "ISvcUI.h"
#include "IWindowImplementation.h"

#ifdef USEPLUGS
#include "plug/plug.h"
using namespace plug;
#endif
/////////////////////////////////////////////////////////////////////////////
// CScalarDlg dialog

class CScalarDlg : public IWindowImpl<CScalarDlg>, public CDialog, public IControlScalar
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
		QIMAPITF(IControlScalar)	
		QIMAPITF(IWindow)
		QIMAPITF(CBindVariable)
	ENDQUERYINTERFACE();
	CScalarDlg(CWnd* pParent = NULL);   // standard constructor

#ifdef USEPLUGS
	plug::Plug m_plugval;	///< 
	plug::Plug m_plugmin;	///< 
	plug::Plug m_plugmax;	///< 
#endif
	float		m_val, m_min, m_max;
	bool		m_intmode;
	float		m_multfact;
	int			m_inthectrlevent;
	CString		m_tmpstr;
// Dialog Data
	//{{AFX_DATA(CScalarDlg)
	enum { IDD = IDD_SCALAR };
	CStatic	m_title;
	CEdit	m_scalarval;
	CSliderCtrl	m_slider;
	CButton m_buttonLeft;
	CButton m_buttonRight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScalarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScalarDlg)
	afx_msg void OnChangeScalarval();
	afx_msg void OnKillfocusScalarval();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnUpdateScalarval();
	afx_msg void OnBnClickedButtonLeft();
	afx_msg void OnBnClickedButtonRight();
	virtual BOOL OnInitDialog();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_IWINDOW_IMPL_NOSETTITLE(CScalarDlg)
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
	virtual IControlScalar	  *AsControlScalar()  { return this; };
	virtual IControlString	  *AsControlString()  { return NULL; };
	virtual IControlLabel	  *AsControlLabel()   { return NULL; };
	virtual IControlToolbar	 *AsControlToolbar() { return NULL; };
	virtual IControlVector	  *AsControlVector()  { return NULL; };
	virtual IControlStatic	  *AsControlStatic()  { return NULL; };
	virtual IControlScalar	  *AsControlBoolean() { return NULL; };
	virtual IControlScalar	  *AsControlCheck()   { return NULL; };
	virtual IWindow				*AsControlButton()  { return NULL; };
	virtual IControlVector	  *AsControlColor()   { return NULL; };
	/// \name From IControlScalar
	//@{
	virtual LPCSTR		  GetValueAsString();
	virtual IControlScalar* SetIntMode(bool bYes=true);
	virtual IControlScalar* SetValue(float s);
	virtual float		   GetValue();
	virtual IControlScalar* SetBounds(float min, float max);
	virtual IControlScalar* SetStep(float s);
	virtual bool			GetChecked() {return false; }
	virtual IControlScalar* SetChecked( bool bChecked ) { return this; };
#ifdef USEPLUGS
	/*virtual void *GetPlug(int n)
	{
		switch(n)
		{
		case 0://SCALAR_PLUG_VAL:
			return &m_plugval;
		case 1://SCALAR_PLUG_MIN:
			return &m_plugmin;
		case 2://SCALAR_PLUG_MAX:
			return &m_plugmax;
		}
		return NULL;
	}*/
#endif
	//@}
	virtual void UpdateControl();
public:
  typedef enum {SC_SLIDER, SC_EDIT, SC_ITF, SC_PLUG, SC_BVAR, SC_LRBUTTONS} UpdateFrom;
	void UpdateValue(UpdateFrom from);
#ifdef USEPLUGS
  virtual bool plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent); // from PlugContainer
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCALARDLG_H__23D56EBF_CDEC_48BA_B7F5_46373C78F9A4__INCLUDED_)
