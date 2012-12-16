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
#include "BooleanDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 ** Instanciation of an object.
 **/ 
const void * CBooleanDlg::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CBooleanDlg *pDlg = new CBooleanDlg;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_BOOLEAN, pOwner);
	if(bOK)
	{
		CWnd *pW = pDlg->GetParent();
		pDlg->ShowWindow(SW_HIDE);
		//pDlg->RedrawWindow();
		//pDlg->PeekMyself();
		return (void*)(static_cast<IControlScalar*>(pDlg));
	}
	delete pDlg;
	return NULL;
}
/*----------------------------------------------------------------------------------*/ /**

handles incoming data to the console from another plug : lpFrom. lpTo is the console plug...

just dump the string to the console

 **/ //----------------------------------------------------------------------------------

#ifdef USEPLUGS
bool CBooleanDlg::plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent)
{
	float s;
	lpFrom->getValue(s);
	if(m_check.m_hWnd)
	{
		if(s > 0)
		{
			m_check.SetCheck(1);
	  m_val = 1;
		}
		else
		{
			m_check.SetCheck(0);
	  m_val = 0;
		}
	}
	// tell the connected readers that data changed. Set bSendEvent=false if you decide to send this
	// method by your own.
	if(bSendEvent)
		lpTo->flush();
	SetVariableValue(&m_val, 1);
	return false;
}
#endif
/////////////////////////////////////////////////////////////////////////////
// CBooleanDlg dialog


CBooleanDlg::CBooleanDlg(CWnd* pParent /*=NULL*/) :
#ifdef USEPLUGS
	m_plugboolean(PLUG_FLOAT, 0, "bool", &m_val, this),
#endif
	CDialog(CBooleanDlg::IDD, pParent), m_val(0)
{
	//
	// because these aren't dynamicaly allocated
	//
#ifdef USEPLUGS
	m_plugboolean.setDesc("plug to ...");
#endif

	IMPLSMARTREFINIT
	//{{AFX_DATA_INIT(CBooleanDlg)
	//}}AFX_DATA_INIT
}


void CBooleanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBooleanDlg)
	DDX_Control(pDX, IDC_CHECK, m_check);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBooleanDlg, CDialog)
	//{{AFX_MSG_MAP(CBooleanDlg)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK, OnCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////
/**
 ** We need to setup another control that the window itself
 **/
IWindow* CBooleanDlg::SetTitle(LPCSTR title)
{
	m_check.SetWindowText(title);
	IWindowImpl<CBooleanDlg>::SetTitle(title); 
	return this;
};
IWindow* CBooleanDlg::GetTitle(char * title, int maxlen)
{
	m_check.GetWindowText(title, maxlen);
	return this;
};
float CBooleanDlg::GetValue()
{
	if(m_check.GetCheck()) return 1.0f;
	return 0;
}
LPCSTR CBooleanDlg::GetValueAsString()
{
	const char * str;
	if(m_check.GetCheck()) str = "true";
	else str = "false";
	return str;
}
IControlScalar* CBooleanDlg::SetIntMode(bool bYes)
{
	return this;
}
IControlScalar* CBooleanDlg::SetValue(float s)
{
	if(s > 0)
	{
		m_check.SetCheck(1);
#ifdef USEPLUGS
		m_plugboolean.setValue(1.0f);
#endif
	}
	else
	{
		m_check.SetCheck(0);
#ifdef USEPLUGS
		m_plugboolean.setValue(0.0f);
#endif
	}
	return this;
}
IControlScalar* CBooleanDlg::SetBounds(float min, float max)
{
	return this;
}
IControlScalar* CBooleanDlg::SetStep(float s)
{
	return this;
}
void CBooleanDlg::UpdateControl()
{
	float s;
	GetVariableValue(&s, 1);
	if(s > 0)
	{
		m_check.SetCheck(1);
#ifdef USEPLUGS
		m_plugboolean.setValue(1.0f);
#endif
	}
	else
	{
		m_check.SetCheck(0);
#ifdef USEPLUGS
		m_plugboolean.setValue(0.0f);
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBooleanDlg message handlers

void CBooleanDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	RECT rc;
	if(m_check.m_hWnd) 
	{
		m_check.GetWindowRect(&rc);
		m_check.SetWindowPos(NULL, 0,0, cx, rc.bottom-rc.top, SWP_NOACTIVATE);
	}
}


BOOL CBooleanDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
void CBooleanDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBooleanDlg::OnCheck() 
{
	bool prevbval = m_val == 0 ? false : true;
	m_val = (float)(m_check.GetCheck() ? 1 : 0);
	bool bval = m_val == 0 ? false : true;
	for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
	{
		m_pclients[ic]->CheckBoxChanged(this, bval, prevbval);
		m_val = bval ? 1.0f : 0.0f;
	}
	for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
	{
		g_WindowHandler.m_pclients[ic]->CheckBoxChanged(this, bval, prevbval);
		m_val = bval ? 1.0f : 0.0f;
	}
	SetVariableValue(&m_val, 1);
	SetValue((float)m_val);
}
