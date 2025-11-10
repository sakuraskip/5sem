#include <iostream>
#include <windows.h>
#include <string>
#include <chrono>   // добавляем для замера времени

using namespace std;

/*
https://learn.microsoft.com/ru-ru/windows/win32/ProcThread/creating-a-child-process-with-redirected-input-and-output
 helps a lot  */ 
int main(int argc, char** argv)
{
    int processAmount, from, to;
    if (argc < 4)
    {
        cerr << "usage: {processes amount} {from} {to}" << endl;
        return 1;
    }
    processAmount = atoi(argv[1]);
    from = atoi(argv[2]);
    to = atoi(argv[3]);

    if (processAmount <= 0) // keep the number less than 10
    {
        cerr << "processAmount is 0 or less" << endl;
        return 1;
    }
    if (to < from)
    {
        cerr << "{to} parameter should be more than {from}" << endl;
        return 1;
    }
    int totalNums = to - from + 1;
    int numsForOneProcess = totalNums / processAmount;
    int numsRemained = totalNums - (numsForOneProcess * processAmount);

    PROCESS_INFORMATION pi[processAmount] = {};
    HANDLE handles[processAmount] = {};
    HANDLE readPipes[10] = {};

    int newFrom = from;

    auto startTime = chrono::high_resolution_clock::now();

    for (int i = 0; i < processAmount; i++)
    {
        int newTo = newFrom + numsForOneProcess - 1;
        if (i == processAmount - 1)
        {
            newTo += numsRemained;
        }

        SECURITY_ATTRIBUTES sa = {};
        sa.nLength = sizeof(sa);
        sa.bInheritHandle = TRUE;
        sa.lpSecurityDescriptor = NULL;

        HANDLE childReadPipe;
        HANDLE childWritePipe;
        if (!CreatePipe(&childReadPipe, &childWritePipe, &sa, 0))
        {
            cerr << "create pipe error " << endl;
            return 1;
        }
        if (!SetHandleInformation(childReadPipe, HANDLE_FLAG_INHERIT, 0))
        {
            cerr << "sethandleinformation error " << endl;
            return 1;
        }
        PROCESS_INFORMATION localPI;
        STARTUPINFOA si;
        ZeroMemory(&localPI, sizeof(PROCESS_INFORMATION));
        ZeroMemory(&si, sizeof(STARTUPINFO));

        si.cb = sizeof(STARTUPINFO);
        si.hStdOutput = childWritePipe;
        si.hStdError = childWritePipe;
        si.dwFlags |= STARTF_USESTDHANDLES;

        string prompt = "child.exe " + to_string(newFrom) + " " + to_string(newTo);
        if (!CreateProcessA(NULL, (LPSTR)prompt.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &localPI))
        {
            cerr << "create process error " << endl;

            return 1;
        }

        cout << "process " << i << " created" << endl;
        CloseHandle(childWritePipe);
        pi[i] = localPI;
        handles[i] = localPI.hProcess;
        readPipes[i] = childReadPipe;

        newFrom = newTo + 1;
    }
    auto endTime = chrono::high_resolution_clock::now();
    for (int j = 0; j < processAmount; j++)
    {
        cout << "process " << j << " :" << endl;
        DWORD dwRead;
        CHAR chbuf[BUFSIZ];
        HANDLE hReadPipe = readPipes[j];
        BOOL bSuccess = FALSE;

        for (;;)
        {
            bSuccess = ReadFile(hReadPipe, chbuf, BUFSIZ - 1, &dwRead, NULL);
            if (!bSuccess || dwRead == 0) break;
            chbuf[dwRead] = '\0';
            cout << chbuf;
        }
        cout << endl;
        CloseHandle(hReadPipe);
    }

    WaitForMultipleObjects(processAmount, handles, TRUE, 60000);
    for (int i = 0; i < processAmount; i++)
    {
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
        cout << "closed process numba " << i << endl;
    }

    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    cout << "time calculating: " << duration.count() << " ms" << endl;

    return 0;
}