/**
\file Cardio.cpp Graphics

 **/

#include "stdafx.h"
#include "nv_SvcMFCUI.h"
#include "Cardio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 ** Instanciation of an object.
 **/ 
const void * CCardio::NewObjectFunc(unsigned long lparam1, unsigned long lparam2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CCardio *pDlg = new CCardio;
	CWnd *pOwner;
	if(lparam1) pOwner = CWnd::FromHandle((HWND)lparam1);
	else pOwner = &g_WindowHandler;
	BOOL bOK = pDlg->Create(IDD_CARDIO, pOwner);
	if(bOK)
	{
		CWnd *pW = pDlg->GetParent();
		//pDlg->ShowWindow(SW_SHOWNOACTIVATE);
		//pDlg->RedrawWindow();
		//pDlg->PeekMyself();
		return (void*)(static_cast<ICardio*>(pDlg));
	}
	delete pDlg;
	return NULL;
}
/////////////////////////////////////////////////////////////////////////////
// CCardio construction/destruction

CCardio::CCardio() :
	CDialog(CCardio::IDD, NULL),
	m_XFreq(0),
	m_YMin(-1),
	m_YMax(1),
	m_YScale(1), 
	m_YOffset(0)
{
	IMPLSMARTREFINIT
	//{{AFX_DATA_INIT(CCardio)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCardio::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCardio)
	//}}AFX_DATA_MAP
}

CCardio::~CCardio()
{
}
void CCardio::ResizeBitmap(int w, int h)
{
	if(hbitmap[0])
	{
		oldbmp[0] = SelectObject(bmpDC[0], oldbmp[0]);
		DeleteObject(hbitmap[0]);
	}
	if(hbitmap[1])
	{
		oldbmp[1] = SelectObject(bmpDC[1], oldbmp[1]);
		DeleteObject(hbitmap[1]);
	}
	HDC dc = ::GetDC(NULL);
	hbitmap[0] = CreateCompatibleBitmap(dc, w,h);
	hbitmap[1] = CreateCompatibleBitmap(dc, w,h);
	bmpW = w;
	bmpH = h;
	::ReleaseDC(NULL, dc);
	oldbmp[0] = SelectObject(bmpDC[0], hbitmap[0]);
	oldbmp[1] = SelectObject(bmpDC[1], hbitmap[1]);
	lpbmpDC[0] = &(bmpDC[0]);//bmpDC;
	lpbmpDC[1] = &(bmpDC[1]);//bmpDC+1;
	RefreshRect();
}

void CCardio::DrawScene()
{
	CDC cdc;
	RECT rc;
	BOOL bRes;
	cdc.Attach(*(lpbmpDC[0]));
	// Shift the bitmap
	if(m_XFreq >= 1)
	{
		bRes = BitBlt(*lpbmpDC[1], 0,0,bmpW-1, bmpH, *lpbmpDC[0], 1,0, SRCCOPY);
		bRes = BitBlt(*lpbmpDC[0], 0,0,bmpW, bmpH, *lpbmpDC[1], 0,0, SRCCOPY);
	}
	// Draw pencils
	rc.bottom = bmpH;
	rc.top = 0; 
	rc.left = bmpW-1;
	rc.right = bmpW;
	cdc.FillSolidRect( &rc, (COLORREF)0x301010);//iP->second.color);
	t_pencils::iterator iP;
	for(iP = m_pencils.begin(); iP != m_pencils.end(); iP++)
	{
		rc.bottom = (int)((iP->second.y + m_YOffset) * m_YScale) + bmpH/2;
		rc.top = (int)((iP->second.prevy + m_YOffset) * m_YScale) + bmpH/2; 
		if(rc.bottom > rc.top)
		{
			rc.bottom += 1;
			rc.top -= 1;
		}
		else
		{
			rc.bottom -= 1;
			rc.top += 1;
		}
		rc.left = bmpW-2;
		rc.right = bmpW;
		iP->second.prevy = iP->second.y;
		cdc.FillSolidRect( &rc, iP->second.color);
	}
	cdc.Detach();
}
/**
 ** If the pencil doesn't exist, we create it.
 **/
void CCardio::SetPencil(int id, float y, unsigned long color)
{
	s_pencil &Pencil = m_pencils[id];
	Pencil.color = color;
//	Pencil.prevy = Pencil.y;
	Pencil.y = -y;
	m_ForceRefresh++;
}
/**
 ** 
 **/
void CCardio::DelPencil(int id)
{
}
/**
 ** 
 **/
void CCardio::Update(float dt)
{
	m_XFreq += dt;
	if((m_XFreq < 1))//&&(m_ForceRefresh==0))
		return;

	RefreshRect();
	//if(m_ForceRefresh == 0)
		m_XFreq -= 1.0f;
	//m_ForceRefresh = 0;
}
void CCardio::SetYBounds(float ymin, float ymax)
{
	if(ymin < ymax)
	{
		m_YMin = ymin;
		m_YMax = ymax;
	}
	else
	{
		m_YMin = ymax;
		m_YMax = ymin;
	}
	m_YOffset = (m_YMax + m_YMin)/2.0f;
	m_YScale = bmpH/((m_YMax - m_YMin));
}
/////////////////////////////////////////////////////////////////////////////
// CCardio diagnostics

#ifdef _DEBUG
void CCardio::AssertValid() const
{
	CDialog::AssertValid();
}

void CCardio::Dump(CDumpContext& dc) const
{
	CDialog::Dump(dc);
}

#endif //_DEBUG

/**
 * Closing Event.
 **/
void CCardio::OnFileClose()
{
	// To close the frame, just send a WM_CLOSE, which is the equivalent
	// choosing close from the system menu.

	SendMessage(WM_CLOSE);
}

/////////////////////////////////////////////////////////////////////////////
// CCardio

BEGIN_MESSAGE_MAP(CCardio, CDialog)
	//{{AFX_MSG_MAP(CCardio)
		ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/****************************************************************************/
/**
 **
 **/
int CCardio::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	//====> Timeline parms
	//
	//
	//----> Device ctxt
	//
	CDC *hdc = GetDC();
	bmpDC[0] = CreateCompatibleDC(hdc->m_hDC);
	bmpDC[1] = CreateCompatibleDC(hdc->m_hDC);
	ReleaseDC(hdc);
	hbitmap[0] = NULL;
	hbitmap[1] = NULL;
	oldbmp[0] = oldbmp[1] = NULL;
	COLORREF color;
	//OleTranslateColor(m_clrBackColor, NULL, &color);
	color = RGB(180,180,200);
	hbrush_bgnd = CreateSolidBrush(color);
	color = RGB(10,100,200);
	hpen_tline = CreatePen(PS_SOLID, 0, color);
	//
	//----> Font
	//
	hfont = CreateFont(
		-MulDiv(8, GetDeviceCaps(bmpDC[0], LOGPIXELSY), 72),	// logical height of font 
		0,	// logical average character width 
		0,	// angle of escapement 
		0,	// base-line orientation angle 
		FW_NORMAL,	// font weight FW_BOLD
		FALSE,	// italic attribute flag 
		FALSE,	// underline attribute flag 
		FALSE,	// strikeout attribute flag 
		DEFAULT_CHARSET,	// character set identifier 
		OUT_TT_ONLY_PRECIS,	// output precision 
		CLIP_DEFAULT_PRECIS,	// clipping precision 
		PROOF_QUALITY,	// output quality 
		DEFAULT_PITCH,	// pitch and family 
		"Courrier"	// pointer to typeface name string 
	   );
	return 0;
}

void CCardio::OnDestroy()
{
	CDialog::OnDestroy();
	oldbmp[0] = SelectObject(bmpDC[0], oldbmp[0]);
	oldbmp[1] = SelectObject(bmpDC[1], oldbmp[1]);
	oldpen = SelectObject(bmpDC[0], oldpen);
	oldbrush = SelectObject(bmpDC[0], oldbrush);
	oldfont = SelectObject(bmpDC[0], oldfont);
	//
	//----> Delete objects
	//
	//DeleteObject(hbitmap);
	DeleteObject(hbrush_bgnd);
	//DeleteObject(hpen_tline);
	//DeleteObject(hpen_arrows);
	DeleteObject(hfont);
	DeleteDC(bmpDC[0]);
	DeleteDC(bmpDC[1]);

	for(int ic = 0; ic < m_pclients.size(); ic++)
		m_pclients[ic]->rtWindowDestroyed(static_cast<IWindow*>(this));
	
}

void CCardio::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	m_YScale = bmpH/((m_YMax - m_YMin));
	ResizeBitmap(cx, cy);
}

void CCardio::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	int w = dc.m_ps.rcPaint.right-dc.m_ps.rcPaint.left;
	int h = dc.m_ps.rcPaint.bottom-dc.m_ps.rcPaint.top;
	if(w >= dc.m_ps.rcPaint.right)
		w = dc.m_ps.rcPaint.right-1;
	if(h >= dc.m_ps.rcPaint.bottom)
		h = dc.m_ps.rcPaint.bottom-1;
	BOOL bRes = BitBlt(dc, 
		dc.m_ps.rcPaint.left,
		dc.m_ps.rcPaint.top,
		w, h,
		*lpbmpDC[0], 
		dc.m_ps.rcPaint.left,
		dc.m_ps.rcPaint.top,
		SRCCOPY);
}
BOOL CCardio::OnEraseBkgnd(CDC* pDC) 
{
	return 1;
}

void CCardio::OnMouseMove(UINT nFlags, CPoint point) 
{
	//
	// Left button
	//
	if(nFlags & MK_LBUTTON)
	{
	}
	//
	// right button
	//
	else if(nFlags & MK_RBUTTON)
	{
	}
	//CDialog::OnMouseMove(nFlags, point);
}


void CCardio::RefreshRect(RECT *rc)
{
	RECT Rect = {0,0, bmpW, bmpH};
	DrawScene();
	if(!rc)
		rc = &Rect;
	InvalidateRect(rc, FALSE);
	UpdateWindow();
}

void CCardio::OnClose() 
{
	//
	// Send the event
	//
	for(int ic = 0; ic < m_pclients.size(); ic++)
	{
		// HACK : This forces this object to not be deleted inside an event.
		AddRef(); 
		m_pclients[ic]->rtWindowClosed(static_cast<IWindow*>(this));
		Release(); 
	}
	//CDialog::OnClose();
}


 */