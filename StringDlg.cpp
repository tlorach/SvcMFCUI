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
// StringDlg.cpp : implementation file
//
#include "stdafx.h"
#include "SvcMFCUI.h"
#include "StringDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 ** Instanciation of an object.
 **/ 
const void * CStringDlg::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStringDlg *pDlg = new CStringDlg;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_STRING, pOwner);
	if(bOK)
	{
		CWnd *pW = pDlg->GetParent();
		pDlg->ShowWindow(SW_HIDE);
		//pDlg->RedrawWindow();
		//pDlg->PeekMyself();
		return (void*)(static_cast<IControlString*>(pDlg));
	}
	delete pDlg;
	return NULL;
}

/*----------------------------------------------------------------------------------*/ /**

handles incoming data to the console from another plug : lpFrom. lpTo is the console plug...

just dump the string to the console

 **/ //----------------------------------------------------------------------------------
#ifdef USEPLUGS
bool CStringDlg::plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent)
{
	if(!(m_hWnd))
		return false;
	String str;
	lpFrom->getValue(str);
	if(!str.empty())
	{
		m_str.SetSel(0,-1);
		m_str.ReplaceSel(str.c_str());
#pragma message("CStringDlg::plug_IncomingData : NOT sure we want to do this here...")
		if(((str.c_str() != m_string)))
		{
			for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
				m_pclients[ic]->StringChanged(this);
			for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
				g_WindowHandler.m_pclients[ic]->StringChanged(this);
		}
	  // tell the connected readers that data changed. Set bSendEvent=false if you decide to send this
	  // method by your own.
	  if(bSendEvent)
		  lpTo->flush();
	}
	return false;
}
#endif
/////////////////////////////////////////////////////////////////////////////
// CStringDlg dialog


CStringDlg::CStringDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStringDlg::IDD, pParent)
#ifdef USEPLUGS
	, m_plugstring(PLUG_STRING, 0, "str", &m_string, this)
#endif
{
	//
	// because these aren't dynamicaly allocated
	//
#ifdef USEPLUGS
	m_plugstring.setDesc("plug to ...");
#endif

	m_bShowBrowseButton = false;
	IMPLSMARTREFINIT
	//{{AFX_DATA_INIT(CStringDlg)
	//}}AFX_DATA_INIT
}


void CStringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStringDlg)
	DDX_Control(pDX, IDC_BROWSEFILE, m_browse);
	DDX_Control(pDX, IDC_TITLE, m_title);
	DDX_Control(pDX, IDC_EDIT1, m_str);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStringDlg, CDialog)
	//{{AFX_MSG_MAP(CStringDlg)
	ON_WM_SIZE()
	ON_EN_UPDATE(IDC_EDIT1, OnUpdateString)
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BROWSEFILE, OnBrowsefile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////
/**
 ** We need to setup another control that the window itself
 **/
IWindow* CStringDlg::SetTitle(LPCSTR title)
{
	m_title.SetWindowText(title);
	IWindowImpl<CStringDlg>::SetTitle(title); 
	return this;
};
IWindow* CStringDlg::GetTitle(char * title, int maxlen)
{
	m_title.GetWindowText(title, maxlen);
	return this;
};
IControlString* CStringDlg::SetString(LPCSTR str, bool bReplaceSelected)
{
	if(!str)
		return this;

	m_tmpstr = str;
	if(!bReplaceSelected)
		m_str.SetSel(0,-1);
	m_str.ReplaceSel(str);
	GetDlgItemText(m_str.GetDlgCtrlID(), m_tmpstr); // to respect the bReplaceSelected condition
#ifdef USEPLUGS
	m_plugstring.setValue((LPCSTR)m_tmpstr);
#endif
	OnUpdateString();
	return this;
}
LPCSTR CStringDlg::GetString()
{
	GetDlgItemText(m_str.GetDlgCtrlID(), m_tmpstr);
	return m_tmpstr;
}
LPCSTR CStringDlg::GetSelectedString()
{
	GetDlgItemText(m_str.GetDlgCtrlID(), m_tmpstr);
	return m_tmpstr;
}
int CStringDlg::GetStringLength()
{
	assert(!"function not implemented, yet");
	return 0;
}
IControlString* CStringDlg::SelectString(int start, int len)
{
	assert(!"function not implemented, yet");
	return this;
}
IControlString* CStringDlg::ClearString(bool bOnlySelected)
{
	assert(!"function not implemented, yet");
	return this;
}
LPCSTR CStringDlg::ShowFileBrowseButton(LPCSTR filter,int bYes)
{
	if(bYes >= 0)
	{
		m_bShowBrowseButton = bYes ? true : false;
		if(bYes) 
		{
			if(filter) 
				m_filter = filter;
			else
				m_filter = "All|*.*||";
		}
		RECT rc;
		GetClientRect(&rc);
		OnSize(0, rc.right, rc.bottom);
	}
	return m_bShowBrowseButton ? (LPCSTR)m_filter : NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CStringDlg message handlers

void CStringDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	RECT rc, rcbrowse;
	if(m_title.m_hWnd) 
	{
		m_title.GetWindowRect(&rc);
		if(m_bShowBrowseButton)
			m_browse.GetWindowRect(&rcbrowse);
		else
			rcbrowse.right = rcbrowse.left = 0;
		m_title.SetWindowPos(NULL, 0,0, cx*0.2, rc.bottom-rc.top, SWP_NOACTIVATE);
		m_str.SetWindowPos(NULL, cx*0.2,0, cx*0.8-(rcbrowse.right-rcbrowse.left), rc.bottom-rc.top, SWP_NOACTIVATE);
		if(m_bShowBrowseButton)
		{
			m_browse.SetWindowPos(NULL, cx-(rcbrowse.right-rcbrowse.left),0, rcbrowse.right-rcbrowse.left, rc.bottom-rc.top, SWP_NOACTIVATE);
			m_browse.ShowWindow(SW_SHOWNOACTIVATE);
		}
		else m_browse.ShowWindow(SW_HIDE);
	}
}


BOOL CStringDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
void CStringDlg::OnUpdateStringval() 
{
}

void CStringDlg::OnUpdateString() 
{
	CString		m_string;
	GetDlgItemText(m_str.GetDlgCtrlID(), m_string);
	if((m_tmpstr == m_string))
	{
		String str;
		str = m_tmpstr;
		for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
			m_pclients[ic]->StringChanged(this);
		for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
			g_WindowHandler.m_pclients[ic]->StringChanged(this);
	}
	else
	{
#ifdef USEPLUGS
		m_plugstring.setValue((LPCSTR)m_string);
#endif
	}
	m_tmpstr = m_string;
}

void CStringDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar)
	{
	case VK_RETURN:
		GetDlgItemText(m_str.GetDlgCtrlID(), m_tmpstr);
		break;
	}
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
/*************************************************************************/ /**



 */ /*********************************************************************/
void CStringDlg::OnBrowsefile() 
{
 // bring up dialog box to select a .txt or *.* file
	CFileDialog	 traceFile(true, NULL, NULL, OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY,
							m_filter, this); 
 
	if (traceFile.DoModal() == IDOK)
	{
		SetString(traceFile.GetPathName(), false);
	}
}
