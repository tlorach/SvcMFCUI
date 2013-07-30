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
/** \file SvcMFCUI.h
 **
 **/ /****************************************************************************/
#if !defined(AFX_WINDOWS_H__AA0BFC0B_D33C_4AF6_B9EA_AC02C6AA532F__INCLUDED_)
#define AFX_WINDOWS_H__AA0BFC0B_D33C_4AF6_B9EA_AC02C6AA532F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "SvcFactorybase.h"

#ifndef String
#include <string>
typedef std::string String;
#endif

#include "ISvcUI.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CWindowsApp
// See Windows.cpp for the implementation of this class
//

class CWindowsApp : public CWinApp
{
public:
	CWindowsApp();
	~CWindowsApp();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindowsApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CWindowsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//	DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

enum VarBindType {
	VBT_FLOAT = 0,
	VBT_INT,
	VBT_UINT,
	VBT_LONG,
	VBT_ULONG,
	VBT_BOOL,
	VBT_CHAR,
	VBT_STRING
};

class CBindVariable
{
protected:
  void				   *m_bindPtr; // pointer of a place where we want the control to write it value
  int				   m_bindDim; // dimension of the variable
  VarBindType		   m_bindType;// type of the m_bindPtr
public:
	CBindVariable()
	{
		m_bindPtr = NULL;
		m_bindDim = 0;
	}
	void  BindVariable(void* val, int dim, VarBindType type)
	{
		m_bindPtr = val;
		m_bindDim = dim;
		m_bindType = type;
		UpdateControl();
	}
	virtual void UpdateControl() = 0;

	void GetVariableValue(float *f, int dim)
	{
		switch(m_bindType) 
		{
		case VBT_STRING:
			f[0] = atof((char*)m_bindPtr);// TODO: for > 1
			break;
		case VBT_FLOAT:
			if((dim >= 1)&&(m_bindDim >= 1)) f[0] = ((float*)m_bindPtr)[0];
			if((dim >= 2)&&(m_bindDim >= 2)) f[1] = ((float*)m_bindPtr)[1];
			if((dim >= 3)&&(m_bindDim >= 3)) f[2] = ((float*)m_bindPtr)[2];
			if((dim == 4)&&(m_bindDim == 4)) f[3] = ((float*)m_bindPtr)[3];
			break;
		case VBT_INT:
			if((dim >= 1)&&(m_bindDim >= 1)) f[0] = (float)((int*)m_bindPtr)[0];
			if((dim >= 2)&&(m_bindDim >= 2)) f[1] = (float)((int*)m_bindPtr)[1];
			if((dim >= 3)&&(m_bindDim >= 3)) f[2] = (float)((int*)m_bindPtr)[2];
			if((dim == 4)&&(m_bindDim == 4)) f[3] = (float)((int*)m_bindPtr)[3];
			break;
		case VBT_UINT:
			if((dim >= 1)&&(m_bindDim >= 1)) f[0] = (float)((unsigned int*)m_bindPtr)[0];
			if((dim >= 2)&&(m_bindDim >= 2)) f[1] = (float)((unsigned int*)m_bindPtr)[1];
			if((dim >= 3)&&(m_bindDim >= 3)) f[2] = (float)((unsigned int*)m_bindPtr)[2];
			if((dim == 4)&&(m_bindDim == 4)) f[3] = (float)((unsigned int*)m_bindPtr)[3];
			break;
		case VBT_LONG:
			if((dim >= 1)&&(m_bindDim >= 1)) f[0] = (float)((long*)m_bindPtr)[0];
			if((dim >= 2)&&(m_bindDim >= 2)) f[1] = (float)((long*)m_bindPtr)[1];
			if((dim >= 3)&&(m_bindDim >= 3)) f[2] = (float)((long*)m_bindPtr)[2];
			if((dim == 4)&&(m_bindDim == 4)) f[3] = (float)((long*)m_bindPtr)[3];
			break;
		case VBT_ULONG:
			if((dim >= 1)&&(m_bindDim >= 1)) f[0] = (float)((unsigned long*)m_bindPtr)[0];
			if((dim >= 2)&&(m_bindDim >= 2)) f[1] = (float)((unsigned long*)m_bindPtr)[1];
			if((dim >= 3)&&(m_bindDim >= 3)) f[2] = (float)((unsigned long*)m_bindPtr)[2];
			if((dim == 4)&&(m_bindDim == 4)) f[3] = (float)((unsigned long*)m_bindPtr)[3];
			break;
		case VBT_BOOL:
			if((dim >= 1)&&(m_bindDim >= 1)) f[0] = ((bool*)m_bindPtr)[0] ? 1.0 : 0.0;
			if((dim >= 2)&&(m_bindDim >= 2)) f[1] = ((bool*)m_bindPtr)[1] ? 1.0 : 0.0;
			if((dim >= 3)&&(m_bindDim >= 3)) f[2] = ((bool*)m_bindPtr)[2] ? 1.0 : 0.0;
			if((dim == 4)&&(m_bindDim == 4)) f[3] = ((bool*)m_bindPtr)[3] ? 1.0 : 0.0;
			break;
		case VBT_CHAR:
			if((dim >= 1)&&(m_bindDim >= 1)) f[0] = (float)((char*)m_bindPtr)[0];
			if((dim >= 2)&&(m_bindDim >= 2)) f[1] = (float)((char*)m_bindPtr)[1];
			if((dim >= 3)&&(m_bindDim >= 3)) f[2] = (float)((char*)m_bindPtr)[2];
			if((dim == 4)&&(m_bindDim == 4)) f[3] = (float)((char*)m_bindPtr)[3];
			break;
		}
	}
	void SetVariableValue(float *f, int dim)
	{
		if(!m_bindPtr) return;
		switch(m_bindType) 
		{
		case VBT_STRING:
			if(dim == 1) sprintf((char*)m_bindPtr,"%f", f[0]);
			else if(dim == 2) sprintf((char*)m_bindPtr,"%f", f[0], f[1]);
			else if(dim == 3) sprintf((char*)m_bindPtr,"%f", f[0], f[1], f[2]);
			else if(dim == 4) sprintf((char*)m_bindPtr,"%f %f %f %f", f[0], f[1], f[2], f[3]);
			break;
		case VBT_FLOAT:
			if((dim == 4)&&(m_bindDim == 4)) ((float*)m_bindPtr)[3] = f[3];
			if((dim >= 3)&&(m_bindDim >= 3)) ((float*)m_bindPtr)[2] = f[2];
			if((dim >= 2)&&(m_bindDim >= 2)) ((float*)m_bindPtr)[1] = f[1];
			if((dim >= 1)&&(m_bindDim >= 1)) ((float*)m_bindPtr)[0] = f[0];
			break;
		case VBT_INT:
			if((dim == 4)&&(m_bindDim == 4)) ((int*)m_bindPtr)[3] = (int)f[3];
			if((dim >= 3)&&(m_bindDim >= 3)) ((int*)m_bindPtr)[2] = (int)f[2];
			if((dim >= 2)&&(m_bindDim >= 2)) ((int*)m_bindPtr)[1] = (int)f[1];
			if((dim >= 1)&&(m_bindDim >= 1)) ((int*)m_bindPtr)[0] = (int)f[0];
			break;
		case VBT_UINT:
			if((dim == 4)&&(m_bindDim == 4)) ((unsigned int*)m_bindPtr)[3] = (unsigned int)f[3];
			if((dim >= 3)&&(m_bindDim >= 3)) ((unsigned int*)m_bindPtr)[2] = (unsigned int)f[2];
			if((dim >= 2)&&(m_bindDim >= 2)) ((unsigned int*)m_bindPtr)[1] = (unsigned int)f[1];
			if((dim >= 1)&&(m_bindDim >= 1)) ((unsigned int*)m_bindPtr)[0] = (unsigned int)f[0];
			break;
		case VBT_LONG:
			if((dim == 4)&&(m_bindDim == 4)) ((long*)m_bindPtr)[3] = (long)f[3];
			if((dim >= 3)&&(m_bindDim >= 3)) ((long*)m_bindPtr)[2] = (long)f[2];
			if((dim >= 2)&&(m_bindDim >= 2)) ((long*)m_bindPtr)[1] = (long)f[1];
			if((dim >= 1)&&(m_bindDim >= 1)) ((long*)m_bindPtr)[0] = (long)f[0];
			break;
		case VBT_ULONG:
			if((dim == 4)&&(m_bindDim == 4)) ((unsigned long*)m_bindPtr)[3] = (unsigned long)f[3];
			if((dim >= 3)&&(m_bindDim >= 3)) ((unsigned long*)m_bindPtr)[2] = (unsigned long)f[2];
			if((dim >= 2)&&(m_bindDim >= 2)) ((unsigned long*)m_bindPtr)[1] = (unsigned long)f[1];
			if((dim >= 1)&&(m_bindDim >= 1)) ((unsigned long*)m_bindPtr)[0] = (unsigned long)f[0];
			break;
		case VBT_BOOL:
			if((dim == 4)&&(m_bindDim == 4)) ((bool*)m_bindPtr)[3] = f[3] == 0.0 ? false : true;
			if((dim >= 3)&&(m_bindDim >= 3)) ((bool*)m_bindPtr)[2] = f[2] == 0.0 ? false : true;
			if((dim >= 2)&&(m_bindDim >= 2)) ((bool*)m_bindPtr)[1] = f[1] == 0.0 ? false : true;
			if((dim >= 1)&&(m_bindDim >= 1)) ((bool*)m_bindPtr)[0] = f[0] == 0.0 ? false : true;
			break;
		case VBT_CHAR:
			if((dim == 4)&&(m_bindDim == 4)) ((char*)m_bindPtr)[3] = (char)f[3];
			if((dim >= 3)&&(m_bindDim >= 3)) ((char*)m_bindPtr)[2] = (char)f[2];
			if((dim >= 2)&&(m_bindDim >= 2)) ((char*)m_bindPtr)[1] = (char)f[1];
			if((dim >= 1)&&(m_bindDim >= 1)) ((char*)m_bindPtr)[0] = (char)f[0];
			break;
		}
	}
	void GetVariableValue(int *f, int dim)
	{
		switch(m_bindType) 
		{
		case VBT_STRING:
			f[0] = atof((char*)m_bindPtr);// TODO: for > 1
			break;
		case VBT_FLOAT:
			if((dim >= 1)&&(m_bindDim >= 1)) f[0] = (int)((float*)m_bindPtr)[0];
			if((dim >= 2)&&(m_bindDim >= 2)) f[1] = (int)((float*)m_bindPtr)[1];
			if((dim >= 3)&&(m_bindDim >= 3)) f[2] = (int)((float*)m_bindPtr)[2];
			if((dim == 4)&&(m_bindDim == 4)) f[3] = (int)((float*)m_bindPtr)[3];
			break;
		case VBT_INT:
			if((dim >= 1)&&(m_bindDim >= 1)) f[0] = (int)((int*)m_bindPtr)[0];
			if((dim >= 2)&&(m_bindDim >= 2)) f[1] = (int)((int*)m_bindPtr)[1];
			if((dim >= 3)&&(m_bindDim >= 3)) f[2] = (int)((int*)m_bindPtr)[2];
			if((dim == 4)&&(m_bindDim == 4)) f[3] = (int)((int*)m_bindPtr)[3];
			break;
		case VBT_UINT:
			if((dim >= 1)&&(m_bindDim >= 1)) f[0] = (int)((unsigned int*)m_bindPtr)[0];
			if((dim >= 2)&&(m_bindDim >= 2)) f[1] = (int)((unsigned int*)m_bindPtr)[1];
			if((dim >= 3)&&(m_bindDim >= 3)) f[2] = (int)((unsigned int*)m_bindPtr)[2];
			if((dim == 4)&&(m_bindDim == 4)) f[3] = (int)((unsigned int*)m_bindPtr)[3];
			break;
		case VBT_LONG:
			if((dim >= 1)&&(m_bindDim >= 1)) f[0] = (int)((long*)m_bindPtr)[0];
			if((dim >= 2)&&(m_bindDim >= 2)) f[1] = (int)((long*)m_bindPtr)[1];
			if((dim >= 3)&&(m_bindDim >= 3)) f[2] = (int)((long*)m_bindPtr)[2];
			if((dim == 4)&&(m_bindDim == 4)) f[3] = (int)((long*)m_bindPtr)[3];
			break;
		case VBT_ULONG:
			if((dim >= 1)&&(m_bindDim >= 1)) f[0] = (int)((unsigned long*)m_bindPtr)[0];
			if((dim >= 2)&&(m_bindDim >= 2)) f[1] = (int)((unsigned long*)m_bindPtr)[1];
			if((dim >= 3)&&(m_bindDim >= 3)) f[2] = (int)((unsigned long*)m_bindPtr)[2];
			if((dim == 4)&&(m_bindDim == 4)) f[3] = (int)((unsigned long*)m_bindPtr)[3];
			break;
		case VBT_BOOL:
			if((dim >= 1)&&(m_bindDim >= 1)) f[0] = ((bool*)m_bindPtr)[0] ? 1.0 : 0.0;
			if((dim >= 2)&&(m_bindDim >= 2)) f[1] = ((bool*)m_bindPtr)[1] ? 1.0 : 0.0;
			if((dim >= 3)&&(m_bindDim >= 3)) f[2] = ((bool*)m_bindPtr)[2] ? 1.0 : 0.0;
			if((dim == 4)&&(m_bindDim == 4)) f[3] = ((bool*)m_bindPtr)[3] ? 1.0 : 0.0;
			break;
		case VBT_CHAR:
			if((dim >= 1)&&(m_bindDim >= 1)) f[0] = (int)((char*)m_bindPtr)[0];
			if((dim >= 2)&&(m_bindDim >= 2)) f[1] = (int)((char*)m_bindPtr)[1];
			if((dim >= 3)&&(m_bindDim >= 3)) f[2] = (int)((char*)m_bindPtr)[2];
			if((dim == 4)&&(m_bindDim == 4)) f[3] = (int)((char*)m_bindPtr)[3];
			break;
		}
	}
	void SetVariableValue(int *f, int dim)
	{
		if(!m_bindPtr) return;
		switch(m_bindType) 
		{
		case VBT_STRING:
			if(dim == 1) sprintf((char*)m_bindPtr,"%d", f[0]);
			else if(dim == 2) sprintf((char*)m_bindPtr,"%d", f[0], f[1]);
			else if(dim == 3) sprintf((char*)m_bindPtr,"%d", f[0], f[1], f[2]);
			else if(dim == 4) sprintf((char*)m_bindPtr,"%d %d %d %d", f[0], f[1], f[2], f[3]);
			break;
		case VBT_FLOAT:
			if((dim == 4)&&(m_bindDim == 4)) ((float*)m_bindPtr)[3] = (float)f[3];
			if((dim >= 3)&&(m_bindDim >= 3)) ((float*)m_bindPtr)[2] = (float)f[2];
			if((dim >= 2)&&(m_bindDim >= 2)) ((float*)m_bindPtr)[1] = (float)f[1];
			if((dim >= 1)&&(m_bindDim >= 1)) ((float*)m_bindPtr)[0] = (float)f[0];
			break;
		case VBT_INT:
			if((dim == 4)&&(m_bindDim == 4)) ((int*)m_bindPtr)[3] = (int)f[3];
			if((dim >= 3)&&(m_bindDim >= 3)) ((int*)m_bindPtr)[2] = (int)f[2];
			if((dim >= 2)&&(m_bindDim >= 2)) ((int*)m_bindPtr)[1] = (int)f[1];
			if((dim >= 1)&&(m_bindDim >= 1)) ((int*)m_bindPtr)[0] = (int)f[0];
			break;
		case VBT_UINT:
			if((dim == 4)&&(m_bindDim == 4)) ((unsigned int*)m_bindPtr)[3] = (unsigned int)f[3];
			if((dim >= 3)&&(m_bindDim >= 3)) ((unsigned int*)m_bindPtr)[2] = (unsigned int)f[2];
			if((dim >= 2)&&(m_bindDim >= 2)) ((unsigned int*)m_bindPtr)[1] = (unsigned int)f[1];
			if((dim >= 1)&&(m_bindDim >= 1)) ((unsigned int*)m_bindPtr)[0] = (unsigned int)f[0];
			break;
		case VBT_LONG:
			if((dim == 4)&&(m_bindDim == 4)) ((long*)m_bindPtr)[3] = (long)f[3];
			if((dim >= 3)&&(m_bindDim >= 3)) ((long*)m_bindPtr)[2] = (long)f[2];
			if((dim >= 2)&&(m_bindDim >= 2)) ((long*)m_bindPtr)[1] = (long)f[1];
			if((dim >= 1)&&(m_bindDim >= 1)) ((long*)m_bindPtr)[0] = (long)f[0];
			break;
		case VBT_ULONG:
			if((dim == 4)&&(m_bindDim == 4)) ((unsigned long*)m_bindPtr)[3] = (unsigned long)f[3];
			if((dim >= 3)&&(m_bindDim >= 3)) ((unsigned long*)m_bindPtr)[2] = (unsigned long)f[2];
			if((dim >= 2)&&(m_bindDim >= 2)) ((unsigned long*)m_bindPtr)[1] = (unsigned long)f[1];
			if((dim >= 1)&&(m_bindDim >= 1)) ((unsigned long*)m_bindPtr)[0] = (unsigned long)f[0];
			break;
		case VBT_BOOL:
			if((dim == 4)&&(m_bindDim == 4)) ((bool*)m_bindPtr)[3] = f[3] == 0.0 ? false : true;
			if((dim >= 3)&&(m_bindDim >= 3)) ((bool*)m_bindPtr)[2] = f[2] == 0.0 ? false : true;
			if((dim >= 2)&&(m_bindDim >= 2)) ((bool*)m_bindPtr)[1] = f[1] == 0.0 ? false : true;
			if((dim >= 1)&&(m_bindDim >= 1)) ((bool*)m_bindPtr)[0] = f[0] == 0.0 ? false : true;
			break;
		case VBT_CHAR:
			if((dim == 4)&&(m_bindDim == 4)) ((char*)m_bindPtr)[3] = (char)f[3];
			if((dim >= 3)&&(m_bindDim >= 3)) ((char*)m_bindPtr)[2] = (char)f[2];
			if((dim >= 2)&&(m_bindDim >= 2)) ((char*)m_bindPtr)[1] = (char)f[1];
			if((dim >= 1)&&(m_bindDim >= 1)) ((char*)m_bindPtr)[0] = (char)f[0];
			break;
		}
	}
};
/*************************************************************************/ /**
 ** Singleton Interface of Windows message loop
 **/ /*************************************************************************/ 
class WindowHandler : public IWindowHandler, public CDialog
{
	struct ltstr
	{
	  bool operator()(LPCSTR s1, LPCSTR s2) const
	  {
		char ss1[20], ss2[20];
		const char *ps1 = s1;
		const char *ps2 = s2;
		if(!HIWORD(s1))
		{
			sprintf_s(ss1, 20, "%p",s1);
			ps1 = ss1;
		}
		if(!HIWORD(s2))
		{
			sprintf_s(ss2, 20, "%p",s2);
			ps2 = ss2;
		}
		//assert((HIWORD(s1)&&HIWORD(s2))||((!HIWORD(s1))&&(!HIWORD(s2))));
		//if((!HIWORD(s1))||(!HIWORD(s2)))
		//	return (unsigned int)s1 < (unsigned int)s2;
		return strcmp(ps1, ps2) < 0;
	  }
	};
	typedef std::map<LPCSTR, SmartPtr<IWindow>, ltstr> MapWindows;
	MapWindows			  m_pWindows;
	MSG					 msg;
protected:
	vector<IEventsWnd *>	m_pclients;
public:
	DECL_OBJECTDESC_VARIABLE();
	IMPLSMARTREF()
	BEGINQUERYINTERFACE()
	ENDQUERYINTERFACE();
	static const void *WindowHandler::NewObjectFunc(size_t lparam1, size_t lparam2);
	WindowHandler()
	{
	   
	}
	~WindowHandler()
	{
		//m_pWindows.clear();
		MapWindows::iterator iW = m_pWindows.begin();
		while(iW != m_pWindows.end())
		{
			IWindow *pW = iW->second;
			//if(!pW->GetParentContainer())
			{
				iW->second = NULL;
			}
			++iW;
		}
	}
	//
	// Get Interfaces of instanciated objects :
	//
	IWindow*			Get(LPCSTR szID);
	IWindowLog*		 GetLog(LPCSTR szID);
	IWindowConsole*	 GetConsole(LPCSTR szID);
	IWindowContainer*   GetContainer(LPCSTR szID);
    IWindowFolding*   GetFoldingContainer(LPCSTR szID);
	IWindowSplitter*	GetSplitter(LPCSTR szID);
	IControlVector*	 GetVector(LPCSTR szID);
//QUESTION: Shall we change the Class (IControlScalar) for Checkbox ?
	IControlScalar*	 GetCheck(LPCSTR szID);
	IControlScalar*	 GetScalar(LPCSTR szID);
	IControlString*	 GetString(LPCSTR szID);
	IControlLabel*	 GetLabel(LPCSTR szID);
	IControlToolbar*	GetToolbar(LPCSTR szID);
	IControlVector*	 GetColor(LPCSTR szID);
	IControlCombo*	  GetCombo(LPCSTR szID);
//QUESTION: Shall we change the Class (IControlScalar) for Button ctrl ?
	IControlScalar*	 GetButton(LPCSTR szID);
	IControlCombo*	  GetRadio(LPCSTR szID);
	//
	// Remove from the window handler (potentially, will destroy the window is not referenced anywhere else)
	//
	bool			RemoveFromWindowHandler(LPCSTR szID);
	bool			AddToWindowHandler(LPCSTR szID, IWindow *pWin);
	//
	// Create Objects (handled by WindowHandler object) :
	//
	IWindowLog*		 CreateWindowLog(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);
	IWindowConsole*	 CreateWindowConsole(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);
	IWindowContainer*   CreateWindowContainer(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);
	IWindowFolding*	 CreateWindowFolding(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);
	IWindowSplitter*	CreateWindowSplitter(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL, bool verticalSplitter=false);
	IWindowContainer*   CreateWindowTab(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);
	IProgressBar*	   CreateWindowProgressBar(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);

	IControlVector*	 CreateCtrlVector(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);
	IControlScalar*	 CreateCtrlCheck(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);
	IControlScalar*	 CreateCtrlScalar(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);
	IControlString*	 CreateCtrlString(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);
	IControlLabel*	 CreateCtrlLabel(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);
	IControlToolbar*	CreateCtrlToolbar(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);
	IControlVector*	 CreateCtrlColor(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);
	IControlCombo*	  CreateCtrlCombo(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);
	IWindow*			CreateCtrlButton(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);
	IControlCombo*	  CreateCtrlRadio(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);
	IControlListBox*	CreateCtrlListBox(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL);

	bool				Destroy(LPCSTR szID);
	bool				DestroyAll();
	//
	// Events when no msg pump available
	//
	// return only when windows has finished.
	unsigned int HandleMessageLoop_Blocking();
	// returns only wen a Key was pressed : Key code value returned
	unsigned int HandleMessageLoop_ReturnOnKeyPressed();
	// returns At every event. Key Code returned. Or 0 otherwise.
	unsigned int HandleMessageLoop_OnePass();
	void		 PostMsgQuit();
	//
	// returns false if the method handled the message
	//
	bool MsftMsgProc( void* hWnd, unsigned int uMsg, unsigned int wParam, unsigned long lParam ) {return false; }; ///< specific to Microsoft msg pump
	//
	// Glut event mgt
	//
	void glutReshape(int w, int h) {};
	void glutKeys(unsigned char c, int x, int y) {};
	bool glutMouse(int button, int state, int x, int y) { return false; };
	bool glutMotion(int x, int y) { return false; };
	bool glutPassiveMotion(int x, int y) { return false; };
	//
	// For DX9 or OGL
	// DX9 needs a device pointer. OGL doesn't : using the current context
	//
	long CreateUIResources(LPCSTR windowName, void *devicePtr=NULL) { return 0; };
	long ResetUIResources() { return 0; };   // OGL doesn't need it
	void LostUIResources() {};	// OGL doesn't need it
	void DestroyUIResources() {}; // OGL doesn't need it
	//
	// Events for DX10 (following the names of DX10 DXUTgui )
	//
	long OnD3D10CreateDevice(void *devitf) { return 0; };
	long OnD3D10ResizedSwapChain( void *devitf, int w, int h ) { return 0; };
	void OnD3D10ReleasingSwapChain() {};
	void OnD3D10DestroyDevice() {};
	/** When rendering the UI is necessary
	*/
	void RenderUI(float tick) {};
	//
	// Events for all the windows/controls in this handler
	//
	vector<IEventsWnd *> &GetRegisteredClients() // for internal use
	{ return m_pclients; }
	void Register(IEventsWnd *pClient)
	{
	  if(pClient)
		m_pclients.push_back(pClient);
	}
	void UnRegister(IEventsWnd *pClient=NULL)
	{
	  vector<IEventsWnd *>::iterator iCl;
	  iCl = m_pclients.begin();
	  while(iCl != m_pclients.end())
	  {
		if((*iCl) == pClient)
		{
		  *iCl = NULL;
		  return;
		}
		++iCl;
	  }
	}

	//
	// easy binding for variables (avoid managing them via events)
	//
	struct VBindStruct {
		VBindStruct() {pBaseContainer=NULL ; dim=0; ptr=NULL;}
		VBindStruct(CBindVariable* p, int d, void* p2, VarBindType t) { pBaseContainer=p ; dim=d; ptr=p2; type=t;}
		CBindVariable*  pBaseContainer;
		int		 dim;
		void*	   ptr;
		VarBindType type;
	};
	typedef std::map<void*, VBindStruct> VBind;
	VBind m_ptrBind;
	bool VariableUnbind(void *ptr);
	bool VariableFlush(void *ptr);
	bool VariableBind(IWindow *pIWin, float *vec, int dim=1);
	bool VariableBind(IWindow *pIWin, int *pVal, int dim=1);
	bool VariableBind(IWindow *pIWin, unsigned int *pVal, int dim=1);
	bool VariableBind(IWindow *pIWin, long *pVal, int dim=1);
	bool VariableBind(IWindow *pIWin, unsigned long *pVal, int dim=1);
	bool VariableBind(IWindow *pIWin, bool *pVal);
	bool VariableBind(IWindow *pIWin, char *pVal);
	bool VariableBind(IWindow *pIWin, char *string, int maxlen);

	void RefreshAll();

	friend class CButtonDlg;
	friend class CBooleanDlg;
	friend class CColorDlg;
	friend class CComboDlg;
	friend class ConsoleWnd;
	friend class CDlgProgress;
	friend class FoldingContainer;
	friend class CLogWnd;
	friend class CScalarDlg;
	friend class SimpleContainer;
	friend class SplitterContainer;
	friend class CStringDlg;
	friend class CTabWnd;
	friend class ToolBarWnd;
	friend class CVectorDlg;
};

extern WindowHandler g_WindowHandler;

#endif // !defined(AFX_WINDOWS_H__AA0BFC0B_D33C_4AF6_B9EA_AC02C6AA532F__INCLUDED_)

