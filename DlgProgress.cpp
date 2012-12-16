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

/** \file DlgProgress.cpp
 **
 **/ /****************************************************************************/
#include "stdafx.h"
#include "SvcMFCUI.h"
#include "DlgProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 ** Instanciation of an object.
 **/ 
const void * CDlgProgress::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDlgProgress *pDlg = new CDlgProgress;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_PROGRESS, pOwner);
	if(bOK)
	{
		CWnd *pW = pDlg->GetParent();
		pDlg->SetWindowPos(&CWnd::wndTop/*Most*/, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
		pDlg->ShowWindow(SW_HIDE);
		pDlg->RedrawWindow();
		//pDlg->PeekMyself();
		return (void*)(static_cast<IProgressBar*>(pDlg));
	}
	delete pDlg;
	return NULL;
}
/*----------------------------------------------------------------------------------*/ /**

handles incoming data to the console from another plug : lpFrom. lpTo is the console plug...

just dump the string to the console

 **/ //----------------------------------------------------------------------------------
#ifdef USEPLUGS
bool CDlgProgress::plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent)
{
	String str;
	switch(lpTo->getId())
	{
	case 0: // percent
		{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		char tmpstr[10];
		float l;
		lpFrom->getValue(l);
		if(l < 0)		// WARNING : To check this...
			l = 0;
		else if(l > 100)
			l = 100;
		percent = l;
		if(!(m_hWnd))
			break;
		sprintf(tmpstr, "%d%%", (int)l);
		m_Percent.SetWindowText(tmpstr);
		m_Progress.SetRange(0,100);
		m_Progress.SetPos((int)l);
		m_Percent.RedrawWindow();
		m_Progress.RedrawWindow();
		//PeekMyself();
		}
		break;
	case 1: // msg
		{
		if(!(m_hWnd))
			break;
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		lpFrom->getValue(str);
		if(!str.empty())
	{
	  m_string = str;
			m_Msg.SetWindowText(str.c_str());
	}
		//PeekMyself();
		}
		break;
	}
	// tell the connected readers that data changed. Set bSendEvent=false if you decide to send this
	// method by your own.
	if(bSendEvent)
		lpTo->flush();
	return false;
}
#endif
/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog

CDlgProgress::CDlgProgress()
	: CDialog(CDlgProgress::IDD, NULL)
#ifdef USEPLUGS
	,m_plugpercent(PLUG_FLOAT, 0, "percent", &percent, this)
	,m_plugmsg(PLUG_STRING, 1, "msg", &m_string, this)
#endif
{
	//
	// because these aren't dynamicaly allocated
	//
#ifdef USEPLUGS
	m_plugpercent.setDesc("plug to ...");
	m_plugmsg.setDesc("plug to ...");
#endif

	IMPLSMARTREFINIT
	//{{AFX_DATA_INIT(CDlgProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProgress)
	DDX_Control(pDX, IDC_MESSAGE, m_Msg);
	DDX_Control(pDX, IDC_PERCENT, m_Percent);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}
IWindow* CDlgProgress::SetTitle(LPCSTR title)
{
	m_Msg.SetWindowText(title);
	//IWindowImpl<CDlgProgress>::SetTitle(title); 
	return this;
}
IWindow* CDlgProgress::GetTitle(char * title, int maxlen)
{
	m_Msg.GetWindowText(title, maxlen);
	return this;
}

BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
	//{{AFX_MSG_MAP(CDlgProgress)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress message handlers
BOOL CDlgProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProgress::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
