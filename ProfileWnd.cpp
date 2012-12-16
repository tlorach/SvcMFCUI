
#include "stdafx.h"
#include "nv_SvcMFCUI.h"
#include "ProfileWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 ** Instanciation of an object.
 **/ 
const void * CProfileWnd::NewObjectFunc(unsigned long lparam1, unsigned long lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CProfileWnd *pDlg = new CProfileWnd;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_PROFILING, pOwner);
	if(bOK)
	{
		CWnd *pW = pDlg->GetParent();
		//pDlg->ShowWindow(SW_SHOWNOACTIVATE);
		//pDlg->RedrawWindow();
		//pDlg->PeekMyself();
		return (void*)(static_cast<IDlgClientProfiling*>(pDlg));
	}
	delete pDlg;
	return NULL;
}
/////////////////////////////////////////////////////////////////////////////
// CProfileWnd

CProfileWnd::CProfileWnd() :
	CDialog(CProfileWnd::IDD, NULL)
{
	IMPLSMARTREFINIT
	curline = 0;
}

CProfileWnd::~CProfileWnd()
{
}

void CProfileWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfileWnd)
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_FPS, m_fps);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProfileWnd, CDialog)
	//{{AFX_MSG_MAP(CProfileWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnLvnItemchangedList)
END_MESSAGE_MAP()

void CProfileWnd::Printf(char* fmt, ... )
{
	va_list  vlist;
	char buff[1024];

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// Get output string
	va_start(vlist, fmt);
	vsprintf(buff, fmt, vlist);
//...

	va_end(vlist);
}
void CProfileWnd::Clear()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//m_list.DeleteAllItems();
	int n= m_list.GetItemCount();
	while(curline<n)
	{
		m_list.DeleteItem(curline);
		n--;
	}
	curline = 0;
}
#define SETCOL(nn,str)\
	w = m_list.GetStringWidth(str);\
	w2 = m_list.GetColumnWidth(nn);\
	if( w2 < w)\
		m_list.SetColumnWidth(nn, w*2);
void CProfileWnd::AddLine(LPCSTR ave, LPCSTR min, LPCSTR max, LPCSTR name, LPCSTR card)
{
	int n;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	n= m_list.GetItemCount();
	if(curline >= n)
	{
		m_list.InsertItem(curline, ave);
	}
	else
		m_list.SetItemText(curline, 0, ave);
	m_list.SetItemText(curline, 1, min);
	m_list.SetItemText(curline, 2, max);
	m_list.SetItemText(curline, 3, card);
	m_list.SetItemText(curline, 4, name);

	int w,w2;
	SETCOL(0,ave)
	SETCOL(1,min)
	SETCOL(2,max)
	SETCOL(3,card)
	SETCOL(4,name)
	curline++;
}
void CProfileWnd::SetFPS(int fps)
{
	char tmp[10];
	sprintf(tmp, "%d fps",fps);
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_fps.SetWindowText(tmp);
}
/////////////////////////////////////////////////////////////////////////////
// CProfileWnd message handlers

int CProfileWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CProfileWnd::OnSize(UINT nType, int cx, int cy) 
{
	CRect rc;
	CDialog::OnSize(nType, cx, cy);

	GetClientRect(&rc);
	if(m_list.m_hWnd) m_list.SetWindowPos(NULL,rc.left,rc.top+30, rc.right, rc.bottom ,SWP_NOACTIVATE);
	//if(m_fps.m_hWnd) m_fps.SetWindowPos(NULL,rc.left,rc.top, rc.right, rc.top+30 ,SWP_NOACTIVATE);
	
}

BOOL CProfileWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_list.ModifyStyleEx(0, WS_EX_STATICEDGE);

	m_list.InsertColumn(0,"name", LVCFMT_LEFT, 120);
	m_list.InsertColumn(0,"#", LVCFMT_LEFT, 10);
	m_list.InsertColumn(0,"Max", LVCFMT_LEFT, 65);
	m_list.InsertColumn(0,"Min", LVCFMT_LEFT, 65);
	m_list.InsertColumn(0,"Ave", LVCFMT_LEFT, 5);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProfileWnd::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
