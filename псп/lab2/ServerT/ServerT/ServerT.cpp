#pragma warning(disable:4996) 
#include "WinSock2.h"
#pragma comment(lib, "WS2_32.lib")
#include <iostream>
#include <cstring>
#include <string>

using namespace std;

string GetErrorMsgText(int code)
{
    string msgText;
    switch (code)
    {
    case WSAEINTR: msgText = "WSAEINTR"; break;
    case WSAEACCES:          msgText = "WSAEACCES"; break;
    case WSAEFAULT:          msgText = "WSAEFAULT"; break;
    case WSAEINVAL:          msgText = "WSAEINVAL"; break;
    case WSAEMFILE:          msgText = "WSAEMFILE"; break;
    case WSAEWOULDBLOCK:     msgText = "WSAEWOULDBLOCK"; break;
    case WSAEINPROGRESS:     msgText = "WSAEINPROGRESS"; break;
    case WSAEALREADY:        msgText = "WSAEALREADY"; break;
    case WSAENOTSOCK:        msgText = "WSAENOTSOCK"; break;
    case WSAEDESTADDRREQ:    msgText = "WSAEDESTADDRREQ"; break;
    case WSAEMSGSIZE:        msgText = "WSAEMSGSIZE"; break;
    case WSAEPROTOTYPE:      msgText = "WSAEPROTOTYPE"; break;
    case WSAENOPROTOOPT:     msgText = "WSAENOPROTOOPT"; break;
    case WSAEPROTONOSUPPORT: msgText = "WSAEPROTONOSUPPORT"; break;
    case WSAESOCKTNOSUPPORT: msgText = "WSAESOCKTNOSUPPORT"; break;
    case WSAEOPNOTSUPP:      msgText = "WSAEOPNOTSUPP"; break;
    case WSAEPFNOSUPPORT:    msgText = "WSAEPFNOSUPPORT"; break;
    case WSAEAFNOSUPPORT:    msgText = "WSAEAFNOSUPPORT"; break;
    case WSAEADDRINUSE:      msgText = "WSAEADDRINUSE"; break;
    case WSAEADDRNOTAVAIL:   msgText = "WSAEADDRNOTAVAIL"; break;
    case WSAENETDOWN:        msgText = "WSAENETDOWN"; break;
    case WSAENETUNREACH:     msgText = "WSAENETUNREACH"; break;
    case WSAENETRESET:       msgText = "WSAENETRESET"; break;
    case WSAECONNABORTED:    msgText = "WSAECONNABORTED"; break;
    case WSAECONNRESET:      msgText = "WSAECONNRESET"; break;
    case WSAENOBUFS:         msgText = "WSAENOBUFS"; break;
    case WSAEISCONN:         msgText = "WSAEISCONN"; break;
    case WSAENOTCONN:        msgText = "WSAENOTCONN"; break;
    case WSAESHUTDOWN:       msgText = "WSAESHUTDOWN"; break;
    case WSAETIMEDOUT:       msgText = "WSAETIMEDOUT"; break;
    case WSAECONNREFUSED:    msgText = "WSAECONNREFUSED"; break;
    case WSAEHOSTDOWN:       msgText = "WSAEHOSTDOWN"; break;
    case WSAEHOSTUNREACH:    msgText = "WSAEHOSTUNREACH"; break;
    case WSAEPROCLIM:        msgText = "WSAEPROCLIM"; break;
    case WSASYSNOTREADY:          msgText = "WSASYSNOTREADY"; break;
    case WSAVERNOTSUPPORTED:      msgText = "WSAVERNOTSUPPORTED"; break;
    case WSANOTINITIALISED:       msgText = "WSANOTINITIALISED"; break;
    case WSAEDISCON:              msgText = "WSAEDISCON"; break;
    case WSATYPE_NOT_FOUND:       msgText = "WSATYPE_NOT_FOUND"; break;
    case WSAHOST_NOT_FOUND:       msgText = "WSAHOST_NOT_FOUND"; break;
    case WSATRY_AGAIN:            msgText = "WSATRY_AGAIN"; break;
    case WSANO_RECOVERY:          msgText = "WSANO_RECOVERY"; break;
    case WSANO_DATA:              msgText = "WSANO_DATA"; break;
    case WSA_INVALID_HANDLE:      msgText = "WSA_INVALID_HANDLE"; break;
    case WSA_INVALID_PARAMETER:   msgText = "WSA_INVALID_PARAMETER"; break;
    case WSA_IO_INCOMPLETE:       msgText = "WSA_IO_INCOMPLETE"; break;
    case WSA_IO_PENDING:          msgText = "WSA_IO_PENDING"; break;
    case WSA_NOT_ENOUGH_MEMORY:   msgText = "WSA_NOT_ENOUGH_MEMORY"; break;
    case WSA_OPERATION_ABORTED:   msgText = "WSA_OPERATION_ABORTED"; break;
    case WSASYSCALLFAILURE:       msgText = "WSASYSCALLFAILURE"; break;
    }
    return msgText;
}

string SetErrorMsgText(string msgText, int errorCode)
{
    return msgText + GetErrorMsgText(errorCode);
};

void processClientApp(SOCKET sC)
{
    char inputBuffer[50];
    char outputBuffer[50] = "server: understood have a nice day\n";
    int lengthInputBuffer = 0;
    int lengthOutputBuffer = 0;
    const char* eof = "\0";
    string buff;
    int messageCount = 0;

    while (true)
    {
        if ((lengthInputBuffer = recv(sC, inputBuffer, sizeof(inputBuffer), NULL)) == SOCKET_ERROR)
            throw SetErrorMsgText("recv: ", WSAGetLastError());
        if (lengthInputBuffer == 0)
        {
            cout << "client disconnected\n";
            break;
        }
        buff.append(inputBuffer, lengthInputBuffer);

        size_t endlinePos;
        while ((endlinePos = buff.find('\n')) != string::npos)
        {
            messageCount++;

            string singleMessage = buff.substr(0, endlinePos);
            string output = singleMessage + " :" + to_string(messageCount);
            cout << "client says: " << output << endl;
            buff.erase(0, endlinePos + 1);

            const char* messageToClient = output.c_str();
            if ((lengthOutputBuffer = send(sC, messageToClient, strlen(messageToClient), NULL)) == SOCKET_ERROR)
                throw SetErrorMsgText("send: ", WSAGetLastError());

            if ((lengthInputBuffer = recv(sC, inputBuffer, sizeof(inputBuffer), NULL)) == SOCKET_ERROR)
                throw SetErrorMsgText("recv: ", WSAGetLastError());

        }

        memset(inputBuffer, 0, sizeof(inputBuffer));

    }

    // CLOSE SOCKET
    if (closesocket(sC) == SOCKET_ERROR)
        throw SetErrorMsgText("closesocket: ", WSAGetLastError());
}

int main()
{
    SOCKET sS;// сокет сервер
    WSADATA wsadata;
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsadata) != 0)
            throw SetErrorMsgText("Startup: ", WSAGetLastError());
        if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket: ", WSAGetLastError());

        sockaddr_in servSettings;
        servSettings.sin_family = AF_INET;
        servSettings.sin_port = htons(2000);
        servSettings.sin_addr.S_un.S_addr = INADDR_ANY;

        if (bind(sS, (sockaddr*)&servSettings, sizeof(servSettings)) == SOCKET_ERROR)
            throw SetErrorMsgText("bind connect: ", WSAGetLastError());
        
        if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
            throw SetErrorMsgText("listen: ", WSAGetLastError());

        while (true)
        {
           
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(sS, &readfds);

            timeval timeout; timeout.tv_sec = 60; timeout.tv_usec = 0;

            int selRes = select(0, &readfds, NULL, NULL, &timeout);
            if (selRes == SOCKET_ERROR)
                throw SetErrorMsgText("select: ", WSAGetLastError());
            else if (selRes == 0)
            {
                cout << "no client connected in 60s, waiting\n";
                break;
            }

            SOCKET sC; // сокет клиент
            sockaddr_in client;
            memset(&client, 0, sizeof(client));
            int lengthClient = sizeof(client);

            if ((sC = accept(sS, (sockaddr*)&client, &lengthClient)) == INVALID_SOCKET)
                throw SetErrorMsgText("accept client: ", WSAGetLastError());

            cout << "client address: " << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << "\n";

            processClientApp(sC);

           
        }
        if (closesocket(sS) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket: ", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleaunp: ", WSAGetLastError());
    }

    catch (string msg)
    {
        cout << "\n" << msg;
    }
    return 0;
}