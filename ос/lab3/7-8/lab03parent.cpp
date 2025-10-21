#include <iostream>
#include <unistd.h>
#include <windows.h>
#include <string.h>
using namespace std;


int main(int argc, char** argv)
{

   int processAmount,from,to;
   if(argc < 4)
   {
        cerr << "usage: {processes amount} {from} {to}" << endl;
        return 1;
   }
   processAmount = atoi(argv[1]);
    from = atoi(argv[2]);
    to = atoi(argv[3]);

    if(processAmount <= 0)
    {
        cerr << "processAmount is 0 or less" << endl;
        return 1;
    }
    if(to < from)
    {
        cerr << "{to} parameter should be more than {from}" << endl;
        return 1;
    }
    int totalNums = to - from;
    int numsForOneProcess = totalNums/processAmount;
    int numsRemained = totalNums - (numsForOneProcess* processAmount);

    PROCESS_INFORMATION pi[processAmount];
    HANDLE handles[processAmount];

    int newFrom = from;
    for(int i=0;i<processAmount;i++)
    {
        int newTo = newFrom+numsForOneProcess;
        if(i == processAmount -1)
        {
            newTo +=numsRemained;
        }

        SECURITY_ATTRIBUTES sa = {};
        sa.nLength=sizeof(sa);
        sa.bInheritHandle=TRUE;
        sa.lpSecurityDescriptor = NULL;

        HANDLE childReadPipe;
        HANDLE childWritePipe;
        if(!CreatePipe(&childReadPipe,&childWritePipe,&sa,0))
        {
            cerr << "create pipe error " << endl;
            return 1;
        }
        if(!SetHandleInformation(childReadPipe,HANDLE_FLAG_INHERIT,0))
        {
            cerr << "sethandleinformation error " << endl;
            return 1;
        }
        PROCESS_INFORMATION pi;
        STARTUPINFO si;
        ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));
        ZeroMemory(&si,sizeof(STARTUPINFO));

        si.cb = sizeof(STARTUPINFO);
        si.hStdOutput = childWritePipe;
        si.hStdError = childWritePipe;
        si.dwFlags  |=STARTF_USESTDHANDLES;
        string prompt = "child.exe "+ to_string(newFrom) + " " + to_string(newTo);
        if(!CreateProcess(NULL,(LPWSTR)prompt.c_str(),NULL,NULL,TRUE,0,NULL,NULL,&si,&pi))
        {
            cerr << "create process error " << endl;

            return 1;
        }
        // probably fine but idk, two places said different things
        // CloseHandle(pi.hProcess);
        // CloseHandle(pi.hThread); 
        cout << "process " << i << " created" << endl;
        CloseHandle(childWritePipe);

    }

   
}