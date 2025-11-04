#include "HT.h"
#include <conio.h>
using namespace std;
int main(int argc, char** argv)
{
	if ( argc < 2 )
	{
		cerr << "usage: ht11open.exe {filename}" << endl;
		return 1;
	}
	char* filename = argv[1];
	
	if (!HT::InitMutex())
	{
		cerr << "initMutex failed" << endl;
		return 1;
	}

	WaitForSingleObject(HT::globalMutex, INFINITE);
	HT::HTHANDLE* ht = HT::Open(filename);
	if ( ht == NULL )
	{
		cerr << "ht11Open: Open error" << endl;
		return 1;
	}
	if ( ht != NULL )
	{
		cout << "ht-storage opened\nfilename: " << filename << " snapInterval: " << ht->SecSnapshotInterval << " capacity: " << ht->Capacity
			<< " maxkeylength: " << ht->MaxKeyLength << " maxpayloadlength: " << ht->MaxPayloadLength << endl;
	}
	cout << "\npress any key to close the application " << endl;
	do
	{
		Sleep(ht->SecSnapshotInterval * 1000);
		HT::Snap(ht);
	} while ( !_kbhit() );

	bool closed = HT::Close(ht);
	ReleaseMutex(HT::globalMutex);
	if (closed)
	{
		cout << "closed successfully" << endl;
		return 0;
	}
	cout << "close error" << endl;
	return 0;

}