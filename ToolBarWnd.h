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
#if !defined(AFX_TOOLBAR_H__61C6794E_9AA4_46E0_9617_FEDD72CF38E3__INCLUDED_)
#define AFX_TOOLBAR_H__61C6794E_9AA4_46E0_9617_FEDD72CF38E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBarWnd.h : header file
//

#include "ISvcUI.h"
#ifdef USEPYTHON
#include "ISvcScripting.h"
#endif
#include "IWindowImplementation.h"
#ifdef USEPLUGS
#include "plug/plug.h"
using namespace plug;
#endif
#include <vector>
using namespace std;

typedef enum
{
	EMPTY = 0,
	BMP = 1,
	TGA = 2,
	PNG = 3
} PICTYPE;
/*************************************************************************/ /**
CItem item : contains all we need to handle a button
 */ /*********************************************************************/
struct CItem 
#ifdef USEPLUGS
    : public PlugContainer
#endif
{
	String	pythoncmd;
#ifdef USEPLUGS
	Plug	itemplug;
#endif
	CButton button;
	CBitmap bmpimage;
	CBitmap bmpbutton;
	CString title;
	CString m_imagefilename;
	bool	m_showtext;
	int		m_states;
	int		m_curstate;
	int		m_ddsidx;
	size_t	tag;
	float	ratio;
	int imagew, imageh;

	CItem(LPCSTR name, int states, unsigned short tag);
	~CItem();

	int Create(CWnd *parent, int num, int x, LPCSTR t, int states, size_t tg, BOOL bitmap);
	int CreateFromFile(CWnd *parent, int num, int x, LPCSTR t, LPCSTR imagefilename, int ddsidx, int states, size_t tg);
	int CreateFromMemoryBMP(CWnd *parent, int num, int x, LPCSTR t, void * imagedata, int states, size_t tg);
	int CreateFromMemoryPNG(CWnd *parent, int num, int x, LPCSTR t, void * imagedata, int states, size_t tg);
	int CreateFromMemoryTGA(CWnd *parent, int num, int x, LPCSTR t, void * imagedata, int states, size_t tg);
	int CreateFromMemoryDDS(CWnd *parent, int num, int x, LPCSTR t, void * imagedata, int ddsidx, int states, size_t tg);
	void SetState(int state);
	int GetState();
	int Resize(int newx, int newy);
	void SetPos(int newx, int newy);
	void ShowText(bool bYes);

#ifdef USEPLUGS
	bool plug_IncomingData(IPlug *lpSrc, IPlug *lpDst, bool bSendEvent);
#endif

};

typedef vector<CItem *> TItems;
/*************************************************************************/ /**
ToolBarWnd dialog
 */ /*********************************************************************/
class ToolBarWnd : public IWindowImpl<ToolBarWnd>, public CDialog, public IControlToolbar
#ifdef USEPLUGS
    , public PlugContainer
#endif
{
// Construction
public:
	/// \name Factory stuff
	//@{
	DECL_OBJECTDESC_VARIABLE();
	static const void *NewObjectFunc(size_t lparam1, size_t lparam2); // DECL_NEWOBJECTFUNC(CTabWnd, IDlgLog);
	//@}
	IMPLSMARTREF();
	BEGINQUERYINTERFACE();
		QIMAPITF(IControlToolbar)	
		QIMAPITF(IWindow)
	ENDQUERYINTERFACE();
	virtual ~ToolBarWnd();

	ToolBarWnd(CWnd* pParent = NULL);   // standard constructor
	void RecalcLayout();

	bool m_vertical;
	bool m_showtext;

	TItems		m_items;
	int			m_freex;
	int			m_maxheight, m_minheight;
#ifdef USEPYTHON
	IPython*	_python; // should be SmartPtr<IPython>
#endif
// Dialog Data
	//{{AFX_DATA(ToolBarWnd)
	enum { IDD = IDD_TOOLBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ToolBarWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ToolBarWnd)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_IWINDOW_IMPL(ToolBarWnd)
	virtual IWindowContainer	*AsWindowContainer(){ return NULL; };
    virtual IWindowFolding	*AsWindowFolding(){ return NULL; };
	virtual IWindowConsole	  *AsWindowConsole()  { return NULL; };
	virtual IWindowHandler	  *AsWindowHandler()  { return NULL; };
	virtual IWindowLog		  *AsWindowLog()	  { return NULL; };
	virtual IWindowSplitter	 *AsWindowSplitter() { return NULL; };
	virtual IProgressBar		*AsProgressBar()	{ return NULL; };
	virtual IControlCombo	   *AsControlCombo()   { return NULL; };
	virtual IControlListBox	 *AsControlListBox() { return NULL; };
	virtual IControlScalar	  *AsControlScalar()  { return NULL; };
	virtual IControlString	  *AsControlString()  { return NULL; };
	virtual IControlLabel	  *AsControlLabel()   { return NULL; };
	virtual IControlToolbar	 *AsControlToolbar() { return this; };
	virtual IControlVector	  *AsControlVector()  { return NULL; };
	virtual IControlStatic	  *AsControlStatic()  { return NULL; };
	virtual IControlScalar	  *AsControlBoolean() { return NULL; };
	virtual IControlScalar	  *AsControlCheck()   { return NULL; };
	virtual IWindow				*AsControlButton()  { return NULL; };
	virtual IControlVector	  *AsControlColor()   { return NULL; };
	/// \name From IControlToolbar
	//@{
	virtual IControlToolbar* AddItem(LPCSTR title, LPCSTR imagefilename, int ddsidx=0, int states=1, size_t tag=0);
	virtual IControlToolbar* AddItemFromMemoryBMP(LPCSTR title, void * imagedata, int states=1, size_t tag=0);
	virtual IControlToolbar* AddItemFromMemoryPNG(LPCSTR title, void * imagedata, int states=1, size_t tag=0);
	virtual IControlToolbar* AddItemFromMemoryTGA(LPCSTR title, void * imagedata, int states=1, size_t tag=0);
	virtual IControlToolbar* AddItemFromMemoryDDS(LPCSTR title, void * imagedata, int ddsidx, int states=1, size_t tag=0);
	virtual IControlToolbar* InsertItemFromMemoryBMP(int pos, LPCSTR title, void * imagedata, int states=1, size_t tag=0);
	virtual IControlToolbar* InsertItemFromMemoryPNG(int pos, LPCSTR title, void * imagedata, int states=1, size_t tag=0);
	virtual IControlToolbar* InsertItemFromMemoryTGA(int pos, LPCSTR title, void * imagedata, int states=1, size_t tag=0);
	virtual IControlToolbar* InsertItemFromMemoryDDS(int pos, LPCSTR title, void * imagedata, int ddsidx=0, int states=1, size_t tag=0);
	virtual IControlToolbar* DeleteItem(int pos);
	virtual IControlToolbar* DeleteItem(LPCSTR title);
	virtual IControlToolbar* ItemsSetState(int item, int state);
	virtual int			  ItemsGetState(int item);
	virtual IControlToolbar* Vertical(int bYes=-1);
	virtual IControlToolbar* ShowText(bool bYes=true);
	virtual IControlToolbar* SetMinMaxHeight(int hmin, int hmax);
	virtual IControlToolbar* GetItemInfos(int item, int &states, size_t &tag,
					char * title, int titlesz, 
					char * imagefname, int imagefnamesz,
					int &ddsidx);
	/// associate a python script to a button : pIPython is IPython class from SvcScripting.h
	virtual IControlToolbar* SetPythonInterface(void * pIPython);
	virtual IControlToolbar* SetItemPythonScriptEvent(int item, LPCSTR script);
	virtual LPCSTR GetItemPythonScriptEvent(int item);
	//@}

#ifdef USEPLUGS
	virtual bool plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent); // from PlugContainer
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBAR_H__61C6794E_9AA4_46E0_9617_FEDD72CF38E3__INCLUDED_)
	