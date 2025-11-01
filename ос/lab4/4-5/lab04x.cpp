#include <windows.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
    int N;
    char name[] = "User-d4c889c4";
    int nameLength = sizeof(name)/sizeof(name[0]);
    int nameIter = 0;
    if( argc <= 1 )
    {
        cerr << "usage: lab04x {N}" << endl;
        return 1;
    }
    N = atoi(argv[1]);
    if( N <= 0 )
    {
        cerr << "N should be more than 0" << endl;
        return 1;
    }
    for(int i = 0; i <= N; i++)
    {
        printf("%d - %d - %d - %c\n",getpid(),_threadid,i,name[nameIter++]);
        if( nameIter >= nameLength )
        {
            nameIter = 0;
        }
        Sleep(300);
    }


}