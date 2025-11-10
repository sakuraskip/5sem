#include "HT.h"
#include <csignal>
#include <cstdlib>
#include <ctime>

using namespace std;

bool active = true;

void sigintHandler(int sig)
{
    cout << "\nexiting... " << endl;
    active = false;
}

int main(int argc, char** argv)
{
    signal(SIGINT, sigintHandler);
    srand((unsigned int)time(NULL)); 

    HANDLE m = OpenMutexA(SYNCHRONIZE, FALSE, "ht_mutex");
    if (!m)
    {
        cerr << "os11Open is not running" << endl;
        system("pause");
        return 1;
    }

    DWORD mutexCatch = WaitForSingleObject(m, 3000);
    if (mutexCatch == WAIT_OBJECT_0)
    {
        cerr << "no mutex to wait" << endl;
        CloseHandle(m);
        system("pause");
        return 1;
    }
    ReleaseMutex(m);

    const int maxKeys = 50;

    HT::HTHANDLE* ht = HT::Open("table.ht");
    if (ht == NULL)
    {
        CloseHandle(m);
        return 1;
    }

    while (active)
    {
        int idx = rand() % maxKeys;
        string key = "key" + to_string(idx);

        HT::Element* e = new HT::Element(key.c_str(), key.length());

        HT::Element* toUpdate = HT::Get(ht, e);

        if (toUpdate != nullptr)
        {
            int currentValue = 0;
            memcpy(&currentValue, toUpdate->payload, sizeof(int));
            currentValue += 1;
            HT::Update(ht, toUpdate, &currentValue, sizeof(int));

            cout << "\nupdated item with key: " << key << ", new value: " << currentValue << endl;
        }
        else
        {
            cerr << "\nitem with key " << key << " not found" << endl;
        }
        delete toUpdate;
        delete e;
        Sleep(1000);
    }

    HT::Close(ht);
    CloseHandle(m);
    return 0;
}