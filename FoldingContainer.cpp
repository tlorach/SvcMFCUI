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
// FoldingContainer.cpp : implementation file
//
#include "stdafx.h"
#include "SvcMFCUI.h"
#include "FoldingContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define HALFM 5
#define LINE 4

/**
 ** Instanciation of an object.
 **/ 
const void * FoldingContainer::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	FoldingContainer *pDlg = new FoldingContainer;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_FOLDINGCONTAINER, pOwner);
	if(bOK)
	{
CWnd *pown = pDlg->GetWindow(GW_OWNER);
		pDlg->ShowWindow(SW_HIDE);
		return (void*)(static_cast<IWindowFolding*>(pDlg));
	}
	delete pDlg;
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// FoldingContainer dialog


FoldingContainer::FoldingContainer(CWnd* pParent /*=NULL*/)
	: CDialog(FoldingContainer::IDD, pParent)
{
	IMPLSMARTREFINIT
	m_width = m_height = 0;
	m_unfolded = true;
	m_prevhscroll= m_prevvscroll = 0;
	m_pclients.clear();
	//{{AFX_DATA_INIT(FoldingContainer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
FoldingContainer::~FoldingContainer()
{
}


void FoldingContainer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FoldingContainer)
	DDX_Control(pDX, IDC_PIC, m_vline);
	DDX_Control(pDX, IDC_CHECK1, m_check);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FoldingContainer, CDialog)
	//{{AFX_MSG_MAP(FoldingContainer)
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**
 ** We need to setup another control that the window itself
 **/
IWindow* FoldingContainer::SetTitle(LPCSTR title)
{
	m_check.SetWindowText(title);
	IWindowImpl<FoldingContainer>::SetTitle(title); 
	return this;
};
IWindow* FoldingContainer::GetTitle(char * title, int maxlen)
{
	m_check.GetWindowText(title, maxlen);
	return this;
};
/**
 ** 
 **/
int FoldingContainer::GetNumItems()
{
	return (int)m_controls.size();
}
/**
 ** 
 **/
int FoldingContainer::UpdatePositions(bool resizeme)
{
	CWnd *pwnd;
	int s = m_check.GetCheck();
	for(unsigned int i=0; i<m_controls.size(); i++)
	{
		CWnd *pwnd = m_controls[i].cwnd;
		pwnd->ShowWindow(s ? SW_SHOWNOACTIVATE : SW_HIDE);
	}
	// resize the window
	RECT rc, clientrc;
	m_check.GetWindowRect(&rc);
	int checkheight = rc.bottom-rc.top;
	m_height = checkheight;
	GetClientRect(&clientrc);
	GetWindowRect(&rc);
	int dh = (rc.bottom - rc.top)-(clientrc.bottom - clientrc.top);
	if(s)
	  for(unsigned int i=0; i<m_controls.size(); i++)
	  {
		pwnd = m_controls[i].cwnd;
		RECT rc;
		pwnd->GetWindowRect(&rc);
		pwnd->SetWindowPos(NULL, HALFM*2+LINE, m_height, clientrc.right-clientrc.left-(HALFM*2+LINE), rc.bottom-rc.top, SWP_NOACTIVATE|SWP_NOZORDER);
		m_height += rc.bottom-rc.top;
	  }
	m_check.SetWindowPos(NULL, 0,0, clientrc.right-clientrc.left, checkheight, SWP_NOACTIVATE|SWP_NOMOVE);
	m_vline.SetWindowPos(NULL, HALFM, 0, LINE, m_height, SWP_NOACTIVATE);
	if(resizeme)
		SetWindowPos(&wndTop, 0, 0, rc.right-rc.left, m_height+dh, SWP_NOMOVE|SWP_NOACTIVATE);
	//PeekMyself(); crashed some apps...
	return m_height;
}
/**
 ** 
 **/
IWindowContainer* FoldingContainer::AddItem(LPCSTR title, IWindow *lpWnd, bool bUpdateComponents)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!lpWnd) 
		return this;
	for(unsigned int i=0; i<m_controls.size(); i++)
	{
		if(m_controls[i].itf == lpWnd)
			return this; // already in here
	}
	lpWnd->SetTitle(title);
	CWnd *pwnd = (CWnd *)lpWnd->GetCWnd();
	m_controls.push_back(FoldingContainer::TWindows(lpWnd, pwnd));
	lpWnd->SetParentContainer(this);
	// change the style of the child window
	BOOL bRes;
	bRes = pwnd->ModifyStyle(WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW|WS_OVERLAPPED, WS_CHILDWINDOW|WS_VISIBLE,0);
	pwnd->ShowWindow(SW_SHOWNOACTIVATE);
	CWnd * par = pwnd->SetParent(this);
	//PeekMyself();
	//UpdatePositions();
	//pwnd = GetParent();
	//if(pwnd)
	//{
	//	pwnd->SendMessage(WM_USER+10, 1, 0);
	//}
	return this;
}
/*IWindowContainer* FoldingContainer::AddItem(LPCSTR title, LPCSTR windowname, bool bUpdateComponents)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWnd *pwnd;
	if(!windowname) 
		return this;
	if(windowname)
		pwnd = FindWindow(NULL,windowname);
	if(!pwnd) 
		return this;
	for(unsigned int i=0; i<m_controls.size(); i++)
	{
		if(m_controls[i].cwnd == pwnd)
			return this; // already in here
	}
	m_controls.push_back(TWindows(NULL, pwnd));
	//PeekMyself();
	BOOL bRes;
	bRes = pwnd->ModifyStyle(WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW|WS_OVERLAPPED, WS_CHILDWINDOW|WS_VISIBLE,0);
	pwnd->ShowWindow(SW_SHOWNOACTIVATE);
	CWnd * par = pwnd->SetParent(this);
	//PeekMyself();

	UpdatePositions();
	return this;
}*/
/**
 ** 
 **/
IWindowContainer* FoldingContainer::RemoveItem(IWindow *lpWnd, bool bUpdateComponents)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	for(unsigned int i=0; i<m_controls.size(); i++)
	{
		if(m_controls[i].itf == lpWnd)
		{
			RemoveItem(i, NULL, bUpdateComponents);
			return this;
		}
	}
	return this;
}
/**
 ** 
 **/
IWindowContainer* FoldingContainer::RemoveItem(int Itemnum, IWindow **lpWnd, bool bUpdateComponents)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRes;
	CWnd *pwnd;
	if((Itemnum >= (int)m_controls.size())||(m_controls[Itemnum].cwnd == NULL)) 
	{
		return this;
	}
	pwnd = m_controls[Itemnum].cwnd;
	bRes = pwnd->ModifyStyle(WS_CHILDWINDOW, WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW, 0);
	//PeekMyself(); // makes infinite loop...
	CWnd * par = pwnd->SetParent(NULL);
	//pwnd->ShowWindow(SW_SHOW);
	//PeekMyself();
	if(lpWnd) 
	{
		*lpWnd = m_controls[Itemnum].itf;
		(*lpWnd)->AddRef();
	}
	m_controls[Itemnum].itf = NULL;
	m_controls[Itemnum].cwnd = NULL;
	m_controls.erase(m_controls.begin() + Itemnum);
	if(bUpdateComponents)
		UpdatePositions();
	return this;
}
/**
 ** 
 **/
IWindowContainer* FoldingContainer::SelectItem(int Itemnum)
{
	return this;
}
IWindowContainer* FoldingContainer::SelectItem(IWindow* pw)
{
	return this;
}
IWindow* FoldingContainer::GetItemNum(int Itemnum)
{
	if(Itemnum >= m_controls.size())
		return NULL;
	return m_controls[Itemnum].itf;
}
bool FoldingContainer::UnFold(bool bYes)
{
	CWnd *pwnd;
	m_check.SetCheck(bYes);
	UpdatePositions();
	pwnd = GetParent();
	if(pwnd)
	{
		pwnd->SendMessage(WM_USER+10, 1, 0);
	}
	for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
	{
		m_pclients[ic]->WindowContainerChanged(static_cast<IWindow*>(this));
	}
	for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
	{
		g_WindowHandler.m_pclients[ic]->WindowContainerChanged(static_cast<IWindow*>(this));
	}
	return true;
}
bool FoldingContainer::isUnFolded()
{
	return m_check.GetCheck() ? true : false;
}

/////////////////////////////////////////////////////////////////////////////
// FoldingContainer message handlers

void FoldingContainer::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	CDialog::OnMouseMove(nFlags, point);
}

void FoldingContainer::OnDestroy() 
{
	CDialog::OnDestroy();
	
	for(unsigned int i=0; i<m_controls.size(); i++)
	{
		m_controls[i].itf = NULL;
		//m_controls[i].cwnd ????
	}
	
}

void FoldingContainer::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	UpdatePositions();	
}

void FoldingContainer::OnCheck() 
{
	CWnd *pwnd;
	UpdatePositions();
	pwnd = GetParent();
	if(pwnd)
	{
		pwnd->SendMessage(WM_USER+10, 1, 0);
	}
	int s = m_check.GetCheck();
	for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
	{
		m_pclients[ic]->WindowContainerChanged(static_cast<IWindow*>(this));
	}
	for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
	{
		g_WindowHandler.m_pclients[ic]->WindowContainerChanged(static_cast<IWindow*>(this));
	}
}

BOOL FoldingContainer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_check.SetCheck(m_unfolded ? 1 : 0);
	
	RECT rc,rc2,rc3;
	m_check.GetWindowRect(&rc);
	GetWindowRect(&rc2);
	GetClientRect(&rc3);
	rc3.bottom -= (rc.bottom-rc.top);
	SetWindowPos(&wndTop, 0,0, rc2.right-rc2.left, rc2.bottom-rc2.top-rc3.bottom, SWP_NOACTIVATE|SWP_NOMOVE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void FoldingContainer::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
}

LRESULT FoldingContainer::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_USER+10)
	{
		UpdatePositions();
		CWnd *pwnd = GetParent();
		if(pwnd)
		{
			pwnd->SendMessage(WM_USER+10, 1, 0);
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void FoldingContainer::OnClose() 
{
	//
	// Send the event
	//
	// HACK : This forces this object to not be deleted inside an event.
	AddRef(); 
	for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
	{
		m_pclients[ic]->WindowClosed(static_cast<IWindow*>(this));
	}
	for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
	{
		g_WindowHandler.m_pclients[ic]->WindowClosed(static_cast<IWindow*>(this));
	}
	Release(); 
	//CDialog::OnClose();
}
void FoldingContainer::Refresh()
{
	UpdatePositions();
}
