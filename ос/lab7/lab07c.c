#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
    
    if (!hTimer) {
        printf("createwaitable timer fail\n");
        return 1;
    }
    
    LARGE_INTEGER sometime;
    sometime.QuadPart = -30000000; 
    
    SetWaitableTimer(hTimer, &sometime, 3000, NULL, NULL, FALSE);
    
    long long counter = 0;
    int seconds = 0;
    
    while (seconds < 15) {
        counter++;
        
        DWORD result = WaitForSingleObject(hTimer, 0);
        if (result == WAIT_OBJECT_0) {
            seconds += 3;
            printf("iterations after %d seconds: %lld\n", seconds, counter);
        }
    }
    
    printf("total iterations: %lld\n", counter);
    
    CloseHandle(hTimer);
    return 0;
}