#include <windows.h>
#include <iostream>
#include <bitset>

using namespace std;

int main()
{
    int pid = GetCurrentProcessId();
    int tid = GetCurrentThreadId();

    HANDLE hProcess = GetCurrentProcess();
    HANDLE hThread = GetCurrentThread();

    int processPriorityClass = GetPriorityClass(hProcess);
    if (processPriorityClass == 0)
    {
        cerr << "get priority class fail" << endl;
        return 1;
    }

    int threadPriority = GetThreadPriority(hThread);
    if (threadPriority == THREAD_PRIORITY_ERROR_RETURN)
    {
        cerr << "Failed to get thread priority." << endl;
        return 1;
    }

    DWORD_PTR processAffinityMask = 0, systemAffinityMask = 0;
    if (!GetProcessAffinityMask(hProcess, &processAffinityMask, &systemAffinityMask))
    {
        cerr << "Failed to get process affinity mask." << endl;
        return 1;
    }

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    int processorCount = sysInfo.dwNumberOfProcessors;

    int processorNumber = GetCurrentProcessorNumber();

    cout << "pid: " << pid << endl;
    cout << "tid: " << tid << endl;

    cout << "process priority class: " << processPriorityClass << endl;

    cout << "thread priority: " << threadPriority << endl;
    
    cout << "process affinity mask (bin): " << bitset<16>(processAffinityMask) << endl;
    
    cout << "system affinity mask (bin): " << bitset<16>(systemAffinityMask) << endl;
    
    cout << "available processors count: " << processorCount << endl;
    cout << "process number for current thread: " << processorNumber << endl;

    return 0;
}