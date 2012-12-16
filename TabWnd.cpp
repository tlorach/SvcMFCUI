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
// TabWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SvcMFCUI.h"
#include "TabWnd.h"

#include <winuser.h>
#include <windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 ** Instanciation of an object.
 **/ 
const void * CTabWnd::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CTabWnd *pDlg = new CTabWnd;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_TAB, pOwner);
	if(bOK)
	{
		CWnd *pW = pDlg->GetParent();
		pDlg->ShowWindow(SW_HIDE);
		//pDlg->RedrawWindow();
		//pDlg->PeekMyself();
		return (void*)(static_cast<IWindowContainer*>(pDlg));
	}
	delete pDlg;
	return NULL;
}


CTabWnd::CTabWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CTabWnd::IDD, pParent),
	m_currentWnd(NULL)
{
	IMPLSMARTREFINIT
	//{{AFX_DATA_INIT(CTabWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CTabWnd::~CTabWnd()
{
}


void CTabWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabWnd)
	DDX_Control(pDX, IDC_TABCTRL, m_TabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabWnd, CDialog)
	//{{AFX_MSG_MAP(CTabWnd)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRL, OnSelchangeTabctrl)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**
 ** 
 **/
int CTabWnd::GetNumItems()
{
	assert(!"function not implemented, yet");
	return 0;
}
/**
 ** 
 **/
IWindowContainer* CTabWnd::AddItem(LPCSTR title, IWindow *lpWnd, bool bUpdateComponents)
{
	if(!lpWnd) 
	{
		return this;
	}
	for(unsigned int i=0; i<m_controls.size(); i++)
	{
		if(m_controls[i] == lpWnd)
			return this; // already in here
	}
	m_TabCtrl.InsertItem(TCIF_TEXT|TCIF_PARAM,0, title,0, (LPARAM)m_controls.size());
	CWnd *pwnd = (CWnd *)lpWnd->GetCWnd();
	PeekMyself();
	BOOL bRes = pwnd->ModifyStyle(WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW, 
		WS_CHILDWINDOW|DS_CONTROL,0);
	CWnd * par = pwnd->SetParent(this);
	pwnd->ShowWindow(SW_SHOWNOACTIVATE);
	pwnd->Invalidate();
	PeekMyself();
	m_currentWnd = pwnd;
	pwnd->ShowWindow(SW_SHOWNOACTIVATE);

	m_controls.push_back(lpWnd);

	{
	RECT r;
	RECT rc, rc2;
	m_TabCtrl.GetItemRect(0, &rc);
	m_TabCtrl.GetClientRect(&rc2);
	GetClientRect(&r);
	lpWnd->SetLocation(5,rc.bottom+5);
	lpWnd->SetSize(rc2.right-5-5, rc2.bottom-rc.bottom-5-5);
	}
//	OnSize(0, r.right, r.bottom);
	return this;
}
/**
 ** 
 **/
IWindowContainer* CTabWnd::RemoveItem(IWindow *lpWnd, bool bUpdateComponents)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	for(unsigned int i=0; i<m_controls.size(); i++)
	{
		if(m_controls[i] == lpWnd)
			return RemoveItem(i, NULL, bUpdateComponents);
	}
	return this;
}
/**
 ** 
 **/
IWindowContainer* CTabWnd::RemoveItem(int Itemnum, IWindow **lpWnd, bool bUpdateComponents)
{
	BOOL bRes;
	CWnd *pwnd;
	if((Itemnum >= (int)m_controls.size())||(m_controls[Itemnum] == NULL)) 
	{
		return this;
	}
	pwnd = (CWnd *)m_controls[Itemnum]->GetCWnd();
	bRes = pwnd->ModifyStyle(WS_CHILDWINDOW, WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW, 0);
	pwnd->ShowWindow(SW_SHOWNOACTIVATE);
	PeekMyself();
	CWnd * par = pwnd->SetParent(NULL);
	pwnd->ShowWindow(SW_SHOW);
	PeekMyself();
	if(lpWnd) 
	{
		*lpWnd = m_controls[Itemnum];
		(*lpWnd)->AddRef();
	}
	m_controls[Itemnum] = NULL;
	m_TabCtrl.DeleteItem(Itemnum);
	m_controls.erase(m_controls.begin() + Itemnum);
	return this;
}
/**
 ** 
 **/
IWindowContainer* CTabWnd::SelectItem(int Itemnum)
{
	assert(!"function not implemented, yet");
	return this;
}
IWindow* CTabWnd::GetItemNum(int Itemnum)
{
	if(Itemnum >= (int)m_controls.size())
		return NULL;
	return m_controls[Itemnum];
}
/////////////////////////////////////////////////////////////////////////////
// CTabWnd message handlers

void CTabWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(m_TabCtrl.m_hWnd)
	{
		m_TabCtrl.SetWindowPos(NULL, 0,0,cx,cy,SWP_NOACTIVATE);
		RECT rc, rc2;
		TCITEM TabCtrlItem;
		int cur = m_TabCtrl.GetCurSel();
		m_TabCtrl.GetItemRect(cur, &rc);
		m_TabCtrl.GetClientRect(&rc2);
		if(cur < 0)
			return;
		m_TabCtrl.GetItem(cur, &TabCtrlItem);
		if((TabCtrlItem.lParam < (int)m_controls.size()) && (m_controls.size() > 0)) 
		{
			IWindow *pw = m_controls[TabCtrlItem.lParam];
			CWnd *pcwnd = (CWnd *)pw->GetCWnd();
			pw->SetLocation(5,rc.bottom+5);
			pw->SetSize(rc2.right-5-5, rc2.bottom-rc.bottom-5-5);
			//pcwnd->SetWindowPos(NULL, 5,rc.bottom+5 ,rc2.right-5-5, rc2.bottom-rc.bottom-5-5,SWP_NOACTIVATE);
		}
	}
}

void CTabWnd::OnSelchangeTabctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_currentWnd) 
	{
		m_currentWnd->ShowWindow(0);
	}
	TCITEM TabCtrlItem;
	int cur = m_TabCtrl.GetCurSel();
	m_TabCtrl.GetItem(cur, &TabCtrlItem);
	if(TabCtrlItem.lParam < (int)m_controls.size()) 
	{
		IWindow *pw = m_controls[TabCtrlItem.lParam];
		m_currentWnd = (CWnd *)pw->GetCWnd();
		m_currentWnd->ShowWindow(SW_SHOWNOACTIVATE);
		RECT r;
		GetClientRect(&r);
		OnSize(0, r.right, r.bottom);
	}
	*pResult = 0;
}

void CTabWnd::OnDestroy() 
{
	CDialog::OnDestroy();
	
	for(unsigned int i=0; i<m_controls.size(); i++)
	{
		CWnd *pwnd;
		pwnd = (CWnd *)m_controls[i]->GetCWnd();
		BOOL bRes = pwnd->ModifyStyle(WS_CHILDWINDOW, WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW, 0);
		pwnd->ShowWindow(SW_SHOWNOACTIVATE);
		PeekMyself();
		CWnd * par = pwnd->SetParent(NULL);
		PeekMyself();
		m_controls[i] = NULL;
	}
	
}
void CTabWnd::Refresh()
{
}
