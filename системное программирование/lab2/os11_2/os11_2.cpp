#include "HT.h"
#include <csignal>
using namespace std;

bool active = true;
void sigintHandler(int sig)
{
	cout << "exiting... " << endl;
	active = false;
}
int main(int argc, char** argv)
{
	signal(SIGINT, sigintHandler);
	HANDLE m = OpenMutexA(SYNCHRONIZE, FALSE, "ht_mutex");

	if ( !m )
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

	int maxKeys = 50;
	int i = 0;

	HT::HTHANDLE* ht = HT::Open("test2.ht");
	if (ht == NULL)
	{
		CloseHandle(m);
		return 1;
	}
	while(active)
	{
		string key = "key" + to_string(i%maxKeys);
		HT::Element* e = new HT::Element(key.c_str(), key.length(), 0, 4);
		if ( HT::Insert(ht, e) )
		{
			cout << "\ninserted " << i << " element" << endl;
		}
		delete e;
		i++;
		Sleep(1000);
		
	}
	HT::Close(ht);
	CloseHandle(m);
	return 0;
	
	


}