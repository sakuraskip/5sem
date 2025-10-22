#include <iostream>
#include <windows.h>
#include <string.h>
using namespace std;

int main(int argc, char** argv)
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    

    ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));
    ZeroMemory(&si,sizeof(STARTUPINFOA));

    si.cb = sizeof(si);
    si.lpTitle = "hi dear child process";

   
    if( !CreateProcessA("child.exe",NULL,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi))
    {
        cerr << "create process fail " << endl;
        return 1;
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    system("pause");
}