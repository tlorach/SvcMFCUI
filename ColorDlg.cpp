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
// ColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SvcMFCUI.h"
#include "ColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 ** Instanciation of an object.
 **/ 
const void * CColorDlg::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CColorDlg *pDlg = new CColorDlg;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_COLOR, pOwner);
	if(bOK)
	{
		CWnd *pW = pDlg->GetParent();
		pDlg->ShowWindow(SW_HIDE);
		//pDlg->RedrawWindow();
		//pDlg->PeekMyself();
		return (void*)(static_cast<IControlVector*>(pDlg));
	}
	delete pDlg;
	return NULL;
}

/*----------------------------------------------------------------------------------*/ /**

handles incoming data to the console from another plug : lpFrom. lpTo is the console plug...

just dump the string to the console

 **/ //----------------------------------------------------------------------------------
#ifdef USEPLUGS
bool CColorDlg::plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent)
{
	float r,g,b,a;
	lpFrom->getValue(r,b,g,a);
	m_vec[0] = r;
	m_vec[1] = g;
	m_vec[2] = b;
	m_vec[3] = a;
  UpdateValue();
	// tell the connected readers that data changed. Set bSendEvent=false if you decide to send this
	// method by your own.
	if(bSendEvent)
		lpTo->flush();
	return false;
}
#endif
/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog


CColorDlg::CColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorDlg::IDD, pParent)
#ifdef USEPLUGS
	,m_plugvector(PLUG_FLOATVEC4, 0, "color", m_vec, this)
#endif
{
	//
	// because these aren't dynamicaly allocated
	//
#ifdef USEPLUGS
	m_plugvector.setDesc("plug to ...");
#endif

	m_vec[0] = m_vec[1] = m_vec[2] = m_vec[3] = 0;
	IMPLSMARTREFINIT
	//{{AFX_DATA_INIT(CColorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorDlg)
	DDX_Control(pDX, IDC_COMPW, m_compw);
	DDX_Control(pDX, IDC_TITLE, m_title);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON1, m_colorbutton);
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialog)
	//{{AFX_MSG_MAP(CColorDlg)
	ON_WM_SIZE()
	ON_EN_UPDATE(IDC_COMPW, OnUpdateCompw)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnClickedButtonColor)
END_MESSAGE_MAP()

/**
 ** We need to setup another control that the window itself
 **/
IWindow* CColorDlg::SetTitle(LPCSTR title)
{
	m_title.SetWindowText(title);
	IWindowImpl<CColorDlg>::SetTitle(title); 
	return this;
};
IWindow* CColorDlg::GetTitle(char * title, int maxlen)
{
	m_title.GetWindowText(title, maxlen);
	return this;
};
IControlVector* CColorDlg::SetIntMode(bool bYes)
{
	assert(!"function not implemented, yet");
	return this;
}
IControlVector* CColorDlg::SetValue(int n, float val)
{
	if((n>=0)&&(n<4)) 
		m_vec[n] = val;
#ifdef USEPLUGS
	m_plugvector.setValue(m_vec);
#endif
	UpdateValue();
	return this;
}
IControlVector* CColorDlg::SetValue(float x, float y, float z, float w)
{
#ifdef USEPLUGS
	m_plugvector.setValue(x,y,z,w);
#endif
	/*m_vec[0] = x;
	m_vec[1] = y; done in setValue()
	m_vec[2] = z;
	m_vec[3] = w;*/
	UpdateValue();
	return this;
}
IControlVector* CColorDlg::SetDimension(int dim)
{
	assert(!"function not implemented, yet");
	return this;
}
IControlVector* CColorDlg::GetValuesAsInt(int *x, int *y, int *z, int *w)
{
	if(x) *x = (int)m_vec[0];
	if(y) *y = (int)m_vec[1];
	if(z) *z = (int)m_vec[2];
	if(w) *w = (int)m_vec[3];
	return this;
}
IControlVector* CColorDlg::GetValuesAsFloat(float *x, float *y, float *z, float *w)
{
	if(x) *x = (float)m_vec[0];
	if(y) *y = (float)m_vec[1];
	if(z) *z = (float)m_vec[2];
	if(w) *w = (float)m_vec[3];
	return this;
}
IControlVector* CColorDlg::GetValuesAsDouble(double *x, double *y, double *z, double *w)
{
	if(x) *x = (double)m_vec[0];
	if(y) *y = (double)m_vec[1];
	if(z) *z = (double)m_vec[2];
	if(w) *w = (double)m_vec[3];
	return this;
}
void CColorDlg::UpdateValue()
{
	m_compw.SetSel(0,-1);
	char tmp[40];
	sprintf(tmp,"%.2f", m_vec[3]);
	m_compw.SetFocus();
	m_compw.ReplaceSel(tmp);
	m_compw.SetSel(0,-1);
	COLORREF c;
  c = ((0xFF & (DWORD)(256.0*m_vec[3])));
  c <<= 8;
  c |= ((0xFF & (DWORD)(256.0*m_vec[2])));
  c <<= 8;
  c |= ((0xFF & (DWORD)(256.0*m_vec[1])));
  c <<= 8;
  c |= ((0xFF & (DWORD)(256.0*m_vec[0])));
		m_colorbutton.m_c = c;
		m_colorbutton.Invalidate();
	//PeekMyself();
}
void CColorDlg::UpdateControl()
{
	GetVariableValue(m_vec, 4);
	UpdateValue();
}
/////////////////////////////////////////////////////////////////////////////
// CColorDlg message handlers

void CColorDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	int x=0;
	RECT rc;
	if(m_title.m_hWnd) 
	{
		m_title.GetWindowRect(&rc);
		m_title.SetWindowPos(NULL, x,0, cx/3, rc.bottom-rc.top, SWP_NOACTIVATE);
		x += cx/3;
		m_colorbutton.SetWindowPos(NULL, x,0, cx/3, rc.bottom-rc.top, SWP_NOACTIVATE);
		x += cx/3;
		m_compw.SetWindowPos(NULL, x,0, cx/3, rc.bottom-rc.top, SWP_NOACTIVATE);
	}
}


BOOL CColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateValue();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CColorDlg::OnUpdateCompw() 
{
	CWnd *pfocused = GetFocus();
	if(pfocused == &m_compw)
	{
		GetDlgItemText(m_compw.GetDlgCtrlID(), m_tmpstr);
		m_vec[3] = (float)atof(m_tmpstr);
	}
	for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
	{
		m_pclients[ic]->ColorPicker(this, m_vec, m_vec);
	}
	for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
	{
		g_WindowHandler.m_pclients[ic]->ColorPicker(this, m_vec, m_vec);
	}
#ifdef USEPLUGS
	m_plugvector.flush(); //setValue(m_vec,0);
#endif
	SetVariableValue(m_vec, 4);	
}

void CColorDlg::OnColor() 
{
}

void CColorDlg::OnClickedButtonColor()
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
	if(ok)
	{
		COLORREF c = cc.rgbResult;
		m_colorbutton.m_c = c;
		m_colorbutton.Invalidate();
		m_vec[0] = ((float)(c & 0xFF))/255.0f;
		c = c >> 8;
		m_vec[1] = ((float)(c & 0xFF))/255.0f;
		c = c >> 8;
		m_vec[2] = ((float)(c & 0xFF))/255.0f;
#ifdef USEPLUGS
		m_plugvector.flush(); //setValue(m_vec);
#endif
		SetVariableValue(m_vec, 4);	
	}
}
