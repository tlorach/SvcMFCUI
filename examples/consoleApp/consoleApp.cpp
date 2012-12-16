// serverExample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ISVCUI.h"

nvSvc::ISvcFactory *g_pFactUI = NULL;
IWindowHandler     *g_pWinHandler = NULL;
IWindowConsole     *g_pConsole = NULL;
IWindowLog         *g_pLog = NULL;
IWindowFolding     *g_pControls = NULL;
IProgressBar       *g_pProgress = NULL;

float fval1 = 1.2f;
float fval2 = 1.4f;
int ival1 = 2;
bool bval1 = true;
bool bval2 = false;

class ControlsEvents : public IEventsWnd
{
    void ScalarChanged(IControlScalar *pWin, float &v, float prev)
    {
    };
    void Button(IWindow *pWin, int pressed) 
    {
        if(!strcmp(pWin->GetID(), "Q"))
        {
            exit(0);
        }
    };
    void ToolbarChanged(IControlToolbar *pWin, int selecteditem, int prev) 
    {
    }
    void CheckBoxChanged(IControlScalar *pWin, bool &value, bool prev)
    {
        g_pLog->AddMessage("CheckBox Changed");
        g_pConsole->Printf("CheckBox Changed\n");
    }
    void ComboSelectionChanged(IControlCombo *pWin, unsigned int selectedidx)
    {
    };
};
static ControlsEvents controlsEvents;

/////////////////////////////////////////////////////////////////////
//
//
int _tmain(int argc, _TCHAR* argv[])
{
    UISERVICE_LOAD(g_pFactUI, g_pWinHandler);
    if(g_pWinHandler)
    {
        g_pControls =  g_pWinHandler->CreateWindowFolding("CTRLS", "Controls");
        g_pControls->SetVisible(1)->SetLocation(30,0)->SetSize(500,400);
        {
            g_pConsole = g_pWinHandler->CreateWindowConsole("CONSOLE", "Log");
            g_pConsole->SetVisible()->SetLocation(530,0)->SetSize(500,400);
            g_pLog = g_pWinHandler->CreateWindowLog("LOG", "Log", g_pControls);

            g_pWinHandler->CreateCtrlButton("Q", "Exit", g_pControls);
            g_pWinHandler->CreateCtrlButton("KUI", "Kill UI", g_pControls);
            // example on how to create a specialized implementation for one guy
            class TestMessageEvent : public IEventsWnd
            {
                void Button(IWindow *pWin, int pressed) 
                {
                    g_pLog->AddMessage("Test Message");
                    g_pConsole->Printf("Test Message\n");
                    IControlScalar* p = g_pWinHandler->GetCheck("TAG3");
                    p->SetValue(true);
                }
            };
            static TestMessageEvent sTestMessageEvent;
            g_pWinHandler->CreateCtrlButton("A", "Test Message", g_pControls)
                ->Register(&sTestMessageEvent);
            // boundaries
            IWindowFolding* pWF =  g_pWinHandler->CreateWindowFolding("BOUNDS", "Graphs", g_pControls);
            {
                g_pWinHandler->VariableBind(
                    g_pWinHandler->CreateCtrlScalar("BMAX", "Scale", pWF)->SetBounds(-1.0,2.0)->SetUserData((void*)2,0)
                    , &fval1, 1);
                g_pWinHandler->VariableBind(
                    g_pWinHandler->CreateCtrlScalar("BIAS", "Bias", pWF)->SetBounds(-2.0,2.0)->SetUserData((void*)3,0)
                    , &fval2, 1);
                g_pWinHandler->CreateCtrlScalar("RCYCLE", "Cycle", pWF)->SetIntMode()->SetValue(1.0)->SetBounds(1.0,10.0)->SetUserData((void*)4,0);
                g_pWinHandler->VariableBind(g_pWinHandler->CreateCtrlScalar("TAG1", "Num Blah", pWF)->SetIntMode()->SetBounds(1.0,15.0)->SetUserData((void*)5,0), &ival1);
                g_pWinHandler->VariableBind(g_pWinHandler->CreateCtrlCheck("TAG2", "Toggle", pWF)->SetUserData((void*)1,0), &bval1);
                g_pWinHandler->CreateCtrlCheck("FS", "FullScreen", pWF)->SetUserData((void*)2,0);
                g_pWinHandler->VariableBind(g_pWinHandler->CreateCtrlCheck("TAG3", "control", pWF)->SetUserData((void*)3,0), &bval2);
            }
            // sliders for testing SM values
            pWF =  g_pWinHandler->CreateWindowFolding("Tag3", "Items", g_pControls);
            for(int i=0; i<6; i++)
            {
                char name[10];
                sprintf(name, "Item#%d", i);
                g_pWinHandler->CreateCtrlScalar((LPCSTR)(100+i), name, pWF)->SetBounds(0.0,15.0)->SetUserData((void*)1,i);
            }
        }
        g_pWinHandler->Register(&controlsEvents);
        g_pControls->UnFold(true);
    }
    else
        printf("No UI service (no dll) !\n");

    // Main Loop
    // TODO: handle Ctrl-C
    if(g_pLog)
        g_pLog->AddMessage("Entering a loop for the UI");
    else
        printf("Entering a loop\n");
    static float t = 0.0f;
    do {
        if(g_pWinHandler)
            g_pWinHandler->HandleMessageLoop_OnePass();
        Sleep(1000/60);
        t += 0.01f;
        fval1 = (float)cosf(t);
        // now flush the variable in the UI... connected stuff will get updated
        if(g_pWinHandler) g_pWinHandler->VariableFlush(&fval1);
        fval2 = (float)cosf(t);
        if(g_pWinHandler) g_pWinHandler->VariableFlush(&fval2);
        bval1 = fval2>0.0f ? true : false;
        if(g_pWinHandler) g_pWinHandler->VariableFlush(&bval1);

        printf("fval1 = %f\n", fval1);
        printf("fval2 = %f\n", fval2);
        printf("bval1 = %s\n", bval1 ? "true":"false");
        if((!g_pWinHandler)&&(t > 2.0f))
            break; // artificially break if no UI, after few iterations
    } while(1);
    return 0;
}

