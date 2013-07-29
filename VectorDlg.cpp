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
// VectorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SvcMFCUI.h"
#include "VectorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 ** Instanciation of an object.
 **/ 
const void * CVectorDlg::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CVectorDlg *pDlg = new CVectorDlg;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_VECTOR, pOwner);
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
bool CVectorDlg::plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent)
{
	lpFrom->getValue(m_vec[0],m_vec[1],m_vec[2],m_vec[3]);
	if(m_hWnd)
	UpdateValue(CVectorDlg::FROMPLUG);
	// tell the connected readers that data changed. Set bSendEvent=false if you decide to send this
	// method by your own.
	if(bSendEvent)
		lpTo->flush();
	return false;
}
#endif
/////////////////////////////////////////////////////////////////////////////
// CVectorDlg dialog


CVectorDlg::CVectorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVectorDlg::IDD, pParent),
#ifdef USEPLUGS
	m_plugvector(PLUG_FLOATVEC4, 0, "vec", m_vec, this),
#endif
	m_intmode(false),
	m_dim(4)
{
	//
	// because these aren't dynamicaly allocated
	//
#ifdef USEPLUGS
	m_plugvector.setDesc("plug to ...");
#endif

	m_vec[0] = m_vec[1] = m_vec[2] = m_vec[3] = 0;
	IMPLSMARTREFINIT
	//{{AFX_DATA_INIT(CVectorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVectorDlg)
	DDX_Control(pDX, IDC_TITLE, m_title);
	DDX_Control(pDX, IDC_COMPZ, m_compz);
	DDX_Control(pDX, IDC_COMPY, m_compy);
	DDX_Control(pDX, IDC_COMPX, m_compx);
	DDX_Control(pDX, IDC_COMPW, m_compw);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVectorDlg, CDialog)
	//{{AFX_MSG_MAP(CVectorDlg)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_EN_UPDATE(IDC_COMPW, OnUpdateVectorval)
	ON_EN_UPDATE(IDC_COMPX, OnUpdateVectorval)
	ON_EN_UPDATE(IDC_COMPY, OnUpdateVectorval)
	ON_EN_UPDATE(IDC_COMPZ, OnUpdateVectorval)
	//}}AFX_MSG_MAP
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
  ON_EN_CHANGE(IDC_COMPZ, OnEnChangeCompz)
END_MESSAGE_MAP()

/**
 ** We need to setup another control that the window itself
 **/
IWindow* CVectorDlg::SetTitle(LPCSTR title)
{
	m_title.SetWindowText(title);
	IWindowImpl<CVectorDlg>::SetTitle(title); 
	return this;
};
IWindow* CVectorDlg::GetTitle(char * title, int maxlen)
{
	m_title.GetWindowText(title, maxlen);
	return this;
};
IControlVector* CVectorDlg::SetIntMode(bool bYes)
{
	m_intmode = bYes;
	UpdateValue(FROMITF);
	return this;
}
IControlVector* CVectorDlg::SetValue(int n, float val)
{
	if((n>=0)&&(n<4)) 
		m_vec[n] = val;
	UpdateValue(FROMITF);
	return this;
}
IControlVector* CVectorDlg::SetValue(float x, float y, float z, float w)
{
	m_vec[0] = x;
	m_vec[1] = y;
	m_vec[2] = z;
	m_vec[3] = w;
	UpdateValue(FROMITF);
	return this;
}
IControlVector* CVectorDlg::SetDimension(int dim)
{
	if(dim < 2) dim = 2;
	if(dim > 4) dim = 4;
	m_dim = dim;
	return this;
}
IControlVector* CVectorDlg::GetValuesAsInt(int *x, int *y, int *z, int *w)
{
	if(x) *x = (int)m_vec[0];
	if(y) *y = (int)m_vec[1];
	if(z) *z = (int)m_vec[2];
	if(w) *w = (int)m_vec[3];
	return this;
}
IControlVector* CVectorDlg::GetValuesAsFloat(float *x, float *y, float *z, float *w)
{
	if(x) *x = (float)m_vec[0];
	if(y) *y = (float)m_vec[1];
	if(z) *z = (float)m_vec[2];
	if(w) *w = (float)m_vec[3];
	return this;
}
IControlVector* CVectorDlg::GetValuesAsDouble(double *x, double *y, double *z, double *w)
{
	if(x) *x = (double)m_vec[0];
	if(y) *y = (double)m_vec[1];
	if(z) *z = (double)m_vec[2];
	if(w) *w = (double)m_vec[3];
	return this;
}
void CVectorDlg::UpdateValue(UpdateFrom from)
{
	bool bnewdata = false;
  if(from == FROMEDIT)
  {
	  float f;
	  CWnd *pfocused = GetFocus();
	  if(pfocused == &m_compx)
	  {
		  GetDlgItemText(m_compx.GetDlgCtrlID(), m_tmpstr);
		  f = (float)atof(m_tmpstr);
		  if(f != m_vec[0])
			  bnewdata = true;
		  m_vec[0] = f;
	  }
	  if(pfocused == &m_compy)
	  {
		  GetDlgItemText(m_compy.GetDlgCtrlID(), m_tmpstr);
		  f = (float)atof(m_tmpstr);
		  if(f != m_vec[1])
			  bnewdata = true;
		  m_vec[1] = f;
	  }
	  if(pfocused == &m_compz)
	  {
		  GetDlgItemText(m_compz.GetDlgCtrlID(), m_tmpstr);
		  f = (float)atof(m_tmpstr);
		  if(f != m_vec[2])
			  bnewdata = true;
		  m_vec[2] = f;
	  }
	  if(pfocused == &m_compw)
	  {
		  GetDlgItemText(m_compw.GetDlgCtrlID(), m_tmpstr);
		  f = (float)atof(m_tmpstr);
		  if(f != m_vec[3])
			  bnewdata = true;
		  m_vec[3] = f;
	  }
	}
	if(from == FROMBVAR)
	{
	  GetVariableValue(m_vec, m_dim);
	}
	if(m_intmode)
	{
		m_vec[0] = (float)((int)m_vec[0]);
		m_vec[1] = (float)((int)m_vec[1]);
		m_vec[2] = (float)((int)m_vec[2]);
		m_vec[3] = (float)((int)m_vec[3]);
	}
	if(from != FROMBVAR)
	{
	  SetVariableValue(m_vec, m_dim);
	}
	if(from != FROMPLUG)
	{
#ifdef USEPLUGS
	  m_plugvector.flush(); //setValue(m_vec,0);
#endif
	}
    if(from == FROMEDIT)
    {
        for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
        {
          if(m_pclients[ic])
              m_pclients[ic]->VectorChanged(this, m_vec, m_vec, m_dim);
            //TODO VectorChanged args...
        }
        for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
        {
          if(g_WindowHandler.m_pclients[ic])
              g_WindowHandler.m_pclients[ic]->VectorChanged(this, m_vec, m_vec, m_dim);
            //TODO VectorChanged args...
        }
    }
    if(from != FROMEDIT)
    {
    m_compx.SetSel(0,-1);
      m_compy.SetSel(0,-1);
      m_compz.SetSel(0,-1);
      m_compw.SetSel(0,-1);
      char tmp[40];
      sprintf(tmp,"%.2f", m_vec[0]);
      m_compx.ReplaceSel(tmp);
      m_compx.SetSel(0,-1);
      sprintf(tmp,"%.2f", m_vec[1]);
      m_compy.ReplaceSel(tmp);
      m_compy.SetSel(0,-1);
      sprintf(tmp,"%.2f", m_vec[2]);
      m_compz.ReplaceSel(tmp);
      m_compz.SetSel(0,-1);
      sprintf(tmp,"%.2f", m_vec[3]);
      m_compw.ReplaceSel(tmp);
      m_compw.SetSel(0,-1);
    }
}
void CVectorDlg::UpdateControl()
{
	UpdateValue(FROMBVAR);
}
/////////////////////////////////////////////////////////////////////////////
// CVectorDlg message handlers

void CVectorDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	int x=0;
	RECT rc;
	if(m_title.m_hWnd) 
	{
		m_compx.GetWindowRect(&rc);
		m_title.SetWindowPos(NULL, x,0, cx/5.1, rc.bottom-rc.top, SWP_NOACTIVATE);
		x += cx/5;
		m_compx.SetWindowPos(NULL, x,0, cx/5.1, rc.bottom-rc.top, SWP_NOACTIVATE);
		x += cx/5;
		m_compy.SetWindowPos(NULL, x,0, cx/5.1, rc.bottom-rc.top, SWP_NOACTIVATE);
		x += cx/5;
		m_compz.SetWindowPos(NULL, x,0, cx/5.1, rc.bottom-rc.top, SWP_NOACTIVATE);
		x += cx/5;
		m_compw.SetWindowPos(NULL, x,0, cx/5.1, rc.bottom-rc.top, SWP_NOACTIVATE);
	}
}


BOOL CVectorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateValue(FROMITF);
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
void CVectorDlg::OnUpdateVectorval() 
{
}

BOOL CVectorDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	return __super::OnCommand(wParam, lParam);
}

BOOL CVectorDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	BOOL b = __super::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	if((!m_compx.GetSafeHwnd())
	 ||(!m_compy.GetSafeHwnd())
	 ||(!m_compz.GetSafeHwnd())
	 ||(!m_compw.GetSafeHwnd())	 )
	 return b;
	if(((nID == m_compx.GetDlgCtrlID())
		||(nID == m_compy.GetDlgCtrlID())
		||(nID == m_compz.GetDlgCtrlID())
		||(nID == m_compw.GetDlgCtrlID()))
		&&(nCode == 0x501)) // ENTER key
	{
		UpdateValue(FROMEDIT);
	}

	return b;
}

void CVectorDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	__super::OnOK();
}

void CVectorDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CVectorDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case VK_TAB:
		this->NextDlgCtrl();
		break;
	case VK_RETURN:
			GetDlgItemText(m_compx.GetDlgCtrlID(), m_tmpstr);
			m_vec[0] = (float)atof(m_tmpstr);
			GetDlgItemText(m_compy.GetDlgCtrlID(), m_tmpstr);
			m_vec[1] = (float)atof(m_tmpstr);
			GetDlgItemText(m_compz.GetDlgCtrlID(), m_tmpstr);
			m_vec[2] = (float)atof(m_tmpstr);
			GetDlgItemText(m_compw.GetDlgCtrlID(), m_tmpstr);
			m_vec[3] = (float)atof(m_tmpstr);
			for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
		if(m_pclients[ic])
				  m_pclients[ic]->VectorChanged(this, m_vec, m_vec, m_dim);
#ifdef USEPLUGS
			m_plugvector.flush(); //setValue(m_vec,0);
#endif
			break;
	}
	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CVectorDlg::OnEnChangeCompz()
{
  // TODO:  If this is a RICHEDIT control, the control will not
  // send this notification unless you override the __super::OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.

  // TODO:  Add your control notification handler code here
}
