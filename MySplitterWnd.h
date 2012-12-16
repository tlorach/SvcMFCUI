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
#if !defined(AFX_MYSPLITTERWND_H__F1624841_C017_465D_A162_01D5A25952A3__INCLUDED_)
#define AFX_MYSPLITTERWND_H__F1624841_C017_465D_A162_01D5A25952A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySplitterWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySplitterWnd frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define WASACVIEW CWnd

class CMySplitterWnd : public CWnd
{
	DECLARE_DYNAMIC(CMySplitterWnd)

// Construction
public:
	CMySplitterWnd();
	// Create a single view type splitter with multiple splits
	BOOL Create(CWnd* pParentWnd,
				int nMaxRows, int nMaxCols, SIZE sizeMin,
				CCreateContext* pContext,
				DWORD dwStyle = WS_CHILD | WS_VISIBLE |
					WS_HSCROLL | WS_VSCROLL | SPLS_DYNAMIC_SPLIT,
				UINT nID = AFX_IDW_PANE_FIRST);

	// Create a multiple view type splitter with static layout
	BOOL CreateStatic(CWnd* pParentWnd,
				int nRows, int nCols,
				DWORD dwStyle = WS_CHILD | WS_VISIBLE,
				UINT nID = AFX_IDW_PANE_FIRST);

	virtual BOOL CreateView(int row, int col, CRuntimeClass* pViewClass,
			SIZE sizeInit, CCreateContext* pContext);

// Attributes
public:
	int GetRowCount() const {return m_nRows; };
	int GetColumnCount() const {return m_nCols;};

	// information about a specific row or column
	void GetRowInfo(int row, int& cyCur, int& cyMin) const;
	void SetRowInfo(int row, int cyIdeal, int cyMin);
	void GetColumnInfo(int col, int& cxCur, int& cxMin) const;
	void SetColumnInfo(int col, int cxIdeal, int cxMin);

	// for setting and getting shared scroll bar style
	DWORD GetScrollStyle() const;
	void SetScrollStyle(DWORD dwStyle);

	// views inside the splitter
	CWnd* GetPane(int row, int col) const;
	BOOL IsChildPane(CWnd* pWnd, int* pRow, int* pCol);
	BOOL IsChildPane(CWnd* pWnd, int& row, int& col); // obsolete
	int IdFromRowCol(int row, int col) const;

	BOOL IsTracking();  // TRUE during split operation

// Operations
public:
	virtual void RecalcLayout();	// call after changing sizes

// Overridables
protected:
	// to customize the drawing
	enum ESplitType { splitBox, splitBar, splitIntersection, splitBorder };
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
	virtual void OnInvertTracker(const CRect& rect);

public:
	// for customizing scrollbar regions
	virtual BOOL CreateScrollBarCtrl(DWORD dwStyle, UINT nID);

	// for customizing DYNAMIC_SPLIT behavior
	virtual BOOL SplitRow(int cyBefore);
	virtual BOOL SplitColumn(int cxBefore);
	virtual void DeleteRow(int rowDelete);
	virtual void DeleteColumn(int colDelete);

	// determining active pane from focus or active view in frame
	virtual CWnd* GetActivePane(int* pRow = NULL, int* pCol = NULL);
	virtual void SetActivePane(int row, int col, CWnd* pWnd = NULL);
protected:
	CWnd* GetActivePane(int& row, int& col); // obsolete

public:
	// high level command operations - called by default view implementation
	virtual BOOL CanActivateNext(BOOL bPrev = FALSE);
	virtual void ActivateNext(BOOL bPrev = FALSE);
	virtual BOOL DoKeyboardSplit();

// Implementation
public:
	virtual ~CMySplitterWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// implementation structure
	struct CRowColInfo
	{
		int nMinSize;	   // below that try not to show
		int nIdealSize;	 // user set size
		// variable depending on the available size layout
		int nCurSize;	   // 0 => invisible, -1 => nonexistant
	};

protected:
	// customizable implementation attributes (set by constructor or Create)
	CRuntimeClass* m_pDynamicViewClass;
	int m_nMaxRows, m_nMaxCols;

	// implementation attributes which control layout of the splitter
	int m_cxSplitter, m_cySplitter;		 // size of splitter bar
	int m_cxBorderShare, m_cyBorderShare;   // space on either side of splitter
	int m_cxSplitterGap, m_cySplitterGap;   // amount of space between panes
	int m_cxBorder, m_cyBorder;			 // borders in client area

	// current state information
	int m_nRows, m_nCols;
	BOOL m_bHasHScroll, m_bHasVScroll;
	CRowColInfo* m_pColInfo;
	CRowColInfo* m_pRowInfo;

	// Tracking info - only valid when 'm_bTracking' is set
	BOOL m_bTracking, m_bTracking2;
	CPoint m_ptTrackOffset;
	CRect m_rectLimit;
	CRect m_rectTracker, m_rectTracker2;
	int m_htTrack;

	// implementation routines
	BOOL CreateCommon(CWnd* pParentWnd, SIZE sizeMin, DWORD dwStyle, UINT nID);
	virtual int HitTest(CPoint pt) const;
	virtual void GetInsideRect(CRect& rect) const;
	virtual void GetHitRect(int ht, CRect& rect);
public:
	virtual void TrackRowSize(int y, int row);
	virtual void TrackColumnSize(int x, int col);
protected:
	virtual void DrawAllSplitBars(CDC* pDC, int cxInside, int cyInside);
	virtual void SetSplitCursor(int ht);
	CWnd* GetSizingParent();

	// starting and stopping tracking
	virtual void StartTracking(int ht);
	virtual void StopTracking(BOOL bAccept);

	// special command routing to frame
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//{{AFX_MSG(CMySplitterWnd)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
	afx_msg void OnCancelMode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDisplayChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//{{AFX_VIRTUAL(CMySplitterWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_MYSPLITTERWND_H__F1624841_C017_465D_A162_01D5A25952A3__INCLUDED_)
