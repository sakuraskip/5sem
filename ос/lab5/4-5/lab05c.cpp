#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <chrono>
using namespace std::chrono;
using namespace std;


DWORD WINAPI ThreadFunction(LPVOID lpParam)
{
    int* params = (int*)lpParam;
    int priority = params[0];
    DWORD_PTR affinityMask = (DWORD_PTR)params[1];
    int threadNum = params[2];
    
    int totalIterations = 200000;
    int interval = 1000;
    int delayMs = 200;

    int pid = GetCurrentProcessId();
    int tid = GetCurrentThreadId();
    HANDLE hThread = GetCurrentThread();

    SetThreadPriority(hThread, priority);
    SetThreadAffinityMask(hThread, affinityMask);

    clock_t startTime = clock();

    for (int i = 1; i <= totalIterations; i++)
    {
        if (i % interval == 0)
        {
            Sleep(delayMs);

            int processPriorityClass = GetPriorityClass(GetCurrentProcess());
            int threadPriority = GetThreadPriority(hThread);
            int processorNumber = GetCurrentProcessorNumber();

            cout << "Thread " << threadNum << " - i: " << i << endl;
            cout << "PID: " << pid << endl;
            cout << "TID: " << tid << endl;
            cout << "process priority class: " << processPriorityClass << endl;
            cout << "thread priority: " << threadPriority << endl;
            cout << "process numba: " << processorNumber << endl;
            cout << "------------------------" << endl;
        }
    }

    clock_t endTime = clock();
    double res = double(endTime - startTime) / CLOCKS_PER_SEC;

    cout << "thread " << threadNum << " time: " << res << " seconds" << endl;
    
    delete[] params;
    return 0;
}

int main(int argc, char** argv)
{
    if (argc < 5)
    {
        cout << "usage: lab05b.exe {affinity mask} {parent priority} {priority1} {priority2}" << endl;
        return 1;
    }

    DWORD_PTR affinityMask = static_cast<DWORD_PTR>(_atoi64(argv[1]));
    int parentPriority = atoi(argv[2]);
    int priority1 = atoi(argv[3]);
    int priority2 = atoi(argv[4]);

    HANDLE hParent = GetCurrentProcess();
    if (!SetPriorityClass(hParent, parentPriority))
    {
        cerr << "set priority error" << endl;
        return 1;
    }

    int* params1 = new int[3];
    params1[0] = priority1;
    params1[1] = (int)affinityMask;
    params1[2] = 1;

    int* params2 = new int[3];
    params2[0] = priority2;
    params2[1] = (int)affinityMask;
    params2[2] = 2;

    HANDLE hThread1 = CreateThread(NULL, 0, ThreadFunction, params1, 0, NULL);
    if (hThread1 == NULL)
    {
        cerr << "Create thread 1 fail " << endl;
        delete[] params1;
        return 1;
    }

    HANDLE hThread2 = CreateThread(NULL, 0, ThreadFunction, params2, 0, NULL);
    if (hThread2 == NULL)
    {
        cerr << "Create thread 2 fail" << endl;
        CloseHandle(hThread1);
        delete[] params1;
        delete[] params2;
        return 1;
    }

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    CloseHandle(hThread1);
    CloseHandle(hThread2);

    cout << "keypress?......??????" << endl;
    system("pause");

    return 0;
}