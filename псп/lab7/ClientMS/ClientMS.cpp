#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

string SetErrorMsgText(string msgText, int errorCode)
{
	return msgText + to_string(errorCode);
};
string GetHostName() {
	char hostname[256];
	DWORD size = sizeof(hostname);
	GetComputerNameA(hostname, &size);
	return string(hostname);
}

string mailNAME = "\\\\.\\mailslot\\KOROBKA";
int main()
{
	HANDLE mail;
	string hostname;
	int messageCount = 0;
	cout << "hostname?: ";
	cin >> hostname;
	cout << endl;
	string task6Name = "\\\\" + hostname + "\\mailslot\\KOROBKA";
	string task7Name = "\\\\*\\mailslot\\KOROBKA";
	try
	{
		mail = CreateFileA(task7Name.c_str(), GENERIC_WRITE,FILE_SHARE_READ| FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if (mail == INVALID_HANDLE_VALUE)
		{
			throw SetErrorMsgText("create mail slot error", 1);
		}
		for (int i = 0; i < 1000; i++)
		{
			char inputBuffer[255] = "hello from client";
			DWORD inputBytes = 0;

			if (!WriteFile(mail, inputBuffer, sizeof(inputBuffer), &inputBytes, NULL))
			{
				throw SetErrorMsgText("readfile error", 2);
			}

		}
		
		if (!CloseHandle(mail))
		{
			throw SetErrorMsgText("close handle error", 3);
		}
		cout << "exiting" << endl;

	}
	catch (string msg)
	{
		cout << "\nexception: " << msg << endl;
	}

	system("pause");
	return 0;
}