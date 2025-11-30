#include "windows.h"
#include <cstdlib>
#include <iostream>
#include <queue>
#include <ctime>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;

CRITICAL_SECTION cs;
CONDITION_VARIABLE condvarProducer;  
CONDITION_VARIABLE condvarConsumer;
int maxsize = 10; 
int currentSize = 0;
volatile bool isWorking = true;
int itemCounter = 0;
int consumedCounter = 0;

int lastPeriodProduced = 0;
int lastPeriodConsumed = 0;
steady_clock::time_point startTime;
steady_clock::time_point lastStatTime;

queue<int> buffer;



DWORD WINAPI Producer(LPVOID parm)
{
    int* parms = (int*)parm;
    int from = parms[0];
    int to = parms[1];
    
    while(isWorking)
    {
        int sleepTime = from + (rand() % (to - from + 1));
        Sleep(sleepTime);
        
        EnterCriticalSection(&cs);

        while(currentSize == maxsize)
        {
            cout << "producer blocked; buffer full" << endl;
            SleepConditionVariableCS(&condvarProducer, &cs, INFINITE);
            if (!isWorking) {
                LeaveCriticalSection(&cs);
                return 0;
            }
        }
        
        int newItem = ++itemCounter;
        buffer.push(newItem);
        currentSize = buffer.size();
        
        cout << "producer added item " << newItem << " buffer size " << currentSize << endl;

        WakeConditionVariable(&condvarConsumer);
        cout << "consumer awakened" << endl;

        LeaveCriticalSection(&cs);
    }
    return 0;
}

DWORD WINAPI Consumer(LPVOID parm)
{
    int* parms = (int*)parm;
    int from = parms[0];
    int to = parms[1];
    
    while(isWorking)
    {
        int sleepTime = from + (rand() % (to - from + 1));
        Sleep(sleepTime);
        
        EnterCriticalSection(&cs);

        while(currentSize == 0)
        {
            cout << "consumer blocked buffer empty" << endl;
            SleepConditionVariableCS(&condvarConsumer, &cs, INFINITE);
            if (!isWorking) {
                LeaveCriticalSection(&cs);
                return 0;
            }
        }
        
        int consumedItem = buffer.front();
        buffer.pop();
        currentSize = buffer.size();
        consumedCounter++;
        
        cout << "consumer removed item " << consumedItem << " buffer size " << currentSize << endl;

        WakeConditionVariable(&condvarProducer);
        cout << "producer awakened" << endl;

        LeaveCriticalSection(&cs);
    }
    return 0;
}

void PrintStatistics() {
    auto currentTime = steady_clock::now();
    auto totalDuration = duration_cast<milliseconds>(currentTime - startTime);
    auto statDuration = duration_cast<milliseconds>(currentTime - lastStatTime);
    
    double totalSeconds = totalDuration.count()/1000.0;
    double intervalSeconds = statDuration.count() / 1000.0;
    
    int producedThisPeriod = itemCounter - lastPeriodProduced;
    int consumedThisPeriod = consumedCounter - lastPeriodConsumed;
    
    double fillPercentage = (currentSize / (double)maxsize) * 100.0;
    double avgProductionTime = intervalSeconds > 0 ? intervalSeconds / producedThisPeriod : 0;
    double productionSpeed = intervalSeconds > 0 ? producedThisPeriod / intervalSeconds : 0;
    double avgConsumptionTime = intervalSeconds > 0 ? intervalSeconds / consumedThisPeriod : 0;
    double consumptionSpeed = intervalSeconds > 0 ? consumedThisPeriod / intervalSeconds : 0;
    
    cout << "current time: " << totalSeconds << endl;
    cout << "total produced: " << itemCounter << endl;
    cout << "total consumed: " << consumedCounter << endl;
    cout << "current buffer size: " << currentSize << endl;
    cout << "buffer fill percentage: " << fillPercentage << endl;
    cout << "produced this period: " << producedThisPeriod << endl;
    cout << "average production time: " << avgProductionTime << endl;
    cout << "production speed: " << productionSpeed << endl;
    cout << "total producer time: " << totalSeconds << endl;
    cout << "consumed this period: " << consumedThisPeriod << endl;
    cout << "average consumption time: " << avgConsumptionTime << endl;
    cout << "consumption speed: " << consumptionSpeed << endl;
    cout << "total consumer time: " << totalSeconds << endl;
    
    lastPeriodProduced = itemCounter;
    lastPeriodConsumed = consumedCounter;
    lastStatTime = currentTime;
}

DWORD WINAPI StatThread(LPVOID param) {
    while (isWorking) {
        Sleep(5000); 
        EnterCriticalSection(&cs);
        PrintStatistics();
        LeaveCriticalSection(&cs);
    }
    return 0;
}

int main(int argc, char ** argv)
{
    if(argc < 6)
    {
        cerr << "usage: lab06conds buffersize prodSleepTimeFrom prodSleepTimeTo consSleepTimeFrom consSleepTimeFrom" << endl;
        return 1;
    }

    maxsize = atoi(argv[1]);
    int prodSleepTimeFrom = atoi(argv[2]);
    int prodSleepTimeTo = atoi(argv[3]);
    int consSleepTimeFrom = atoi(argv[4]);
    int consSleepTimeTo = atoi(argv[5]);

    InitializeCriticalSection(&cs);
    InitializeConditionVariable(&condvarProducer);
    InitializeConditionVariable(&condvarConsumer);
    
    srand(time(0)); 
    
    cout << "username: user-d4c889c4" << endl;
    cout << "current buffer size: 0" << endl;
    cout << "max buffer size: " << maxsize << endl;
    
    startTime = steady_clock::now();
    lastStatTime = startTime;
    
    int producerParams[2] = {prodSleepTimeFrom, prodSleepTimeTo};   
    int consumerParams[2] = {consSleepTimeFrom, consSleepTimeTo};   
   
    HANDLE producerThreads[2];
    HANDLE consumerThreads[2];
    
    HANDLE statThread = CreateThread(NULL, 0, StatThread, NULL, 0, NULL);
    
    for (int i = 0; i < 2; i++) {
        producerThreads[i] = CreateThread(NULL, 0, Producer, producerParams, 0, NULL);
        if (producerThreads[i] == NULL) {
            cerr << "error creating producer thread " << i << endl;
            return 1;
        }
    }
    
    for (int i = 0; i < 2; i++) {
        consumerThreads[i] = CreateThread(NULL, 0, Consumer, consumerParams, 0, NULL);
        if (consumerThreads[i] == NULL) {
            cerr << "error creating consumer thread " << i << endl;
            return 1;
        }
    }
    
    cout << "60seconds work let it go" << endl;
    Sleep(60000);
    
    cout << "stopping " << endl;
    isWorking = false;
    
    WakeAllConditionVariable(&condvarProducer);
    WakeAllConditionVariable(&condvarConsumer);
    
    WaitForMultipleObjects(2, producerThreads, TRUE, INFINITE);
    WaitForMultipleObjects(2, consumerThreads, TRUE, INFINITE);
    WaitForSingleObject(statThread, INFINITE);
    
    for (int i = 0; i < 2; i++) {
        CloseHandle(producerThreads[i]);
    }
    for (int i = 0; i < 2; i++) {
        CloseHandle(consumerThreads[i]);
    }
    CloseHandle(statThread);
    
    DeleteCriticalSection(&cs);
    
    return 0;
}