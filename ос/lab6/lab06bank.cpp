#include <iostream>
using namespace std;

const int processCount = 5;
const int resourceTypes = 3;

bool isSafeState(int resourceUsage[][resourceTypes], int maxNeed[][resourceTypes], int remainingNeed[][resourceTypes], int available[]) {
    
    int finished[processCount] = {0};
    int safeSequence[processCount];
    int sequenceIndex = 0;
    int work[resourceTypes];
    
    for(int i = 0; i < resourceTypes; i++)
        work[i] = available[i];
    
    for (int iteration = 0; iteration < processCount; iteration++)
    {
        for (int processIndex = 0; processIndex < processCount; processIndex++)
        {
            if (finished[processIndex] == 0)
            {
                bool canAllocate = true;
                for (int resourceIndex = 0; resourceIndex < resourceTypes; resourceIndex++)
                {
                    if (remainingNeed[processIndex][resourceIndex] > work[resourceIndex])
                    {
                        canAllocate = false;
                        break;
                    }
                }
                
                if (canAllocate)
                {
                    safeSequence[sequenceIndex++] = processIndex;
                    for (int resourceIndex = 0; resourceIndex < resourceTypes; resourceIndex++)
                    {
                        work[resourceIndex] += resourceUsage[processIndex][resourceIndex];
                    }
                    finished[processIndex] = 1;
                }
            }
        }
    }
    
    for (int processIndex = 0; processIndex < processCount; processIndex++)
    {
        if (finished[processIndex] == 0) {
            return false;
        }
    }
    
    cout << "safe sequence: ";
    for (int i = 0; i < processCount - 1; i++)
    {
        cout << "P" << safeSequence[i] << " ";
    }
    cout << "p" << safeSequence[processCount - 1] << endl;
    
    return true;
}

bool requestResources(int processId, int request[], int resourceUsage[][resourceTypes], int maxNeed[][resourceTypes], int remainingNeed[][resourceTypes], int available[]) {
    
    cout << "process p" << processId << " requests: ";
    for (int j = 0; j < resourceTypes; j++)
    {
        cout << request[j] << (j < resourceTypes-1 ? " " : "");
    }
    cout << endl;
    
    for (int j = 0; j < resourceTypes; j++)
    {
        if (request[j] > remainingNeed[processId][j])
        {
            cout << "request wants more than available" << endl;
            return false;
        }
    }
    
    for (int j = 0; j < resourceTypes; j++)
    {
        if (request[j] > available[j])
        {
            cout << "resources not available" << endl;
            return false;
        }
    }
    
    int oldAvailable[resourceTypes];
    int oldAllocated[processCount][resourceTypes];
    int oldRemainingNeed[processCount][resourceTypes];
    
    for(int i = 0; i < resourceTypes; i++)
        oldAvailable[i] = available[i];
        
    for(int i = 0; i < processCount; i++)
    {
        for(int j = 0; j < resourceTypes; j++)
        {
            oldAllocated[i][j] = resourceUsage[i][j];
            oldRemainingNeed[i][j] = remainingNeed[i][j];
        }
    }
    
    for (int j = 0; j < resourceTypes; j++)
    {
        available[j] -= request[j];
        resourceUsage[processId][j] += request[j];
        remainingNeed[processId][j] -= request[j];
    }
    
    if (isSafeState(resourceUsage, maxNeed, remainingNeed, available))
    {
        cout << "request completed" << endl;
        return true;
    }
    else {
        for(int i = 0; i < resourceTypes; i++)
            available[i] = oldAvailable[i];
            
        for(int i = 0; i < processCount; i++)
        {
            for(int j = 0; j < resourceTypes; j++)
            {
                resourceUsage[i][j] = oldAllocated[i][j];
                remainingNeed[i][j] = oldRemainingNeed[i][j];
            }
        }
        cout << "request denied, unstable state" << endl;
        return false;
    }
}

int main() {
    int resourceUsage[processCount][resourceTypes] = 
    {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };
    
    int maxNeed[processCount][resourceTypes] =
    {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };
    
    int remainingNeed[processCount][resourceTypes];
    for(int i = 0; i < processCount; i++)
    {
        for(int j = 0; j < resourceTypes; j++)
        {
            remainingNeed[i][j] = maxNeed[i][j] - resourceUsage[i][j];
        }
    }
    
    int totalResources[resourceTypes] = {10, 5, 7};
    int available[resourceTypes] = {3, 3, 2};
    bool initialSafe = isSafeState(resourceUsage, maxNeed, remainingNeed, available);
    
    if (initialSafe) {
        cout << "system is safe" << endl;
        
        
        int request1[resourceTypes] = {1, 0, 1};
        requestResources(1, request1, resourceUsage, maxNeed, remainingNeed, available);
        
        
    } else {
        cout << "system unstable at the start" << endl;
    }

    return 0;
}