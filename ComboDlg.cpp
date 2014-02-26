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
// ComboDlg.cpp : implementation file
//
/*************************************************************************/ /**
TO USE TO USE TO USE TO USE TO USE TO USE TO USE TO USE TO USE TO USE 
			{
				CHOOSECOLOR cc;
				static COLORREF acrCustClr[16]; // array of custom colors 
				static DWORD rgbCurrent;		// initial color selection
				ZeroMemory(&cc, sizeof(CHOOSECOLOR));
				cc.lStructSize = sizeof(CHOOSECOLOR);
				cc.hwndOwner = m_hWnd;
				cc.lpCustColors = (LPDWORD) acrCustClr;
				cc.rgbResult = rgbCurrent;
				cc.Flags = CC_FULLOPEN | CC_RGBINIT;
				BOOL ok = ChooseColor(&cc);
			}
*/

#include "stdafx.h"
#include "SvcMFCUI.h"
#include "ComboDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 ** Instanciation of an object.
 **/ 
const void * CComboDlg::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CComboDlg *pDlg = new CComboDlg;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_COMBO, pOwner);
	if(bOK)
	{
		CWnd *pW = pDlg->GetParent();
		pDlg->ShowWindow(SW_HIDE);
		//pDlg->RedrawWindow();
		//pDlg->PeekMyself();
		return (void*)(static_cast<IControlCombo*>(pDlg));
	}
	delete pDlg;
	return NULL;
}
/*----------------------------------------------------------------------------------*/ /**

handles incoming data to the console from another plug : lpFrom. lpTo is the console plug...

just dump the string to the console

 **/ //----------------------------------------------------------------------------------
#ifdef USEPLUGS
bool CComboDlg::plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent)
{
	DWORD v;
	lpFrom->getValue(v);
	if(!(m_hWnd))
		return false;
	m_curtag = v;
	for(int i=0; i< m_combo.GetCount(); i++)
	{
		if(m_combo.GetItemData(i) == v)
		{
			m_combo.SetCurSel(i);
			int data = m_combo.GetItemData(i);
			SetVariableValue(&data, 1);
			m_selected = i;
			if(bSendEvent)
				lpTo->flush();
			return true;
		}
	}
	return false;
}
#endif
/////////////////////////////////////////////////////////////////////////////
// CComboDlg dialog


CComboDlg::CComboDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComboDlg::IDD, pParent)
#ifdef USEPLUGS
	,m_plugcombo(PLUG_DWORD, 0, "combo", &m_curtag, this)
#endif
{
	//
	// because these aren't dynamicaly allocated
	//
#ifdef USEPLUGS
	m_plugcombo.setDesc("plug to ...");
#endif

	m_selected = 0;
	IMPLSMARTREFINIT
	//{{AFX_DATA_INIT(CComboDlg)
	//}}AFX_DATA_INIT
}


void CComboDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComboDlg)
	DDX_Control(pDX, IDC_COMBO, m_combo);
	DDX_Control(pDX, IDC_TITLE, m_title);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComboDlg, CDialog)
	//{{AFX_MSG_MAP(CComboDlg)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////
void CComboDlg::SetupHExtent()
{
	// Find the longest string in the combo box.
	CString	  str;
	CSize	  sz;
	int	  dx = 0;
	TEXTMETRIC   tm;
	CDC*	  pDC = m_combo.GetDC();
	CFont*	  pFont = m_combo.GetFont();

	// Select the listbox font, save the old font
	CFont* pOldFont = pDC->SelectObject(pFont);
	// Get the text metrics for avg char width
	pDC->GetTextMetrics(&tm);

	for (int i = 0; i < m_combo.GetCount(); i++)
	{
	   m_combo.GetLBText(i, str);
	   sz = pDC->GetTextExtent(str);

	   // Add the avg width to prevent clipping
	   sz.cx += tm.tmAveCharWidth;
   
	   if (sz.cx > dx)
		  dx = sz.cx;
	}
	// Select the old font back into the DC
	pDC->SelectObject(pOldFont);
	m_combo.ReleaseDC(pDC);

	// Set the horizontal extent so every character of all strings can 
	// be scrolled to.
	m_combo.SetHorizontalExtent(dx);
}
/**
 ** We need to setup another control that the window itself
 **/
IWindow* CComboDlg::SetTitle(LPCSTR title)
{
	m_title.SetWindowText(title);
	IWindowImpl<CComboDlg>::SetTitle(title); 
	return this;
};
IWindow* CComboDlg::GetTitle(char * title, int maxlen)
{
	m_title.GetWindowText(title, maxlen);
	return this;
};
IControlCombo*	CComboDlg::RemoveAllItems()
{
	m_combo.ResetContent();
	return this;
}
IControlCombo*	CComboDlg::AddItem( const char* strText, size_t data)
{
	int i = m_combo.InsertString(m_combo.GetCount(), strText);
	m_combo.SetItemData(i, data);
	//SetupHExtent();
	/*if(m_combo.GetCurSel() < 0)
	{
		m_combo.SetCurSel(0);
#ifdef USEPLUGS
		m_plugcombo.setValue((DWORD)m_combo.GetItemData(0));
#endif
	}*/
	//m_combo.SetTopIndex(m_combo.GetCount());
	return this;
}
/*int		CComboDlg::insert(int index, LPCSTR str, unsigned long ultag)
{
	int i = m_combo.InsertString(index, str);
	m_combo.SetItemData(i,ultag);
	SetupHExtent();
	/ *if(m_combo.GetCurSel() < 0)
	{
		m_combo.SetCurSel(0);
#ifdef USEPLUGS
		m_plugcombo.setValue((DWORD)m_combo.GetItemData(0));
#endif
	}* /
	m_combo.SetTopIndex(0);
	return i;
}*/
IControlCombo*	CComboDlg::RemoveItem( UINT index, bool bUpdateComponents )
{
	int i = m_combo.DeleteString(index);
	SetupHExtent();
	if(m_combo.GetCurSel() == index)
		m_combo.SetCurSel(0);
	m_combo.SetTopIndex(0);
	return this;
}
/*int		CComboDlg::select(int index)
{
	m_selected = index;
#ifdef USEPLUGS
	m_plugcombo.setValue((DWORD)m_combo.GetItemData(index));
#endif
	return index;//m_combo.SetCurSel(index);
}*/
int	 CComboDlg::GetSelectedIndex() const
{
	return m_combo.GetCurSel();
}
size_t   CComboDlg::GetSelectedData()
{
	return m_combo.GetItemData(m_selected);
}
/*virtual LPCSTR   CComboDlg::GetSelectedText()
{
	m_combo.GetLBText(m_selected, m_tmpstr);
	return (LPCSTR)m_tmpstr;
}*/
static ComboBoxItem tempData;
ComboBoxItem* CComboDlg::GetSelectedItem()
{
  m_combo.GetLBText(m_selected, m_tmpstr);
  strcpy_s(tempData.strText, 255, m_tmpstr);
  tempData.data = m_combo.GetItemData(m_selected);
  //tempData.rcActive;
  tempData.bVisible = true;
  return &tempData;
}
ComboBoxItem* CComboDlg::GetItem( UINT index )
{
  if(index >= (UINT)m_combo.GetCount())
	return NULL;
  m_combo.GetLBText(index, m_tmpstr);
  strcpy_s(tempData.strText, 255, m_tmpstr);
  tempData.data = m_combo.GetItemData(index);
  //tempData.rcActive;
  tempData.bVisible = true;
  return &tempData;
}

UINT	CComboDlg::GetNumItems()
{
  return m_combo.GetCount();
}

IControlCombo* CComboDlg::SetSelectedByIndex( UINT index )
{
	m_combo.SetCurSel((int)index);
	return this;
}
IControlCombo* CComboDlg::SetSelectedByText( const char* strText )
{
	assert(!"function not implemented, yet");
	return this;
}
IControlCombo* CComboDlg::SetSelectedByData( size_t data )
{
    for(int i=0; i<m_combo.GetCount(); i++)
    {
        size_t d = m_combo.GetItemData(i);
        if(d == data)
        {
            m_combo.SetCurSel(i);
            break;
        }
    }
	return this;
}
int	 CComboDlg::GetScrollBarWidth() const
{
	assert(!"function not implemented, yet");
	return 0;
}
size_t   CComboDlg::GetItemData( const char* strText )
{
	assert(!"function not implemented, yet");
	return NULL;
}
size_t   CComboDlg::GetItemData( int nIndex )
{
	if(nIndex >= m_combo.GetCount())
		return 0;
	return m_combo.GetItemData(nIndex);
}
bool	CComboDlg::ContainsItem( const char* strText, UINT iStart )
{
	assert(!"function not implemented, yet");
	return false;
}
int	 CComboDlg::FindItem( const char* strText, UINT iStart)
{
	assert(!"function not implemented, yet");
	return 0;
}
IControlCombo*	CComboDlg::SetDropHeight( UINT nHeight )
{
	assert(!"function not implemented, yet");
	return this;
}
IControlCombo*	CComboDlg::SetScrollBarWidth( int nWidth )
{
	assert(!"function not implemented, yet");
	return this;
}

void CComboDlg::UpdateControl()
{
	int data;
	GetVariableValue(&data, 1);
	for(int i = 0; i< m_combo.GetCount(); i++)
	{
		int d = m_combo.GetItemData(i);
		if(d == data)
		{
			m_combo.SetCurSel(i);
#ifdef USEPLUGS
			//m_plugcombo.setValue((DWORD)d);
#endif
			break;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// CComboDlg message handlers

void CComboDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	RECT rc;
	if(m_title.m_hWnd) 
	{
		m_title.GetWindowRect(&rc);
		m_title.SetWindowPos(NULL, 0,0, cx*0.3f, rc.bottom-rc.top, SWP_NOACTIVATE);
		m_combo.SetWindowPos(NULL, cx*0.3f,0, cx*0.7f, rc.bottom-rc.top, SWP_NOACTIVATE);
	}
}


BOOL CComboDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CComboDlg::OnSelchangeCombo() 
{
	if(m_selected == m_combo.GetCurSel())
		return;
	m_selected = m_combo.GetCurSel();
#ifdef USEPLUGS
	m_plugcombo.setValue((DWORD)m_combo.GetItemData(m_selected));
#endif
	for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
	{
		m_pclients[ic]->ComboSelectionChanged(this, m_selected);
	}
	for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
	{
		g_WindowHandler.m_pclients[ic]->ComboSelectionChanged(this, m_selected);
	}
	int data = m_combo.GetItemData(m_selected);
	SetVariableValue(&data, 1);
}
