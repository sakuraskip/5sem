#include <iostream>
#include <unistd.h>
#include <windows.h>

using namespace std;

int main(int argc, char** argv)
{
   
        STARTUPINFOA si;
        PROCESS_INFORMATION pi,pi2,pi3;

        HANDLE handles[3];

        int processes = 0;
        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        ZeroMemory( &pi, sizeof(pi) );
        //will not launch, parameters cant be included in applicationName parameter
        if(CreateProcessA("lab03.exe 10",NULL,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi)) 
        {
            cout << "process 1 created" << endl;
            handles[processes++] = pi.hProcess;
        } else cout << "process 1 failed" << endl;
        _sleep(514);
        if(CreateProcessA(NULL,"lab03.exe 10",NULL,NULL,FALSE,0,NULL,NULL,&si,&pi2))
        {
            cout << "process 2 created" << endl;
            handles[processes++] = pi2.hProcess;

        } else cout << "process 2 failed" << endl;
        
        _sleep(511);
        if(CreateProcessA("lab03.exe"," 10",NULL,NULL,FALSE,0,NULL,NULL,&si,&pi3))
        {
            cout << "process 3 created" << endl;
            handles[processes++] = pi3.hProcess;

        } else cout << "process 1 failed" << endl;
    
        if(WaitForMultipleObjects(processes,handles,TRUE,60000) == WAIT_FAILED)
        {
            cout << "wait failed";
        }
        else cout << "wait completed" << endl;

        if(pi.hProcess != NULL)
        {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
         if(pi2.hProcess != NULL)
        {
            CloseHandle(pi2.hProcess);
            CloseHandle(pi2.hThread);
        } if(pi3.hProcess != NULL)
        {
            CloseHandle(pi3.hProcess);
            CloseHandle(pi3.hThread);
        }
    

}