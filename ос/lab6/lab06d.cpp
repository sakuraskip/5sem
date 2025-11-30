#include "windows.h"
#include <cstdlib>
#include <iostream>
using namespace std;

HANDLE hEvent;

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout << "usage: lab06c.exe 1/0 (parent or child)" << endl;
        return 1;
    }
    
    int parent = atoi(argv[1]);
    int parm = 90;
   
    if(parent == 1)
    {
        hEvent = CreateEventA(NULL, TRUE, FALSE, "Global\\lab06d_event");
        if(hEvent == NULL)
        {
            cerr << "Create event error: " << GetLastError() << endl;
            return 1;
        }
        cout << "Event created" << endl;
        
        STARTUPINFOA si[2];
        PROCESS_INFORMATION pi[2];
        
        for(int i = 0; i < 2; i++)
        {
            ZeroMemory(&si[i], sizeof(STARTUPINFOA));
            si[i].cb = sizeof(STARTUPINFOA);
            ZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));
                        
            if(!CreateProcessA(NULL, "bin\\lab06d.exe 0", NULL, NULL, FALSE, 0, NULL, NULL, &si[i], &pi[i]))
            {
                cerr << "Create process " << i << " error: " << GetLastError() << endl;
                CloseHandle(hEvent);
                return 1;
            }
        }
        
        char name[] = "User-d4c889c4";
        int nameLength = sizeof(name)/sizeof(name[0]);
        int nameIter = 0;
 
        for(int i = 0; i < 15; i++)
        {
            printf("Parent %d - %d - %d - %c\n", 
                   GetCurrentProcessId(), GetCurrentThreadId(), i, name[nameIter++]);
            
            if(nameIter >= nameLength)
            {
                nameIter = 0;
            }
            Sleep(105);
        }
        
        SetEvent(hEvent);
        
        for(int i = 15; i <= 90; i++)
        {
            printf("Parent 15 %d - %d - %d - %c\n", 
                   GetCurrentProcessId(), GetCurrentThreadId(), i, name[nameIter++]);
            
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
        CloseHandle(hEvent);
        
    }
    else
    {
        hEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, "Global\\lab06d_event");
        if(hEvent == NULL)
        {
            cerr << "Open event error: " << GetLastError() << endl;
            return 1;
        }
        
        WaitForSingleObject(hEvent, INFINITE);
        
        char name[] = "User-d4c889c4";
        int nameLength = sizeof(name)/sizeof(name[0]);
        int nameIter = 0;
 
        for(int i = 0; i <= 90; i++)
        {
            printf("Child: %d - %d - %d - %c\n", 
                   GetCurrentProcessId(), GetCurrentThreadId(), i, name[nameIter++]);
            
            if(nameIter >= nameLength)
            {
                nameIter = 0;
            }
            Sleep(110);
        }
        
        CloseHandle(hEvent);
    }
    
    return 0;
}