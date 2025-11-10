#include <windows.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string>
#define MAX_THREADS 2
using namespace std;

DWORD WINAPI lab04x(LPVOID lpParam)
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
        printf("%d - %d - %d - %c\n",getpid(),_threadid,i,name[nameIter++]);
        if( nameIter >= nameLength )
        {
            nameIter = 0;
        }
        Sleep(300);
    }

    return 0;

}
int main(int argc, char** argv)
{
    int parm = 100;
    int parm1=50;
    int parm2 = 125;
    DWORD dwThreadIdArray[MAX_THREADS];
    HANDLE handles[MAX_THREADS];

    handles[0] = CreateThread(NULL,0,lab04x,&parm1,0,&dwThreadIdArray[0]);
    if( handles[0] == NULL )
    {
        cerr << "create thread 0 error" << endl;
        ExitProcess(1);
    }
    handles[1] = CreateThread(NULL,0,lab04x,&parm2,0,&dwThreadIdArray[1]);
    if( handles[1] == NULL )
    {
        cerr << "create thread 1 error" << endl;
        ExitProcess(1);
    }
    lab04x(&parm);
    if( WaitForMultipleObjects(MAX_THREADS,handles,TRUE,60000) == WAIT_FAILED )
    {
        cerr << "wait for threads error" << endl;
        return 1;
    }
    for(int i = 0;i < MAX_THREADS;i++)
    {
        CloseHandle(handles[i]);
        cout << "closed thread " << i << endl;
    }
    return 0;
}