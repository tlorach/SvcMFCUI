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
#ifndef __IWINDOWIMPL__
#define __IWINDOWIMPL__

#pragma warning(disable : 4311) // float *__w64 cast to unsigned long
#pragma warning(disable : 4355) // this referenced in constructor

#include "ISvcUI.h"
#include <vector>

extern int g_lastWinX, g_lastWinY, g_lastWinW, g_lastWinH;

/*************************************************************************/ /**
	place this define in the class.
	Here we tell IWindow interface implementation is done by IWindowImpl class
 */ /*********************************************************************/
#define DECLARE_IWINDOW_IMPL_NOSETTITLE(cl) \
	virtual IWindow* PeekMyself()				   { return IWindowImpl<cl>::PeekMyself(); }\
	virtual IWindow* OnRender(float fElapsedTime)   { return IWindowImpl<cl>::OnRender(fElapsedTime); } \
	virtual bool MsftMsgProc( void* hWnd, unsigned int uMsg, unsigned int wParam, unsigned long lParam ) { return IWindowImpl<cl>::MsftMsgProc(hWnd, uMsg, wParam, lParam); } \
	virtual IWindow* Minimize(int bYes=1)		   { return IWindowImpl<cl>::Minimize(bYes); }\
	virtual void *GetCWnd()						 { return IWindowImpl<cl>::GetCWnd(); } \
	virtual void *GetHandle()			   { return IWindowImpl<cl>::GetHandle(); } \
	virtual IWindow* Register(IEventsWnd *pDataBase) { return IWindowImpl<cl>::Register(pDataBase); }\
	virtual IWindow* UnRegister(IEventsWnd *pClient=NULL) { return IWindowImpl<cl>::UnRegister(pClient); }\
	virtual IWindow* SetEnabled( bool bEnabled )	{ return IWindowImpl<cl>::SetEnabled(bEnabled); }\
	virtual bool GetEnabled()					   { return IWindowImpl<cl>::GetEnabled(); }\
	virtual IWindow* SetVisible(int bYes=1)		 { return IWindowImpl<cl>::SetVisible(bYes); }\
	virtual int GetVisible()						{ return IWindowImpl<cl>::GetVisible(); } \
	virtual unsigned int		GetType()		   { return IWindowImpl<cl>::GetType(); }\
	virtual LPCSTR			  GetID() const	   { return IWindowImpl<cl>::GetID(); }\
	virtual IWindow* SetID( LPCSTR ID )			 { return IWindowImpl<cl>::SetID(ID); }\
	virtual IWindow* SetLocation(int x, int y)	  { return IWindowImpl<cl>::SetLocation(x,y); }\
	virtual IWindow* SetSize(int width, int height) { return IWindowImpl<cl>::SetSize(width, height); }\
	virtual IWindow* GetPosition(int *x, int *y, int *width, int *height) { return IWindowImpl<cl>::GetPosition(x, y, width, height); }\
	virtual IWindow* SetHotkey( UINT nHotkey )	  { return IWindowImpl<cl>::SetHotkey(nHotkey); }\
	virtual UINT GetHotkey()						{ return IWindowImpl<cl>::GetHotkey(); }\
	virtual IWindow* SetUserData( void *pUserData, size_t UserData ) { return IWindowImpl<cl>::SetUserData(pUserData, UserData); }\
	virtual void *GetUserData(size_t *lData) const { return IWindowImpl<cl>::GetUserData(lData); }\
	virtual IWindow* SetZPos(int z=1)			   { return IWindowImpl<cl>::SetZPos(z); }\
	virtual IWindow* SetBGColor(float r, float g, float b, float a) { return IWindowImpl<cl>::SetBGColor(r,g,b,a); }\
	virtual IWindow* SetBorderColor(float r, float g, float b) { return IWindowImpl<cl>::SetBorderColor(r,g,b); }\
	virtual IWindow* EnableBG(bool bYes)			{ return IWindowImpl<cl>::EnableBG(bYes); }\
	virtual IWindow* EnableBorders(bool bYes)	   { return IWindowImpl<cl>::EnableBorders(bYes); }\
	virtual IWindowContainer *GetParentContainer()  { return IWindowImpl<cl>::GetParentContainer(); }\
	virtual IWindow* SetParentContainer(IWindowContainer *pWin) { return IWindowImpl<cl>::SetParentContainer(pWin); }\
	virtual IWindow* Destroy(bool bDestroyChildren) { return IWindowImpl<cl>::Destroy(bDestroyChildren); }\
	virtual bool IsValid()						  { return IWindowImpl<cl>::IsValid(); }\
	//virtual IWindow* PeekMyself() { IWindowImpl<cl>::PeekMyself(); };\
	virtual IWindow* SetVisible(int bYes) { return IWindowImpl<cl>::SetVisible(bYes); \
		CWnd *pwnd = GetParent(); \
		if(pwnd)	pwnd->SendMessage(WM_USER+10, 1, 0); \
		}; \
	virtual void Hide() { IWindowImpl<cl>::Hide(); \
		CWnd *pwnd = GetParent(); \
		if(pwnd)	pwnd->SendMessage(WM_USER+10, 1, 0); \
		}; \
	virtual int Visible(int bYes) { \
		int yes = IWindowImpl<cl>::Visible(bYes); \
		if(bYes < 0) return yes;\
		CWnd *pwnd = GetParent(); \
		if(pwnd)	pwnd->SendMessage(WM_USER+10, 1, 0); \
		return yes;\
		}; \
	virtual int Minimize(int bYes) { \
		return 0;\
		}; \
	virtual void Register(IEventsWnd *pClient) { IWindowImpl<cl>::Register(pClient); };\
	virtual void UnRegister(IEventsWnd *pDataBase) { IWindowImpl<cl>::UnRegister(pDataBase); };\
	virtual void *GetCWnd() { return IWindowImpl<cl>::GetCWnd(); };\
	virtual void *GetHandle() { return IWindowImpl<cl>::GetHandle(); };\
	virtual void SetPosition(int x, int y, int width, int height) { IWindowImpl<cl>::SetPosition(x, y, width, height); };\
	virtual void GetPosition(int *x, int *y, int *width, int *height) { IWindowImpl<cl>::GetPosition(x, y, width, height); };\
	virtual void SetZPos(int z=0) { IWindowImpl<cl>::SetZPos(z); };\
	virtual void SetUserLong(size_t userparam1, size_t userparam2) { IWindowImpl<cl>::SetUserLong(userparam1, userparam2); };\
	virtual size_t GetUserLong(size_t *userparam1=NULL, size_t *userparam2=NULL) { return IWindowImpl<cl>::GetUserLong(userparam1, userparam2); };\
	virtual long OnRender(float fElapsedTime) { return IWindowImpl<cl>::OnRender(fElapsedTime); }\
	virtual bool MsftMsgProc( void* hWnd, unsigned int uMsg, unsigned int wParam, unsigned long lParam ) { return IWindowImpl<cl>::MsftMsgProc( hWnd, uMsg, wParam, lParam ); }\
	virtual IWindowContainer *GetParentContainer() { return IWindowImpl<cl>::GetParentContainer(); }\
	virtual void SetParentContainer(IWindowContainer *pWin)	{ IWindowImpl<cl>::m_pParentContainer = pWin; }\
	virtual void Destroy() { return IWindowImpl<cl>::Destroy(); }\
	virtual bool IsValid() { return IWindowImpl<cl>::IsValid(); }

#define DECLARE_IWINDOW_IMPL(cl) \
	DECLARE_IWINDOW_IMPL_NOSETTITLE(cl) \
	virtual IWindow* SetTitle(LPCSTR title)			 { return IWindowImpl<cl>::SetTitle(title); };\
	virtual IWindow* GetTitle(char * title, int maxlen) { return IWindowImpl<cl>::GetTitle(title, maxlen); };

/*************************************************************************/ /**
  Implementation of the basics for a window : IWindow.

  This can allow you to avoid doing it.
  Just put DECLARE_IWINDOW_IMPL in your class and inherit it from IWindowImpl<...>.
 */ /*********************************************************************/
template<class _T>
class IWindowImpl //: public CBindVariable
{
	//_T *pT; // we need it to downcast
	char * m_strID;
public:
	void *userData1;
	size_t userData2;
	std::vector<IEventsWnd *> m_pclients;
  IWindowContainer *m_pParentContainer;
	IWindowImpl() 
	{
	m_pParentContainer = NULL;
		userData1 = NULL;
		userData2 = 0;
		m_strID = NULL;
	}

	~IWindowImpl()
	{
		_T* pT = static_cast<_T*>(this);
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		if(pT->IsKindOf(RUNTIME_CLASS(CWnd))) // check if this is a window : if we destroyed it in late we don't want to call windows related stuff
		  pT->DestroyWindow();
		if(HIWORD(m_strID))
		  delete [] m_strID;
	}
	IWindow* PeekMyself()
	{
		_T* pT = static_cast<_T*>(this);
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		/*MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
		 TranslateMessage(&msg);
		 DispatchMessage(&msg);
		}*/
		return pT;
	}
	IWindow* OnRender(float fElapsedTime)
	{
		_T* pT = static_cast<_T*>(this);
		return pT;
	}
	bool MsftMsgProc( void* hWnd, unsigned int uMsg, unsigned int wParam, unsigned long lParam )
	{
		return false;
	}
	IWindow* Minimize(int bYes)
	{
		_T* pT = static_cast<_T*>(this);
		//TODO: pT->
		return pT;
	}
	/**
	 ** Returns the CWnd class ptr in void* (you'll have to cast it as CWnd
	 **/
	void *GetCWnd()
	{
		_T* pT = static_cast<_T*>(this);
		return static_cast<CWnd*>(pT);
	}
	/**
	 ** returns the Handle of the window you'll have to cast it un HWND...
	 **/
	void *GetHandle()
	{
		_T* pT = static_cast<_T*>(this);
		return (void*)pT->m_hWnd;
	}
	IWindow* Register(IEventsWnd *pClient)
	{
		_T* pT = static_cast<_T*>(this);
		if(pClient)
			m_pclients.push_back(pClient);
		return pT;
	}
	IWindow* UnRegister(IEventsWnd *pDataBase)
	{
		_T* pT = static_cast<_T*>(this);
		vector<IEventsWnd *>::iterator icl;
		icl = m_pclients.begin();
		while(icl != m_pclients.end())
		{
			if((*icl) == pDataBase)
			{
				(*icl) = NULL;
				return pT;
			}
			++icl;
		}
		return pT;
	}
	IWindow* SetEnabled( bool bEnabled )
	{
		_T* pT = static_cast<_T*>(this);
		//TODO: pT->
		return pT;
	}
	bool GetEnabled()
	{
		return true; //TODO
	}
	IWindow* SetVisible(int bYes)
	{
		_T* pT = static_cast<_T*>(this);
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		pT->ShowWindow(bYes ? SW_SHOWNOACTIVATE : 0);
		return pT;
	}
	int GetVisible()
	{
		_T* pT = static_cast<_T*>(this);
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		return pT->IsWindowVisible() ? 1:0;
		//PeekMyself();
	}
	unsigned int GetType()
	{
		return 0; //TODO
	}
	LPCSTR			  GetID() const
	{
		return m_strID;
	}
	IWindow* SetID( LPCSTR ID )
	{
		_T* pT = static_cast<_T*>(this);
		if(HIWORD(m_strID))
		  delete [] m_strID;
		if(HIWORD(ID))
		{
			m_strID = new char[strlen(ID)+1];
			strcpy(m_strID, ID);
		} else {
			m_strID = (char*)ID;
		}
		return pT;
	}
	IWindow* SetTitle(LPCSTR title)
	{
		_T* pT = static_cast<_T*>(this);
		if(!title)
			return pT;
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		pT->SetWindowText(CString(title));
		//PeekMyself();
		return pT;
	}
	IWindow* GetTitle(char * title, int maxlen)
	{
		_T* pT = static_cast<_T*>(this);
		if(!title)
			return pT;
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		pT->GetWindowText(title, maxlen);
		//PeekMyself();
		return pT;
	}
	IWindow* SetLocation(int x, int y)
	{
		_T* pT = static_cast<_T*>(this);
		UINT flags = SWP_NOACTIVATE|SWP_NOZORDER;
		flags |= SWP_NOSIZE;
		//if(pT->GetParent() != NULL)
		//	return pT;
		BOOL visible;
		if(!(visible = pT->IsWindowVisible()))
		pT->ShowWindow(SW_SHOWNOACTIVATE);
		pT->SetWindowPos(NULL, x, y, 0, 0, flags);
		if(!visible)
		pT->ShowWindow(0);
		//
		// ask for updating any tree structure from parents
		//
		//PeekMyself();
		CWnd *pwnd = pT->GetParent();
		if(pwnd)
		{
			pwnd->SendMessage(WM_USER+10, 1, 0);
		} else {
			g_lastWinX = x;
			g_lastWinY = y;
		}
		return pT;
	}
	IWindow* SetSize(int width, int height)
	{
		RECT rc;
		_T* pT = static_cast<_T*>(this);
		UINT flags = SWP_NOACTIVATE|SWP_NOZORDER;
		flags |= SWP_NOMOVE;
		BOOL visible;
		if(!(visible = pT->IsWindowVisible()))
		pT->ShowWindow(SW_SHOWNOACTIVATE);
		pT->GetWindowRect(&rc);
		CWnd *pwnd = pT->GetParent();
		if((pwnd) || (width == 0))
			width = rc.right-rc.left;
		if(height == 0)
			height = rc.bottom-rc.top;
		if(!pwnd)
			height += 30; // borders of window...
		pT->SetWindowPos(NULL, 0, 0, width, height, flags);
		if(!visible)
			pT->ShowWindow(0);
		//
		// ask for updating any tree structure from parents
		//
		//PeekMyself();
		if(pwnd)
		{
			pwnd->SendMessage(WM_USER+10, 1, 0);
		} else {
			g_lastWinW = width;
			g_lastWinH = height;
		}
		return pT;
	}
	IWindow* GetPosition(int *x, int *y, int *width, int *height)
	{
		RECT rc;
		_T* pT = static_cast<_T*>(this);
		pT->GetWindowRect(&rc);
		if(pT->GetParent() != NULL)
		{
			if(x) *x = -1;
			if(y) *y = -1;
			if(width) *width = rc.right-rc.left;
			if(height) *height = rc.bottom-rc.top;
			return pT;
		}
		if(x) *x = rc.left;
		if(y) *y = rc.top;
		if(height) *height = rc.bottom-rc.top;
		if(width) *width = rc.right-rc.left;
		return pT;
	}
	IWindow* SetHotkey( UINT nHotkey )
	{
		_T* pT = static_cast<_T*>(this);
		return pT;
	}
	UINT GetHotkey()
	{
		return 0;
	}
	IWindow* SetUserData( void *pUserData, size_t UserData )
	{
		userData1 = pUserData;
		userData2 = UserData;
		_T* pT = static_cast<_T*>(this);
		return pT;
	}
	void *GetUserData(size_t *lData) const
	{
		if(lData) *lData = userData2;
		return userData1;
	}

	IWindow* SetZPos(int z)
	{
		_T* pT = static_cast<_T*>(this);
		if(z == 0)
			pT->SetWindowPos(&CWnd::wndBottom, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
		else if(z == 1)
			pT->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
		else if(z >= 2)
			pT->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
		else if(z <= -1)
			pT->SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
		return pT;
	}
	IWindow* SetBGColor(float r, float g, float b, float a)
	{
		_T* pT = static_cast<_T*>(this);
		//TODO: pT->
		return pT;
	}
	IWindow* SetBorderColor(float r, float g, float b)
	{
		_T* pT = static_cast<_T*>(this);
		//TODO: pT->
		return pT;
	}
	IWindow* EnableBG(bool bYes)
	{
		_T* pT = static_cast<_T*>(this);
		//TODO: pT->
		return pT;
	}
	IWindow* EnableBorders(bool bYes)
	{
		_T* pT = static_cast<_T*>(this);
		//TODO: pT->
		return pT;
	}


	IWindowContainer *GetParentContainer()
	{
		return m_pParentContainer;
	}
	IWindow* SetParentContainer(IWindowContainer *pWin)
	{
		return m_pParentContainer = pWin;
	}

	IWindow* Destroy(bool bDestroyChildren)
	{
		_T* pT = static_cast<_T*>(this);
		pT->AddRef();
		pT->SetVisible(0);
		vector<IEventsWnd *>::const_iterator iclient;
		iclient = m_pclients.begin();
		while(iclient != m_pclients.end())
		{
			if(*iclient)
				(*iclient)->DestroyingWindow(pT);
			++iclient;
		}
		//
		IWindowContainer *pCont = (IWindowContainer *)pT->QueryInterface("IWindowContainer");
		if(pCont)
			while(pCont->GetNumItems())
			{
				if(bDestroyChildren) 
					pCont->GetItemNum(0)->Destroy();
				else
					pCont->RemoveItem(0, NULL, false);
			}
		// TODO : Release resources...
		IWindowContainer *pP = pT->GetParentContainer();
		if(pP)
		{
			pP->RemoveItem(pT, false);
		}
		g_WindowHandler.Destroy(GetID());
		pT->DestroyWindow();
		//delete pT;
		pT->Release();
		return NULL;//pT
	}
	// false is this was destroyed. This can be used to release the reference to this window
	bool IsValid()
	{
		_T* pT = static_cast<_T*>(this);
		return pT->m_hWnd ? true : false;
	}

};
#endif

