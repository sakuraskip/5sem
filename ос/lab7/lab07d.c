#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFO si1, si2;
    PROCESS_INFORMATION pi1, pi2;
    
    ZeroMemory(&si1, sizeof(si1));
    si1.cb = sizeof(si1);
    ZeroMemory(&pi1, sizeof(pi1));
    
    ZeroMemory(&si2, sizeof(si2));
    si2.cb = sizeof(si2);
    ZeroMemory(&pi2, sizeof(pi2));
    
    
    if (!CreateProcessA(NULL, "bin\\lab07x.exe 60", NULL, NULL, FALSE, 0, NULL, NULL, &si1, &pi1)) {
        printf("failed to create first process 2\n");
        return 1;
    }
    
    if (!CreateProcessA(NULL, "bin\\lab07x.exe 120", NULL, NULL, FALSE, 0, NULL, NULL, &si2, &pi2)) {
        printf("failed to create second process 1\n");
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
        return 1;
    }
    
    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hThread);
    
    printf("waiting for child processes\n");
    
    HANDLE procs[2] = { pi1.hProcess, pi2.hProcess };
    WaitForMultipleObjects(2, procs, TRUE, INFINITE);
    
    
   
    CloseHandle(pi1.hProcess);
    CloseHandle(pi2.hProcess);
    
    printf("parent process finished\n");
    return 0;
}