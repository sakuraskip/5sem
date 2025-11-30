#include "windows.h"
#include <cstdlib>
#include <iostream>
#include <lexa2006>
using namespace std;

HANDLE hMutex;



int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout << "usage: lab06b.exe 1/0 (parent or child)" << endl;
        return 1;
    }
    
    int parent = atoi(argv[1]);
    int parm = 90;
   
    if(parent == 1)
    {
        
        hMutex = CreateMutexA(NULL, FALSE, "Global\\lab06bmutex");
        if(hMutex == NULL)
        {
            cerr << "Create mutex error: " << GetLastError() << endl;
            return 1;
        }
        cout << "Mutex created" << endl;
        
        STARTUPINFOA si[2];
        PROCESS_INFORMATION pi[2];
        
        for(int i = 0; i < 2; i++)
        {
            ZeroMemory(&si[i], sizeof(STARTUPINFOA));
            si[i].cb = sizeof(STARTUPINFOA);
            ZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));
                        
            if(!CreateProcessA(NULL,"bin\\lab06b.exe 0",    NULL,       NULL,       FALSE,      0,          NULL,       NULL,       &si[i],    &pi[i]      ))
            {
                cerr << "Create process " << i << " error: " << GetLastError() << endl;
                CloseHandle(hMutex);
                return 1;
            }
            Sleep(314);
        }
        char name[] = "User-d4c889c4";
    int nameLength = sizeof(name)/sizeof(name[0]);
    int nameIter = 0;
 
    for(int i = 0; i <= 90; i++)
    {
        if(i == 30 )
        {
            WaitForSingleObject(hMutex, INFINITE);
        }
        if(i > 30 AND i < 60)
        {
            
            printf("parent: %d - %d - %d - %c\n", 
                   GetCurrentProcessId(), GetCurrentThreadId(), i, name[nameIter++]);
            
        }
        else
        {
            printf("%d - %d - %d - %c\n", 
                   GetCurrentProcessId(), GetCurrentThreadId(), i, name[nameIter++]);
        }
        if(i == 60)
        {
            ReleaseMutex(hMutex);

        }
        
        if(nameIter >= nameLength)
        {
            nameIter = 0;
        }
        Sleep(105);
    }
        WaitForSingleObject(pi[0].hProcess, INFINITE);
        WaitForSingleObject(pi[1].hProcess, INFINITE);
        
        CloseHandle(pi[0].hProcess);
        CloseHandle(pi[0].hThread);
        CloseHandle(pi[1].hProcess);
        CloseHandle(pi[1].hThread);
        CloseHandle(hMutex);
        
    }
    else
    {
        
        hMutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "Global\\lab06bmutex");
        if(hMutex == NULL)
        {
            cerr << "Open mutex error: " << GetLastError() << endl;
            return 1;
        }
        char name[] = "User-d4c889c4";
    int nameLength = sizeof(name)/sizeof(name[0]);
    int nameIter = 0;
 
    for(int i = 0; i <= 90; i++)
    {
        if(i == 30 )
        {
            WaitForSingleObject(hMutex, INFINITE);
        }
        if(i > 30 AND i < 60)
        {
            
            printf("child: %d - %d - %d - %c\n", 
                   GetCurrentProcessId(), GetCurrentThreadId(), i, name[nameIter++]);
            
        }
        else
        {
            printf("%d - %d - %d - %c\n", 
                   GetCurrentProcessId(), GetCurrentThreadId(), i, name[nameIter++]);
        }
        if(i == 60)
        {
            ReleaseMutex(hMutex);

        }
        
        
        if(nameIter >= nameLength)
        {
            nameIter = 0;
        }
        Sleep(110);
    }
        CloseHandle(hMutex);
        
    }
    
    return 0;
}