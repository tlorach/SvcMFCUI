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
// BooleanDlg.cpp : implementation file
//
#include "stdafx.h"
#include "SvcMFCUI.h"
#include "ButtonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**
 ** Instanciation of an object.
 **/ 
const void * CButtonDlg::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CButtonDlg *pDlg = new CButtonDlg;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_BUTTON, pOwner);
	if(bOK)
	{
		CWnd *pW = pDlg->GetParent();
pDlg->ShowWindow(SW_SHOWNOACTIVATE);
		//pDlg->ShowWindow(SW_HIDE);
		//pDlg->RedrawWindow();
		//pDlg->PeekMyself();
		return (void*)(static_cast<IWindow*>(pDlg));
	}
	delete pDlg;
	return NULL;
}
/*----------------------------------------------------------------------------------*/ /**

handles incoming data to the console from another plug : lpFrom. lpTo is the console plug...

just dump the string to the console

 **/ //----------------------------------------------------------------------------------
#ifdef USEPLUGS
bool CButtonDlg::plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent)
{
	return false;
}
#endif
/////////////////////////////////////////////////////////////////////////////
// CButtonDlg dialog


CButtonDlg::CButtonDlg(CWnd* pParent /*=NULL*/) :
#ifdef USEPLUGS
	m_plugboolean(PLUG_FLOAT, 0, "bool", &m_val, this),
#endif
	CDialog(CButtonDlg::IDD, pParent), m_val(0)
{
	//
	// because these aren't dynamicaly allocated
	//
#ifdef USEPLUGS
	m_plugboolean.setDesc("plug to ...");
#endif

	IMPLSMARTREFINIT
	//{{AFX_DATA_INIT(CButtonDlg)
	//}}AFX_DATA_INIT
}


void CButtonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CButtonDlg)
	DDX_Control(pDX, IDC_BUTTON1, m_button);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CButtonDlg, CDialog)
	//{{AFX_MSG_MAP(CButtonDlg)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CButtonDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////
/**
 ** We need to setup another control that the window itself
 **/
IWindow* CButtonDlg::SetTitle(LPCSTR title)
{
	m_button.SetWindowText(title);
	IWindowImpl<CButtonDlg>::SetTitle(title); 
	return this;
};
IWindow* CButtonDlg::GetTitle(char * title, int maxlen)
{
	m_button.GetWindowText(title, maxlen);
	return this;
};

/////////////////////////////////////////////////////////////////////////////
// CButtonDlg message handlers

void CButtonDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	RECT rc;
	if(m_button.m_hWnd) 
	{
		m_button.GetWindowRect(&rc);
		m_button.SetWindowPos(NULL, 0,0, cx, rc.bottom-rc.top, SWP_NOACTIVATE);
	}
}


BOOL CButtonDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
void CButtonDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CButtonDlg::OnBnClickedOk()
{
	for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
	{
		m_pclients[ic]->Button(this, 1);
	}
	for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
	{
		g_WindowHandler.m_pclients[ic]->Button(this, 1);
	}
}

void CButtonDlg::OnBnClickedButton1()
{
	for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
	{
		m_pclients[ic]->Button(this, 1);
	}
	for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
	{
		g_WindowHandler.m_pclients[ic]->Button(this, 1);
	}
}
