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
#include "LogWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int _gColText[] = { IDS_COL_1, IDS_COL_2};
static int _gColSize[] = { 200, 100};
void CLogWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogWnd)
	DDX_Control(pDX, IDC_LISTLOGGING, m_ListLog);
	//}}AFX_DATA_MAP
}
/**
 ** Instanciation of an object.
 **/ 
const void * CLogWnd::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CLogWnd *pDlg = new CLogWnd;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_LOG, pOwner);
	if(bOK)
	{
		CWnd *pW = pDlg->GetParent();
		pDlg->ShowWindow(SW_HIDE);
		//pDlg->RedrawWindow();
		//pDlg->PeekMyself();
		return (void*)(static_cast<IWindowLog*>(pDlg));
	}
	delete pDlg;
	return NULL;
}

static int GetImageId(CString &s);
/*----------------------------------------------------------------------------------*/ /**

handles incoming data to the console from another plug : lpFrom. lpTo is the console plug...

just dump the string to the console

 **/ //----------------------------------------------------------------------------------
#ifdef USEPLUGS
bool CLogWnd::plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent)
{
	String str;
	if(!(m_hWnd))
		return false;
	lpFrom->getValue(str);
	if(!str.empty())
	{
	m_lastlogstr = str;
		CString strbuff(str.c_str());
		int pos, img;
		do
		{
			img = 0;
			//
			// Get a line
			//
			CString aaa = strbuff.SpanExcluding("\n");
			//
			// Find a header
			//
			pos = aaa.Find(">>");
			if(pos > 0)
			{
				CString header;
				header = aaa.Left(pos);
				aaa = aaa.Right(aaa.GetLength() - pos-2);
				img = GetImageId(header);
			}
			//
			// Insert it into the list of messages
			//
			if(aaa.GetLength() > 0)
			{
                CString bbb = aaa.SpanExcluding("||");
			    int ic = m_ListLog.GetItemCount();
			    m_ListLog.InsertItem(ic , bbb, img);
                // For now let's do it once...
		        int subi = aaa.Find("||",0);
		        if(subi >= 0)
		        {
			        bbb = aaa.Right(aaa.GetLength() - subi - 2);
                    m_ListLog.SetItemText(ic, 1, bbb);
		        }
			}
			//
			// Go to next one.
			//
			pos = strbuff.Find("\n",0);
			if(pos >= 0)
			{
				aaa = strbuff.Right(strbuff.GetLength() - pos-1);
				strbuff = aaa;
			}
		} while(pos >= 0);
		m_ListLog.EnsureVisible(m_ListLog.GetItemCount()-1, TRUE);

	}
	// tell the connected readers that data changed. Set bSendEvent=false if you decide to send this
	// method by your own.
	if(bSendEvent)
		lpTo->flush();
	return false;
}
#endif
/////////////////////////////////////////////////////////////////////////////
// CLogWnd

//I MPLEMENT_DYNCREATE(CLogWnd, CDialog)

CLogWnd::CLogWnd() :
	CDialog(CLogWnd::IDD, NULL)
#ifdef USEPLUGS
	,m_pluglogstr(PLUG_STRING, 0, "log", &m_lastlogstr, this)
#endif
{
	//
	// because these aren't dynamicaly allocated
	//
#ifdef USEPLUGS
	m_pluglogstr.setDesc("plug to ...");
#endif

	IMPLSMARTREFINIT
	//{{AFX_DATA_INIT(CDlgProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CLogWnd::~CLogWnd()
{
}


BEGIN_MESSAGE_MAP(CLogWnd, CDialog)
	//{{AFX_MSG_MAP(CLogWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
    ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LISTLOGGING, &CLogWnd::OnLvnItemActivateListlogging)
    ON_NOTIFY(HDN_ITEMCLICK, 0, &CLogWnd::OnHdnItemclickListlogging)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_LISTLOGGING, &CLogWnd::OnLvnColumnclickListlogging)
    ON_NOTIFY(NM_CLICK, IDC_LISTLOGGING, &CLogWnd::OnNMClickListlogging)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTLOGGING, &CLogWnd::OnLvnItemchangedListlogging)
END_MESSAGE_MAP()

IWindowLog* CLogWnd::Clear()
{
	m_ListLog.DeleteAllItems();
	return this;
}
/**
 ** return an index depending on what is the string
 **/
static int GetImageId(CString &s)
{
	s.MakeLower();
	if(s == "warning")
	{
		return 1;
	}
	else if(s == "error")
	{
		return 2;
	}
	else if(s == "note")
	{
		return 3;
	}
	else if(s == "net")
	{
		return 4;
	}
	else if(s == "no")
	{
		return 5;
	}
	else if(s == "event")
	{
		return 6;
	}
	else if(s == "yes")
	{
		return 7;
	}
	return 0;
}
/**
 **
 **/
IWindowLog* CLogWnd::AddMessage(int level, LPCSTR fmt, ...)
{
	//TODO: Add a way to free this up, later !
	static char *fmt2 = NULL;
	static size_t fmt2_sz = 0;
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
	CString strbuff(fmt2);
	int pos;
	do
	{
		//
		// Get a line
		//
		CString aaa = strbuff.SpanExcluding("\n");
		//
		// Insert it into the list of messages
		//
		if(aaa.GetLength() > 0)
		{
            CString bbb = aaa.SpanExcluding("||");
			int ic = m_ListLog.GetItemCount();
			m_ListLog.InsertItem(ic , bbb, level);
            // For now let's do it once...
		    int subi = aaa.Find("||",0);
		    if(subi >= 0)
		    {
			    bbb = aaa.Right(aaa.GetLength() - subi - 2);
                m_ListLog.SetItemText(ic, 1, bbb);
		    }
		}
		//
		// Go to next one.
		//
		pos = strbuff.Find("\n",0);
		if(pos >= 0)
		{
			aaa = strbuff.Right(strbuff.GetLength() - pos-1);
			strbuff = aaa;
		}
	} while(pos >= 0);
	m_ListLog.EnsureVisible(m_ListLog.GetItemCount()-1, TRUE);
#ifdef USEPLUGS
	m_pluglogstr.flush();
#endif
	//PeekMyself();
	return this;
}
IWindowLog* CLogWnd::AddMessage(LPCSTR fmt, ...)
{
	//TODO: Add a way to free this up, later !
	static char *fmt2 = NULL;
	static size_t fmt2_sz = 0;
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

#ifdef USEPLUGS
	m_pluglogstr.setValue(fmt2);
#else
	m_lastlogstr = fmt2;
	CString strbuff(m_lastlogstr.c_str());
	int pos, img;
	do
	{
		img = 0;
		//
		// Get a line
		//
		CString aaa = strbuff.SpanExcluding("\n");
		//
		// Find a header
		//
		pos = aaa.Find(">>");
		if(pos > 0)
		{
			CString header;
			header = aaa.Left(pos);
			aaa = aaa.Right(aaa.GetLength() - pos-2);
			img = GetImageId(header);
		}
		//
		// Insert it into the list of messages
		//
		if(aaa.GetLength() > 0)
		{
            CString bbb = aaa.SpanExcluding("||");
		    int ic = m_ListLog.GetItemCount();
		    m_ListLog.InsertItem(ic , bbb, img);
            // For now let's do it once...
	        int subi = aaa.Find("||",0);
	        if(subi >= 0)
	        {
		        bbb = aaa.Right(aaa.GetLength() - subi - 2);
                m_ListLog.SetItemText(ic, 1, bbb);
	        }
		}
		//
		// Go to next one.
		//
		pos = strbuff.Find("\n",0);
		if(pos >= 0)
		{
			aaa = strbuff.Right(strbuff.GetLength() - pos-1);
			strbuff = aaa;
		}
	} while(pos >= 0);
	m_ListLog.EnsureVisible(m_ListLog.GetItemCount()-1, TRUE);
#endif
    va_end(vlist);

	//PeekMyself();
	return this;
}
/////////////////////////////////////////////////////////////////////////////
// CLogWnd message handlers

int CLogWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}
void CLogWnd::BuildColumns(int nCols, int * nWidth, int * iCol)
{
	ASSERT(nCols);
	ASSERT(nWidth);
	ASSERT(iCol);
	ASSERT(m_ListLog);

	//insert columns
	int i;
	LV_COLUMN lvc;

    numCols = nCols;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	CString	strTemp;
	for(i = 0; i < nCols; i++)
	{
		lvc.iSubItem = i;
		strTemp.LoadString(iCol[i]);
		lvc.pszText = (char*)(LPCTSTR)strTemp;
		lvc.cx = nWidth[i];
		lvc.fmt = LVCFMT_LEFT;
		m_ListLog.InsertColumn(i,&lvc);
	}
}

void CLogWnd::OnClose() 
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

void CLogWnd::OnSize(UINT nType, int cx, int cy) 
{
	CRect rc;
	CDialog::OnSize(nType, cx, cy);
	GetClientRect(&rc);
	if(m_ListLog.m_hWnd) m_ListLog.SetWindowPos(NULL,rc.left,rc.top, rc.right, rc.bottom ,SWP_NOACTIVATE);

    int w = (rc.right - rc.left);
    for(int i=1; i<numCols; i++)
    {
        LV_COLUMN lvc;
        lvc.mask = LVCF_WIDTH;
        m_ListLog.GetColumn(i, &lvc);
        w -= lvc.cx;
    }
    m_ListLog.SetColumnWidth(0, w);
}

BOOL CLogWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	BOOL bRes = m_imglist.Create(IDB_LOGIMG,16,1,0xFFFFFF);
	m_ListLog.SetImageList(&m_imglist, LVSIL_SMALL);

	m_ListLog.ModifyStyleEx(0, WS_EX_STATICEDGE);
    m_ListLog.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	BuildColumns(1, _gColSize, _gColText);

	/*m_ListLog.InsertItem(0, "Some sample text.");
	m_ListLog.SetItem(0,1,LVIF_TEXT,"More text.",
		0,LVIF_TEXT,LVIF_TEXT,0);*/
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogWnd::OnLvnItemActivateListlogging(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    //m_ListLog.EditLabel(pNMIA->iItem);
	for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
	{
	if(m_pclients[ic])
		  m_pclients[ic]->ListBoxSelection(this, pNMIA->iItem, pNMIA->iItem, true);
	}
	for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
	{
		if(g_WindowHandler.m_pclients[ic])
			  g_WindowHandler.m_pclients[ic]->ListBoxSelection(this, pNMIA->iItem, pNMIA->iItem, true);
	}
    *pResult = 0;
}

void CLogWnd::OnHdnItemclickListlogging(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    *pResult = 0;
}

void CLogWnd::OnLvnColumnclickListlogging(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    *pResult = 0;
}

void CLogWnd::OnNMClickListlogging(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	for(unsigned int ic = 0; ic < m_pclients.size(); ic++)
	{
	if(m_pclients[ic])
		  m_pclients[ic]->ListBoxSelection(this, pNMItemActivate->iItem, pNMItemActivate->iItem, false);
	}
	for(unsigned int ic = 0; ic < g_WindowHandler.m_pclients.size(); ic++)
	{
		if(g_WindowHandler.m_pclients[ic])
			  g_WindowHandler.m_pclients[ic]->ListBoxSelection(this, pNMItemActivate->iItem, pNMItemActivate->iItem, false);
	}

    *pResult = 0;
}

void CLogWnd::OnLvnItemchangedListlogging(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
}

IWindowLog* CLogWnd::AddColumns(int n, int* psz, LPSTR *pstr)
{
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    int w;
    if((psz == NULL) && (numCols == 1))
    {
        RECT rc;
        GetClientRect(&rc);
        w = (rc.right - rc.left)/(numCols + n);
        if(w < 10) w = 10;
        m_ListLog.SetColumnWidth(0, w);
    }
	for(int i = 0; i < n; i++)
	{
		lvc.iSubItem = numCols + i;
		lvc.pszText = "...";
        if(pstr && pstr[i])
		    lvc.pszText = pstr[i];
		lvc.cx = w;
        if(psz)
		    lvc.cx = psz[i];
		lvc.fmt = LVCFMT_LEFT;
		m_ListLog.InsertColumn(numCols + i,&lvc);
	}
    numCols += n;
    return this;
}
IWindowLog* CLogWnd::DelColumns(int n=1)
{
    assert(!"TODO");
    return this;
}
int         CLogWnd::GetNumColumns()
{
    return numCols;
}
IWindowLog* CLogWnd::SetColumSize(int c, int sz)
{
    m_ListLog.SetColumnWidth(c, sz);
    return this;
}
int         CLogWnd::GetNumItems()
{
    int ic = m_ListLog.GetItemCount();
    return ic;
}

IWindow* CLogWnd::SetTitle(LPCSTR title)
{
    LV_COLUMN lvc;
    lvc.mask = LVCF_TEXT;
    lvc.pszText = (LPSTR)title;
    m_ListLog.SetColumn(0,&lvc);
    return this;
}
IWindow* CLogWnd::GetTitle(char * title, int maxlen)
{
    LV_COLUMN lvc;
    lvc.mask = LVCF_TEXT;
    m_ListLog.GetColumn(0, &lvc);
    if(lvc.pszText)
        strncpy(title, lvc.pszText, maxlen);
    return this;
}


