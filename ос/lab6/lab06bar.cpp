#include <windows.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <atomic>
#include <ctime>
using namespace std;

vector<vector<vector<int>>> matrices;
atomic<int>** result;  
SYNCHRONIZATION_BARRIER barrier1, barrier2;
int ThreadsNumber, matrixAmount, matrixSize;

DWORD WINAPI threadFunc(LPVOID param) {
    int thread_id = (int)(INT_PTR)param;
    int start = thread_id * (matrixAmount / ThreadsNumber);
    int end;
    if(thread_id == ThreadsNumber-1) {
        end = matrixAmount;
    } else {
        end = start + (matrixAmount/ThreadsNumber);
    }

    vector<vector<int>> part(matrixSize, vector<int>(matrixSize, 0));
    
    for (int m = start; m < end; m++) {
        for (int i = 0; i < matrixSize; i++) {
            for (int j = 0; j < matrixSize; j++) {
                part[i][j] += matrices[m][i][j];
            }
        }
    }

    EnterSynchronizationBarrier(&barrier1, 0);

    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            result[i][j].fetch_add(part[i][j], memory_order_relaxed);
        }
    }

    EnterSynchronizationBarrier(&barrier2, 0);

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "usage: lab06bar threads matrices size" << endl;
        return 1;
    }

    ThreadsNumber = atoi(argv[1]);
    matrixAmount = atoi(argv[2]);
    matrixSize = atoi(argv[3]);

    srand(time(0));

    matrices.resize(matrixAmount);
    for (int m = 0; m < matrixAmount; m++) {
        matrices[m].resize(matrixSize, vector<int>(matrixSize));
        for (int i = 0; i < matrixSize; i++) {
            for (int j = 0; j < matrixSize; j++) {
                matrices[m][i][j] = rand() % 4001 - 2000;
            }
        }
    }

    result = new atomic<int>*[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        result[i] = new atomic<int>[matrixSize];
        for (int j = 0; j < matrixSize; j++) {
            result[i][j].store(0);
        }
    }

    InitializeSynchronizationBarrier(&barrier1, ThreadsNumber, -1);
    InitializeSynchronizationBarrier(&barrier2, ThreadsNumber, -1);

    HANDLE* threads = new HANDLE[ThreadsNumber];
    for (int i = 0; i < ThreadsNumber; i++) {
        threads[i] = CreateThread(NULL, 0, threadFunc, (LPVOID)(INT_PTR)i, 0, NULL);
    }

    WaitForMultipleObjects(ThreadsNumber, threads, TRUE, INFINITE);

    for (int i = 0; i < matrixSize; i++) {
        cout << "[";
        for (int j = 0; j < matrixSize; j++) {
            
            cout << result[i][j].load() << " ";
        }
        cout <<"]" <<  endl;
    }

    for (int i = 0; i < matrixSize; i++) {
        delete[] result[i];
    }
    delete[] result;

    for (int i = 0; i < ThreadsNumber; i++) {
        CloseHandle(threads[i]);
    }
    delete[] threads;
    
    DeleteSynchronizationBarrier(&barrier1);
    DeleteSynchronizationBarrier(&barrier2);

    return 0;
}