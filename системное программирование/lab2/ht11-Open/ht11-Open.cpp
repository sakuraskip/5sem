#include "HT.h"

using namespace std;

int main(int argc, char** argv)
{
	if ( argc < 2 )
	{
		cerr << "usage: ht11open.exe {filename}" << endl;
		return 1;
	}
	char* filename = argv[1];
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

}