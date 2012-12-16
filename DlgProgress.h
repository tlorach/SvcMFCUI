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
/** \file DlgProgress.h
 **
 **/ /****************************************************************************/
#if !defined(AFX_DLGPROGRESS_H__506F6487_8E23_48AE_A756_C1F702EBF382__INCLUDED_)
#define AFX_DLGPROGRESS_H__506F6487_8E23_48AE_A756_C1F702EBF382__INCLUDED_

#include "ISvcUI.h"
#include "IWindowImplementation.h"
#ifdef USEPLUGS
#include "plug/plug.h"
using namespace plug;
#endif
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProgress.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog

class CDlgProgress : public IWindowImpl<CDlgProgress>, public CDialog, public IProgressBar
#ifdef USEPLUGS
    , public PlugContainer
#endif
{
// Construction
public:
	/// \name Factory stuff
	//@{
	DECL_OBJECTDESC_VARIABLE();
	static const void *NewObjectFunc(size_t lparam1, size_t lparam2); // DECL_NEWOBJECTFUNC(CDlgProgress, IProgressBar);
	//@}
	IMPLSMARTREF();
	BEGINQUERYINTERFACE();
		QIMAPITF(IProgressBar)	
		QIMAPITF(IWindow)
	ENDQUERYINTERFACE();
	CDlgProgress();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProgress)
	enum { IDD = IDD_PROGRESS };
	CStatic	m_Msg;
	CStatic	m_Percent;
	CProgressCtrl	m_Progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProgress)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	String m_string;
public:
	float percent;
#ifdef USEPLUGS
	plug::Plug m_plugpercent;	///< controling percent variable
	plug::Plug m_plugmsg;		///< controling the message on the top of the progress bar
#endif

	IProgressBar* SetPercent(float l)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		/*char tmpstr[10];
		if(l < 0)
			l = 0;
		else if(l > 100)
			l = 100;*/
#ifdef USEPLUGS
		m_plugpercent.setValue(l);
#endif
		/*sprintf(tmpstr, "%d%%", (int)l); // !!!!!! should be already done in Plug_IncomingDataCB()
		m_Percent.SetWindowText(tmpstr); // TO CHECK
		m_Progress.SetRange(0,100);
		m_Progress.SetPos((int)l);
		m_Percent.RedrawWindow();
		m_Progress.RedrawWindow();
		PeekMyself();*/
		return this;
	}
	IProgressBar* AddPercent(float l)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		char tmpstr[10];
		percent += l;//m_Progress.GetPos();
		if(percent < 0)
			percent = 0;
		else if(percent > 100)
			percent = 100;
		sprintf(tmpstr, "%d%%", (int)percent);
		m_Percent.SetWindowText(tmpstr);
		m_Progress.SetRange(0,100);
		m_Progress.SetPos((int)percent);
		m_Percent.RedrawWindow();
		m_Progress.RedrawWindow();
		PeekMyself();
		return this;
	}
	IProgressBar* SetMessage(LPCSTR lpstr)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		if(!lpstr)
			return this;
#ifdef USEPLUGS
		m_plugmsg.setValue(lpstr);
#endif
		m_Msg.SetWindowText(lpstr);// done in Plug_IncomingDataCB ?
		PeekMyself();
		return this;
	}
#ifdef USEPLUGS
/*	void *GetPlug(int n) 
	{
		switch(n)
		{
		case 0: 
			return &m_plugpercent; 
		case 1: 
			return &m_plugmsg; 
		}
		return NULL;
	}*/
#endif
	DECLARE_IWINDOW_IMPL_NOSETTITLE(CDlgProgress)
	virtual IWindow* SetTitle(LPCSTR title);
	virtual IWindow* GetTitle(char * title, int maxlen);
	virtual IWindowContainer	*AsWindowContainer(){ return NULL; };
    virtual IWindowFolding	*AsWindowFolding(){ return NULL; };
	virtual IWindowConsole	  *AsWindowConsole()  { return NULL; };
	virtual IWindowHandler	  *AsWindowHandler()  { return NULL; };
	virtual IWindowLog		  *AsWindowLog()	  { return NULL; };
	virtual IWindowSplitter	 *AsWindowSplitter() { return NULL; };
	virtual IProgressBar		*AsProgressBar()	{ return this; };
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
#ifdef USEPLUGS
	virtual bool plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent); // from PlugContainer
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROGRESS_H__506F6487_8E23_48AE_A756_C1F702EBF382__INCLUDED_)
