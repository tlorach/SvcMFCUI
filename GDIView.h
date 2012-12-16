#if !defined(AFX_GDIVIEW_H__5E52E72C_3CCC_4E61_AF60_72E383CE5B2F__INCLUDED_)
#define AFX_GDIVIEW_H__5E52E72C_3CCC_4E61_AF60_72E383CE5B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IWindowImplementation.h"

/////////////////////////////////////////////////////////////////////////////
// GDIView view

class GDIView : public IWindowImpl<GDIView>, public CScrollView, public IGDIView, public ISmartRef
{
	float m_zoom;
	int m_gdiwidth, m_gdiheight;
	void FillOutsideRect(CDC* pDC, CBrush* pBrush);
public:
	CWnd *m_pwnd;
	/// \name Factory stuff
	//@{
	DECL_OBJECTDESC_VARIABLE();
	static const void *NewObjectFunc(unsigned long lparam1, unsigned long lparam2); // DECL_NEWOBJECTFUNC(GDIView, IGDIView);
	//@}
    IMPLSMARTREF();
    BEGINQUERYINTERFACE();
		QIMAPITF(IGDIView)
		QIMAPITF(IWindow)
    ENDQUERYINTERFACE();
	GDIView(CWnd *parent=NULL);           // protected constructor used by dynamic creation
	virtual ~GDIView();
	DECLARE_DYNCREATE(GDIView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GDIView)
	public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Generated message map functions
	//{{AFX_MSG(GDIView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_IWINDOW_IMPL(GDIView)
	/// \name from IWindowContainer
	//@{
	virtual void ShowSliders(bool bYes=true);
	virtual void SetZoom(float zoom=1);
	virtual void SetOffset(float x=0, float y=0);
	virtual void SetViewFrame(float x1, float y1, float x2, float y2);
	virtual void *GetPlug(int n) { return NULL; }
	//@}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDIVIEW_H__5E52E72C_3CCC_4E61_AF60_72E383CE5B2F__INCLUDED_)
