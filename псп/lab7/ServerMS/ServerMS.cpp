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
	cout << "hostname: " << GetHostName();
	HANDLE mail;
	try
	{
		mail = CreateMailslotA(mailNAME.c_str(), 500, 180000, NULL);
		if (mail == INVALID_HANDLE_VALUE)
		{
			throw SetErrorMsgText("create mail slot error", 1);
		}
		for(;;)
		{
			char inputBuffer[255];
			DWORD inputBytes = 0;

			if (!ReadFile(mail, inputBuffer, sizeof(inputBuffer), &inputBytes, NULL))
			{
				DWORD er = GetLastError();
				if (er == ERROR_SEM_TIMEOUT)
				{
					cout << "timed out";
					throw SetErrorMsgText("connection timed out", er);
				}
				else throw SetErrorMsgText("readfile error", 2);
			}
			inputBuffer[inputBytes-1] = '\0';
			cout << "client says: " << inputBuffer << endl;
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