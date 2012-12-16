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
//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_ConsoleWnd_H__5E52E72C_3CCC_4E61_AF60_72E383CE5B2F__INCLUDED_)
#define AFX_ConsoleWnd_H__5E52E72C_3CCC_4E61_AF60_72E383CE5B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IWindowImplementation.h"

#ifdef USEPLUGS
#include "plug/plug.h"
using namespace plug;
#endif
#pragma message("TODO: fix the path of afxcmn.h !!!")
#include <afxcmn.h>
/////////////////////////////////////////////////////////////////////////////
// ConsoleWnd view

class ConsoleWnd : public IWindowImpl<ConsoleWnd>, public CDialog, public IWindowConsole
#ifdef USEPLUGS
    , public PlugContainer
#endif
{
	CHARFORMAT2 cf;
	bool bChangeFromPrintf; ///< dirty hack. Sorry
#ifdef USEPLUGS
	plug::Plug m_pluginput;	///< for the connection with the console through plugs for input
	plug::Plug m_plugoutput;	///< for the connection with the console through plugs for output
#endif
public:
	String m_laststringout;	///< used by the plug to cache the last string for output
	String m_laststringin;	///< used by the plug to cache the last string for input
	CWnd *m_pwnd;
	/// \name Factory stuff
	//@{
	DECL_OBJECTDESC_VARIABLE();
	static const void *NewObjectFunc(size_t lparam1, size_t lparam2); // DECL_NEWOBJECTFUNC(ConsoleWnd, IWindowConsole);
	//@}
	IMPLSMARTREF();
	BEGINQUERYINTERFACE();
		QIMAPITF(IWindowConsole)
		QIMAPITF(IWindow)
	ENDQUERYINTERFACE();
	ConsoleWnd();		   // protected constructor used by dynamic creation
	virtual ~ConsoleWnd();

// Attributes
public:

// Operations
public:

// Dialog Data
	//{{AFX_DATA(ConsoleWnd)
	enum { IDD = IDD_CONSOLE };
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ConsoleWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Generated message map functions
	//{{AFX_MSG(ConsoleWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_IWINDOW_IMPL(ConsoleWnd)
	virtual IWindowContainer	*AsWindowContainer(){ return NULL; };
    virtual IWindowFolding	*AsWindowFolding(){ return NULL; };
	virtual IWindowConsole	  *AsWindowConsole()  { return this; };
	virtual IWindowHandler	  *AsWindowHandler()  { return NULL; };
	virtual IWindowLog		  *AsWindowLog()	  { return NULL; };
	virtual IWindowSplitter	 *AsWindowSplitter() { return NULL; };
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
	/// \name from IWindowConsole
	//@{
	virtual IWindowConsole* SetColor(unsigned long rgbcolor);
	virtual IWindowConsole* SetColor(unsigned char r, unsigned char g, unsigned char b);
	virtual IWindowConsole* SetItalic(bool bYes=true);
	virtual IWindowConsole* SetBold(int val=1);
	virtual IWindowConsole* SetUnderline(bool bYes=true);
	virtual IWindowConsole* SetFontSize(int sz=0);
	virtual IWindowConsole* SetFont(LPCSTR name=NULL);

	virtual IWindowConsole* Clear();
	virtual IWindowConsole* SetCursor(int x, int y);
	virtual IWindowConsole* SetCursorToEnd();
	virtual IWindowConsole* SetCursorToTop();
	virtual IWindowConsole* Printf(LPCSTR fmt, ... );
	virtual IWindowConsole* Printf(int level, LPCSTR fmt, ... );
	//@}
  CFont		 m_font;
  CRichEditCtrl m_redit;
  afx_msg void OnEnUpdateRichedit22();
#ifdef USEPLUGS
  virtual bool plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent); // from PlugContainer
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ConsoleWnd_H__5E52E72C_3CCC_4E61_AF60_72E383CE5B2F__INCLUDED_)
