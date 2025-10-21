#include <iostream>
#include <unistd.h>
#include <windows.h>
using namespace std;

bool isPrime(int num)
{
    if(num < 2 ) return false;
    if(num == 2) return true;
    if(num%2 == 0) return false;
    for(int i=3;(i*i)<=num;i+=2)
    {
        if(num% i ==0)
        {
            return false;
        }
    }
        return true;

}
int main(int argc, char** argv)
{
    int primeCounter =0;
    int from;
    int to;

    const int maxPrimeCount = 10000;
    try
    {
        if(argc < 3)
        {
            cerr << "usage: lab03child.exe {from} {to}" << endl;
            return 1;
        }
        else
        {
            from = atoi(argv[1]);
            to = atoi(argv[2]);
            cerr << "from: " << from << " to: " << to << endl;
            
        }
        if(from > to)
        {
            cerr << "'from' should be less than 'to'" << endl;
            return 1;
        }

        int* primeNumber = new int[maxPrimeCount];
        ZeroMemory(primeNumber,sizeof(int)*maxPrimeCount);

        for(int i = from;i<to;i++)
        {
            if(isPrime(i))
            {
                if(primeCounter >=maxPrimeCount)
                {
                    break;
                }
                else primeNumber[primeCounter++] = i;
            }
        }
        for(int i = 0;i<primeCounter;i++)
        {
            cout << primeNumber[i] << " ";
        }
        cout << endl;
        delete[] primeNumber;
        return 0;
    }
   catch(exception& ex)
   {
        cerr << ex.what() << endl;
   }
}