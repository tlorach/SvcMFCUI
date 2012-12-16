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
// ToolBarWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SvcMFCUI.h"
#include "ToolBarWnd.h"
#include "tga.h"
#include "nv_dds_common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef USEPLUGS
bool ToolBarWnd::plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent)
{
  return false;
}
#endif
/**
 ** Instanciation of an object.
 **/ 
const void * ToolBarWnd::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ToolBarWnd *pDlg = new ToolBarWnd;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_TOOLBAR, pOwner);
	if(bOK)
	{
		pDlg->ShowWindow(SW_HIDE);
		return (void*)(static_cast<IControlToolbar*>(pDlg));
	}
	delete pDlg;
	return NULL;
}
/////////////////////////////////////////////////////////////////////////////
// CItem
CItem::~CItem()
{
	if(button.GetSafeHwnd())
		button.DestroyWindow();
}

CItem::CItem(LPCSTR name, int states, unsigned short tag)
#ifdef USEPLUGS
: itemplug(states == 3 ? PLUG_BYTE : PLUG_BOOL, tag, name, &m_curstate, this)
#endif
{
	m_curstate = 0;
	m_ddsidx = 0;
	tag = 0;
	ratio = 0;
}
#ifdef USEPLUGS
bool CItem::plug_IncomingData(IPlug *lpSrc, IPlug *lpDst, bool bSendEvent)
{
	bool bRes = PlugContainer::plug_IncomingData(lpSrc, lpDst, bSendEvent);
	if(button.GetSafeHwnd())
		button.SetCheck(m_curstate);
 	// tell the connected readers that data changed. Set bSendEvent=false if you decide to send this
	// method by your own.
	if(bSendEvent)
		lpDst->flush();

	return bRes;
}
#endif
void CItem::SetPos(int newx, int newy)
{
	button.SetWindowPos(NULL, newx, newy, 0,0, SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER);
}
int CItem::Resize(int newx, int newy)
{
	BOOL b;
	RECT rctxt;
  int w = ratio ? (int)(ratio * newy) : newx; // ratio = 0 if we don't use any image.
	int texty;
	// compute extra bitmap for text
	if(m_showtext)
	{
		CDC *pdc = button.GetDC();
		pdc->DrawText(title, &rctxt, DT_CALCRECT);
	if(ratio)
		  w += 10+(rctxt.right-rctxt.left);
		texty = (rctxt.bottom-rctxt.top);
	}
	button.SetWindowPos(NULL, 0,0, w,newy, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
	// recompute the picture
	RECT rc;
	button.GetClientRect(&rc);
	HBITMAP oldbmp = (HBITMAP)bmpbutton.Detach();

	b = bmpbutton.CreateCompatibleBitmap(button.GetDC() ,rc.right, rc.bottom);
	// Make the copy
	CDC dcimage, dcbutton;
	b = dcimage.CreateCompatibleDC(NULL);
	b = dcbutton.CreateCompatibleDC(NULL);
	CBitmap* oldbmpbutton = dcbutton.SelectObject(&bmpbutton);
	CBitmap* oldbmpimage = dcimage.SelectObject(&bmpimage);

	w = ratio ? (int)(ratio * rc.bottom) : newx;
	if(m_showtext)
	{
		dcbutton.FillRect(CRect(0,0,rc.right-1,rc.bottom-1), (CBrush*)dcimage.SelectStockObject(GRAY_BRUSH));
		dcbutton.DrawText(title, CRect(w,(rc.bottom-texty)/2, rc.right-1,rc.bottom-1), DT_CENTER);
	}
	dcbutton.SetStretchBltMode(HALFTONE);
	b = dcbutton.StretchBlt(0, 0, w, rc.bottom, &dcimage, 0, 0, imagew, imageh, SRCCOPY);

	dcbutton.SelectObject(oldbmpbutton);
	dcimage.SelectObject(oldbmpimage);
	dcbutton.DeleteDC();
	dcimage.DeleteDC();

	button.SetBitmap(bmpbutton);
	DeleteObject(oldbmp);
	button.GetWindowRect(&rc);
	return rc.right-rc.left;
}
int CItem::Create(CWnd *parent, int num, int x, LPCSTR t, int states, size_t tg, BOOL bitmap)
{
	RECT rc;
	parent->GetClientRect(&rc);
	if(t) 
	{
		title = t;
	}
	m_states = states;
	tag = tg;
	DWORD style;
	switch(states)
	{
	case 2:
		style = BS_AUTOCHECKBOX;
		break;
	case 3:
		style = BS_AUTO3STATE;
		break;
	default:
	case 0:
	case 1:
		style = BS_PUSHBUTTON;
		break;
	}
	if(bitmap) 
	{
		style |= BS_BITMAP;
	}
	// Create a push button.
	rc.bottom -= rc.top;
	rc.top = 0;
	rc.left = x;
  rc.right = (int)((float)rc.bottom * (ratio?ratio:1));
	BOOL bOK = button.Create(_T(title), WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_GROUP|BF_FLAT|style, rc, parent, num+100);
	if(!bOK) 
	{
		return -1;
	}
	SetPos(x,0);
	return Resize(rc.right, rc.bottom);
}
int CItem::CreateFromFile(CWnd *parent, int num, int x, LPCSTR t, LPCSTR imagefilename, int ddsidx, int states, size_t tg)
{
	BOOL b = FALSE;
	m_ddsidx = ddsidx;
	m_states = states;
	//
	// Load the picture
	//
	if(imagefilename)
	{
		m_imagefilename = imagefilename;
		if(!strcmp(".tga", imagefilename+strlen(imagefilename)-4)) 
		{
			targaLoader loader;
			if(loader.loadTGA(imagefilename) == targaLoader::TGA_NO_ERROR)
			{
				b = bmpimage.CreateCompatibleBitmap( CDC::FromHandle(::GetDC(NULL)) ,loader.m_nImageWidth, loader.m_nImageHeight);
				BITMAPINFO bi;
				bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bi.bmiHeader.biBitCount = loader.m_nImageBits;
				bi.bmiHeader.biClrImportant = 0;
				bi.bmiHeader.biClrUsed = 0;
				bi.bmiHeader.biCompression = 0;
				bi.bmiHeader.biWidth = loader.m_nImageWidth;
				bi.bmiHeader.biHeight = loader.m_nImageHeight;
				bi.bmiHeader.biPlanes = 1;
				bi.bmiHeader.biSizeImage = loader.m_nImageWidth*loader.m_nImageHeight*loader.m_nImageBits;
				int l = SetDIBits(::GetDC(NULL), bmpimage, 0, loader.m_nImageHeight, loader.m_nImageData, &bi, DIB_RGB_COLORS);
							//b = bmpimage.CreateBitmap(loader.m_nImageWidth, loader.m_nImageHeight, 1, loader.m_nImageBits, loader.m_nImageData);
							ratio = (float)loader.m_nImageWidth/(float)loader.m_nImageHeight;
							imagew = loader.m_nImageWidth;
							imageh = loader.m_nImageHeight;
			}
		}
		else if(!strcmp(".dds", imagefilename+strlen(imagefilename)-4)) 
		{
			dds_common::CDDSImage loader;
			if(loader.load(imagefilename))
			{
				if(ddsidx >= loader.get_size()) 
					ddsidx = loader.get_size()-1;
				dds_common::CTexture &tex = loader[ddsidx];
				imagew = tex.get_height();
				imageh = tex.get_width();
				ratio = (float)imagew/(float)imageh;
				b = bmpimage.CreateCompatibleBitmap( CDC::FromHandle(::GetDC(NULL)) ,imagew, imageh);
				BITMAPINFO bi;
				bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bi.bmiHeader.biBitCount = tex.get_depth();
				bi.bmiHeader.biClrImportant = 0;
				bi.bmiHeader.biClrUsed = 0;
				bi.bmiHeader.biCompression = 0;
				bi.bmiHeader.biWidth = imagew;
				bi.bmiHeader.biHeight = imageh;
				bi.bmiHeader.biPlanes = 1;
				bi.bmiHeader.biSizeImage = imagew*imageh*tex.get_depth();
				int l = SetDIBits(::GetDC(NULL), bmpimage, 0, imageh, (char*)tex, &bi, DIB_RGB_COLORS);
			}
		}
	}
	int ret = Create(parent, num, x, t, states, tg, b);
	return ret;
}
int CItem::CreateFromMemoryBMP(CWnd *parent, int num, int x, LPCSTR t, void * imagedata, int states, size_t tg)
{
	return Create(parent, num, x, t, states, tg, FALSE);
}
int CItem::CreateFromMemoryPNG(CWnd *parent, int num, int x, LPCSTR t, void * imagedata, int states, size_t tg)
{
	return Create(parent, num, x, t, states, tg, FALSE);
}
int CItem::CreateFromMemoryTGA(CWnd *parent, int num, int x, LPCSTR t, void * imagedata, int states, size_t tg)
{
	BOOL b = FALSE;
	//
	// Load the picture
	//
	if(imagedata)
	{
		targaLoader loader;
		if(loader.loadTGAFromMemory((const char *)imagedata) == targaLoader::TGA_NO_ERROR)
		{
			b = bmpimage.CreateCompatibleBitmap( CDC::FromHandle(::GetDC(NULL)) ,loader.m_nImageWidth, loader.m_nImageHeight);
			BITMAPINFO bi;
			bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bi.bmiHeader.biBitCount = loader.m_nImageBits;
			bi.bmiHeader.biClrImportant = 0;
			bi.bmiHeader.biClrUsed = 0;
			bi.bmiHeader.biCompression = 0;
			bi.bmiHeader.biWidth = loader.m_nImageWidth;
			bi.bmiHeader.biHeight = loader.m_nImageHeight;
			bi.bmiHeader.biPlanes = 1;
			bi.bmiHeader.biSizeImage = loader.m_nImageWidth*loader.m_nImageHeight*loader.m_nImageBits;
			int l = SetDIBits(::GetDC(NULL), bmpimage, 0, loader.m_nImageHeight, loader.m_nImageData, &bi, DIB_RGB_COLORS);
						//b = bmpimage.CreateBitmap(loader.m_nImageWidth, loader.m_nImageHeight, 1, loader.m_nImageBits, loader.m_nImageData);
						ratio = (float)loader.m_nImageWidth/(float)loader.m_nImageHeight;
						imagew = loader.m_nImageWidth;
						imageh = loader.m_nImageHeight;
		}
	}
	int ret = Create(parent, num, x, t, states, tg, b);
	return ret;
}
int CItem::CreateFromMemoryDDS(CWnd *parent, int num, int x, LPCSTR t, void * imagedata, int ddsidx, int states, size_t tg)
{
	m_ddsidx = ddsidx;
	return Create(parent, num, x, t, states, tg, FALSE);
}
void CItem::SetState(int state)
{
	m_curstate = state;
	button.SetCheck(state);
}
int CItem::GetState()
{
	m_curstate = button.GetCheck();
	return m_curstate;
}
void CItem::ShowText(bool bYes)
{
	m_showtext = bYes;
	CRect rc;
	button.GetClientRect(&rc);
	Resize(rc.bottom - rc.top, rc.bottom - rc.top);
}

/////////////////////////////////////////////////////////////////////////////
// ToolBarWnd dialog


ToolBarWnd::ToolBarWnd(CWnd* pParent /*=NULL*/)
	: CDialog(ToolBarWnd::IDD, pParent)
{
	IMPLSMARTREFINIT
	m_vertical = false;
	m_maxheight = 100;
	m_minheight = 20;
	m_showtext = true;
#ifdef USEPYTHON
	_python = NULL;
#endif
	//{{AFX_DATA_INIT(ToolBarWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
ToolBarWnd::~ToolBarWnd()
{
}


void ToolBarWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ToolBarWnd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ToolBarWnd, CDialog)
	//{{AFX_MSG_MAP(ToolBarWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// IControlToolbar 
IControlToolbar* ToolBarWnd::GetItemInfos(int item, int &states, size_t &tag,
				char * title, int titlesz, 
				char * imagefname, int imagefnamesz,
				int &ddsidx)
{
	if(item >= (int)m_items.size())
		return this;//false;
	CItem *pitem = m_items[item];
	tag = pitem->tag;
	states = pitem->m_states;
	ddsidx = pitem->m_ddsidx;
	if(title)
	{
		if(pitem->title.GetLength() >= titlesz)
		{
			strncpy(title, (LPCSTR)pitem->title, titlesz-1);
			title[titlesz-1] = '\0';
		}
		else
			strcpy(title, (LPCSTR)pitem->title);
	}
	if(imagefname)
	{
		if(pitem->m_imagefilename.GetLength() >= imagefnamesz)
		{
			strncpy(imagefname, (LPCSTR)pitem->m_imagefilename, imagefnamesz-1);
			imagefname[imagefnamesz-1] = '\0';
		}
		else
			strcpy(imagefname, (LPCSTR)pitem->m_imagefilename);
	}
	return this;//true;
}
/*************************************************************************/ /**
	
 */
#ifdef USEPLUGS
void *ToolBarWnd::GetPlug(int n)
{
	if((n < (int)m_items.size())&&(m_items[n]))
		return (void *)&(m_items[n]->itemplug);
	return NULL;
}
#endif
/*************************************************************************/ /**
	
 */
IControlToolbar* ToolBarWnd::AddItem(LPCSTR title, LPCSTR imagefilename, int ddsidx, int states, size_t tag)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CItem *pitem = new CItem(title, states, (unsigned short)tag);
	pitem->m_showtext = m_showtext;
	int res = pitem->CreateFromFile(this, (int)m_items.size(), m_freex, title, imagefilename, ddsidx, states, tag);
	if(res >= 0) 
	{
		m_freex += res;
		m_items.push_back(pitem);
		RecalcLayout();
		return this;//m_items.size()-1;
	}
	return this;
}
/*************************************************************************/ /**
	
 */
IControlToolbar* ToolBarWnd::AddItemFromMemoryBMP(LPCSTR title, void * imagedata, int states, size_t tag)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CItem *pitem = new CItem(title, states, (unsigned short)tag);
	pitem->m_showtext = m_showtext;
	int res = pitem->CreateFromMemoryBMP(this, (int)m_items.size(), m_freex, title, imagedata, states, tag);
	if(res >= 0) 
	{
		m_freex += res;
		m_items.push_back(pitem);
		RecalcLayout();
		return this;//m_items.size()-1;
	}
	return this;
}
/*************************************************************************/ /**
	
 */
IControlToolbar* ToolBarWnd::AddItemFromMemoryPNG(LPCSTR title, void * imagedata, int states, size_t tag)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CItem *pitem = new CItem(title, states, (unsigned short)tag);
	pitem->m_showtext = m_showtext;
	int res = pitem->CreateFromMemoryPNG(this, (int)m_items.size(), m_freex, title, imagedata, states, tag);
	if(res >= 0) 
	{
		m_freex += res;
		m_items.push_back(pitem);
		RecalcLayout();
		return this;//m_items.size()-1;
	}
	return this;
}
/*************************************************************************/ /**
	
 */
IControlToolbar* ToolBarWnd::AddItemFromMemoryTGA(LPCSTR title, void * imagedata, int states, size_t tag)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CItem *pitem = new CItem(title, states, (unsigned short)tag);
	pitem->m_showtext = m_showtext;
	int res = pitem->CreateFromMemoryTGA(this, (int)m_items.size(), m_freex, title, imagedata, states, tag);
	if(res >= 0) 
	{
		m_freex += res;
		m_items.push_back(pitem);
		RecalcLayout();
		return this;//m_items.size()-1;
	}
	return this;
}
/*************************************************************************/ /**
	
 */
IControlToolbar* ToolBarWnd::AddItemFromMemoryDDS(LPCSTR title, void * imagedata, int ddsidx, int states, size_t tag)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CItem *pitem = new CItem(title, states, (unsigned short)tag);
	pitem->m_showtext = m_showtext;
	int res = pitem->CreateFromMemoryDDS(this, (int)m_items.size(), m_freex, title, imagedata, ddsidx, states, tag);
	if(res >= 0) 
	{
		m_freex += res;
		m_items.push_back(pitem);
		RecalcLayout();
		return this;//m_items.size()-1;
	}
	return this;
}
/*************************************************************************/ /**
	
 */
IControlToolbar* ToolBarWnd::InsertItemFromMemoryBMP(int pos, LPCSTR title, void * imagedata, int states, size_t tag)
{
	assert(!"function not implemented, yet");
	return this;
}
/*************************************************************************/ /**
	
 */
IControlToolbar* ToolBarWnd::InsertItemFromMemoryPNG(int pos, LPCSTR title, void * imagedata, int states, size_t tag)
{
	assert(!"function not implemented, yet");
	return this;
}
/*************************************************************************/ /**
	
 */
IControlToolbar* ToolBarWnd::InsertItemFromMemoryTGA(int pos, LPCSTR title, void * imagedata, int states, size_t tag)
{
	assert(!"function not implemented, yet");
	return this;
}
/*************************************************************************/ /**
	
 */
IControlToolbar* ToolBarWnd::InsertItemFromMemoryDDS(int pos, LPCSTR title, void * imagedata, int ddsidx, int states, size_t tag)
{
	assert(!"function not implemented, yet");
	return this;
}
/*************************************************************************/ /**
	
 */
IControlToolbar* ToolBarWnd::DeleteItem(int pos)
{
	TItems::iterator iItem = m_items.begin();
	for(int i=0; i<pos; i++)
	{
		if(i >= (int)m_items.size())
			return this;//false;
		++iItem;
	}
	if(*iItem) delete *iItem;
	m_items.erase(iItem);
	return this;//true;
}
/*************************************************************************/ /**
	
 */
IControlToolbar* ToolBarWnd::DeleteItem(LPCSTR title)
{
	TItems::iterator iItem = m_items.begin();
	while(iItem != m_items.end())
	{
		if(*iItem)
		{
			if(!strcmp((LPCSTR)(*iItem)->title, title))
			{
				delete *iItem;
				m_items.erase(iItem);
				return this;//true;
			}
		}
		++iItem;
	}
	return this;//false;
}
IControlToolbar* ToolBarWnd::SetPythonInterface(void * pIPython)
{
#ifdef USEPYTHON
	_python = (IPython*)pIPython;
#endif
	return this;
}
IControlToolbar* ToolBarWnd::SetItemPythonScriptEvent(int item, LPCSTR script)
{
#ifdef USEPYTHON
	if(item >= (int)m_items.size())
		return this;//false;
	if(!m_items[item])
		return this;//false;
	m_items[item]->pythoncmd = script;
#endif
	return this;//true;
}
LPCSTR ToolBarWnd::GetItemPythonScriptEvent(int item)
{
#ifdef USEPYTHON
	if(item >= (int)m_items.size())
		return false;
	if(!m_items[item])
		return false;
	return m_items[item]->pythoncmd.c_str();
#else
    return NULL;
#endif
}

/*************************************************************************/ /**
	
 */
IControlToolbar* ToolBarWnd::ItemsSetState(int item, int state)
{
	m_items[item]->SetState(state);
	return this;
}
/*************************************************************************/ /**
	
 */
int ToolBarWnd::ItemsGetState(int item)
{
	return m_items[item]->GetState();
}
/*************************************************************************/ /**
	
 */
IControlToolbar* ToolBarWnd::Vertical(int bYes)
{
	if(bYes >= 0)
	{
		m_vertical = bYes ? true : false;
		RecalcLayout();
	}
	return this;
}
/*************************************************************************/ /**
	
 */
void ToolBarWnd::RecalcLayout()
{
	CRect rc;
	GetClientRect(&rc);
	if(m_vertical) 
	{
		float dy = (float)(rc.bottom)/(float)m_items.size();
		float y = 0;
		if(dy > m_maxheight) dy= (float)m_maxheight;
		else if(dy < m_minheight) dy= (float)m_minheight;
		for(unsigned int i=0; i<m_items.size(); i++)
		{
			m_items[i]->SetPos(0, (int)y);
			m_items[i]->Resize(rc.right-rc.left, (int)dy);
			y += dy;
		}
		m_freex = (int)y;
	}
	else
	{
		int w = 0;
		int dy = rc.bottom;
		if(dy > m_maxheight) dy= m_maxheight;
		else if(dy < m_minheight) dy= m_minheight;
		m_freex = 0;
	w = (rc.right-rc.left);
	if(m_items.size()) 
	  w /= (int)m_items.size();
		for(unsigned int i=0; i<m_items.size(); i++)
		{
			m_items[i]->SetPos(m_freex, 0);
			w = m_items[i]->Resize(w, dy);
			m_freex += w;
		}
	}
}
IControlToolbar* ToolBarWnd::ShowText(bool bYes)
{
	m_showtext = bYes;
	for(unsigned int i=0; i<m_items.size(); i++)
	{
		m_items[i]->ShowText(bYes);
	}
	RecalcLayout();
	return this;
}
IControlToolbar* ToolBarWnd::SetMinMaxHeight(int hmin, int hmax)
{
	m_minheight = hmin;
	m_maxheight = hmax;
	RecalcLayout();
	return this;
}
/////////////////////////////////////////////////////////////////////////////
// ToolBarWnd message handlers

void ToolBarWnd::OnDestroy() 
{
	for(unsigned int i=0; i<m_items.size(); i++)
	{
		CItem *pitem = m_items[i];
		delete pitem;
	}
	CDialog::OnDestroy();
}

void ToolBarWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	RecalcLayout();
	
}

LRESULT ToolBarWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == (WM_USER+10))
	{
		RECT rc;
		GetClientRect(&rc);
		OnSize(0, rc.right-rc.left, rc.bottom-rc.top);
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL ToolBarWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL ToolBarWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if((wParam >= 100) && (wParam < 100+m_items.size()))
	{
		for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
		{
			m_pclients[ic]->ToolbarChanged(this, (unsigned long)wParam-100, (unsigned long)wParam-100);
		}
		for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
		{
			g_WindowHandler.m_pclients[ic]->ToolbarChanged(this, (unsigned long)wParam-100, (unsigned long)wParam-100);
		}
#ifdef USEPYTHON
		if(_python && (!m_items[wParam-100]->pythoncmd.empty()))
		{
			_python->ExecuteString(m_items[wParam-100]->pythoncmd.c_str());
			LPCSTR strres = _python->GetStringResult();
			m_items[wParam-100]->SetState(atoi(strres));
		}
#endif
#ifdef USEPLUGS
		m_items[wParam-100]->itemplug.setValue(m_items[wParam-100]->GetState());
#endif
	}
	
	return CDialog::OnCommand(wParam, lParam);
}

void ToolBarWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

void ToolBarWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void ToolBarWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDialog::OnMouseMove(nFlags, point);
}

BOOL ToolBarWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(pWnd != this)
	{
		pWnd->SetFocus();
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
