#include <iostream>
#include <Windows.h>
#include <string>
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

int main()
{
	int messageAmount = 0;
	do
	{
		cout << "message amount?: ";
		cin >> messageAmount;
	} while (messageAmount <= 0);

	string serverName = GetHostName();
	string pipename = "\\\\" + serverName + "\\pipe\\paipa";

	try
	{
		int num = 1;
		char message[255];
		char inputBuffer[255];

		for (int i = 0; i < messageAmount; i++)
		{
			sprintf_s(message, sizeof(message), "hello from client %d", num);

			DWORD bytesRead = 0;
			if (!CallNamedPipeA(pipename.c_str(),
				message,
				strlen(message) + 1,
				inputBuffer,
				sizeof(inputBuffer),
				&bytesRead,
				NMPWAIT_WAIT_FOREVER))
			{
				throw SetErrorMsgText("call named pipe error", 5);
			}

			inputBuffer[bytesRead] = '\0';
			cout << "sent: " << message << endl;
			cout << "received: " << inputBuffer << endl;

			string response = inputBuffer;
			int lastSpace = response.find_last_of(" ");
			if (lastSpace != string::npos)
			{
				string numStr = response.substr(lastSpace + 1);
				num = stoi(numStr) + 1;
			}
			else
			{
				num++;
			}
		}
	}
	catch (string msg)
	{
		cerr << msg << endl;
	}

	system("pause");
}