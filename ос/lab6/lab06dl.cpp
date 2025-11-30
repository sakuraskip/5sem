#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int processCount = 4;
    int resourceCount = 5;

    int C[processCount][resourceCount] = {
        {0, 1, 1, 1, 2},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 0, 1},
        {2, 1, 0, 0, 0}
    };

    int R[processCount][resourceCount] = {
        {1, 1, 0, 2, 1},
        {0, 1, 0, 2, 1},
        {0, 2, 0, 3, 1},
        {0, 2, 1, 1, 0}
    };
    //  int C[processCount][resourceCount] = {
    //     {1,1,2,1,3},
    //     {2,2,2,1,0},
    //     {2,1,3,1,0},
    //     {1,1,2,2,1}
    // };

    // int R[processCount][resourceCount] = {
    //     {1,0,2,1,1},
    //     {2,0,1,1,0},
    //     {1,1,0,1,0},
    //     {1,1,1,1,0}
    // };

    int E[resourceCount] = {2, 4, 1, 4, 4};

    int A[resourceCount] = {0, 1, 0, 2, 1};

    int Need[processCount][resourceCount];
    for (int i = 0; i < processCount; i++)
        for (int j = 0; j < resourceCount; j++)
            Need[i][j] = C[i][j] - R[i][j];

    bool finished[processCount] = {false};

    bool progress;

    do {
        progress = false;

        for (int i = 0; i < processCount; i++)
        {
            if (finished[i] == false)
            {
                bool canFinish = true;
                for (int j = 0; j < resourceCount; j++)
                {
                    if (Need[i][j] > A[j])
                    {
                        canFinish = false;
                        break;
                    }
                }

                if (canFinish)
                {
                    for (int j = 0; j < resourceCount; j++)
                    {
                        A[j] += R[i][j]; 
                        if(A[j] > E[j])
                        {
                            A[j] = E[j];
                        }
                    }
                        

                    finished[i] = true;
                    progress = true;

                    cout << "process " << i+1 << " finished" << endl;;
                }
            }
        }
    } while (progress);

    bool deadlock = false;
    cout << "processes in deadlock: " << endl;;
    for (int i = 0; i < processCount; i++)
    {
        if (finished[i] == false)
        {
            cout << "proc " << i+1 << endl;;
            deadlock = true;
        }
    }

    if (deadlock == false)
    {
        cout << "no deadlock happy :)." << endl;
    }

    return 0;
}