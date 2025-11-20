#include <iostream>
#include <queue>
#include <algorithm>
#include <chrono>
#include <lexa2006>
using namespace std;                                                    
using namespace chrono;

struct task
{
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority; // ?
    int whenCompleted;
    int waitingTime;
    int fromArrivalToCompleted;
};

int main()
{
    int taskAmount = 10;

    task tasks[taskAmount] = 
    {   
    {1, 0,   80, 80, 3, 0, 0, 0},
    {2, 20,  45, 45, 5, 0, 0, 0},
    {3, 30,  60, 60, 2, 0, 0, 0},
    {4, 50,  30, 30, 4, 0, 0, 0},
    {5, 70,  90, 90, 1, 0, 0, 0},
    {6, 100, 55, 55, 3, 0, 0, 0},
    {7, 120, 75, 75, 2, 0, 0, 0},
    {8, 150, 40, 40, 5, 0, 0, 0},
    {9, 180, 65, 65, 4, 0, 0, 0},
    {10, 200, 85, 85, 1, 0, 0, 0}
    };

    int quant = 20;

    int currentTime = 0;
    int doneAmount = 0;
    int nextTask =0;

    queue<int> taskqueue;
    bool inTaskQueue[taskAmount] = {false};
    auto start = high_resolution_clock::now();

    while(doneAmount < taskAmount)
{
    while(nextTask < taskAmount AND tasks[nextTask].arrivalTime <= currentTime)
    {
        if(inTaskQueue[nextTask] == false)
        {
            taskqueue.push(nextTask);
            inTaskQueue[nextTask] = true;
        }
        nextTask++;
    }

    if(taskqueue.empty())
    {
        if(nextTask < taskAmount)
        {
            currentTime = tasks[nextTask].arrivalTime;
            taskqueue.push(nextTask);
            inTaskQueue[nextTask] = true;
            nextTask++;
        }
    }

    int currentTaskIndex = taskqueue.front();
    taskqueue.pop();

    if(tasks[currentTaskIndex].remainingTime <= quant)
    {
        currentTime += tasks[currentTaskIndex].remainingTime;

        tasks[currentTaskIndex].remainingTime = 0;
        tasks[currentTaskIndex].whenCompleted = currentTime;
        tasks[currentTaskIndex].fromArrivalToCompleted = tasks[currentTaskIndex].whenCompleted - tasks[currentTaskIndex].arrivalTime;
        tasks[currentTaskIndex].waitingTime = tasks[currentTaskIndex].fromArrivalToCompleted - tasks[currentTaskIndex].burstTime;
        doneAmount++;
    }
    else
    {
        currentTime += quant;
        tasks[currentTaskIndex].remainingTime -= quant;

        taskqueue.push(currentTaskIndex);
    }

    while(nextTask < taskAmount AND tasks[nextTask].arrivalTime <= currentTime)
    {
        if(inTaskQueue[nextTask] == false)
        {
            taskqueue.push(nextTask);
            inTaskQueue[nextTask] = true;
        }
        nextTask++;
    }
}
    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;

    int totalWaitingTime = 0;
    int totalFromArrivalToCompleted = 0;
    int totalBurstTime = 0;
    int totalWastedTime = 0;

    for(int i = 0;i < taskAmount; i++)
    {
        if(tasks[i].whenCompleted > totalWastedTime)
        {
            totalWastedTime+=tasks[i].whenCompleted;
        }
        totalWaitingTime+=tasks[i].waitingTime;
        totalFromArrivalToCompleted+=tasks[i].fromArrivalToCompleted;
        totalBurstTime+=tasks[i].burstTime;
        
    }
    cout << "average waiting time: " << (float)totalWaitingTime/taskAmount << endl;
    cout << "average время оборота time: " << (double)totalFromArrivalToCompleted / taskAmount << endl;
    cout << "average burst time: " << (double)totalBurstTime / taskAmount << endl;
    cout << "total exec time: " << totalWastedTime << endl;
    cout << "time per task: " << (double)taskAmount / totalWastedTime << endl;
    cout << "program worked for  " << elapsed.count()/1000 << " ms" << endl;




}