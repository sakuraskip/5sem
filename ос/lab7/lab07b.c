#include <windows.h>
#include <stdio.h>

int main() {
    LARGE_INTEGER frequency, start, current;
    long long counter = 0;
    BOOL fivePassed = FALSE;
    BOOL tenPassed = FALSE;
    BOOL fifteenPassed = FALSE;
    
    QueryPerformanceFrequency(&frequency);
    
    QueryPerformanceCounter(&start);
    
    while (1) {
        counter++;
        
        QueryPerformanceCounter(&current);
        
        double elapsed = (double)(current.QuadPart - start.QuadPart) / frequency.QuadPart;
        
        if (!fivePassed && elapsed >= 5.0) {
            printf("iterations after 5 seconds: %lld\n", counter);
            fivePassed = TRUE;
        }
        
        if (!tenPassed && elapsed >= 10.0) {
            printf("iterations after 10 seconds: %lld\n", counter);
            tenPassed = TRUE;
        }
        
        if (!fifteenPassed && elapsed >= 15.0) {
            printf("total iterations after 15 seconds: %lld\n", counter);
            fifteenPassed = TRUE;
            break; 
        }
    }
    
    return 0;
}