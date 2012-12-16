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
#include "stdafx.h"
#include "SvcMFCUI.h"
#include "ConsoleWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 ** Instanciation of an object.
 **/ 
const void * ConsoleWnd::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  BOOL bOK = AfxInitRichEdit2();
  if(!bOK)
	return NULL;
  ConsoleWnd *pDlg = new ConsoleWnd;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	bOK = pDlg->Create(IDD_CONSOLE, pOwner);
	if(bOK)
	{
	  	pDlg->Clear();
		pDlg->SetFont("Arial");//"Courier New");
		//pDlg->SetFontSize(10);
		CWnd *pW = pDlg->GetParent();
		pDlg->ShowWindow(SW_HIDE);
		//pDlg->RedrawWindow();
		//pDlg->PeekMyself();
		return (void*)(static_cast<IWindowConsole*>(pDlg));
	}
	delete pDlg;
	return NULL;
}
/*----------------------------------------------------------------------------------*/ /**

handles incoming data to the console from another plug : lpFrom. lpTo is the console plug...

just dump the string to the console

 **/ //----------------------------------------------------------------------------------
#ifdef USEPLUGS
bool ConsoleWnd::plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent)
{
	String str;
	lpFrom->getValue(str);
	if(!str.empty())
		Printf((char*)str.c_str());
//		Printf("%s\n", str.c_str()); doesn't work !!

  PlugContainer::plug_IncomingData(lpFrom, lpTo, bSendEvent);
	return false;
}
#endif
/////////////////////////////////////////////////////////////////////////////
// ConsoleWnd

ConsoleWnd::ConsoleWnd() :
	CDialog(ConsoleWnd::IDD, NULL)
#ifdef USEPLUGS
	,m_plugoutput(PLUG_STRING, 0, "strout", &m_laststringout, this)
	,m_pluginput(PLUG_STRING, 0, "strin", &m_laststringin, this)
#endif
{
	m_pwnd = NULL;
	ZeroMemory((void*)&cf, sizeof(cf));
  cf.cbSize = sizeof(cf);
	IMPLSMARTREFINIT
	//{{AFX_DATA_INIT(ConsoleWnd)
	//}}AFX_DATA_INIT
	//
	// because these aren't dynamicaly allocated
	//
#ifdef USEPLUGS
	m_pluginput.setDesc("plug to receive strings from other parts and display them in the console");
	m_plugoutput.setDesc("plug to send what the user can type in the console");
#endif
	bChangeFromPrintf = false;
}

ConsoleWnd::~ConsoleWnd()
{
}


BEGIN_MESSAGE_MAP(ConsoleWnd, CDialog)
	//{{AFX_MSG_MAP(ConsoleWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
  ON_EN_UPDATE(IDC_RICHEDIT22, OnEnUpdateRichedit22)
END_MESSAGE_MAP()

void ConsoleWnd::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(ConsoleWnd)
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_RICHEDIT22, m_redit);
}

////////////////////////////////////////////////////////////////////////////
// ConsoleWnd diagnostics

#ifdef _DEBUG
void ConsoleWnd::AssertValid() const
{
	CDialog::AssertValid();
}

void ConsoleWnd::Dump(CDumpContext& dc) const
{
	//CRichEditCtrl::Dump(dc);
}
#endif //_DEBUG

/**
 ** 
 **/
IWindowConsole* ConsoleWnd::SetItalic(bool bYes)
{
	cf.dwMask |= CFM_ITALIC;
	if(bYes)
	{
		cf.dwEffects |= CFE_ITALIC;
	}
	else
	{
		cf.dwEffects &= ~CFE_ITALIC;
	}
	m_redit.SetSelectionCharFormat(cf);
	return this;
}
/**
 ** 
 **/
IWindowConsole* ConsoleWnd::SetBold(int val)
{
	cf.dwMask |= CFM_BOLD|CFM_STRIKEOUT;
	if(val)
	{
		cf.dwEffects |= CFE_BOLD;
	}
	else
	{
		cf.dwEffects &= ~CFE_BOLD;
	}
	m_redit.SetSelectionCharFormat(cf);
	return this;
}
/**
 ** 
 **/
IWindowConsole* ConsoleWnd::SetUnderline(bool bYes)
{
	cf.dwMask |= CFM_UNDERLINE;
	if(bYes)
	{
		cf.dwEffects |= CFE_UNDERLINE;
	}
	else
	{
		cf.dwEffects &= ~CFE_UNDERLINE;
	}
	m_redit.SetSelectionCharFormat(cf);
	return this;
}
/**
 ** 
 **/
IWindowConsole* ConsoleWnd::SetColor(unsigned long rgbcolor)
{
	cf.dwMask |= CFM_COLOR;
	if(rgbcolor)
	{
		cf.dwEffects &= ~CFE_AUTOCOLOR;
		cf.crTextColor = RGB((rgbcolor>>16)&0xFF, (rgbcolor>>8)&0xFF, (rgbcolor&0xFF));
	}
	else
	{
		cf.dwEffects |= CFE_AUTOCOLOR;
	}
	m_redit.SetSelectionCharFormat(cf);
	return this;
}
/**
 ** 
 **/
IWindowConsole* ConsoleWnd::SetColor(unsigned char r, unsigned char g, unsigned char b)
{
	cf.dwMask |= CFM_COLOR;
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	cf.crTextColor = RGB(r,g,b);
	m_redit.SetSelectionCharFormat(cf);
	return this;
}
/**
 ** 
 **/
IWindowConsole* ConsoleWnd::SetFontSize(int sz)
{
	cf.dwMask |= CFM_SIZE;
	cf.yHeight = sz;
	m_redit.SetSelectionCharFormat(cf);
	return this;
}
/**
 ** 
 **/
IWindowConsole* ConsoleWnd::SetFont(LPCSTR name)
{
   m_font.DeleteObject();
   m_font.CreateFont(
   14,						// nHeight
   0,						 // nWidth
   0,						 // nEscapement
   0,						 // nOrientation
   FW_NORMAL,				 // nWeight
   FALSE,					 // bItalic
   FALSE,					 // bUnderline
   0,						 // cStrikeOut
   ANSI_CHARSET,			  // nCharSet
   OUT_DEFAULT_PRECIS,		// nOutPrecision
   CLIP_DEFAULT_PRECIS,	   // nClipPrecision
   DEFAULT_QUALITY,		   // nQuality
   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
   name);		   // lpszFacename
   m_redit.SetFont(&m_font);
	return this;
}

/**
 ** 
 **/
IWindowConsole* ConsoleWnd::Clear()
{
  m_redit.SetSel(0, -1);
  m_redit.Clear();
  m_redit.SetSel(0, 0);
  return this;
}
/**
 ** 
 **/
IWindowConsole* ConsoleWnd::SetCursor(int x, int y)
{
	assert(!"function not implemented, yet");
	return this;
}
/**
 ** 
 **/
IWindowConsole* ConsoleWnd::SetCursorToEnd()
{
  //m_redit.SetCaretPos(m_redit.PosFromChar(m_redit.GetTextLengthEx(GTL_NUMCHARS)));
  //m_redit.LineScroll(20);//-m_redit.LineFromChar(m_redit.CharFromPos(m_redit.GetCaretPos())));
  //m_redit.CharFromPos(m_redit.GetCaretPos());
  /*int nFirstVisible = m_redit.GetFirstVisibleLine();
  if (nFirstVisible > 0)
  {
	 m_redit.LineScroll(nFirstVisible, 0);
  }*/
  return this;
}
/**
 ** 
 **/
IWindowConsole* ConsoleWnd::SetCursorToTop()
{
	assert(!"function not implemented, yet");
	return this;
}
IWindowConsole* ConsoleWnd::Printf(int level, LPCSTR fmt, ... )
{
	static char dest[1024];
	LPCSTR *ppc = (LPCSTR*)_ADDRESSOF(fmt);
	switch(level)
	{
	case 1: //Warning
		SetBold(true);
		SetColor((56<<16) | (184<<8) | 194 );
		break;
	case 2: //Error
		SetColor(0x00FF0000);
		SetBold(true);
		SetUnderline(true);
		break;
	case 3: //Note
		SetColor(0);
		break;
	case 4: //NetW
		SetColor(0);
		break;
	case 5: //No
		SetColor((204<<16) | (83<<8) | 101 );
		break;
	case 6: //Event
		SetColor(0);
		break;
	case 7: //Yes
		SetColor((14<<16) | (201<<8) | 37 );
		break;
	case 0:
		SetColor(0);
		break;
	default:
		break;
	}
	ConsoleWnd::Printf(fmt, (va_list)&(ppc[1]));
	SetColor(0);
	SetBold(false);
	SetUnderline(false);

	return this;
}
/**
 ** uses some token for italic, underline, bold, font size, font name, color.
 ** 
 ** <LI> ///italics///
 ** <LI> '''bold'''
 ** <LI> ___underline___
 ** <LI> col{FF0080} : color FF0080
 ** <LI> size{12} : font size 12
 ** <LI> n : return.
 **/
IWindowConsole* ConsoleWnd::Printf(LPCSTR fmt, ... )
{
	bool italic = 0;
	bool bold = 0;
	bool underline = 0;
	LPSTR	pstr1, prevstr;
	int	nlines = 0;
	//TODO: Add a way to free this up, later !
	static char *fmt2 = NULL;
	static size_t fmt2_sz = 0;
	static char zdza[] = {0x0D, 0x0A};
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!m_hWnd)
		return this;
	if(!fmt)
		return this;

	va_list  vlist;
	// Get output string
	va_start(vlist, fmt);
	int retval;
  //size_t sz = strlen(fmt)+1;
  if(!fmt2)
  {
	fmt2_sz = 2048;
	fmt2 = (char*)malloc(fmt2_sz);
  }
  while((retval = _vsnprintf(fmt2, fmt2_sz, fmt, vlist)) < 0) // means there wasn't anough room
  {
	fmt2_sz *= 2;
	if(fmt2) free(fmt2);
	fmt2 = (char*)malloc(fmt2_sz);
  }

	bChangeFromPrintf = true;
m_redit.SetSel(m_redit.GetTextLengthEx(GTL_NUMCHARS)-1, m_redit.GetTextLengthEx(GTL_NUMCHARS)-1);
	pstr1 = fmt2;
	prevstr = fmt2;
	while(*pstr1 != '\0')
	{
		if((*pstr1 == '\n')
		||((pstr1[0] == '\\') && (pstr1[1] == 'n')))
		{
			nlines++;
			if(prevstr != pstr1) 
			{
				char c = *pstr1;
				*pstr1 = '\0';
				if(m_redit.m_hWnd)
					m_redit.ReplaceSel(prevstr);
				*pstr1 = c;
			}
			if ((pstr1[0] == '\\')&&(pstr1[1] == 'n'))
				pstr1++;
			pstr1++;
			prevstr = pstr1;
			if(m_redit.m_hWnd)
				m_redit.ReplaceSel(zdza);
			continue;
		}
	if((pstr1[0] == '/') && (pstr1[1] == '/') && (pstr1[2] == '/'))
	{
		  *pstr1 = '\0';
		  m_redit.ReplaceSel(prevstr);
	  italic ^= true;
	  SetItalic(italic);
	  pstr1+=3;
  		prevstr = pstr1;
	}
	else if((pstr1[0] == '_') && (pstr1[1] == '_') && (pstr1[2] == '_'))
	{
		  *pstr1 = '\0';
		  m_redit.ReplaceSel(prevstr);
	  underline ^= true;
	  SetUnderline(underline);
	  pstr1+=3;
  		prevstr = pstr1;
	}
	else if((pstr1[0] == '\'') && (pstr1[1] == '\'') && (pstr1[2] == '\''))
	{
		  *pstr1 = '\0';
		  m_redit.ReplaceSel(prevstr);
	  bold ^= true;
	  SetBold(bold);
	  pstr1+=3;
  		prevstr = pstr1;
	}
	else if((pstr1[0] == 'c') 
		&& (pstr1[1] == 'o')
		&& (pstr1[2] == 'l')
		&& (pstr1[3] == '{'))
	{
		  *pstr1 = '\0';
		  m_redit.ReplaceSel(prevstr);
	  pstr1+=4;
	  unsigned long c;
	  pstr1[6]='\0';
	  sscanf(pstr1,"%x", &c);
	  SetColor(c);
	  pstr1+=7;
  		prevstr = pstr1;
	}
	else if((pstr1[0] == 's') 
		&& (pstr1[1] == 'i')
		&& (pstr1[2] == 'z')
		&& (pstr1[3] == 'e')
		&& (pstr1[4] == '{'))
	{
		  *pstr1 = '\0';
		  m_redit.ReplaceSel(prevstr);
	  pstr1+=5;
	  unsigned long c;
	  pstr1[2]='\0';
	  sscanf(pstr1,"%d", &c);
	  SetFontSize(c);
	  pstr1+=3;
  		prevstr = pstr1;
	}
	else 
	  pstr1++;
	}
	if((prevstr != pstr1)&&(m_redit.m_hWnd))
		m_redit.ReplaceSel(prevstr);
	//TODO: improve this: locate the cursor and scroll to it...
	//m_redit.LineScroll(nlines);
	//PeekMyself();
	bChangeFromPrintf = false;
	return this;
}

/////////////////////////////////////////////////////////////////////////////
// ConsoleWnd message handlers

int ConsoleWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
   m_font.CreateFont(
   14,						// nHeight
   0,						 // nWidth
   0,						 // nEscapement
   0,						 // nOrientation
   FW_NORMAL,				 // nWeight
   FALSE,					 // bItalic
   FALSE,					 // bUnderline
   0,						 // cStrikeOut
   ANSI_CHARSET,			  // nCharSet
   OUT_DEFAULT_PRECIS,		// nOutPrecision
   CLIP_DEFAULT_PRECIS,	   // nClipPrecision
   DEFAULT_QUALITY,		   // nQuality
   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
   "Arial");		   // lpszFacename
	return 0;
}

void ConsoleWnd::OnSize(UINT nType, int cx, int cy) 
{
	CRect rc;
	CDialog::OnSize(nType, cx, cy);
	GetClientRect(&rc);
	if(m_redit.m_hWnd) m_redit.SetWindowPos(NULL,rc.left,rc.top, rc.right, rc.bottom ,SWP_NOACTIVATE);
}

void ConsoleWnd::OnClose() 
{
	//
	// Send the event
	//
	for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
	{
		// HACK : This forces this object to not be deleted inside an event.
		AddRef(); 
		m_pclients[ic]->WindowClosed(this);
		Release(); 
	}
	for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
	{
		// HACK : This forces this object to not be deleted inside an event.
		AddRef(); 
		g_WindowHandler.m_pclients[ic]->WindowClosed(this);
		Release(); 
	}
	//CDialog::OnClose();
}


void ConsoleWnd::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_pwnd) m_pwnd->DestroyWindow();
	m_font.DeleteObject();
}
#ifdef USEPLUGS
/*void *ConsoleWnd::GetPlug(int n)
{
	switch(n)
	{
	case 0:
		return &m_pluginput;
	case 1:
		return &m_plugoutput;
	}
	return NULL;
}*/
#endif

void ConsoleWnd::OnEnUpdateRichedit22()
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
static int previouslines = -1;
	char buf[255];
	if(previouslines < 0)
		previouslines = m_redit.GetLineCount();
	int curlines = m_redit.GetLineCount();
	int dif = curlines - previouslines;
	previouslines = curlines;
	if(dif == 1 && (!bChangeFromPrintf))
	{
		int l = m_redit.LineFromChar(-1);
		m_redit.GetLine(l-1, buf, 254);
		int len = m_redit.LineIndex(l-1);
		len = m_redit.LineLength(len);
		buf[len] = '\0';
		m_laststringout = buf;
#ifdef USEPLUGS
		m_plugoutput.flush();
#endif
		m_laststringout = "";

		for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
			if(m_pclients[ic]) m_pclients[ic]->ConsoleChanged(this, buf, buf);
		for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
			if(g_WindowHandler.m_pclients[ic]) g_WindowHandler.m_pclients[ic]->ConsoleChanged(this, buf, buf);
		// TODO: replace the string with the one that would be changed from the callback... if needed	
	}
}
/**
	handles the last string when 'return' is pressed
 **/
void ConsoleWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void ConsoleWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL ConsoleWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::OnCommand(wParam, lParam);
}

