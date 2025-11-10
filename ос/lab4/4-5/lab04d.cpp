#include <windows.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <tlhelp32.h>
#define MAX_THREADS 2
using namespace std;

int main()
{
    DWORD current_pid = GetCurrentProcessId();

    HANDLE threadsnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);

    THREADENTRY32 thread;
    thread.dwSize = sizeof(THREADENTRY32);

    if( !Thread32First(threadsnap,&thread) )
    {
        cerr << "thread32first error" << endl;
        CloseHandle(threadsnap);
        return 1;
    }

    while(Thread32Next(threadsnap,&thread))
    {
        if(thread.th32OwnerProcessID == current_pid)
        {
            cout << "thread id: " << thread.th32ThreadID << endl;
        }
    }
    CloseHandle(threadsnap);
    return 0;
}