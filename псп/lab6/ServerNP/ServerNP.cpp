#include <Windows.h>
#include <iostream>
#include <string>
using namespace std;

string SetErrorMsgText(string msgText, int errorCode)
{
	return msgText + to_string(errorCode);
};
string pipename = "\\\\.\\pipe\\paipa";

int main()
{
	HANDLE pipe;
	try
	{
		pipe = CreateNamedPipeA(
			pipename.c_str(),
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			NULL, NULL,
			INFINITE,
			NULL
		);
		if (!pipe)
		{
			throw SetErrorMsgText("create named pipe error: ", 1);
		}

		while (true)
		{

			if (!ConnectNamedPipe(pipe, NULL))
			{
				throw SetErrorMsgText("connect named pipe error: ", 2);
			}
			cout << "client connected" << endl;

			char inputBuffer[255];
			DWORD bytesRead = 0;

			while (true)
			{
				if (!ReadFile(pipe, inputBuffer, sizeof(inputBuffer), &bytesRead, NULL))
				{
					break;
				}

				if (bytesRead > 0)
				{
					inputBuffer[bytesRead] = '\0';
					cout << "Message from client: " << inputBuffer << endl;

					string clientMsg = inputBuffer;
					size_t lastSpace = clientMsg.find_last_of(" ");
					int num = 1;

					if (lastSpace != string::npos)
					{
						string numStr = clientMsg.substr(lastSpace + 1);
						num = stoi(numStr);
					}

					char message[255];
					sprintf_s(message, sizeof(message), "hello from server %d", num);

					DWORD bytesWritten = 0;
					if (!WriteFile(pipe, message, strlen(message) + 1, &bytesWritten, NULL))
					{
						throw SetErrorMsgText("write file error", 3);
					}
					cout << "Sent to client: " << message << endl;
				}
			}

			cout << "Client disconnected" << endl;

			if (!DisconnectNamedPipe(pipe))
			{
				throw SetErrorMsgText("disconnect named pipe error: ", 3);
			}
		}

		if (!CloseHandle(pipe))
		{
			throw SetErrorMsgText("close handle error: ", 4);
		}
	}
	catch (string msg)
	{
		cerr << msg << endl;
		if (pipe) CloseHandle(pipe);
	}
	system("pause");
}