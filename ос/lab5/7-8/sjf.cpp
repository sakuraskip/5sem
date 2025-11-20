#include <iostream>
#include <climits>
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
    int priority;
    int whenCompleted;
    int waitingTime;
    int fromArrivalToCompleted;
};

void calculateTurnAroundTime(task tasks[], int taskAmount) {
   for (int i = 0; i < taskAmount; i++) {
      tasks[i].fromArrivalToCompleted = tasks[i].whenCompleted - tasks[i].arrivalTime;
      tasks[i].waitingTime = tasks[i].fromArrivalToCompleted - tasks[i].burstTime;
      if (tasks[i].waitingTime < 0) tasks[i].waitingTime = 0;
   }
}





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
   cout << "exec order: " << endl;
   
 int completedCount = 0;
   int currentTime = 0;
   int minRemainingTime = INT_MAX;
   int currentTaskIndex = 0;
   bool foundShortest = false;
   
   auto startTime = high_resolution_clock::now();
   
   while (completedCount != taskAmount)
   {
      for (int j = 0; j < taskAmount; j++)
      {
         if ((tasks[j].arrivalTime <= currentTime) AND 
             (tasks[j].remainingTime < minRemainingTime) AND 
             tasks[j].remainingTime > 0)
            {
            minRemainingTime = tasks[j].remainingTime;
            currentTaskIndex = j;
            foundShortest = true;
         }
      }
      
      if (!foundShortest) {
         currentTime++;
         continue;
      }
      
      tasks[currentTaskIndex].remainingTime--;
      minRemainingTime = tasks[currentTaskIndex].remainingTime;
      if (minRemainingTime == 0)
         minRemainingTime = INT_MAX;
         
      if (tasks[currentTaskIndex].remainingTime == 0) {
         cout << tasks[currentTaskIndex].id << " -> ";
         completedCount++;
         foundShortest = false;
         tasks[currentTaskIndex].whenCompleted = currentTime + 1;
      }
      currentTime++;
   }
   
   auto endTime = high_resolution_clock::now();
   duration<double> elapsed = endTime - startTime;
   
   calculateTurnAroundTime(tasks, taskAmount);
   
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
   
   cout << "\naverage waiting time: " << (float)totalWaitingTime/taskAmount << endl;
    cout << "average turn around time: " << (double)totalTurnAroundTime / taskAmount << endl;
    cout << "average burst time: " << (double)totalBurstTime / taskAmount << endl;
    cout << "total exec time: " << totalCompletionTime << endl;
    cout << "time per task: " << (double)taskAmount / totalCompletionTime << endl;
    cout << "program worked for  " << elapsed.count()/1000 << " ms" << endl;
   return 0;
}