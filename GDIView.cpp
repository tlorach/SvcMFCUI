
#include "stdafx.h"
#include "nv_SvcMFCUI.h"
#include "GDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 ** Instanciation of an object.
 **/ 
const void * GDIView::NewObjectFunc(unsigned long lparam1, unsigned long lparam2)
{
	return NULL;
	/*
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	RECT rc = {0,0,50,50};
	CWnd *pwnd;
	{
		pwnd = new CEmptytMDIChildWnd;
		if (!((CEmptytMDIChildWnd*)pwnd)->Create(NULL, "gdiviewmdiwin",WS_CHILD|WS_VISIBLE|WS_OVERLAPPEDWINDOW))
		{
			delete pwnd;
			return FALSE;
		}
		pwnd->GetClientRect(&rc);
	}
	GDIView *pwndgdi = new GDIView(pwnd);
	if (!pwndgdi->Create(NULL, "gdiview", WS_CHILD|WS_VISIBLE,rc,pwnd,10))
	{
		pwnd->DestroyWindow();
		//delete pwnd;
		delete pwndgdi;
		return FALSE;
	}
	((CEmptytMDIChildWnd*)pwnd)->m_childwnd = pwndgdi;
	//pwnd->PeekMyself();
	return (void*)(static_cast<IGDIView*>(pwndgdi));*/
}
/////////////////////////////////////////////////////////////////////////////
// GDIView

IMPLEMENT_DYNCREATE(GDIView, CScrollView)

GDIView::GDIView(CWnd *parent) :
	m_gdiwidth(200),
	m_gdiheight(200),
	m_pwnd(parent),
	m_zoom(1)
{
	IMPLSMARTREFINIT
}

GDIView::~GDIView()
{
}


BEGIN_MESSAGE_MAP(GDIView, CScrollView)
	//{{AFX_MSG_MAP(GDIView)
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GDIView drawing

void GDIView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
}

void GDIView::OnDraw(CDC* pDC)
{
	RECT rc={0,0,50,50};
	pDC->Ellipse(&rc);
	RECT rc2={0,0,200,200};
	pDC->Ellipse(&rc2);
	CBrush br(HS_BDIAGONAL,0x802020);
	FillOutsideRect(pDC, &br);
}

void GDIView::FillOutsideRect(CDC* pDC, CBrush* pBrush)
{
///	ASSERT_VALID(pDC);
//	ASSERT_VALID(pBrush);

	// fill rect outside the image
	CRect rect;
	GetClientRect(rect);
	rect.right = (int)((float)rect.right * m_zoom);
	rect.bottom = (int)((float)rect.bottom * m_zoom);
	ASSERT(rect.left == 0 && rect.top == 0);
	rect.left = m_gdiwidth;
	if (!rect.IsRectEmpty())
		pDC->FillRect(rect, pBrush);    // vertical strip along the side
	rect.left = 0;
	rect.right = m_gdiwidth;
	rect.top = m_gdiheight;
	if (!rect.IsRectEmpty())
		pDC->FillRect(rect, pBrush);    // horizontal strip along the bottom
}
/////////////////////////////////////////////////////////////////////////////
// GDIView diagnostics

#ifdef _DEBUG
void GDIView::AssertValid() const
{
	CScrollView::AssertValid();
}

void GDIView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// GDIView message handlers

int GDIView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetScrollSizes(MM_TEXT, CSize(m_gdiwidth,m_gdiheight));

	return 0;
}

void GDIView::ShowSliders(bool bYes)
{
}
void GDIView::SetZoom(float zoom)
{
}
void GDIView::SetOffset(float x, float y)
{
}
void GDIView::SetViewFrame(float x1, float y1, float x2, float y2)
{
}

BOOL GDIView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	RECT rc;
	float step = m_zoom * (float)zDelta * 0.001f;
	m_zoom += step;
	GetClientRect(&rc);
	SetScrollSizes(MM_TEXT, CSize((int)((float)m_gdiwidth/m_zoom), (int)((float)m_gdiheight/m_zoom)));
	RedrawWindow();
	return TRUE;
}

void GDIView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
}

void GDIView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CScrollView::OnPrepareDC(pDC, pInfo);
	if(m_zoom != 1)
	{
		pDC->SetMapMode(MM_ANISOTROPIC);
		RECT rc;
		GetClientRect(&rc);
		pDC->SetViewportExt(rc.right, rc.bottom);
		pDC->SetWindowExt((int)(m_zoom*(float)rc.right), (int)(m_zoom*(float)rc.bottom));  // window is in logical coordinates
	}
}
void GDIView::OnClose() 
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
	//CScrollView::OnClose();
}


void GDIView::OnDestroy() 
{
	CScrollView::OnDestroy();
	if(m_pwnd) m_pwnd->DestroyWindow();
	for(int ic = 0; ic < m_pclients.size(); ic++)
		m_pclients[ic]->rtWindowDestroyed(static_cast<IWindow*>(this));
}
