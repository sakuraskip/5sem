#include <iostream>
#include <unistd.h>
#include <windows.h>
using namespace std;

int main(int argc, char** argv)
{
    int iter =-1;
    if(argc < 2)
    {
        const char* envParm = getenv("ITER_NUM");
        iter = atoi(envParm);
        cout << "env parameter: " << iter << endl;
    }
    else
    {
        iter = atoi(argv[1]);
        cout << "cmd parameter: " << iter << endl;
    }
   

    if(iter >0)
    {
        for(unsigned int i=0;i<iter;i++)
        {
            cout << "PID: " << getpid() << endl;
            _sleep(500);
        }
    }
    else
    {
        cout << "no parameter found or it was 0 or less" << endl;
        ExitProcess(1);
    }

}