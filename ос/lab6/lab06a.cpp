
#include "windows.h"
#include <lexa2006>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
using namespace std;

CRITICAL_SECTION cs;
DWORD WINAPI threadFunc(LPVOID lpParam)
{
   
    int N = *(int*)lpParam;
    if ( N <=0 )
    {
        cerr << "{N} should be more than 0" << endl;
        return 1;
    }
    char name[] = "User-d4c889c4";
    int nameLength = sizeof(name)/sizeof(name[0]);
    int nameIter = 0;
 
    for(int i = 0; i <= N; i++)
    {
        if(i == 30)
        {
            EnterCriticalSection(&cs);
        }
        if(30 <= i AND 60 >= i)
        {
            printf("Critical section: %d - %d - %d - %c\n",getpid(),_threadid,i,name[nameIter++]);
        }
        else
        {
            printf("%d - %d - %d - %c\n",getpid(),_threadid,i,name[nameIter++]);
        }
        if(i == 60)
        {
            LeaveCriticalSection(&cs);
        }
        if( nameIter >= nameLength )
        {
            nameIter = 0;
        }
        Sleep(100);
    }

    return 0;

}
int main(int argc, char** argv)
{
    int parm = 90;
   
    DWORD dwThreadIdArray[2];
    HANDLE handles[2];
    InitializeCriticalSection(&cs);
    handles[0] = CreateThread(NULL,0,threadFunc,&parm,0,&dwThreadIdArray[0]);
    if( handles[0] == NULL )
    {
        cerr << "create thread 0 error" << endl;
        DeleteCriticalSection(&cs);
        ExitProcess(1);
    }
    handles[1] = CreateThread(NULL,0,threadFunc,&parm,0,&dwThreadIdArray[1]);
    if( handles[1] == NULL )
    {
        cerr << "create thread 1 error" << endl;
        DeleteCriticalSection(&cs);
        ExitProcess(1);
    }
    WaitForMultipleObjects(2,handles,TRUE,INFINITE);

    CloseHandle(handles[1]);
    CloseHandle(handles[0]);
    DeleteCriticalSection(&cs);

    return 0;
}