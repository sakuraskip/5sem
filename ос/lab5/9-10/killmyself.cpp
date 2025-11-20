#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <lexa2006>

#define cpuIOChance 75
#define notCpuIOChange 15 // :)

using namespace std;

 int queue0Quant = 15;  
 int queue1Quant = 60;  
 int queue2Quant = 150; 

struct Task {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    bool cpu_bound; 
    int currentQueue; 
    int whenCompleted;
    int waitingTime;
    int turnAroundTime;
    int endQueue;
};

vector<Task> tasks;
queue<int> q0, q1, q2;
int currentTime = 0;
int nextTaskIndex = 0;
const int taskAmount = 100;






bool isIOHappened(int isCpuBound)
 {
    
    if (isCpuBound == false)
    {
        return (rand() % 100) < cpuIOChance;
    } else
    {
        return (rand() % 100) < notCpuIOChange;
    }
}

void funnyScheduler()
{
    int completedTasks = 0;
    
    while (completedTasks < taskAmount) {
        while (nextTaskIndex < taskAmount AND tasks[nextTaskIndex].arrivalTime <= currentTime)
        {
            q0.push(nextTaskIndex);
            nextTaskIndex++;
        }
        
        int currentTaskIndex = -1356;
        int quant = 0;
        
        if (q0.empty() == false)
        {
            currentTaskIndex = q0.front();
            q0.pop();
            quant = queue0Quant;
        }
        else if (q1.empty() == false)
        {
            currentTaskIndex = q1.front();
            q1.pop();
            quant = queue1Quant;
        }
        else if (q2.empty() == false)
        {
            currentTaskIndex = q2.front();
            q2.pop();
            quant = queue2Quant;
        }
        else
        {
            if (nextTaskIndex < taskAmount)
            {
                currentTime = tasks[nextTaskIndex].arrivalTime;
                continue;

            } else break;
               
        }
        
        Task& task = tasks[currentTaskIndex];
        int executionTime;
        if(quant > task.remainingTime)
        {
            executionTime = task.remainingTime;
        }
        else
        {
            executionTime = quant;
        }
        task.remainingTime -= executionTime;
        currentTime += executionTime;
        
        if (task.remainingTime == 0)// task completed
        {
            task.whenCompleted = currentTime;
            task.turnAroundTime = currentTime - task.arrivalTime;
            task.waitingTime = task.turnAroundTime - task.burstTime;
            task.endQueue = task.currentQueue;
            completedTasks++;
            continue;
        }
        
        if (isIOHappened(task.cpu_bound))
        {
            if (task.currentQueue > 0) //increasing priority cuz of io
            {
                task.currentQueue--;
            }
        } 
        else //quant ended
        {
            if (task.currentQueue == 0)
            {
                if (executionTime == queue0Quant)
                {
                    task.currentQueue = 1;
                }
            }
            else if (task.currentQueue == 1)
            {
                if (executionTime == queue1Quant)
                {
                    task.currentQueue = 2;
                }
            }
            else // task in queue 2
            {
                int imagineYourLifeDependsOnSomeRandomNumberThatWillBeGeneratedBasedOnExecutionTime = rand() % 3;

                if (imagineYourLifeDependsOnSomeRandomNumberThatWillBeGeneratedBasedOnExecutionTime == 0)
                {
                    task.currentQueue = 0;
                }
                else if (imagineYourLifeDependsOnSomeRandomNumberThatWillBeGeneratedBasedOnExecutionTime == 1)
                {
                    task.currentQueue = 1;
                }
            }
        }
        
        switch (task.currentQueue)
        {
            case 0:
            {
                q0.push(currentTaskIndex);
                break;
            }
            case 1:
            {
                q1.push(currentTaskIndex);
                break;
            }
            case 2:
            {
                q2.push(currentTaskIndex);
                break;
            }
        }
    }
}



int main()
{
    srand(time(0));

    for (int i = 0; i < taskAmount; i++) {
        Task task;
        task.id = i + 1;
        task.arrivalTime = rand() % 750; 
        task.cpu_bound = rand() % 2;
        
        if (task.cpu_bound == true)
        {
            task.burstTime = 50 + rand() % 251; 
        } else
        {
            task.burstTime = 10 + rand() % 91; 
        }
        
        task.remainingTime = task.burstTime;
        task.currentQueue = 0; 
        task.whenCompleted = 0;
        task.waitingTime = 0;
        task.turnAroundTime = 0;
        task.endQueue = 2;// put whatever you want
        
        tasks.push_back(task);
    }
    
    sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) { // funny thingy
        return a.arrivalTime < b.arrivalTime;
    });

    funnyScheduler();


    int totalWaitingTime = 0;
    int totalTurnAroundTime = 0;
    int totalBurstTime = 0;
    int cpuBoundCount = 0;
    int ioBoundCount = 0;
    
    int finalQ0 = 0, finalQ1 = 0, finalQ2 = 0;
    
    // Для расчета средних значений по очередям
    double waitQ0 = 0, waitQ1 = 0, waitQ2 = 0;
    double turnAroundQ0 = 0, turnAroundQ1 = 0, turnAroundQ2 = 0;
    
    cout << "id\ttype\tTimeArrival\tBurstTime\tCompletedTime\tWaitingTime\tTurnAroundTIme\tFinalQueue?" << endl;
    
    for (const auto& task : tasks) {
        totalWaitingTime += task.waitingTime;
        totalTurnAroundTime += task.turnAroundTime;
        totalBurstTime += task.burstTime;
        
        if (task.cpu_bound == true)
        {
            cpuBoundCount++;
        } 
        else ioBoundCount++;
        
         
        switch(task.endQueue)
        {
            case 0: 
                finalQ0++;
                waitQ0 += task.waitingTime;
                turnAroundQ0 += task.turnAroundTime;
                break;
            case 1: 
                finalQ1++;
                waitQ1 += task.waitingTime;
                turnAroundQ1 += task.turnAroundTime;
                break;
            case 2: 
                finalQ2++;
                waitQ2 += task.waitingTime;
                turnAroundQ2 += task.turnAroundTime;
                break;
        }
        
        cout << task.id << "\t"
             << (task.cpu_bound == true ? "CPU" : "I/O") << "\t"
             << task.arrivalTime << "\t\t"
             << task.burstTime << "\t\t"
             << task.whenCompleted << "\t\t"
             << task.waitingTime << "\t\t"
             << task.turnAroundTime << "\t\t"
                << task.currentQueue << endl;
    }
    
    double avgWaitQ0 = waitQ0 / finalQ0;
    double avgWaitQ1 = waitQ1 / finalQ1;
    double avgWaitQ2 = waitQ2 / finalQ2;
    
    double avgTurnAroundQ0 = turnAroundQ0 / finalQ0;
    double avgTurnAroundQ1 = turnAroundQ1 / finalQ1;
    double avgTurnAroundQ2 =  turnAroundQ2 / finalQ2;
    
    cout << "cpu tasks: " << cpuBoundCount << endl;
    cout << "io tasks: " << ioBoundCount << endl;
    cout << "average waiting time: " << (double)totalWaitingTime / taskAmount << " ms" << endl;
    cout << "average turnaround time: " << (double)totalTurnAroundTime / taskAmount << " ms" << endl;
    cout << "average bursttime: " << (double)totalBurstTime / taskAmount << " ms" << endl;
    cout << "program exec time " << currentTime << " ms" << endl;
    cout << "tasks/ms: " << (double)taskAmount / currentTime  << endl;
        cout << "------------------------------------------------------------------------" << endl;

    cout << "high priority queue: " << finalQ0 << " tasks" << endl;
    cout << "normal priority queue: " << finalQ1 << " tasks" << endl;
    cout << "low priority queue: " << finalQ2 << " tasks" << endl;
        cout << "------------------------------------------------------------------------" << endl;

    cout << "high priority average waiting time: " << avgWaitQ0 << " ms" << endl;
        cout << "normal priority average waiting time: " << avgWaitQ1 << " ms" << endl;
    cout << "low priority average waiting time: " << avgWaitQ2 << " ms" << endl;

    cout << "------------------------------------------------------------------------" << endl;
    cout << "high priority  average turnaround time: " << avgTurnAroundQ0 << " ms" << endl;
        cout << "normal priority  average turnaround time: " << avgTurnAroundQ1 << " ms" << endl;
    cout << "low priority  average turnaround time: " << avgTurnAroundQ2 << " ms" << endl;

    
    return 0;
}