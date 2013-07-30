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
//----------------------------------------------------------------------------------
#ifndef __IDlgSettings__
#define __IDlgSettings__

// New in 3.16 : multiple columns in the Log window

#define WINDOWS_VERSION "3.16"

#include "ISvcBase.h"

//
// Declare a factory interface in your code and call these defines to load/unload the service: 
// nvSvc::ISvcFactory* g_pFact;
//
#ifdef _WIN64
#define SUFFIX64 "64_"
#else
#define SUFFIX64 "_"
#endif
#ifdef _DEBUG
#define UIDLLNAME TEXT("SvcMFCUID" SUFFIX64 WINDOWS_VERSION ".dll")
#define UIDLLNAME2 TEXT("SvcMFCUI" SUFFIX64 WINDOWS_VERSION ".dll")
#else
#define UIDLLNAME TEXT("SvcMFCUI" SUFFIX64 WINDOWS_VERSION ".dll")
#define UIDLLNAME2 TEXT("SvcMFCUID" SUFFIX64 WINDOWS_VERSION ".dll")
#endif
//TODO: remove these 2 defines...
#define UISERVICE_LOAD_()    FACTORY_LOAD(g_pFact, UIDLLNAME);
#define UISERVICE_UNLOAD_()    FACTORY_UNLOAD(g_pFact, UIDLLNAME);
// New defines :
#define UISERVICE_LOAD(pFact, pWinHandler)\
    if(!pFact) {\
        FACTORY_LOAD(pFact, UIDLLNAME);\
        if(pFact)\
          pWinHandler = (IWindowHandler*)pFact->GetSingletonOf("windowhandler");\
        else {\
            FACTORY_LOAD(pFact, UIDLLNAME2);\
            if(pFact)\
              pWinHandler = (IWindowHandler*)pFact->GetSingletonOf("windowhandler");\
            else {\
              pWinHandler = NULL;\
              }\
          }\
    }
#define UISERVICE_UNLOAD(pFact, pWinHandler)\
    pWinHandler = NULL;\
    if(pFact)    FACTORY_UNLOAD(pFact, UIDLLNAME);


using namespace nvSvc;

class IEventsWnd;
class IWindowContainer;
class IWindowFolding;
class IWindow;
class IWindowConsole;
class IWindowHandler;
class IWindowLog;
class IWindowSplitter;
class IControlCombo;
class IControlListBox;
class IControlScalar;
class IControlString;
class IControlLabel;
class IControlToolbar;
class IControlVector;
class IProgressBar;
class IControlStatic;
/*************************************************************************/ /**
 ** \brief Common Window definition
 **/ /*************************************************************************/
class IWindow : public ISmartRef //DO WE NEED Smartref ?
{
public:
    DECLSMARTREF(IWindow); ///< Macro for GetRef(), AddRef(), Release() , QueryInterface() implementations
    virtual IWindow* PeekMyself() = 0; ///< We should remove this...
    virtual IWindow* OnRender(float fElapsedTime) = 0; ///< in the case where the UI is rendered with 3D, for example
    virtual bool MsftMsgProc( void* hWnd, unsigned int uMsg, unsigned int wParam, unsigned long lParam ) = 0; ///< specific to Microsoft msg pump

    virtual IWindow* Minimize(int bYes=1) = 0;
    virtual void *GetCWnd() = 0; // void* because we don't want MS here...
    virtual void *GetHandle() = 0; // void* because we don't want MS here (hwnd)...

    virtual IWindow* Register(IEventsWnd *pDataBase) = 0;
    virtual IWindow* UnRegister(IEventsWnd *pClient=NULL) = 0;

    /*virtual bool CanHaveFocus() = 0;
    virtual void OnFocusIn() = 0;
    virtual void OnFocusOut() = 0;
    virtual void OnMouseEnter() = 0;
    virtual void OnMouseLeave() = 0;
    virtual void OnHotkey() = 0;*/

    virtual IWindow* SetEnabled( bool bEnabled ) = 0;
    virtual bool GetEnabled() = 0;
    virtual IWindow* SetVisible(int bYes=1) = 0;
    virtual int GetVisible() = 0; ///< bYes=-1 to query the state

    virtual unsigned int        GetType() = 0;

    virtual LPCSTR              GetID() const = 0;
    virtual IWindow* SetID( LPCSTR ID ) = 0;

    virtual IWindow* SetLocation(int x, int y) = 0;
    virtual IWindow* SetSize(int width, int height) = 0;
    virtual IWindow* GetPosition(int *x, int *y, int *width, int *height) = 0;

    virtual IWindow* SetHotkey( UINT nHotkey ) = 0;
    virtual UINT GetHotkey() = 0;
    virtual IWindow* SetUserData( void *pUserData, size_t UserData = 0 ) = 0;
    virtual void *GetUserData(size_t *lData=NULL) const = 0;

    virtual IWindow* SetZPos(int z=1) = 0;
    virtual IWindow* SetTitle(LPCSTR title) = 0;
    virtual IWindow* GetTitle(char * title, int maxlen) = 0;

    virtual IWindow* SetBGColor(float r, float g, float b, float a) = 0;
    virtual IWindow* SetBorderColor(float r, float g, float b) = 0;
    virtual IWindow* EnableBG(bool bYes) = 0;
    virtual IWindow* EnableBorders(bool bYes) = 0;
    //
    // links with parent (new from v1.37)
    //
    virtual IWindowContainer *GetParentContainer() = 0;
    virtual IWindow* SetParentContainer(IWindowContainer *pWin) = 0;
    //
    /// To destroy the window
	/// \note: this instance may still be referenced to various objects (RefCnt > 0).
    /// so the destroy status will release some resources and will set as invalid.
    /// Note that Destroy will notify registered users with the event 'rtDestroyingWindow', so this is the place where the user
    /// can release the references to it.
    /// (new from v1.37)
    //
    virtual IWindow* Destroy(bool bDestroyChildren = true) = 0;
    virtual bool IsValid() = 0; ///< false is this was destroyed. This can be used to release the reference to this window
    //
    /// \name "Cast" methods
    //@{
    virtual IWindowContainer    *AsWindowContainer() = 0;
    virtual IWindowFolding	*AsWindowFolding() = 0;
    virtual IWindowConsole      *AsWindowConsole() = 0;
    virtual IWindowHandler      *AsWindowHandler() = 0;
    virtual IWindowLog          *AsWindowLog() = 0;
    virtual IWindowSplitter     *AsWindowSplitter() = 0;
    virtual IProgressBar        *AsProgressBar() = 0;

    virtual IControlCombo       *AsControlCombo() = 0;
    virtual IControlListBox     *AsControlListBox() = 0;
    virtual IControlScalar      *AsControlScalar() = 0;
    virtual IControlString      *AsControlString() = 0;
    virtual IControlLabel       *AsControlLabel() = 0;
    virtual IControlToolbar     *AsControlToolbar() = 0;
    virtual IControlVector      *AsControlVector() = 0;
    virtual IControlStatic      *AsControlStatic() = 0;

    virtual IControlScalar      *AsControlBoolean() = 0;
    virtual IControlScalar      *AsControlCheck() = 0;
    virtual IWindow                *AsControlButton() = 0;
    virtual IControlVector      *AsControlColor() = 0;
	//@}
};


/*************************************************************************/ /**
	\brief events that must be implemented by the client if it wants to get back events
	example:
	\code
	class CEvent_tb3 : public IEventsWnd
	{
		virtual void ToolbarChanged(IControlToolbar *pWin, int selecteditem, int prev)
		{ ... }
		...
	}
	static CEvent_tb3 event_tb3;
	g_pToolbar3->Register(&event_tb3);
	\endcode
	
	But the event implementation can be used for many object and even for the whole
	set of windows in the handler : IWindowHandler also has IWindowHandler::Register
 **/ /*************************************************************************/
class IEventsWnd
{
public:
    virtual void MouseEvent(IWindow *pWin, float mx, float my, unsigned int buttons) {};
    virtual void ComboSelectionChanged(IControlCombo *pWin, unsigned int selectedidx) {};
    //virtual void RadioSelectionChanged(IControlScalar *pWin, unsigned int selectedidx) {};
    virtual void ListBoxSelection(IWindow *pWin, int selectstart, int subitem, bool doubleClick) {};
    virtual void Button(IWindow *pWin, int pressed) {};
    virtual void ColorPicker(IWindow *pWin, float *color, float *prev) {};
    virtual void VectorChanged(IControlVector *pWin, float *v, float *prev, int dim) {};
    virtual void ScalarChanged(IControlScalar *pWin, float &v, float prev) {};
    virtual void CheckBoxChanged(IControlScalar *pWin, bool &value, bool prev) {};
    virtual void ToolbarChanged(IControlToolbar *pWin, int selecteditem, int prev) {};
    virtual void StringChanged(IControlString *pWin) {};
    virtual void ConsoleChanged(IWindowConsole *pWin, LPCSTR newstring, LPCSTR prev) {};

    virtual bool WindowClosed(IWindow *) { return false; };
    virtual bool DestroyingWindow(IWindow *) { return false; }; ///< called when we asked for this window to be destroyed (1.37).
    virtual bool WindowResized(IWindow *, int cx, int cy) { return false; };
    virtual bool WindowRepaint(IWindow *) { return false; };
    virtual void WindowContainerChanged(IWindow *) { };
};

/*************************************************************************/ /**
 ** 
 ** \brief Progress bar you can use when loading etc.
 ** 
 **/ /*************************************************************************/
class IProgressBar : public IWindow
{
public:
    virtual IProgressBar* SetPercent(float l) = 0;
    virtual IProgressBar* AddPercent(float l) = 0;
    virtual IProgressBar* SetMessage(LPCSTR lpstr) = 0;
};

/*************************************************************************/ /**
 **
 ** \brief Static control : Just display text
 ** 
 **/ /*************************************************************************/
class IControlStatic : public IWindow
{
public:
    virtual bool GetTextCopy( LPSTR strDest, UINT bufferCount ) = 0;
    virtual LPCSTR GetText()= 0;
    virtual IControlStatic* SetText( LPCSTR strText ) = 0;
};

#define LOG_MSG        0    
#define LOG_WARNING    1
#define LOG_ERROR    2
#define LOG_NOTE    3
#define LOG_DISTRIB    4
#define LOG_NO        5
#define LOG_EVENT    6
#define LOG_YES        7
/*************************************************************************/ /**
 ** \brief Logging dlg box interface
 **
 ** various cases in the line:
 ** <LI> no header : common msg
 ** <LI> "<<warning>> message" : Warning msg
 ** <LI> "<<error>> message" : Error Msg
 ** <LI> "<<note>> message" : event Msg used by entities
 ** <LI> "<<distrib>> message" : Networking msg
 ** <LI> "<<no>> message" : negative Msg
 ** <LI> "<<event>> message" : event Msg
 ** <LI> "<<yes>> message" : yes Msg
 **/ /*************************************************************************/ 
class IWindowLog : public IWindow
{
public:
    virtual IWindowLog* Clear() = 0;
    virtual IWindowLog* AddMessage(LPCSTR fmt, ...) = 0;
    virtual IWindowLog* AddMessage(int level, LPCSTR fmt, ...) = 0;
    virtual int         GetNumItems() = 0;
    virtual IWindowLog* AddColumns(int n=1, int* psz=NULL, LPSTR *pstr=NULL) = 0;
    virtual IWindowLog* DelColumns(int n=1) = 0;
    virtual int         GetNumColumns() = 0;
    virtual IWindowLog* SetColumSize(int c, int sz) = 0;
};


/*************************************************************************/ /**
 ** \brief Cardiogram-like graphic.
 ** \todo create methods to display this timeline...
 ** <LI> ///italics///
 ** <LI> '''bold'''
 ** <LI> ___underline___
 ** <LI> col{FF0080} : color FF0080
 ** <LI> size{12} : font size 12
 ** <LI> n : return.
 **/ /*************************************************************************/ 
class IWindowConsole : public IWindow
{
public:
    virtual IWindowConsole* SetColor(unsigned long rgbcolor) = 0;
    virtual IWindowConsole* SetColor(unsigned char r, unsigned char g, unsigned char b) = 0;
    virtual IWindowConsole* SetItalic(bool bYes=true) = 0;
    virtual IWindowConsole* SetBold(int val=1) = 0;
    virtual IWindowConsole* SetUnderline(bool bYes=true) = 0;
    virtual IWindowConsole* SetFontSize(int sz=0) = 0;
    virtual IWindowConsole* SetFont(LPCSTR name=NULL) = 0;

    virtual IWindowConsole* Clear() = 0;
    virtual IWindowConsole* SetCursor(int x, int y) = 0;
    virtual IWindowConsole* SetCursorToEnd() = 0;
    virtual IWindowConsole* SetCursorToTop() = 0;
    virtual IWindowConsole* Printf(LPCSTR fmt, ... ) = 0;
    virtual IWindowConsole* Printf(int level, LPCSTR fmt, ... ) = 0;
};

/*************************************************************************/ /**
 ** \brief window container
 ** 
 **/ /*************************************************************************/ 
class IWindowContainer : public IWindow
{
public:
    virtual int GetNumItems() = 0;
    virtual IWindowContainer* AddItem(LPCSTR title, IWindow *lpWnd, bool bUpdateComponents=false) = 0;
    //virtual IWindowContainer* AddItem(LPCSTR title, LPCSTR windowname, bool bUpdateComponents=false) = 0;
    virtual IWindowContainer* RemoveItem(int Itemnum, IWindow **lpWnd=NULL, bool bUpdateComponents=false) = 0; 
    virtual IWindowContainer* RemoveItem(IWindow *lpWnd, bool bUpdateComponents=false) = 0;
    virtual IWindow*          GetItemNum(int Itemnum) = 0;
    virtual IWindowContainer* SelectItem(int Itemnum) = 0;
    virtual IWindowContainer* SelectItem(IWindow* pw) = 0;
    virtual void              Refresh() = 0; // recomputes the nested components so they look good
};
/*************************************************************************/ /**
 ** \brief window container
 ** 
 **/ /*************************************************************************/ 
class IWindowFolding : public IWindowContainer
{
public:
    virtual bool UnFold(bool bYes=true) = 0; // for container folding... some other may not answer to this request...
    virtual bool isUnFolded() = 0; // for container folding... some other may not answer to this request...
};
/*************************************************************************/ /**
 ** \brief window container with splitters
 ** 
 **/ /*************************************************************************/ 
class IWindowSplitter : public IWindowContainer
{
public:
    virtual int GetSplitterPos(int row) = 0;
    virtual IWindowSplitter* SetSplitterPos(int row, int y) = 0;
};
/*************************************************************************/ /**
 ** \brief Scalar value DlgBox : title, val, slider
 ** 
 **/ /*************************************************************************/ 
class IControlScalar : public IWindow
{
public:
    virtual LPCSTR          GetValueAsString() = 0;
    virtual IControlScalar* SetIntMode(bool bYes=true) = 0;

    virtual IControlScalar* SetValue(float s) = 0;
    virtual float           GetValue() = 0;

    virtual IControlScalar* SetBounds(float min, float max) = 0;
    virtual IControlScalar* SetStep(float s) = 0;
    // When we drive a checkbox:
    virtual bool            GetChecked() = 0;
    virtual IControlScalar* SetChecked( bool bChecked ) = 0;
};
/*************************************************************************/ /**
 ** \brief Vector 2,3,4 dimension
 ** 
 **/ /*************************************************************************/ 
class IControlVector : public IWindow
{
public:
    virtual IControlVector* SetIntMode(bool bYes=true) = 0;
    virtual IControlVector* SetDimension(int dim) = 0;
    virtual IControlVector* GetValuesAsInt(int *x, int *y=NULL, int *z=NULL, int *w=NULL) = 0;
    virtual IControlVector* GetValuesAsFloat(float *x, float *y=NULL, float *z=NULL, float *w=NULL) = 0;
    virtual IControlVector* GetValuesAsDouble(double *x, double *y=NULL, double *z=NULL, double *w=NULL) = 0;
    virtual IControlVector* SetValue(float x, float y, float z, float w) = 0;
    virtual IControlVector* SetValue(int n, float val) = 0;
};
/*************************************************************************/ /**
 ** \brief Simple 1 line string + optional file browser button
 ** 
 **/ /*************************************************************************/ 
class IControlString : public IWindow
{
public:
    virtual IControlString* SetString(LPCSTR str, bool bReplaceSelected=false) = 0;
    virtual IControlString* SelectString(int start, int len) = 0;
    virtual IControlString* ClearString(bool bOnlySelected=false) = 0;
    virtual LPCSTR          GetString() = 0;
    virtual LPCSTR          GetSelectedString() = 0;
    virtual int             GetStringLength() = 0;
    virtual LPCSTR          ShowFileBrowseButton(LPCSTR filter=NULL,int bYes=1) = 0; ///<filter can be "Text|*.txt|All|*.*||"
};

/*************************************************************************/ /**
 ** \brief Simple Label for static text
 ** 
 **/ /*************************************************************************/ 
class IControlLabel : public IWindow
{
public:
    virtual IControlLabel* SetString(LPCSTR fmt, ...) = 0;
};

/*************************************************************************/ /**
 ** \brief Combo Box Item used to get data from GetSelectedItem
 ** 
 **/ /*************************************************************************/ 
struct ComboBoxItem
{
    char strText[256];
    size_t data;

    RECT  rcActive;
    bool  bVisible;
};

/*************************************************************************/ /**
 ** \brief Combo Box : to choose among various strings
 ** 
 **/ /*************************************************************************/ 
class IControlCombo : public IWindow
{
public:
    virtual IControlCombo*    RemoveAllItems() = 0;
    virtual IControlCombo*    RemoveItem( UINT index, bool bUpdateComponents=false ) = 0;
    virtual IControlCombo*    SetDropHeight( UINT nHeight )  = 0;
    virtual IControlCombo*    SetScrollBarWidth( int nWidth )  = 0;
    virtual IControlCombo*    AddItem( const char* strText, size_t data ) = 0;

    virtual bool    ContainsItem( const char* strText, UINT iStart=0 ) = 0;
    virtual int     FindItem( const char* strText, UINT iStart=0 ) = 0;
    virtual size_t   GetItemData( const char* strText ) = 0;
    virtual size_t   GetItemData( int nIndex ) = 0;
    virtual int     GetScrollBarWidth() const  = 0;

    virtual int            GetSelectedIndex() const  = 0;
    virtual size_t  GetSelectedData() = 0;
    virtual ComboBoxItem*  GetSelectedItem() = 0;

    virtual UINT           GetNumItems() = 0;
    virtual ComboBoxItem*  GetItem( UINT index ) = 0;

    virtual IControlCombo* SetSelectedByIndex( UINT index ) = 0;
    virtual IControlCombo* SetSelectedByText( const char* strText ) = 0;
    virtual IControlCombo* SetSelectedByData( size_t data ) = 0;  
};

/*************************************************************************/ /**
 **
 ** \brief Toolbar
 ** 
 **/ /*************************************************************************/ 
class IControlToolbar : public IWindow
{
public:
    virtual IControlToolbar* AddItem(LPCSTR title, LPCSTR imagefilename = NULL, int ddsidx=0, int states=1, size_t tag=0) = 0;
    virtual IControlToolbar* AddItemFromMemoryBMP(LPCSTR title, void * imagedata, int states=1, size_t tag=0) = 0;
    virtual IControlToolbar* AddItemFromMemoryPNG(LPCSTR title, void * imagedata, int states=1, size_t tag=0) = 0;
    virtual IControlToolbar* AddItemFromMemoryTGA(LPCSTR title, void * imagedata, int states=1, size_t tag=0) = 0;
    virtual IControlToolbar* AddItemFromMemoryDDS(LPCSTR title, void * imagedata, int ddsidx, int states=1, size_t tag=0) = 0;
    virtual IControlToolbar* InsertItemFromMemoryBMP(int pos, LPCSTR title, void * imagedata, int states=1, size_t tag=0) = 0;
    virtual IControlToolbar* InsertItemFromMemoryPNG(int pos, LPCSTR title, void * imagedata, int states=1, size_t tag=0) = 0;
    virtual IControlToolbar* InsertItemFromMemoryTGA(int pos, LPCSTR title, void * imagedata, int states=1, size_t tag=0) = 0;
    virtual IControlToolbar* InsertItemFromMemoryDDS(int pos, LPCSTR title, void * imagedata, int ddsidx=0, int states=1, size_t tag=0) = 0;
    virtual IControlToolbar* DeleteItem(int pos) = 0;
    virtual IControlToolbar* DeleteItem(LPCSTR title) = 0;
    virtual IControlToolbar* ItemsSetState(int item, int state) = 0;
    virtual int              ItemsGetState(int item) = 0;
    virtual IControlToolbar* Vertical(int bYes=-1) = 0;
    virtual IControlToolbar* ShowText(bool bYes=true) = 0;
    virtual IControlToolbar* SetMinMaxHeight(int hmin, int hmax) = 0;
    virtual IControlToolbar* GetItemInfos(int item, int &states, size_t &tag,
                    char * title, int titlesz, 
                    char * imagefname, int imagefnamesz,
                    int &ddsidx) = 0;
    /// associate a python script to a button : pIPython is IPython class from SvcScripting.h
    virtual IControlToolbar* SetPythonInterface(void * pIPython) = 0;
    virtual IControlToolbar* SetItemPythonScriptEvent(int item, LPCSTR script) = 0;
    virtual LPCSTR GetItemPythonScriptEvent(int item) = 0;
};

/*************************************************************************/ /**
 ** \brief ListBoxItem used to pass data to a method of IControlListBox
 **/ /*************************************************************************/ 

struct ListBoxItem
{
    char strText[256];
    void*  pData;

    RECT  rcActive;
    bool  bSelected;
};

/*************************************************************************/ /**
 ** \brief ListBox
 **/ /*************************************************************************/ 
class IControlListBox : public IWindow
{
public:
    //virtual DWORD GetStyle() const { return m_dwStyle; }
    //virtual void SetStyle( DWORD dwStyle ) { m_dwStyle = dwStyle; }
    virtual int GetSize() const = 0;
    virtual int  GetScrollBarWidth() const = 0;
    virtual void SetScrollBarWidth( int nWidth ) = 0;
    virtual IControlListBox* SetBorder( int nBorder, int nMargin ) = 0;
    virtual IControlListBox* AddItem( const char *wszText, void *pData );
    virtual IControlListBox* InsertItem( int nIndex, const char *wszText, void *pData );
    virtual IControlListBox* RemoveItem( int nIndex, bool bUpdateComponents=false );
    virtual IControlListBox* RemoveItemByText( char *wszText, bool bUpdateComponents=false );
    virtual IControlListBox* RemoveItemByData( void *pData, bool bUpdateComponents=false );
    virtual IControlListBox* RemoveAllItems();

    virtual ListBoxItem *GetItem( int nIndex );
    virtual int GetSelectedIndex( int nPreviousSelected = -1 );
    virtual ListBoxItem *GetSelectedItem( int nPreviousSelected = -1 ) = 0;
    virtual IControlListBox* SelectItem( int nNewIndex );
};

/*************************************************************************/ /**
 ** \brief Singleton Interface of Windows message loop
 **/ /*************************************************************************/ 
class IWindowHandler : public ISmartRef
{
public:
    //
    // Get Interfaces of instanciated objects :
    //
    virtual IWindow*            Get(LPCSTR szID) = 0;
    virtual IWindowLog*         GetLog(LPCSTR szID) = 0;
    virtual IWindowConsole*     GetConsole(LPCSTR szID) = 0;
    virtual IWindowContainer*   GetContainer(LPCSTR szID) = 0;
    virtual IWindowFolding*     GetFoldingContainer(LPCSTR szID) = 0;
    virtual IWindowSplitter*    GetSplitter(LPCSTR szID) = 0;
    virtual IControlVector*     GetVector(LPCSTR szID) = 0;
//QUESTION: Shall we change the Class (IControlScalar) for Checkbox ?
    virtual IControlScalar*     GetCheck(LPCSTR szID) = 0;
    virtual IControlScalar*     GetScalar(LPCSTR szID) = 0;
    virtual IControlString*     GetString(LPCSTR szID) = 0;
    virtual IControlLabel*      GetLabel(LPCSTR szID) = 0;
    virtual IControlToolbar*    GetToolbar(LPCSTR szID) = 0;
    virtual IControlVector*     GetColor(LPCSTR szID) = 0;
    virtual IControlCombo*      GetCombo(LPCSTR szID) = 0;
//QUESTION: Shall we change the Class (IControlScalar) for Button ctrl ?
    virtual IControlScalar*     GetButton(LPCSTR szID) = 0;
    virtual IControlCombo*      GetRadio(LPCSTR szID) = 0;
    //
    // Remove/Add from the window handler (potentially, will destroy the window is not referenced anywhere else)
    //
    virtual bool            RemoveFromWindowHandler(LPCSTR szID = NULL) = 0;
    virtual bool            AddToWindowHandler(LPCSTR szID, IWindow *pWin) = 0;
    //
    // Create Objects (handled by WindowHandler object) :
    //
    virtual IWindowLog*         CreateWindowLog(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;
    virtual IWindowConsole*     CreateWindowConsole(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;
    virtual IWindowContainer*   CreateWindowContainer(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;
    virtual IWindowFolding*     CreateWindowFolding(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;
    virtual IWindowSplitter*    CreateWindowSplitter(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL, bool verticalSplitter=false) = 0;
    virtual IWindowContainer*   CreateWindowTab(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;
    virtual IProgressBar*       CreateWindowProgressBar(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;

    virtual IControlVector*     CreateCtrlVector(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;
    virtual IControlScalar*     CreateCtrlCheck(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;
    virtual IControlScalar*     CreateCtrlScalar(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;
    virtual IControlString*     CreateCtrlString(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;
    virtual IControlToolbar*    CreateCtrlToolbar(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;
    virtual IControlVector*     CreateCtrlColor(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;
    virtual IControlCombo*      CreateCtrlCombo(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;
    virtual IWindow*            CreateCtrlButton(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;
    virtual IControlCombo*      CreateCtrlRadio(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;
    virtual IControlListBox*    CreateCtrlListBox(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;
    virtual IControlLabel*      CreateCtrlLabel(LPCSTR szID, LPCSTR title, IWindowContainer * parent=NULL) = 0;

    virtual bool                Destroy(LPCSTR szID) = 0;
    virtual bool                DestroyAll() = 0;
    //
    // Events when no msg pump available
    //
    // return only when windows has finished.
    virtual unsigned int HandleMessageLoop_Blocking() = 0;
    // returns only wen a Key was pressed : Virtual Key code value returned
    virtual unsigned int HandleMessageLoop_ReturnOnKeyPressed() = 0;
    // returns At every event. Virtual Key Code returned. Or 0 otherwise.
    virtual unsigned int HandleMessageLoop_OnePass() = 0;
    // tell the message loop to quit (== WM_QUIT)...
    virtual void         PostMsgQuit() = 0;
    //
    // returns 0 if the method handled the message
    //
    virtual bool MsftMsgProc( void* hWnd, unsigned int uMsg, unsigned int wParam, unsigned long lParam ) = 0; ///< specific to Microsoft msg pump
    //
    // Glut event mgt
    //
    virtual void glutReshape(int w, int h) = 0;
    virtual void glutKeys(unsigned char c, int x, int y) = 0;
    virtual bool glutMouse(int button, int state, int x, int y) = 0;
    virtual bool glutMotion(int x, int y) = 0;
    virtual bool glutPassiveMotion(int x, int y) = 0;
    //
    // For DX9 or OGL
    // DX9 needs a device pointer. OGL doesn't : using the current context
    //
    virtual long CreateUIResources(LPCSTR windowName, void *devicePtr=NULL) = 0;
    virtual long ResetUIResources() = 0;   // OGL doesn't need it
    virtual void LostUIResources() = 0;    // OGL doesn't need it
    virtual void DestroyUIResources() = 0; // OGL doesn't need it
    //
    // Events for DX10 (following the names of DX10 DXUTgui )
    //
    virtual long OnD3D10CreateDevice(void *devitf) = 0;
    virtual long OnD3D10ResizedSwapChain( void *devitf, int w, int h ) = 0;
    virtual void OnD3D10ReleasingSwapChain() = 0;
    virtual void OnD3D10DestroyDevice() = 0;
    /** When rendering the UI is necessary
    */
    virtual void RenderUI(float tick) = 0;
    //
    // Events for all the windows/controls in this handler
    //
    virtual void Register(IEventsWnd *pDataBase) = 0;
    virtual void UnRegister(IEventsWnd *pClient=NULL) = 0;
    //
    // easy binding for variables (avoid managing them via events)
    //
    virtual bool VariableUnbind(void *ptr) = 0;
    virtual bool VariableFlush(void *ptr) = 0;
    virtual bool VariableBind(IWindow *pIWin, float *vec, int dim=1) = 0;
    virtual bool VariableBind(IWindow *pIWin, int *pVal, int dim=1) = 0;
    virtual bool VariableBind(IWindow *pIWin, unsigned int *pVal, int dim=1) = 0;
    virtual bool VariableBind(IWindow *pIWin, long *pVal, int dim=1) = 0;
    virtual bool VariableBind(IWindow *pIWin, unsigned long *pVal, int dim=1) = 0;
    virtual bool VariableBind(IWindow *pIWin, bool *pVal) = 0;
    virtual bool VariableBind(IWindow *pIWin, char *pVal) = 0;
    virtual bool VariableBind(IWindow *pIWin, char *string, int maxlen) = 0;

    virtual void RefreshAll() = 0;
};
//
// Lib entry when used as a library
//
extern IWindowHandler* GetWndMsgHandler();
#endif

/**
\file ISvcUI.h
\brief include files for instanciating SvcMFCUIxxx.dll

The dll that has the windows implementation is : \b SvcMFCUI64_3.14.dll (or other versions appended)
The version of the dll must match the version of this include file, otherwise the instanciation will fail.

Here is how to instanciate this dll service :
\code
IWindowHandler*	g_pWinHandler = NULL;
ISvcFactory*	g_pFact = NULL;
UISERVICE_LOAD(g_pFact, g_pWinHandler);
\endcode
which is the same as doing : 
\code
FACTORY_LOAD(pFact, "SvcMFCUIxxxx.dll"); // doing a GetProcAddress
pWinHandler = (IWindowHandler*)pFact->GetSingletonOf("windowhandler");
\endcode

Singleton you can get from the factory :
- "windowhandler"        : singleton used to handle windows and controls. Use it as the main entry point to create other objects
.

\note the objects here can be created by the windowhandler singleton. It is better to use it instead of creating the controls and windows from the factory : the window handler maintains a list of instanciated objects etc.

Objects you can instanciate with the factory :

- "controltoolbar"    : a toolbar
- "controlvector"     : a simple vector value editor
- "windowtab"         : container window with tabs
- "controlstring"     : a simple string value editor
- "controllabel"      : a simple string value editor
- "windowsplitter"    : splitter container
- "windowsplitterh"   : Horizontal splitter container
- "controlvalue"      : a simple scalar value editor
- "windowlog"         : logging window
- "windowfolding"     : folding container which aligns the controls vertically and allow you to open/close these items
- "progressbar"       : progress bar
- "windowconsole"     : console window
- "controlcombo"      : a combo box
- "color"             : a simple color picker. Acts as a 4D vector
- "check"             : a check box
- "window"            : simple container which aligns the controls vertically
- "button"            : a simple button
.

However these objects are more easily create by using the singletong windowhandler

The singleton "windowhandler" is a good way to maintain and create windows and controls.
Here is how to create a window handler:

\code
	g_pWinHandler = (IWindowHandler*)g_pFact->GetSingletonOf("windowhandler");
\endcode
Hoever the macro UISERVICE_LOAD does it for you:
\code
	ISvcFactory*	g_pFact = NULL;
	IWindowHandler*	g_pWinHandler = NULL;
	UISERVICE_LOAD(g_pFact, g_pWinHandler);
\endcode

and here is an example on how to create some windows and controls via the window handler:
\code
	(g_pMainCont1 = g_pWinHandler->CreateWindowContainer("MainCont1", "Main"))
		->SetSize(400-10,200)->SetLocation(1280 + 10 - 400,10)->SetVisible();
	(g_pToolbar1 = g_pWinHandler->CreateCtrlToolbar("ToolBar1", "Toolbar1", g_pMainCont1))
		->SetSize(200,20);
    g_pWinHandler->CreateCtrlCheck("LINES", "Draw Lines", g_pMainCont1);
    g_pWinHandler->VariableBind(g_pWinHandler->Get("LINES"), (unsigned long*)&g_bDrawLines);
	...
\endcode

*/