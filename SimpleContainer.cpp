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
// SimpleContainer.cpp : implementation file
//

#include "stdafx.h"
#include "SvcMFCUI.h"
#include "SimpleContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 ** Instanciation of an object.
 **/ 
const void * SimpleContainer::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SimpleContainer *pDlg = new SimpleContainer;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_SIMPLECONTAINER, pOwner);
	if(bOK)
	{
		pDlg->ShowWindow(SW_HIDE);
		return (void*)(static_cast<IWindowContainer*>(pDlg));
	}
	delete pDlg;
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// SimpleContainer dialog


SimpleContainer::SimpleContainer(CWnd* pParent /*=NULL*/)
	: CDialog(SimpleContainer::IDD, pParent)
{
	IMPLSMARTREFINIT
	m_width = m_height = 0;
	m_prevhscroll= m_prevvscroll = 0;
	m_pclients.clear();
	//{{AFX_DATA_INIT(SimpleContainer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
SimpleContainer::~SimpleContainer()
{
	//
	// NOTE: Destroy() is removing the element from the vector<>... yep...
	//
	int sz = (int)m_controls.size();
	for(unsigned int i=0; i<m_controls.size(); /*i++*/)
	{
		if(m_controls[i].itf)
		{
			if(m_controls[i].itf->IsValid())
				m_controls[i].itf->Destroy();
			else i++;
		} else i++;
		//	m_controls[i].itf = NULL;
	}
}


void SimpleContainer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SimpleContainer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SimpleContainer, CDialog)
	//{{AFX_MSG_MAP(SimpleContainer)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**
 ** 
 **/
int SimpleContainer::GetNumItems()
{
	return (int)m_controls.size();
}
/**
 ** 
 **/
int SimpleContainer::UpdatePositions()
{
	RECT rcc;
	GetClientRect(&rcc);
	m_height = 0;
	for(unsigned int i=0; i<m_controls.size(); i++)
	{
		CWnd *pwnd = m_controls[i].cwnd;
		RECT rc;
		pwnd->GetWindowRect(&rc);
		pwnd->SetWindowPos(&wndTop, 0, m_height, rcc.right, rc.bottom-rc.top, 0/*SWP_NOREDRAW|SWP_NOACTIVATE|SWP_ASYNCWINDOWPOS|SWP_DEFERERASE|SWP_NOSENDCHANGING*/);
		m_height += rc.bottom-rc.top;
	}
	// WRONG WAY OF DOING...
	int oldmin, oldmax, oldpos;
	int cy = rcc.bottom;
	GetScrollRange( SB_VERT, &oldmin, &oldmax);
	oldpos = GetScrollPos(SB_VERT);
	if(cy > 0)
	{
		int h = m_height-cy;
		if(h<0) h=0;
		SetScrollRange( SB_VERT, 0, h, FALSE);
		m_prevvscroll = 0;
		SetScrollPos(SB_VERT, 0);
	}
	//PeekMyself();
	return m_height;
}
/**
 ** 
 **/
IWindowContainer* SimpleContainer::AddItem(LPCSTR title, IWindow *lpWnd, bool bUpdateComponents)
{
	if(!lpWnd) 
		return this;
	for(unsigned int i=0; i<m_controls.size(); i++)
	{
		if(m_controls[i].itf == lpWnd)
			return this; // already in here
	}
	lpWnd->SetTitle(title);
	CWnd *pwnd = (CWnd *)lpWnd->GetCWnd();
	//PeekMyself();
	BOOL bRes;
	bRes = pwnd->ModifyStyle(WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW|WS_OVERLAPPED, WS_CHILDWINDOW|WS_VISIBLE,0);
	pwnd->ShowWindow(1);
	//PeekMyself();
	pwnd->SetParent(this);
	lpWnd->SetParentContainer(this);
	m_controls.push_back(SimpleContainer::TWindows(lpWnd, pwnd, 0,0));
	if(bUpdateComponents)
		UpdatePositions();
	return this;
}
/*IWindowContainer* SimpleContainer::AddItem(LPCSTR title, LPCSTR windowname, bool bUpdateComponents)
{
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
	PeekMyself();
	BOOL bRes;
	bRes = pwnd->ModifyStyle(WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW|WS_OVERLAPPED, WS_CHILDWINDOW|WS_VISIBLE,0);
	pwnd->ShowWindow(1);
	PeekMyself();
	pwnd->SetParent(this);
	if(bUpdateComponents)
		UpdatePositions();
	m_controls.push_back(TWindows(NULL, pwnd,0,0));
	return this;
}*/
/**
 ** 
 **/
IWindowContainer* SimpleContainer::RemoveItem(IWindow *lpWnd, bool bUpdateComponents)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	for(unsigned int i=0; i<m_controls.size(); i++)
	{
		if(m_controls[i].itf == lpWnd)
			return RemoveItem(i, NULL, bUpdateComponents);
	}
	return this;
}
/**
 ** 
 **/
IWindowContainer* SimpleContainer::RemoveItem(int Itemnum, IWindow **lpWnd, bool bUpdateComponents)
{
	CWnd *pwnd;
	pwnd = m_controls[Itemnum].cwnd;
	if((Itemnum >= (int)m_controls.size())||(pwnd == NULL)) 
	{
		return this;
	}
	//bRes = pwnd->ModifyStyle(WS_CHILDWINDOW, WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW, 0);
	//PeekMyself();
	CWnd * par = pwnd->SetParent(NULL);
	pwnd->ShowWindow(SW_SHOW);
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
IWindowContainer* SimpleContainer::SelectItem(int Itemnum)
{
	assert(!"function not implemented, yet");
	return this;
}
IWindow* SimpleContainer::GetItemNum(int Itemnum)
{
	if(Itemnum >= (int)m_controls.size())
		return NULL;
	return m_controls[Itemnum].itf;
}
/////////////////////////////////////////////////////////////////////////////
// SimpleContainer message handlers

void SimpleContainer::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	switch(nSBCode) 
	{
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		SetScrollPos(SB_HORZ, nPos);
		ScrollWindow(nPos,m_prevhscroll-nPos,0);
		m_prevhscroll = nPos;
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void SimpleContainer::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	switch(nSBCode) 
	{
	case SB_LINEUP:
		if(m_prevvscroll<=0)
			break;
		nPos = m_prevvscroll - 10;
		if(nPos <= 0) nPos = 0;
		ScrollWindow( 0, 10);
		SetScrollPos(SB_VERT, nPos);
		m_prevvscroll = nPos;
		break;
	case SB_LINEDOWN:
		nPos = m_prevvscroll + 10;
		SetScrollPos(SB_VERT, nPos);
		ScrollWindow( 0, -10);
		m_prevvscroll = nPos;
		break;
	case SB_PAGEUP:
		if(m_prevvscroll<=0)
			break;
		nPos = m_prevvscroll - 10;
		if(nPos <= 0) nPos = 0;
		ScrollWindow( 0, 10);
		SetScrollPos(SB_VERT, nPos);
		m_prevvscroll = nPos;
		break;
	case SB_PAGEDOWN:
		nPos = m_prevvscroll + 10;
		SetScrollPos(SB_VERT, nPos);
		ScrollWindow( 0, -10);
		m_prevvscroll = nPos;
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		SetScrollPos(SB_VERT, nPos);
		ScrollWindow( 0, m_prevvscroll - nPos);
		m_prevvscroll = nPos;
		break;
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL SimpleContainer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void SimpleContainer::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	CDialog::OnMouseMove(nFlags, point);
}

void SimpleContainer::OnDestroy() 
{
	CDialog::OnDestroy();
	
	for(unsigned int i=0; i<m_controls.size(); i++)
	{
		m_controls[i].itf = NULL;
		//m_controls[i].cwnd ????
	}
	
}

void SimpleContainer::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_height = 0;
	for(unsigned int i=0; i<m_controls.size(); i++)
	{
		CWnd *pwnd = m_controls[i].cwnd;
		RECT rc;
		pwnd->GetWindowRect(&rc);
		pwnd->SetWindowPos(&wndTop, 0, m_height, cx, rc.bottom-rc.top, 0);
		m_height += rc.bottom-rc.top;
	}
	/*int oldmin, oldmax, oldpos;
	GetScrollRange( SB_VERT, &oldmin, &oldmax);
	oldpos = GetScrollPos(SB_VERT);*/
	if(cy > 0)
	{
		int h = m_height-cy;
		if(h<0) h=0;
		SetScrollRange( SB_VERT, 0, h, TRUE);
		m_prevvscroll = 0;
		SetScrollPos(SB_VERT, 0);
	}
}
// |-------------------------|
// |---------|
void SimpleContainer::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
}

LRESULT SimpleContainer::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == (WM_USER+10))
	{
		UpdatePositions();
		CWnd *pwnd = GetParent();
		if(pwnd)	pwnd->SendMessage(WM_USER+10, 1, 0);
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void SimpleContainer::OnClose() 
{
	//
	// Send the event
	//
	for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
	{
		// HACK : This forces this object to not be deleted inside an event.
		AddRef(); 
		m_pclients[ic]->WindowClosed(static_cast<IWindow*>(this));
		Release(); 
	}
	for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
	{
		// HACK : This forces this object to not be deleted inside an event.
		AddRef(); 
		g_WindowHandler.m_pclients[ic]->WindowClosed(static_cast<IWindow*>(this));
		Release(); 
	}
	//CDialog::OnClose();
}
void SimpleContainer::Refresh()
{
	UpdatePositions();
}