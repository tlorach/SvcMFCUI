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
// Label.cpp : implementation file
//
#include "stdafx.h"
#include "SvcMFCUI.h"
#include "Label.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 ** Instanciation of an object.
 **/ 
const void * CLabel::NewObjectFunc(size_t lparam1, size_t lparam2)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CLabel *pDlg = new CLabel;
    CWnd *pOwner;
    if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
    else pOwner = &g_WindowHandler;
    BOOL bOK = pDlg->Create(IDD_LABEL, pOwner);
    if(bOK)
    {
        CWnd *pW = pDlg->GetParent();
        pDlg->ShowWindow(SW_HIDE);
        //pDlg->RedrawWindow();
        //pDlg->PeekMyself();
        return (void*)(static_cast<IControlLabel*>(pDlg));
    }
    delete pDlg;
    return NULL;
}

/*----------------------------------------------------------------------------------*/ /**

handles incoming data to the console from another plug : lpFrom. lpTo is the console plug...

just dump the string to the console

 **/ //----------------------------------------------------------------------------------
#ifdef USEPLUGS
bool CLabel::plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent)
{
    if(!(m_hWnd))
        return false;
    String str;
    lpFrom->getValue(str);
    if(!str.empty())
    {
      m_edit.SetWindowText(str.c_str());
      // tell the connected readers that data changed. Set bSendEvent=false if you decide to send this
      // method by your own.
      if(bSendEvent)
          lpTo->flush();
    }
    return false;
}
#endif
/////////////////////////////////////////////////////////////////////////////
// CLabel dialog


CLabel::CLabel(CWnd* pParent /*=NULL*/)
    : CDialog(CLabel::IDD, pParent)
#ifdef USEPLUGS
    ,m_plugstring(PLUG_STRING, 0, "str", &m_string, this)
#endif
{
    //
    // because these aren't dynamicaly allocated
    //
#ifdef USEPLUGS
    m_plugstring.setDesc("plug to ...");
#endif

    IMPLSMARTREFINIT
    //{{AFX_DATA_INIT(CLabel)
    //}}AFX_DATA_INIT
}


void CLabel::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT2, m_edit);
}


BEGIN_MESSAGE_MAP(CLabel, CDialog)
    //{{AFX_MSG_MAP(CLabel)
    ON_WM_SIZE()
    ON_WM_KEYDOWN()
    ON_WM_HSCROLL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////
/**
 ** We need to setup another control that the window itself
 **/
IWindow* CLabel::SetTitle(LPCSTR title)
{
    m_edit.SetWindowText(title);
    IWindowImpl<CLabel>::SetTitle(title); 
    return this;
};
IWindow* CLabel::GetTitle(char * title, int maxlen)
{
    m_edit.GetWindowText(title, maxlen);
    return this;
};
IControlLabel* CLabel::SetString(LPCSTR fmt, ...)
{
	static char *fmt2 = NULL;
	static size_t fmt2_sz = 0;
	static char zdza[] = {0x0D, 0x0A};
    if(!fmt)
        return this;
	va_list  vlist;
	// Get output string
	va_start(vlist, fmt);
	int retval;
    //size_t sz = strlen(fmt)+1;
    if(!fmt2)
    {
	    fmt2_sz = 256;
	    fmt2 = (char*)malloc(fmt2_sz);
    }
    while((retval = _vsnprintf(fmt2, fmt2_sz, fmt, vlist)) < 0) // means there wasn't anough room
    {
	    fmt2_sz *= 2;
	    if(fmt2) free(fmt2);
	    fmt2 = (char*)malloc(fmt2_sz);
    }
	//while(*pstr1 != '\0')
	//{
	//	if((*pstr1 == '\n')
	//	||((pstr1[0] == '\\') && (pstr1[1] == 'n')))
	//	{
	//		if ((pstr1[0] == '\\')&&(pstr1[1] == 'n'))
	//			pstr1++;
	//		pstr1++;
	//	}
	//    pstr1++;
	//}
    m_edit.SetWindowText(fmt2);
    return this;
}
/////////////////////////////////////////////////////////////////////////////
// CLabel message handlers

void CLabel::OnSize(UINT nType, int cx, int cy) 
{
    CDialog::OnSize(nType, cx, cy);
    
    if(m_edit.m_hWnd) 
    {
        //RECT rc;
        //m_edit.GetWindowRect(&rc);
        m_edit.SetWindowPos(NULL, 0,0, cx, cy, SWP_NOACTIVATE);
    }
}


BOOL CLabel::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
