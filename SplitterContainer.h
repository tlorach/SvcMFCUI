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
#if !defined(AFX_SPLITTERCONTAINER_H__61C6794E_9AA4_46E0_9617_FEDD72CF38E3__INCLUDED_)
#define AFX_SPLITTERCONTAINER_H__61C6794E_9AA4_46E0_9617_FEDD72CF38E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplitterContainer.h : header file
//
#include "MySplitterWnd.h"

#include "ISvcUI.h"
#include "IWindowImplementation.h"
using namespace std;

#define MAXITEMSROW 10
#define MAXITEMSCOL 10

/////////////////////////////////////////////////////////////////////////////
// SplitterContainer dialog

class SplitterContainer : public IWindowImpl<SplitterContainer>, public CDialog, public IWindowSplitter //, public ISmartRef
{
// Construction
public:
	/// \name Factory stuff
	//@{
	DECL_OBJECTDESC_VARIABLE();
	static const void *NewObjectFunc(size_t lparam1, size_t lparam2); // DECL_NEWOBJECTFUNC(CTabWnd, IDlgLog);
	static const void *NewObjectFuncH(size_t lparam1, size_t lparam2);
	//@}
	IMPLSMARTREF();
	BEGINQUERYINTERFACE();
		QIMAPITF(IWindowContainer)	
		QIMAPITF(IWindowSplitter)	
		QIMAPITF(IWindow)
	ENDQUERYINTERFACE();
	virtual ~SplitterContainer();

	SplitterContainer(CWnd* pParent = NULL);   // standard constructor

	/// Structure to keep track of sub windows we put into this splitter
	struct TWindows
	{
		TWindows() : itf((void*)NULL), cwnd(0) {}
		TWindows(IWindow *_itf, CWnd *_cwnd) : itf(_itf), cwnd(_cwnd) {}
		SmartPtr<IWindow>	itf;
		CWnd				*cwnd;
		int	def_w, def_h;
	};
	TWindows		m_controls[MAXITEMSROW][MAXITEMSCOL];
	int				rows, cols;
	bool			m_bHorizontalMode;
	CMySplitterWnd	m_splitter;
// Dialog Data
	//{{AFX_DATA(SplitterContainer)
	enum { IDD = IDD_SPLITTERCONTAINER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SplitterContainer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SplitterContainer)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_IWINDOW_IMPL(SplitterContainer) // Here we tell IWindow interface implementation is done by IWindowImpl class
	virtual IWindowContainer	*AsWindowContainer(){ return NULL; };
    virtual IWindowFolding	*AsWindowFolding(){ return NULL; };
	virtual IWindowConsole	  *AsWindowConsole()  { return NULL; };
	virtual IWindowHandler	  *AsWindowHandler()  { return NULL; };
	virtual IWindowLog		  *AsWindowLog()	  { return NULL; };
	virtual IWindowSplitter	 *AsWindowSplitter() { return this; };
	virtual IProgressBar		*AsProgressBar()	{ return NULL; };
	virtual IControlCombo	   *AsControlCombo()   { return NULL; };
	virtual IControlListBox	 *AsControlListBox() { return NULL; };
	virtual IControlScalar	  *AsControlScalar()  { return NULL; };
	virtual IControlString	  *AsControlString()  { return NULL; };
	virtual IControlLabel	  *AsControlLabel()   { return NULL; };
	virtual IControlToolbar	 *AsControlToolbar() { return NULL; };
	virtual IControlVector	  *AsControlVector()  { return NULL; };
	virtual IControlStatic	  *AsControlStatic()  { return NULL; };
	virtual IControlScalar	  *AsControlBoolean() { return NULL; };
	virtual IControlScalar	  *AsControlCheck()   { return NULL; };
	virtual IWindow				*AsControlButton()  { return NULL; };
	virtual IControlVector	  *AsControlColor()   { return NULL; };
	/// \name From IWindowContainer & IWindowSplitter
	//@{
	virtual int GetNumItems();
	virtual IWindowContainer* AddItem(LPCSTR title, IWindow *lpWnd, bool bUpdateComponents);
	//virtual IWindowContainer* AddItem(LPCSTR title, LPCSTR windowname, bool bUpdateComponents);
	virtual IWindowContainer* RemoveItem(int Itemnum, IWindow **lpWnd=NULL, bool bUpdateComponents=false); 
	virtual IWindowContainer* RemoveItem(IWindow *lpWnd, bool bUpdateComponents);
	virtual IWindowContainer* SelectItem(int Itemnum);
	virtual IWindow*		  GetItemNum(int Itemnum);
	virtual int GetSplitterPos(int row);
	virtual IWindowSplitter* SetSplitterPos(int row, int y);
	virtual void				Refresh();
	//@}
private:
	int addItem_Row(CWnd *pwnd, IWindow *lpWnd);
	int addItem_Col(CWnd *pwnd, IWindow *lpWnd);
};
/*************************************************************************/ /**
Simple downclassing to have another NewObjectFunc() method for horizontal
splitter.
 */ /*********************************************************************/
class SplitterContainerH : public SplitterContainer
{
public:
	DECL_OBJECTDESC_VARIABLE();
	static const void *NewObjectFunc(size_t lparam1, size_t lparam2); // DECL_NEWOBJECTFUNC(CTabWnd, IDlgLog);
	SplitterContainerH(CWnd* pParent = NULL) : SplitterContainer(pParent) {}
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITTERCONTAINER_H__61C6794E_9AA4_46E0_9617_FEDD72CF38E3__INCLUDED_)
