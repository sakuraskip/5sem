#include <iostream>
#include <algorithm>
#include <chrono>
#include <climits>
#include <queue>
#include <vector>
#include <lexa2006>
using namespace std;
using namespace chrono;

struct task
{
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    int whenCompleted;
    int waitingTime;
    int fromArrivalToCompleted;
};

struct ComparePriority {
    bool operator()(const task* a, const task* b) {
        if (a->priority == b->priority) {
            return a->arrivalTime > b->arrivalTime;
        }
        return a->priority > b->priority;
    }
};

int main() {
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

    for (int i = 0; i < taskAmount; i++) {
        tasks[i].remainingTime = tasks[i].burstTime;
        tasks[i].whenCompleted = 0;
        tasks[i].waitingTime = 0;
        tasks[i].fromArrivalToCompleted = 0;
    }

    auto startTime = high_resolution_clock::now();

    priority_queue<task*, vector<task*>, ComparePriority> readyQueue;
    vector<int> executionOrder;
    int currentTime = 0;
    int completedTasks = 0;
    
    task* currentTask = nullptr;
    vector<int> lastStartTime(taskAmount, -1);

    while (completedTasks < taskAmount) {
        for (int i = 0; i < taskAmount; i++) {
            if (tasks[i].arrivalTime == currentTime) {
                readyQueue.push(&tasks[i]);
            }
        }

        task* nextTask = nullptr;
        if (!readyQueue.empty()) {
            nextTask = readyQueue.top();
        }

        if (nextTask != nullptr AND currentTask != nullptr AND 
            nextTask->priority < currentTask->priority) {
            readyQueue.push(currentTask);
            currentTask = nullptr;
        }

        if (currentTask == nullptr AND !readyQueue.empty()) {
            currentTask = readyQueue.top();
            readyQueue.pop();
            if (executionOrder.empty() || executionOrder.back() != currentTask->id) {
                executionOrder.push_back(currentTask->id);
            }
        }

        if (currentTask != nullptr) {
            currentTask->remainingTime--;
            
            for (int i = 0; i < taskAmount; i++) {
                if (&tasks[i] != currentTask AND 
                    tasks[i].arrivalTime <= currentTime AND 
                    tasks[i].remainingTime > 0) {
                    tasks[i].waitingTime++;
                }
            }

            if (currentTask->remainingTime == 0) {
                currentTask->whenCompleted = currentTime + 1;
                currentTask->fromArrivalToCompleted = currentTask->whenCompleted - currentTask->arrivalTime;
                completedTasks++;
                currentTask = nullptr;
            }
        }

        currentTime++;
    }

    int waitingTimes[taskAmount] = {0};
    int turnAroundTimes[taskAmount] = {0};
    
    for (int i = 0; i < taskAmount; i++) {
        waitingTimes[i] = tasks[i].waitingTime;
        turnAroundTimes[i] = tasks[i].fromArrivalToCompleted;
    }

    auto endTime = high_resolution_clock::now();
    duration<double> elapsed = endTime - startTime;

    int totalWaitingTime = 0;
    int totalTurnAroundTime = 0;
    int totalBurstTime = 0;
    int totalCompletionTime = 0;

    for (int i = 0; i < taskAmount; i++) {
        totalWaitingTime += tasks[i].waitingTime;
        totalTurnAroundTime += tasks[i].fromArrivalToCompleted;
        totalBurstTime += tasks[i].burstTime;
        
        if (tasks[i].whenCompleted > totalCompletionTime) {
            totalCompletionTime = tasks[i].whenCompleted;
        }
    }

    cout << "average waiting time: " << (float)totalWaitingTime/taskAmount << endl;
    cout << "average turn around time: " << (double)totalTurnAroundTime / taskAmount << endl;
    cout << "average burst time: " << (double)totalBurstTime / taskAmount << endl;
    cout << "total exec time: " << totalCompletionTime << endl;
    cout << "time per task: " << (double)taskAmount / totalCompletionTime << endl;
    cout << "program worked for  " << elapsed.count()/1000 << " ms" << endl;
    
    cout << "exec order: ";
    for (size_t i = 0; i < executionOrder.size(); i++) {
        cout << executionOrder[i];
        if (i != executionOrder.size() - 1)
        {
            cout << " -> ";
        } 
    }
    cout << endl;

    return 0;
}