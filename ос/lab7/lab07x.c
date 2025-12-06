#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

BOOL isPrime(int num) {
    if (num < 2) return FALSE;
    if (num == 2) return TRUE;
    if (num % 2 == 0) return FALSE;
    
    for (int i = 3; (i * i) <= num; i += 2) {
        if (num % i == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

int main(int argc, char** argv) {
    int workTimeSeconds;
    
    if (argc < 2) {
        printf("usage: lab07child.exe {time(s)}\n");
        return 1;
    } else {
        workTimeSeconds = atoi(argv[1]);
    }
    
    if (workTimeSeconds <= 0) {
        printf("work time must be positive\n");
        return 1;
    }

    LARGE_INTEGER frequency, startTime, endTime, targetTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&startTime);
    
    targetTime.QuadPart = startTime.QuadPart + (workTimeSeconds * frequency.QuadPart);
    
    const long maxPrimeCount = 100000000;
    int* primeNumber = (int*)malloc(sizeof(int) * maxPrimeCount);
    if (primeNumber == NULL) {
        printf("malloc fail\n");
        return 1;
    }
    
    for (int i = 0; i < maxPrimeCount; i++) {
        primeNumber[i] = 0;
    }
    
    int primeCounter = 0;
    int currentNumber = 0; 
    
    while (1) {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        
        if (currentTime.QuadPart >= targetTime.QuadPart) {
            break;
        }
        
        if (isPrime(currentNumber)) {
            if (primeCounter < maxPrimeCount) {
                primeNumber[primeCounter++] = currentNumber;
            }
        }
        
        currentNumber++;
    }
    
    QueryPerformanceCounter(&endTime);
    double elapsedTime = (double)(endTime.QuadPart - startTime.QuadPart) / frequency.QuadPart;

    
    for (int i = 0; i < primeCounter; i++) {
        if(i%300 == 0)
            printf("%d: %d |", i,primeNumber[i]);
    }
    
    

   
    printf("\nworked for: %.2f s\n", elapsedTime);
    free(primeNumber);
    return 0;
}