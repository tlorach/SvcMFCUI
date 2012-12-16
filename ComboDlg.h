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
#if !defined(AFX_COMBODLG_H__23D56EBF_CDEC_48BA_B7F5_46373C78F9A4__INCLUDED_)
#define AFX_COMBODLG_H__23D56EBF_CDEC_48BA_B7F5_46373C78F9A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ISvcUI.h"
#include "IWindowImplementation.h"

#ifdef USEPLUGS
#include "plug/plug.h"
using namespace plug;
#endif
/////////////////////////////////////////////////////////////////////////////
// CComboDlg dialog

class CComboDlg : public IWindowImpl<CComboDlg>, public CDialog, public IControlCombo
#ifdef USEPLUGS
    , public PlugContainer
#endif
    , public CBindVariable//, public ISmartRef
{
// Construction
public:
	/// \name Factory stuff
	//@{
	DECL_OBJECTDESC_VARIABLE();
	static const void *NewObjectFunc(size_t lparam1, size_t lparam2); // DECL_NEWOBJECTFUNC(CTabWnd, IWindowLog);
	//@}
	IMPLSMARTREF();
	BEGINQUERYINTERFACE();
		QIMAPITF(IControlCombo)	
		QIMAPITF(IWindow)
		QIMAPITF(CBindVariable)
	ENDQUERYINTERFACE();
	CComboDlg(CWnd* pParent = NULL);   // standard constructor

	CString		m_tmpstr;
	int			m_selected;
	DWORD		m_curtag; ///< \TODO :we shouldn't have to use this temporary
#ifdef USEPLUGS
	plug::Plug m_plugcombo;	///< controling m_selected
#endif
// Dialog Data
	//{{AFX_DATA(CComboDlg)
	enum { IDD = IDD_COMBO };
	CComboBox	m_combo;
	CStatic	m_title;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	DECLARE_IWINDOW_IMPL_NOSETTITLE(CComboDlg)
protected:

	// Generated message map functions
	//{{AFX_MSG(CComboDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual IWindow* SetTitle(LPCSTR title);
	virtual IWindow* GetTitle(char * title, int maxlen);
	virtual IWindowContainer	*AsWindowContainer(){ return NULL; };
    virtual IWindowFolding	*AsWindowFolding(){ return NULL; };
	virtual IWindowConsole	  *AsWindowConsole()  { return NULL; };
	virtual IWindowHandler	  *AsWindowHandler()  { return NULL; };
	virtual IWindowLog		  *AsWindowLog()	  { return NULL; };
	virtual IWindowSplitter	 *AsWindowSplitter() { return NULL; };
	virtual IProgressBar		*AsProgressBar()	{ return NULL; };
	virtual IControlCombo	   *AsControlCombo()   { return this; };
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
	/// \name From IControlCombo
	//@{
	virtual IControlCombo*	RemoveAllItems();
	virtual IControlCombo*	RemoveItem( UINT index, bool bUpdateComponents=false);
	virtual IControlCombo*	AddItem( const char* strText, size_t data);
	virtual int				GetSelectedIndex() const ;
	virtual size_t			GetSelectedData();
	virtual ComboBoxItem*   GetSelectedItem();
	virtual UINT	GetNumItems();
	virtual ComboBoxItem* GetItem( UINT index );
	//TODO:
	virtual IControlCombo* SetSelectedByIndex( UINT index );
	virtual IControlCombo* SetSelectedByText( const char* strText );
	virtual IControlCombo* SetSelectedByData( size_t data );  
	virtual int	 GetScrollBarWidth() const ;
	virtual size_t			GetItemData( const char* strText );
	virtual size_t			GetItemData( int nIndex );
	virtual bool	ContainsItem( const char* strText, UINT iStart=0 );
	virtual int	 FindItem( const char* strText, UINT iStart=0 );
	virtual IControlCombo*	SetDropHeight( UINT nHeight ) ;
	virtual IControlCombo*	SetScrollBarWidth( int nWidth ) ;

/*	virtual void	clear();
	virtual int		addString(LPCSTR str, unsigned long ultag);
	virtual int		insert(int index, LPCSTR str, unsigned long ultag);
	virtual int		remove(int index);
	virtual int		select(int index);
	virtual int		getSelected(unsigned long *pultag, LPCSTR *Name);
	virtual unsigned long		getItem(int item, LPCSTR *Name);
	virtual int		getNumItems();
*/
#ifdef USEPLUGS
	virtual void *GetPlug(int n) { return n==0 ? &m_plugcombo : NULL; }
#else
	virtual void *GetPlug(int n) { return NULL; }
#endif
	//@}
	void SetupHExtent();
#ifdef USEPLUGS
	virtual bool plug_IncomingData(IPlug *lpFrom, IPlug *lpTo, bool bSendEvent); // from PlugContainer
#endif
	virtual void UpdateControl();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCALARDLG_H__23D56EBF_CDEC_48BA_B7F5_46373C78F9A4__INCLUDED_)
