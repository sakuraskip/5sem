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
	HANDLE pipe;

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
		for (;;)
		{
			if (WaitNamedPipeA(pipename.c_str(), INFINITE))
			{
				cout << "waited for pipe :)" << endl;
				break;
			}
		}

		pipe = CreateFileA(
			pipename.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			NULL, NULL
		);
		if (pipe == INVALID_HANDLE_VALUE)
		{
			throw SetErrorMsgText("create file pipe error", 1);
		}

		int num = 1;
		char message[255];
		char inputBuffer[255];

		for (int i = 0; i < messageAmount; i++)
		{
			sprintf_s(message, sizeof(message), "hello from client %d", num);

			DWORD bytesRead = 0;
			if (!TransactNamedPipe(pipe, message, strlen(message) + 1, inputBuffer, sizeof(inputBuffer), &bytesRead, NULL))
			{
				throw SetErrorMsgText("transact named pipe error", 5);
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

		if (!CloseHandle(pipe))
		{
			throw SetErrorMsgText("close handle error", 2);
		}
	}
	catch (string msg)
	{
		cerr << msg << endl;
	}

	system("pause");
}