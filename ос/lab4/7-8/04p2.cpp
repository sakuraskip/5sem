#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <chrono>

#define MAX_PRIMES 10000000
#define MAX_PRIMES_PER_THREAD 5000000
using namespace std;

int primes[MAX_PRIMES];
int primesCount = 0;
CRITICAL_SECTION cs;

thread_local int* tlsBuff = nullptr;
thread_local int lCount = 0;

bool IsPrime(int n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0) return false;
    }
    return true;
}

DWORD WINAPI Func(LPVOID lpParam)
{
    int* range = (int*)lpParam;
    int start = range[0];
    int end = range[1];
    delete[] range;

    tlsBuff = new int[MAX_PRIMES_PER_THREAD];
    lCount = 0;

    for (int i = start; i <= end; i++)
    {
        if (IsPrime(i))
        {
            if (lCount < MAX_PRIMES_PER_THREAD)
            {
                tlsBuff[lCount++] = i;
            }
            else break;
        }
    }

    EnterCriticalSection(&cs);
    for (int i = 0; i < lCount; i++)
    {
        primes[primesCount++] = tlsBuff[i];
    }
    LeaveCriticalSection(&cs);

    delete[] tlsBuff;
    tlsBuff = nullptr;
    lCount = 0;

    return 0;
}

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        cerr << "usage: l4primes.exe {threadCount} {start} {end}" << endl;
        return 1;
    }
    int threadCount = atoi(argv[1]);
    int start = atoi(argv[2]);
    int end = atoi(argv[3]);
    if (threadCount <= 0 || start <= 0 || end <= 0 || start >= end)
    {
        cerr << "threadCount: " << threadCount << " start: " << start <<
            " end: " << end << endl;
        cerr << "values should be > than 0\nstart should be less than end" << endl;
        return 1;
    }

    int totalNums = end - start + 1;
    int numsForOneProcess = totalNums / threadCount;
    int numsRemained = totalNums - (numsForOneProcess * threadCount);

    InitializeCriticalSection(&cs);

    auto startTime = chrono::high_resolution_clock::now();

    HANDLE threads[threadCount];
    int currentStart = start;
    for (int i = 0; i < threadCount; i++)
    {
        int currentCount = numsForOneProcess;
        if (i == threadCount - 1)
        {
            currentCount += numsRemained;
        }
        int currentEnd = currentStart + currentCount - 1;

        int* range = new int[2];
        range[0] = currentStart;
        range[1] = currentEnd;

        threads[i] = CreateThread(NULL, 0, Func, range, 0, NULL);
        if (threads[i] == NULL)
        {
            cerr << "create thread fail" << endl;
            delete[] range;
            DeleteCriticalSection(&cs);
            return 1;
        }
        currentStart = currentEnd + 1;
    }

    WaitForMultipleObjects(threadCount, threads, TRUE, 60000);

    auto endTime = chrono::high_resolution_clock::now();

    for (int i = 0; i < threadCount; i++)
    {
        CloseHandle(threads[i]);
    }

    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    cout << "time calculating: " << duration.count() << " ms" << endl;

    std::sort(primes, primes + primesCount);

    system("pause");

    for (int i = 0; i < primesCount; i++)
    {
        cout << primes[i] << " ";
    }
    cout << endl;

    DeleteCriticalSection(&cs);

    return 0;
}