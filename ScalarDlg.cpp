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
// ScalarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SvcMFCUI.h"
#include "ScalarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 ** Instanciation of an object.
 **/ 
const void * CScalarDlg::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CScalarDlg *pDlg = new CScalarDlg;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_SCALAR, pOwner);
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
bool CScalarDlg::plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent)
{
	float f;
	lpFrom->getValue(f);
	switch(lpTo->getId())
	{
	case 0: // val
		{
		if(m_intmode)
			m_val = (float)((int)f); // to check...
	else m_val = f;
		if(!(m_hWnd))
			break;
		f= m_val;
		/*m_slider.SetPos((int)(f*m_multfact));
		if(!m_inthectrlevent)
		{
			m_scalarval.SetSel(0,-1);
			char tmp[40];
			sprintf(tmp,"%.4f", f);
			m_scalarval.ReplaceSel(tmp);
			m_scalarval.SetSel(0,-1);
		}*/
	UpdateValue(CScalarDlg::SC_PLUG);
		//PeekMyself();
		}
		break;
	case 1: // min
		{
	m_min = f;
		if(!(m_hWnd))
			break;
		m_slider.SetRange((int)(m_multfact*f), (int)(m_multfact*m_max), FALSE);
		}
		break;
	case 2: // max
		{
	m_max = f;
		if(!(m_hWnd))
			break;
		m_slider.SetRange((int)(m_multfact*m_min), (int)(m_multfact*f), FALSE);
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
// CScalarDlg dialog


CScalarDlg::CScalarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScalarDlg::IDD, pParent),
	m_val(0), m_min(-100), m_max(100),
#ifdef USEPLUGS
	m_plugval(PLUG_FLOAT, 0, "val", &m_val, this),
	m_plugmin(PLUG_FLOAT, 1, "min", &m_min, this),
	m_plugmax(PLUG_FLOAT, 2, "max", &m_max, this),
#endif
	m_intmode(false),
    m_multfact(100.0),
	m_inthectrlevent(0)
{
	//
	// because these aren't dynamicaly allocated
	//
#ifdef USEPLUGS
	m_plugval.setDesc("plug to ...");
	m_plugmin.setDesc("plug to ...");
	m_plugmax.setDesc("plug to ...");
#endif

	IMPLSMARTREFINIT
	//{{AFX_DATA_INIT(CScalarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CScalarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScalarDlg)
	DDX_Control(pDX, IDC_TITLE, m_title);
	DDX_Control(pDX, IDC_SCALARVAL, m_scalarval);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Control(pDX, IDC_BUTTON2, m_buttonLeft);
	DDX_Control(pDX, IDC_BUTTON1, m_buttonRight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScalarDlg, CDialog)
	//{{AFX_MSG_MAP(CScalarDlg)
	ON_EN_CHANGE(IDC_SCALARVAL, OnChangeScalarval)
	ON_EN_KILLFOCUS(IDC_SCALARVAL, OnKillfocusScalarval)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_EN_UPDATE(IDC_SCALARVAL, OnUpdateScalarval)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButtonRight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**
 ** We need to setup another control that the window itself
 **/
IWindow* CScalarDlg::SetTitle(LPCSTR title)
{
	m_title.SetWindowText(title);
	IWindowImpl<CScalarDlg>::SetTitle(title); 
	return this;
};
IWindow* CScalarDlg::GetTitle(char * title, int maxlen)
{
	m_title.GetWindowText(title, maxlen);
	return this;
};
float CScalarDlg::GetValue()
{
	return m_val;
}
LPCSTR CScalarDlg::GetValueAsString()
{
	GetDlgItemText(m_scalarval.GetDlgCtrlID(), m_tmpstr);
	return m_tmpstr;
}
IControlScalar* CScalarDlg::SetIntMode(bool bYes)
{
	m_intmode = bYes;
    m_multfact = bYes ? 1.0 : 100.0;
    if(!m_intmode)
    {
        m_multfact = 100.0f/(m_max - m_min);
        //if(m_multfact < 1.0f)
        //    m_multfact = 1.0f;
    }
    int tf = (int)(m_multfact/2.0);
    if(tf == 0) tf = 1;
	//m_slider.SetTicFreq(tf);
    m_slider.SetRange((int)(m_multfact*m_min), (int)(m_multfact*m_max), FALSE);
	//UpdateValue(SC_ITF);
	return this;
}
IControlScalar* CScalarDlg::SetValue(float s)
{
	m_val = s;
	UpdateValue(SC_ITF);
	return this;
}
IControlScalar* CScalarDlg::SetBounds(float min, float max)
{
	m_min = min;
	m_max = max;
    if(!m_intmode)
    {
        m_multfact = 100.0f/(max - min);
        //if(m_multfact < 1.0f)
        //    m_multfact = 1.0f;
    }
    int tf = (int)(m_multfact/2.0);
    if(tf == 0) tf = 1;
	//m_slider.SetTicFreq(tf);
	//UpdateValue(SC_ITF);
	return this;
}
IControlScalar* CScalarDlg::SetStep(float s)
{
    m_multfact = s;
    int tf = (int)(m_multfact/2.0);
    if(tf == 0) tf = 1;
	//m_slider.SetTicFreq(tf);
    m_slider.SetRange((int)(m_multfact*m_min), (int)(m_multfact*m_max), FALSE);
	return this;
}
void CScalarDlg::UpdateValue(UpdateFrom from)
{
    /*if(m_val < m_min) 
        m_val = m_min;
    if(m_val > m_max) 
        m_val = m_max;*/
    if(from == SC_EDIT)
    {
        GetDlgItemText(m_scalarval.GetDlgCtrlID(), m_tmpstr);
        m_val = (float)atof(m_tmpstr);
    }
    else if(from == SC_BVAR)
    {
        GetVariableValue(&m_val, 1);
    }
    if(m_intmode)
    {
        m_val = (float)((int)m_val);
    }

    if(from != SC_BVAR)
    {
        SetVariableValue(&m_val, 1);
    }
    if(from != SC_PLUG)
    {
#ifdef USEPLUGS
      m_plugval.flush();// setValue(m_val);
      m_plugmin.flush();// setValue(m_min);
      m_plugmax.flush();// setValue(m_max);
#endif
    }
    if(from != SC_SLIDER)
    {
      m_slider.SetRange(m_multfact*m_min, m_multfact*m_max, FALSE);
      //ASSERT(::IsWindow(m_slider.m_hWnd)); ::SendMessage(m_slider.m_hWnd, TBM_SETPOS, FALSE, m_val*m_multfact);
      m_slider.SetPos(m_val*m_multfact);
    }
    if(from != SC_EDIT)
    {
      m_scalarval.SetSel(0,-1);
      char tmp[40];
      sprintf(tmp,m_intmode ? "%.0f" : "%.4f", m_val);
      m_scalarval.ReplaceSel(tmp);
      m_scalarval.SetSel(0,-1);
    }
    // invoke callbacks only if the changes came from user interface
    if((from == SC_SLIDER)||(from == SC_EDIT))
    {
        for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
        {
        if(m_pclients[ic])
              m_pclients[ic]->ScalarChanged(this, m_val, m_val);
        //TODO: manage the case where the callback changed the value...
        }
        for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
        {
            if(g_WindowHandler.m_pclients[ic])
                  g_WindowHandler.m_pclients[ic]->ScalarChanged(this, m_val, m_val);
            //TODO: manage the case where the callback changed the value...
        }
    }
}

void CScalarDlg::UpdateControl()
{
	UpdateValue(SC_BVAR);
}

/////////////////////////////////////////////////////////////////////////////
// CScalarDlg message handlers

void CScalarDlg::OnChangeScalarval() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	
}

void CScalarDlg::OnKillfocusScalarval() 
{
	// TODO: Add your control notification handler code here
	
}

void CScalarDlg::OnSize(UINT nType, int cx, int cy) 
{
	RECT rc;
	CDialog::OnSize(nType, cx, cy);
	
	int x=0;
	if(m_title.m_hWnd) 
	{
		m_scalarval.GetWindowRect(&rc);
		m_title.SetWindowPos(NULL, x, 0, cx/4, rc.bottom-rc.top, SWP_NOACTIVATE/*|SWP_ASYNCWINDOWPOS|SWP_DEFERERASE|SWP_NOSENDCHANGING*/);
		x += cx/4;
		m_scalarval.SetWindowPos(NULL, x, 0, cx/4, rc.bottom-rc.top, SWP_NOACTIVATE/*|SWP_ASYNCWINDOWPOS|SWP_DEFERERASE|SWP_NOSENDCHANGING*/);
		x += cx/4;
		m_buttonLeft.SetWindowPos(NULL, cx-10-20*2, 0, 20, rc.bottom-rc.top, SWP_NOACTIVATE/*|SWP_ASYNCWINDOWPOS|SWP_DEFERERASE|SWP_NOSENDCHANGING*/);
		m_buttonRight.SetWindowPos(NULL, cx-10-20, 0, 20, rc.bottom-rc.top, SWP_NOACTIVATE/*|SWP_ASYNCWINDOWPOS|SWP_DEFERERASE|SWP_NOSENDCHANGING*/);
		m_slider.GetWindowRect(&rc);
		m_slider.SetWindowPos(NULL, x, 0, cx - x - 10 - 20*2, rc.bottom-rc.top, SWP_NOACTIVATE/*|SWP_ASYNCWINDOWPOS|SWP_DEFERERASE|SWP_NOSENDCHANGING*/);
	}
}

void CScalarDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CWnd *pWnd = GetFocus();
	if(pWnd == &m_slider)
	{
		m_val = (float)m_slider.GetPos()/m_multfact;
		UpdateValue(SC_SLIDER);
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CScalarDlg::OnBnClickedButtonLeft()
{
		m_val -= 1.0f;
		if(m_val < m_min)
			m_val = m_min;
		UpdateValue(SC_LRBUTTONS);
}
void CScalarDlg::OnBnClickedButtonRight()
{
		m_val += 1.0f;
		if(m_val > m_max)
			m_val = m_max;
		UpdateValue(SC_LRBUTTONS);
}

void CScalarDlg::OnUpdateScalarval() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	/*CWnd *pWnd = GetFocus();
	if(pWnd == &m_scalarval)
	{
		GetDlgItemText(m_scalarval.GetDlgCtrlID(), m_tmpstr);
		m_val = (float)atof(m_tmpstr);
		m_slider.SetPos((int)(m_val*m_multfact));
		m_inthectrlevent=1;
		UpdateValue(SC_EDIT);
		m_inthectrlevent=0;
	}
	for(int ic = 0; ic < m_pclients.size(); ic++)
	{
	if(m_pclients[ic])
		  m_pclients[ic]->rtCtrlEvent(this, "scalar", (unsigned long)&m_val, m_val, luser1, luser2 );
	}
#ifdef USEPLUGS
	m_plugval.setValue(m_val,0);
    #endif
	*/
}

BOOL CScalarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    int tf = (int)(m_multfact/2.0);
    if(tf == 0) tf = 1;
	//m_slider.SetTicFreq(tf);
	UpdateValue(SC_ITF);
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CScalarDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	BOOL b = __super::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	if((nID == m_scalarval.GetDlgCtrlID())&&(nCode == 0x501)) // ENTER key
	{
		UpdateValue(SC_EDIT);
	}

	return b;
}

