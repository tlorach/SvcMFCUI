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
/** \file SvcUI.cpp
 **
 **/ /****************************************************************************/
#include "stdafx.h"

#include "SvcMFCUI.h"
#include "ISvcUI.h"
//#include "ProfileWnd.h"
#include "DlgProgress.h"

//#include "GDIView.h"
#include "LogWnd.h"
//#include "Cardio.h"
#include "ConsoleWnd.h"
#include "TabWnd.h"
#include "FoldingContainer.h"
#include "SplitterContainer.h"
#include "ToolBarWnd.h"
#include "ScalarDlg.h"
#include "VectorDlg.h"
#include "StringDlg.h"
#include "Label.h"
#include "ComboDlg.h"
#include "ColorDlg.h"
#include "BooleanDlg.h"
#include "SimpleContainer.h"
#include "ButtonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace nvSvc;

//SimpleContainer defaultowner;
//CButtonDlg defaultowner;

WindowHandler g_WindowHandler;
int g_lastWinX, g_lastWinY, g_lastWinW, g_lastWinH;

const void *WindowHandler::NewObjectFunc(size_t lparam1, size_t lparam2)
{
	return (void*)(static_cast<IWindowHandler*>(&g_WindowHandler));
}

DECLARE_FACTORY_BASICS("Various Windows that help creating applications\n"
					   "without using any microsoft code.", WINDOWS_VERSION)

/*OBJECT_DESCRIPTION_BEGIN(GDIView, "gdiview", "GDI window view with zoom effect. Used to draw with GDI...", "window") OBJECT_DESCRIPTION_END()
//OBJECT_DESCRIPTION_BEGIN(CCardio, "cardio", "cardiogram graphic", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(CProfileWnd, "profilingwnd", "Used to display profiling results", "window") OBJECT_DESCRIPTION_END()
*/
OBJECT_DESCRIPTION_BEGIN(ToolBarWnd, "controltoolbar", "a toolbar", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(CVectorDlg, "controlvector", "a simple vector value editor", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(CTabWnd, "windowtab", "container window with tabs", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(CStringDlg, "controlstring", "a simple string value editor", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(SplitterContainer, "windowsplitter", "splitter container", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(SplitterContainerH, "windowsplitterh", "Horizontal splitter container", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(CScalarDlg, "controlvalue", "a simple scalar value editor", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(CLogWnd, "windowlog", "logging window", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(FoldingContainer, "windowfolding", "folding container which aligns the controls vertically and allow you to open/close these items", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(CDlgProgress, "progressbar", "progress bar", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(ConsoleWnd, "windowconsole", "console window", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(CComboDlg, "controlcombo", "a combo box", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(CColorDlg, "color", "a simple color picker. Acts as a 4D vector", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(CBooleanDlg, "check", "a check box", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(SimpleContainer, "window", "simple container which aligns the controls vertically", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(CButtonDlg, "button", "a simple button", "window") OBJECT_DESCRIPTION_END()
OBJECT_DESCRIPTION_BEGIN(WindowHandler, "windowhandler", "singleton used to handle windows", "window") OBJECT_DESCRIPTION_END()

/**
 ** Used instead of DLLMain(Attach...). A VOIR...
 **/
BOOL CWindowsApp::InitInstance()
{
// This default Window is the owner of everybody.
// We may turn it into a singleton...
BOOL b = g_WindowHandler.Create(IDD_SIMPLECONTAINER, NULL);//IDD_SIMPLECONTAINER is arbitrary

g_lastWinX = g_lastWinY = g_lastWinW = g_lastWinH = 0;
assert(b);
REGISTER_MODULE(AfxGetInstanceHandle());
/*REGISTER_OBJECT_TO_FACTORY(GDIView);
//REGISTER_OBJECT_TO_FACTORY(CCardio);
REGISTER_OBJECT_TO_FACTORY(CProfileWnd);*/
REGISTER_OBJECT_TO_FACTORY(ToolBarWnd);
REGISTER_OBJECT_TO_FACTORY(CVectorDlg);
REGISTER_OBJECT_TO_FACTORY(CTabWnd);
REGISTER_OBJECT_TO_FACTORY(CStringDlg);
REGISTER_OBJECT_TO_FACTORY(SplitterContainer);
REGISTER_OBJECT_TO_FACTORY(SplitterContainerH);
REGISTER_OBJECT_TO_FACTORY(CScalarDlg);
REGISTER_OBJECT_TO_FACTORY(CLogWnd);
REGISTER_OBJECT_TO_FACTORY(FoldingContainer);
REGISTER_OBJECT_TO_FACTORY(CDlgProgress);
REGISTER_OBJECT_TO_FACTORY(ConsoleWnd);
REGISTER_OBJECT_TO_FACTORY(CComboDlg);
REGISTER_OBJECT_TO_FACTORY(CColorDlg);
REGISTER_OBJECT_TO_FACTORY(CBooleanDlg);
REGISTER_OBJECT_TO_FACTORY(SimpleContainer);
REGISTER_OBJECT_TO_FACTORY(CButtonDlg);

REGISTER_SINGLETON_TO_FACTORY(WindowHandler);
return CWinApp::InitInstance();
}
/**
 ** 
 **/
int CWindowsApp::ExitInstance()
{
	g_WindowHandler.DestroyAll();//RemoveFromWindowHandler(NULL);
	g_WindowHandler.DestroyWindow();
	return 0;
}
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CWindowsApp

BEGIN_MESSAGE_MAP(CWindowsApp, CWinApp)
	//{{AFX_MSG_MAP(CWindowsApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//	DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowsApp construction

CWindowsApp::CWindowsApp() : CWinApp("SvcUI")
{
}

CWindowsApp::~CWindowsApp()
{
	if(m_pszAppName) free((void*)m_pszAppName);
	if(m_pszRegistryKey) free((void*)m_pszRegistryKey);
	if(m_pszExeName) free((void*)m_pszExeName);
	if(m_pszHelpFilePath) free((void*)m_pszHelpFilePath);
	if(m_pszProfileName) free((void*)m_pszProfileName);
	m_pszAppName = NULL;
	m_pszRegistryKey = NULL;
	m_pszExeName = NULL;
	m_pszHelpFilePath = NULL;
	m_pszProfileName = NULL;

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWindowsApp object

CWindowsApp theApp;

////////////////////////////
////////////////////////////
////////////////////////////
unsigned int WindowHandler::HandleMessageLoop_ReturnOnKeyPressed()
{
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
//		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
  if(msg.message == WM_KEYDOWN)
  {
	return msg.wParam;
  }
	}
	return 0;
}
unsigned int WindowHandler::HandleMessageLoop_Blocking()
{
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if(msg.message == WM_QUIT)
		{
		  return 0;
		}
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}
unsigned int WindowHandler::HandleMessageLoop_OnePass()
{
while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE) 
{
  if (GetMessage(&msg, NULL, 0, 0)) 
		{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	if(msg.message == WM_KEYDOWN)
	{
	  return msg.wParam;
	}
  } 
}
	return 0;
}
void WindowHandler::PostMsgQuit()
{
	g_WindowHandler.PostMessageA(WM_QUIT, 0,0);
}

IWindow*			WindowHandler::Get(LPCSTR szID)
{
	MapWindows::iterator iW;
	iW = m_pWindows.find(szID);
	if(iW == m_pWindows.end())
		return NULL;
	return iW->second;
}
IWindowLog*		 WindowHandler::GetLog(LPCSTR szID)
{
	IWindow *pW = Get(szID);
	return pW ? pW->AsWindowLog() : NULL;
}
IWindowConsole*	 WindowHandler::GetConsole(LPCSTR szID)
{
	IWindow *pW = Get(szID);
	return pW ? pW->AsWindowConsole() : NULL;
}
IWindowContainer*   WindowHandler::GetContainer(LPCSTR szID)
{
	IWindow *pW = Get(szID);
	return pW ? pW->AsWindowContainer() : NULL;
}
IWindowFolding*   WindowHandler::GetFoldingContainer(LPCSTR szID)
{
	IWindow *pW = Get(szID);
	return pW ? pW->AsWindowFolding() : NULL;
}
IWindowSplitter*	WindowHandler::GetSplitter(LPCSTR szID)
{
	IWindow *pW = Get(szID);
	return pW ? pW->AsWindowSplitter() : NULL;
}
IControlVector*	 WindowHandler::GetVector(LPCSTR szID)
{
	IWindow *pW = Get(szID);
	return pW ? pW->AsControlVector() : NULL;
}
IControlScalar*	 WindowHandler::GetCheck(LPCSTR szID)
{
	IWindow *pW = Get(szID);
	return pW ? pW->AsControlScalar() : NULL;
}
IControlScalar*	 WindowHandler::GetScalar(LPCSTR szID)
{
	IWindow *pW = Get(szID);
	return pW ? pW->AsControlScalar() : NULL;
}
IControlString*	 WindowHandler::GetString(LPCSTR szID)
{
	IWindow *pW = Get(szID);
	return pW ? pW->AsControlString() : NULL;
}
IControlLabel*	 WindowHandler::GetLabel(LPCSTR szID)
{
	IWindow *pW = Get(szID);
	return pW ? pW->AsControlLabel() : NULL;
}
IControlToolbar*	WindowHandler::GetToolbar(LPCSTR szID)
{
	IWindow *pW = Get(szID);
	return pW ? pW->AsControlToolbar() : NULL;
}
IControlVector*	 WindowHandler::GetColor(LPCSTR szID)
{
	IWindow *pW = Get(szID);
	return pW ? pW->AsControlVector() : NULL;
}
IControlCombo*	  WindowHandler::GetCombo(LPCSTR szID)
{
	IWindow *pW = Get(szID);
	return pW ? pW->AsControlCombo() : NULL;
}
IControlScalar*	 WindowHandler::GetButton(LPCSTR szID)
{
	IWindow *pW = Get(szID);
	return pW ? pW->AsControlScalar() : NULL;
}
IControlCombo*	 WindowHandler::GetRadio(LPCSTR szID)
{
	IWindow *pW = Get(szID);
	return pW ? pW->AsControlCombo() : NULL;
}

bool WindowHandler::RemoveFromWindowHandler(LPCSTR szID)
{
	MapWindows::iterator iW;
	if(szID)
	{
		iW = m_pWindows.find(szID);
		if(iW == m_pWindows.end())
			return false;
		m_pWindows.erase(iW);
	} else 
	{
		iW = m_pWindows.begin();
		while(iW != m_pWindows.end())
		{
			IWindow *pW = iW->second;
			iW->second = NULL;
			++iW;
		}
		m_pWindows.clear();
	}
	return true;
}
bool WindowHandler::AddToWindowHandler(LPCSTR szID, IWindow *pWin)
{
	MapWindows::iterator iW;
	iW = m_pWindows.find(szID);
	if(iW != m_pWindows.end())
		m_pWindows.erase(iW);
	pWin->SetID(szID);
	m_pWindows[szID] = pWin;
	return true;
}

#define CREATEWIN(itf, cl)\
	itf* pW = (itf*)cl::NewObjectFunc(0,0);\
	assert(pW);\
	pW->SetID(szID);\
	m_pWindows[pW->GetID()] = pW;\
	pW->SetTitle(title);\
	if(parent) parent->AddItem(title, pW);\
	if((!parent)&&(g_lastWinW != 0)){\
		pW->SetLocation(g_lastWinX, g_lastWinY + g_lastWinH);\
		pW->SetSize(g_lastWinW, 0);	}\
	if(!parent) pW->GetPosition(&g_lastWinX, &g_lastWinY, &g_lastWinW, &g_lastWinH);\
	return pW;

IWindowLog*		 WindowHandler::CreateWindowLog(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	CREATEWIN(IWindowLog, CLogWnd)
}
IProgressBar*	 WindowHandler::CreateWindowProgressBar(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
//	CREATEWIN(IProgressBar, CDlgProgress)
	IProgressBar* pW = (IProgressBar*)CDlgProgress::NewObjectFunc(0,0);
	assert(pW);
	pW->SetID(szID);
	m_pWindows[pW->GetID()] = pW;
	pW->SetTitle(title);
	if(parent) parent->AddItem(title, pW, true);
	return pW;
}
IWindowConsole*	 WindowHandler::CreateWindowConsole(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	CREATEWIN(IWindowConsole, ConsoleWnd)
}
IWindowContainer*   WindowHandler::CreateWindowContainer(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	CREATEWIN(IWindowContainer, SimpleContainer)
}
IWindowFolding*   WindowHandler::CreateWindowFolding(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	CREATEWIN(IWindowFolding, FoldingContainer)
}
IWindowSplitter*	WindowHandler::CreateWindowSplitter(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	CREATEWIN(IWindowSplitter, SplitterContainer)
}
IWindowContainer*   WindowHandler::CreateWindowTab(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	CREATEWIN(IWindowContainer, CTabWnd)
}

IControlVector*	 WindowHandler::CreateCtrlVector(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
//	CREATEWIN(IControlVector, CVectorDlg)
//#define CREATEWIN(itf, cl)
	IControlVector* pW = (IControlVector*)CVectorDlg::NewObjectFunc(0,0);
	assert(pW);
	pW->SetID(szID);
	m_pWindows[pW->GetID()] = pW;
	pW->SetTitle(title);
	if(parent) parent->AddItem(title, pW);
	if((!parent)&&(g_lastWinW != 0)){
		pW->SetLocation(g_lastWinX, g_lastWinY + g_lastWinH);
		pW->SetSize(g_lastWinW, 0);	}
	if(!parent) pW->GetPosition(&g_lastWinX, &g_lastWinY, &g_lastWinW, &g_lastWinH);
	return pW;
}
IControlScalar*	 WindowHandler::CreateCtrlCheck(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	CREATEWIN(IControlScalar, CBooleanDlg)
}
IControlScalar*	 WindowHandler::CreateCtrlScalar(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	CREATEWIN(IControlScalar, CScalarDlg)
}
IControlString*	 WindowHandler::CreateCtrlString(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	CREATEWIN(IControlString, CStringDlg)
}
IControlLabel*	 WindowHandler::CreateCtrlLabel(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	CREATEWIN(IControlLabel, CLabel)
}
IControlToolbar*	WindowHandler::CreateCtrlToolbar(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	CREATEWIN(IControlToolbar, ToolBarWnd)
}
IControlVector*	 WindowHandler::CreateCtrlColor(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	CREATEWIN(IControlVector, CColorDlg)
}
IControlCombo*	  WindowHandler::CreateCtrlCombo(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	CREATEWIN(IControlCombo, CComboDlg)
}
IControlCombo*	 WindowHandler::CreateCtrlRadio(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	assert(!"WindowHandler::CreateCtrlRadio not implemented, yet");
	return NULL;
}
IControlListBox*	WindowHandler::CreateCtrlListBox(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	assert(!"WindowHandler::CreateCtrlListBox not implemented, yet");
	return NULL;
}

IWindow*			WindowHandler::CreateCtrlButton(LPCSTR szID, LPCSTR title, IWindowContainer * parent)
{
	CREATEWIN(IWindow, CButtonDlg)
}
bool WindowHandler::VariableUnbind(void *ptr)
{
	VBind::iterator iV;
	iV = m_ptrBind.find(ptr);
	if(iV != m_ptrBind.end())
	{
		m_ptrBind.erase(iV);
		return true;
	}
	return false;
}
bool WindowHandler::VariableFlush(void *ptr)
{
	VBind::const_iterator iV;
	iV = m_ptrBind.find(ptr);
	if(iV != m_ptrBind.end())
	{
		iV->second.pBaseContainer->UpdateControl();
		return true;
	}
	return false;
}
#define BODY(t)\
	CBindVariable*  pBI;\
	pBI = (CBindVariable*)pIWin->QueryInterface("CBindVariable");\
	m_ptrBind[v] = VBindStruct(pBI, dim, v, t);\
	pBI->BindVariable(v, dim, t);\
	return true;

bool WindowHandler::VariableBind(IWindow *pIWin, float *v, int dim)
{
	BODY(VBT_FLOAT)
}
bool WindowHandler::VariableBind(IWindow *pIWin, int *v, int dim)
{
	BODY(VBT_INT)
}
bool WindowHandler::VariableBind(IWindow *pIWin, unsigned int *v, int dim)
{
	BODY(VBT_UINT)
}
bool WindowHandler::VariableBind(IWindow *pIWin, long *v, int dim)
{
	BODY(VBT_LONG)
}
bool WindowHandler::VariableBind(IWindow *pIWin, unsigned long *v, int dim)
{
	BODY(VBT_ULONG)
}
bool WindowHandler::VariableBind(IWindow *pIWin, bool *v)
{
	int dim=1;
	BODY(VBT_BOOL)
}
bool WindowHandler::VariableBind(IWindow *pIWin, char *v)
{
	int dim=1;
	BODY(VBT_CHAR)
}
bool WindowHandler::VariableBind(IWindow *pIWin, char *v, int dim)
{
	BODY(VBT_STRING)
}

bool WindowHandler::Destroy(LPCSTR szID)
{
	MapWindows::iterator iW;
	iW = m_pWindows.find(szID); //using SmartPtr !
	if(iW == m_pWindows.end())
		return false;
    SmartPtr<IWindow> pWin = iW->second;
	m_pWindows.erase(iW);
	pWin->Destroy();
	return true;

}

bool WindowHandler::DestroyAll()
{
	MapWindows::iterator iW;
	iW = m_pWindows.begin();
	while(iW != m_pWindows.end())
	{
		IWindow *pW = iW->second;
		if(pW->GetParentContainer() == NULL)
			pW->Destroy();
		m_pWindows.erase(iW); //using SmartPtr !
		iW = m_pWindows.begin();
	}
	m_pWindows.clear();
	return true;
}

void WindowHandler::RefreshAll()
{
	MapWindows::iterator iW;
	iW = m_pWindows.begin();
	while(iW != m_pWindows.end())
	{
		IWindow *pW = iW->second;
		IWindowContainer *pCont = (IWindowContainer *)pW->QueryInterface("IWindowContainer");
		if(pCont)
			pCont->Refresh();
		++iW;
	}
}