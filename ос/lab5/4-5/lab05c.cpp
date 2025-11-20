#include <windows.h>
#include <iostream>
#include <cstdlib>

using namespace std;

//1: lab05c.exe 4294967295 32768 0 0
//2: lab05c.exe 4294967295 32768 -2 2
//3: lab05c.exe 1 32768 -2 2

int main(int argc, char** argv)
{
    if (argc < 5)
    {
        cout << "usage: lab05b.exe {affinity mask} {parent priority} {priority1} {priority2}" << endl;
        return 1;
    }

    DWORD_PTR affinityMask = static_cast<DWORD_PTR>(atoi(argv[1]));
    int parentPriority = atoi(argv[2]);
    int priorityClass1 = atoi(argv[3]);
    int priorityClass2 = atoi(argv[4]);

    HANDLE hParent = GetCurrentProcess();
    if (!SetPriorityClass(hParent, parentPriority))
    {
        cerr << "set priority error" << endl;
        return 1;
    }

    STARTUPINFOA si1 = { sizeof(si1) };
    PROCESS_INFORMATION pi1 = {};
    STARTUPINFOA si2 = { sizeof(si2) };
    PROCESS_INFORMATION pi2 = {};

    if (!CreateProcessA("bin/lab05x.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si1, &pi1))
    {
        cerr << "Create process 1 fail " << endl;
        return 1;
    }
    SetPriorityClass(pi1.hProcess, priorityClass1);
    SetProcessAffinityMask(pi1.hProcess, affinityMask);

    if (!CreateProcessA("bin/lab05x.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si2, &pi2))
    {
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
        cerr << "Create process 2 fail" << endl;
        return 1;
    }
    SetPriorityClass(pi2.hProcess, priorityClass2);
    SetProcessAffinityMask(pi2.hProcess, affinityMask);

    CloseHandle(pi1.hThread);
    CloseHandle(pi1.hProcess);

    CloseHandle(pi2.hThread);
    CloseHandle(pi2.hProcess);

    return 0;
}