#include <windows.h>
#include <iostream>
#include <ctime>

using namespace std;

int main()
{
     int totalIterations = 200000;
     int interval = 1000;
     int delayMs = 200;

    int pid = GetCurrentProcessId();
    int tid = GetCurrentThreadId();
    HANDLE hProcess = GetCurrentProcess();
    HANDLE hThread = GetCurrentThread();

    clock_t startTime = clock();

    for (int i = 1; i <= totalIterations; i++)
    {
        if (i % interval == 0)
        {
            Sleep(delayMs);

            int processPriorityClass = GetPriorityClass(hProcess);
            int threadPriority = GetThreadPriority(hThread);
            int processorNumber = GetCurrentProcessorNumber();

            cout << "i: " << i << endl;
            cout << "PID: " << pid << endl;
            cout << "TID: " << tid << endl;
            cout << "process priority class: " << processPriorityClass << endl;
            cout << "thread priority: " << threadPriority << endl;
            cout << "process numba: " << processorNumber << endl;
        }
    }

    clock_t endTime = clock();
    double res = double(endTime - startTime) / 1000;

    cout << "time: " << res << " seconds" << endl;
    system("pause");

    return 0;
}