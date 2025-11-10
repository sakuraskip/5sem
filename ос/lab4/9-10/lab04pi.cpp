#include <windows.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <signal.h>

#define FiberHandle LPVOID 

using namespace std;


bool BCEM_CПATb = false;

FiberHandle mainFiber = nullptr;

int* range;
double* partialSums;
FiberHandle* handles;
int partsize = 0;
int fiberCount =0;
int currentFiber = 0;

BOOL WINAPI SignalHandler(DWORD sig)// if it works for too long - pi becomes 355 :)
{
    if(sig == CTRL_C_EVENT)
    {
        cout << "caught ctrl+c, exiting..." << endl;
        BCEM_CПATb = true;
        return TRUE;
    }
    return FALSE;
    
}
void FiberFunction(LPVOID fiberid)
{
    // int id = *((int*)(&fiberid));// GOOD JOB YOU KILLED IT
    int id = static_cast<int>(reinterpret_cast<size_t>(fiberid));
    while( !BCEM_CПATb )
    {
        double fiberSum = 0;
        int start = range[id];
        int end = start + partsize;

        for(int i = start; i<end;i++)
        {
            if((i-start)%100 == 0 && BCEM_CПATb)
            break;

            double part = 0;
            if(i%2 == 0)
            {
                part = 1/(2.0*i+1.0);
            }
            else
            {
                part = -1.0/(2.0*i+1.0);
            }
            fiberSum +=part;
        }
        partialSums[id] += fiberSum;
        range[id] = end;

        SwitchToFiber(mainFiber);
    }
    SwitchToFiber(mainFiber);
}
int main(int argc, char** argv)
{
    SetConsoleCtrlHandler(SignalHandler,TRUE);
    if(argc < 3)
    {
        cerr << "usage: lab04pi.exe {fibercount} {partsize}" << endl;
        return 1;
    }
    fiberCount = atoi(argv[1]);
    partsize = atoi(argv[2]);
    if(fiberCount <=0 || partsize <=0)
    {
        cerr << "fibercount or partsize cant be <=0" << endl;
        return 1;
    }
    mainFiber = ConvertThreadToFiber(NULL);

    range = new int[fiberCount];
    partialSums = new double[fiberCount];
    handles = new FiberHandle[fiberCount];

    for(int i =0;i<fiberCount;i++)
    {
        range[i] = i*partsize;
        partialSums[i] = 0.0;
        handles[i] = CreateFiber(0,FiberFunction,(LPVOID)i);
    }
    while( !BCEM_CПATb )
    {
        SwitchToFiber(handles[currentFiber]);
        currentFiber = (currentFiber+1)%fiberCount;
    }
    double pi = 0;
    for(int i = 0;i<fiberCount;i++)
    {
        pi+=partialSums[i];
        DeleteFiber(handles[i]);
    }
    printf("%.10f",pi*4);
    delete[] range;
    delete[] partialSums;
    delete[] handles;
    
    return 0;
}