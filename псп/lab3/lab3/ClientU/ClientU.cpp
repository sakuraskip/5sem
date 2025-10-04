#pragma warning(disable:4996) 
#include "WinSock2.h"
#include "WS2tcpip.h"
#pragma comment(lib, "WS2_32.lib")
#include <iostream>
#include <cstring>
#include <tchar.h>
#include <string>
#include <chrono>
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
int main()
{
    SOCKET sC;
    WSADATA wsadata;
    int messageAmount;
    do
    {
        cout << "message amount?: ";
        cin >> messageAmount;
    } while (messageAmount <= 0);
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsadata) != 0)
            throw SetErrorMsgText("Startup: ", WSAGetLastError());

        if ((sC = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket: ", WSAGetLastError());

        sockaddr_in servSettings;
        servSettings.sin_family = AF_INET;
        servSettings.sin_port = htons(2000);
        servSettings.sin_addr.S_un.S_addr = inet_addr("10.30.248.135");//записать адрес сервера
        int servSize = sizeof(servSettings);

        char inputBuffer[50];
        char outputBuffer[50] = "hello from clientU:1\n";
        int lengthInputB = 0;
        int lengthOutputB = 0;

        for (int i = 0; i < messageAmount; i++)
        {
            if ((lengthOutputB = sendto(sC, outputBuffer, strlen(outputBuffer) + 1, 0,(sockaddr*)&servSettings, sizeof(servSettings))) == SOCKET_ERROR)
                throw SetErrorMsgText("sendto : ", WSAGetLastError());

            if ((lengthInputB = recvfrom(sC, inputBuffer, sizeof(inputBuffer) - 1, 0,(sockaddr*)&servSettings, &servSize)) == SOCKET_ERROR)
                throw SetErrorMsgText("recvfrom : ", WSAGetLastError());

           
            inputBuffer[lengthInputB] = '\0';

            cout << "server says: " << inputBuffer << endl;

            string receivedMessage = inputBuffer;
            int pos = receivedMessage.find(':');
            if (pos == string::npos)
                cout << "no ':' found in message" << endl;

            int number = 0;
            try {
                number = stoi(receivedMessage.substr(pos + 1));
                //ACK NACK
            }
            catch (exception ex) {
                number = 0;
            }
            number++;

            string messageToServer = receivedMessage.substr(0, pos) + ":" + to_string(number) + "\n";

            strcpy(outputBuffer, messageToServer.c_str());
        }

        //closing
        if (closesocket(sC) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket: ", WSAGetLastError());

        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleaunp: ", WSAGetLastError());
    }
    catch (string msg)
    {
        cout << "\n" << msg << endl;
    }
    return 0;
}