
#include "HT.h"

using namespace std;
int main(int argc, char** argv)
{
	if (argc < 6)
	{
		cerr << "usage: ht11-create.exe {capacity} {snapInterval} {maxkeylength} {maxpayloadlength} {filename.ht}" << endl;
		return 1;
	}
	int capacity = atoi(argv[1]), snapInterval = atoi(argv[2]), maxkeylength = atoi(argv[3]), maxpayloadlength = atoi(argv[4]);
	char* filename = argv[5];
	if (capacity <= 0 || snapInterval <= 0 || maxkeylength <= 0 || maxpayloadlength <= 0 || )
	{
		cerr << "all the parameters should be more than 0" << endl;
		return 1;
	}
	HT::HTHANDLE* ht1 = HT::Create(capacity,snapInterval,maxkeylength,maxpayloadlength,filename,0);
	if (ht1 != NULL)
	{
		cout << "ht-storage created\nfilename: " << filename << " snapInterval: " << snapInterval << " capacity: " << capacity
			<< " maxkeylength: " << maxkeylength << " maxpayloadlength: " << maxpayloadlength << endl;
	}
	else
	{
		cerr << "ht11-create: create error" << endl;
		return 1;
	}
}