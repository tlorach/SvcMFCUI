// SplitterContainer.cpp : implementation file
//

#include "stdafx.h"
#include "SvcMFCUI.h"
#include "SplitterContainer.h"
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

#include "LogWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 ** Instanciation of an object.
 **/ 
const void * SplitterContainer::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SplitterContainer *pDlg = new SplitterContainer;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_SPLITTERCONTAINER, pOwner);
	if(bOK)
	{
		pDlg->ShowWindow(SW_HIDE);
		return (void*)(static_cast<IWindowSplitter*>(pDlg));
	}
	delete pDlg;
	return NULL;
}

/**
 ** Instanciation of an object.
 ** Here we create the splitter in horizontal direction : downclassing a little.
 **/ 
const void * SplitterContainerH::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SplitterContainerH *pDlg = new SplitterContainerH;
	// turn the splitter in horizontal mode
	pDlg->m_bHorizontalMode = true;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_SPLITTERCONTAINER, pOwner);
	if(bOK)
	{
		return (void*)(static_cast<IWindowSplitter*>(pDlg));
	}
	delete pDlg;
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// SplitterContainer dialog


SplitterContainer::SplitterContainer(CWnd* pParent /*=NULL*/)
	: CDialog(SplitterContainer::IDD, pParent)
{
	m_pclients.clear();
	rows=0;
	cols=0;
	m_bHorizontalMode = false;
	IMPLSMARTREFINIT
	//{{AFX_DATA_INIT(SplitterContainer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
SplitterContainer::~SplitterContainer()
{
}


void SplitterContainer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SplitterContainer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SplitterContainer, CDialog)
	//{{AFX_MSG_MAP(SplitterContainer)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/**
 ** 
 **/
int SplitterContainer::GetNumItems()
{
	return m_bHorizontalMode ? cols : rows;
}
/*************************************************************************/ /**

Append an item at the end of the Row, col zero.

 */ /*********************************************************************/
int SplitterContainer::addItem_Row(CWnd *pwnd, IWindow *lpWnd)
{
	BOOL b = true;
	RECT rc2, rc1, rc;
	if(cols == MAXITEMSROW)
		return 0;
	for(int i=0; i<MAXITEMSROW; i++)
	{
		if(m_controls[i][0].cwnd == pwnd)
			return 0; // already in here
	}
	pwnd->GetWindowRect(&rc2);
	m_splitter.GetWindowRect(&rc1);
	GetClientRect(&rc);
	// add a row if necessary (the 1st time is not necessary)
	int h=0;
	if(rows > 0)
	{
		for(int i=0; i<m_splitter.GetRowCount()-1; i++)
		{
			int ycur, ymin;
			m_splitter.GetRowInfo(i, ycur, ymin);
			h+=ycur;
		}
	}
	// this split may help us to keep the ratio of the window we are adding.
	/*if((rc.bottom - h) > (rc2.bottom-rc2.top))
		b = m_splitter.SplitRow( (rc2.bottom-rc2.top));
	else */
  if(rows == m_splitter.GetRowCount())
	{
		TWindows &tw = m_controls[rows-1][cols-1];
	int dy;
		if((dy=tw.def_h+h+(rc2.bottom-rc2.top) - rc.bottom) > 0)
	{
	  rc.bottom += dy;
	}
		b = m_splitter.SplitRow(tw.def_h);
		if(!b)
			return 0;
	}
	rows++;

	// keep it in a map
	TWindows &tw = m_controls[rows-1][cols-1];
	tw.cwnd = pwnd;
	tw.itf = lpWnd;
	tw.def_w = rc2.right - rc2.left;
	tw.def_h = rc2.bottom - rc2.top;
	// set the size
	m_splitter.SetRowInfo(rows-1, rc2.bottom-rc2.top,10);
	m_splitter.SetColumnInfo(cols-1, rc2.right-rc2.left, 10);
	// change its user val to help the Splitter to find it
	SetWindowLong(pwnd->m_hWnd, /*GWL_USERDATA*/-21, m_splitter.IdFromRowCol(rows-1, cols-1));
	// Change its style and parent
	pwnd->ShowWindow(SW_SHOWNOACTIVATE);
	pwnd->ModifyStyle(WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW|WS_OVERLAPPED, WS_CHILDWINDOW|WS_VISIBLE,0);
	//PeekMyself();
	pwnd->SetParent(&m_splitter);
	m_splitter.RecalcLayout();
	SetSize(rc.right-rc.left, rc.bottom-rc.top);
	//PeekMyself();

	return rows;
}
/*************************************************************************/ /**

Append an item at the end of the Col, Row zero.

 */ /*********************************************************************/
int SplitterContainer::addItem_Col(CWnd *pwnd, IWindow *lpWnd)
{
	BOOL b = true;
	RECT rc2, rc1, rc;
	if(cols == MAXITEMSCOL)
		return 0;
	for(int i=0; i<MAXITEMSCOL; i++)
	{
		if(m_controls[0][i].cwnd == pwnd)
			return 0; // already in here
	}
	pwnd->GetWindowRect(&rc2);
	m_splitter.GetWindowRect(&rc1);
  GetClientRect(&rc);
	// add a row if necessary (the 1st time is not necessary)
	int h=0;
	if(cols > 0)
	{
		for(int i=0; i<m_splitter.GetColumnCount()-1; i++)
		{
			int ycur, ymin;
			m_splitter.GetColumnInfo(i, ycur, ymin);
			h+=ycur;
		}
	}
	/*if((rc.right - h) > (rc2.right-rc2.left))
		b = m_splitter.SplitColumn( (rc2.right-rc2.left));
	else */if(cols == m_splitter.GetColumnCount())
	{
		TWindows &tw = m_controls[rows-1][cols-1];
	int dx;
		if((dx=tw.def_w+h+(rc2.right-rc2.left) - rc.right) > 0)
	{
	  rc.right += dx;
	}
		b = m_splitter.SplitColumn(tw.def_w);
		if(!b)
			return 0;
	}
	if(!b)
		return 0;
	cols++;
	// keep it in a map
	TWindows &tw = m_controls[rows-1][cols-1];
	tw.cwnd = pwnd;
	tw.itf = lpWnd;
	tw.def_w = rc2.right - rc2.left;
	tw.def_h = rc2.bottom - rc2.top;
	// set the size
	m_splitter.SetRowInfo(rows-1, rc2.bottom-rc2.top,10);
	m_splitter.SetColumnInfo(cols-1, rc2.right-rc2.left, 10);
	// change its user val to help the Splitter to find it
	SetWindowLong(pwnd->m_hWnd, /*GWL_USERDATA*/-21, m_splitter.IdFromRowCol(rows-1, cols-1));
	// Change its style and parent
	pwnd->ShowWindow(SW_SHOWNOACTIVATE);
	pwnd->ModifyStyle(WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW|WS_OVERLAPPED, WS_CHILDWINDOW|WS_VISIBLE,0);
	//PeekMyself();
	pwnd->SetParent(&m_splitter);
	m_splitter.RecalcLayout();
	//PeekMyself();

	return cols;
}
/**
 ** 
 **/
IWindowContainer* SplitterContainer::AddItem(LPCSTR title, IWindow *lpWnd, bool bUpdateComponents)
{
	CWnd *pwnd;
	int r;
	if(!lpWnd) 
		return this;
	lpWnd->SetTitle(title);
	// get back the window to put in
	pwnd = (CWnd *)lpWnd->GetCWnd();
	if(m_bHorizontalMode)
		r = addItem_Col(pwnd, lpWnd);
	else
		r = addItem_Row(pwnd, lpWnd);
	lpWnd->SetParentContainer(this);
	//PeekMyself();
	return this;
}
/*IWindowContainer* SplitterContainer::AddItem(LPCSTR title, LPCSTR windowname, bool bUpdateComponents)
{
	CWnd *pwnd;
	if(!windowname) 
		return this;
	if(windowname)
		pwnd = FindWindow(NULL,windowname);
	if(!pwnd) 
		return this;
	if(m_bHorizontalMode)
		addItem_Col(pwnd, NULL);
	else
		addItem_Row(pwnd, NULL);
	return this;
}*/
/**
 ** 
 **/
IWindowContainer* SplitterContainer::RemoveItem(IWindow *lpWnd, bool bUpdateComponents)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	for(int i=0; i<MAXITEMSCOL; i++)
	{
		if(m_bHorizontalMode)
		{
			TWindows &tw = m_controls[0][i];
			if(tw.itf == lpWnd)
				RemoveItem(i, NULL, bUpdateComponents);
		}
		else
		{
			TWindows &tw = m_controls[i][0];
			if(tw.itf == lpWnd)
				RemoveItem(i, NULL, bUpdateComponents);
		}
	}
	return this;
}
/**
 ** 
 **/
IWindowContainer* SplitterContainer::RemoveItem(int Itemnum, IWindow **lpWnd, bool bUpdateComponents)
{
	map<int, TWindows>::iterator iItem;	
	if(m_bHorizontalMode)
	{
		TWindows &tw = m_controls[0][Itemnum];
		if(!tw.cwnd)
			return this;
		tw.itf = NULL;
		tw.cwnd->ModifyStyle(WS_CHILDWINDOW, WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW, 0);
		tw.cwnd->SetParent(NULL);
		tw.cwnd->ShowWindow(SW_SHOW);
		//PeekMyself();
		// if(cols > 1)
		//	m_splitter.DeleteView(m_splitter.IdFromRowCol(Itemnum, 0/*itemy*/));
		//else
			m_splitter.DeleteColumn(Itemnum);
		cols--;
		for(int i=Itemnum; i<MAXITEMSCOL-1;i++)
		{
			for(int j=0; j<MAXITEMSROW;j++)
				m_controls[j][i] = m_controls[j][i+1];
		}
		for(int j=0; j<MAXITEMSROW;j++)
		{
			m_controls[j][MAXITEMSCOL-1].cwnd = NULL;
			m_controls[j][MAXITEMSCOL-1].itf = NULL;
		}
	}
	else
	{
		TWindows &tw = m_controls[Itemnum][0];
		if(!tw.cwnd)
			return this;
		tw.itf = NULL;
		tw.cwnd->ModifyStyle(WS_CHILDWINDOW, WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW, 0);
		tw.cwnd->SetParent(NULL);
		tw.cwnd->ShowWindow(SW_SHOW);
		//PeekMyself();
		// if(cols > 1)
		//	m_splitter.DeleteView(m_splitter.IdFromRowCol(Itemnum, 0/*itemy*/));
		//else
			m_splitter.DeleteRow(Itemnum);
		rows--;
		for(int i=Itemnum; i<MAXITEMSROW-1;i++)
		{
			for(int j=0; j<MAXITEMSCOL;j++)
				m_controls[i][j] = m_controls[i+1][j];
		}
		for(int j=0; j<MAXITEMSCOL;j++)
		{
			m_controls[MAXITEMSROW-1][j].cwnd = NULL;
			m_controls[MAXITEMSROW-1][j].itf = NULL;
		}
	}
	return this;
}
/**
 ** 
 **/
IWindowContainer* SplitterContainer::SelectItem(int Itemnum)
{
	assert(!"function not implemented, yet");
	return this;
}
IWindow* SplitterContainer::GetItemNum(int Itemnum)
{
	if(Itemnum >= MAXITEMSROW)
		return NULL;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(m_bHorizontalMode)
	{
		TWindows &tw = m_controls[0][Itemnum];
		return tw.itf;
	}
	else
	{
		TWindows &tw = m_controls[Itemnum][0];
		return tw.itf;
	}
	return this;
}
/////////////////////////////////////////////////////////////////////////////
// SplitterContainer message handlers

void SplitterContainer::OnDestroy() 
{
	for(int i=0; i<MAXITEMSROW;i++)
	{
		for(int j=0; j<MAXITEMSCOL;j++)
		{
			TWindows &tw = m_controls[i][j];
			if(tw.cwnd)
			{
				tw.cwnd->ModifyStyle(WS_CHILDWINDOW, WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW, 0);
				tw.cwnd->SetParent(NULL);
				//PeekMyself();
			}
			m_controls[i][j].itf = NULL;
		}
	}
	CDialog::OnDestroy();
}

void SplitterContainer::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if(m_splitter.m_hWnd)
		m_splitter.SetWindowPos(NULL, 0,0,cx,cy, 0);
}

LRESULT SplitterContainer::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == (WM_USER+10))
	{
		m_splitter.RecalcLayout();
		CWnd *pwnd = GetParent();
		if(pwnd)	pwnd->SendMessage(WM_USER+10, 1, 0);
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL SplitterContainer::OnInitDialog() 
{
	CDialog::OnInitDialog();

	RECT rc;
	GetClientRect(&rc);
	BOOL b = m_splitter.CreateStatic(this, MAXITEMSROW, MAXITEMSCOL); // we must put at least 2
	m_splitter.ShowWindow(SW_SHOWNOACTIVATE);
	m_splitter.SetWindowPos(NULL, 0,0,rc.right,rc.bottom, 0);
	m_splitter.SetRowInfo(0, 0,10);
	m_splitter.SetColumnInfo(0, rc.right, 10);
	if(m_bHorizontalMode) 
		rows = 1;
	else
		cols = 1;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void SplitterContainer::OnClose() 
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
//
// 
//
int SplitterContainer::GetSplitterPos(int row)
{
	assert(!"WARNING : Not implemented, yet");
	return 0;
}
//
// 
//
IWindowSplitter* SplitterContainer::SetSplitterPos(int row, int y)
{
	assert(!"WARNING : Not implemented, yet");
	return this;
}
void SplitterContainer::Refresh()
{
}


