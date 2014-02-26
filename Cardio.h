/**
 ** \file Cardio.h graphics

 **/

#if !defined(AFX_VPCardio_H__6EB02697_C83D_479E_AA50_17B0C1528A06__INCLUDED_)
#define AFX_VPCardio_H__6EB02697_C83D_479E_AA50_17B0C1528A06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "nv_ISvcUI.h"
#include "IWindowImplementation.h"
#pragma warning(disable : 4786)
#include <map>

/**
 **/
class CCardio :
	public IWindowImpl<CCardio>, 
	public CDialog,
	public ICardio, 
	public PlugContainer//, public ISmartRef
{
    IMPLSMARTREF();
    BEGINQUERYINTERFACE();
		QIMAPITF(ICardio)
		QIMAPITF(IWindow)
    ENDQUERYINTERFACE();
public:
	/// \name Factory stuff
	//@{
	DECL_OBJECTDESC_VARIABLE();
	static const void *NewObjectFunc(unsigned long lparam1, unsigned long lparam2); // DECL_NEWOBJECTFUNC(CCardio, ICardio);
	//@}
	CCardio();

	CWnd *m_pwnd;
protected:
	//-----> GDI objects
	HFONT	hfont;
	HPEN	hpen_tline;
	HBRUSH hbrush_bgnd;
	//-----> Bitmaps that contains the rendered graphics
	HBITMAP hbitmap[2];	// area of work
	int bmpW, bmpH;			// size of this area
	HDC bmpDC[2];		// Compatible Device context used to draw
	HDC *lpbmpDC[2];
	//-----> Backup of previous stuff (SelectObject())
	HGDIOBJ oldbrush;
	HGDIOBJ oldpen;
	HGDIOBJ oldbmp[2];
	HGDIOBJ oldfont;
	//----->
	int m_ForceRefresh;
	float m_XFreq; ///< frequency used to translate the bitmap.
	float m_YMin, m_YMax, m_YScale, m_YOffset;
	typedef struct
	{
		float y, prevy;
		COLORREF color;
	} s_pencil;
	typedef std::map<int,s_pencil> t_pencils;
	t_pencils m_pencils;
public:

// Dialog Data
	//{{AFX_DATA(CCardio)
	enum { IDD = IDD_CARDIO };
	//}}AFX_DATA
	/// \name MFC Wizard created
	//@{
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCardio)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	//@}
// Implementation
public:
	virtual ~CCardio();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	DECLARE_IWINDOW_IMPL(CCardio)
// Generated message map functions
protected:
	void DrawScene();
	void RefreshRect(RECT *rc=NULL);
	void ResizeBitmap(int w, int h);
	/// \name MFC Wizard created messages
	//@{
	//{{AFX_MSG(CCardio)
	afx_msg void OnFileClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	//@}
	DECLARE_MESSAGE_MAP()
	/// \name From the interface ICardio
	//@{
	virtual void SetPencil(int id, float y, unsigned long color);
	virtual void DelPencil(int id);
	virtual void Update(float dt);
	virtual void SetYBounds(float ymin, float ymax);
	virtual void *GetPlug(int n) { return NULL; }
	//@}
  virtual bool plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent); // from PlugContainer
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VPTLineFrm_H__6EB02697_C83D_479E_AA50_17B0C1528A06__INCLUDED_)
