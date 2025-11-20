// ServiceLibrary.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "framework.h"
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <cstring>
#include <string>
#pragma comment(lib, "WS2_32.lib")

using namespace std;
#include "DefineTableService.h" // макро для TableService

#define _CRT_SECURE_NO_WARNINGS


struct Contact
{
    enum TE { EMPTY, ACCEPT, CONTACT } type;
    enum ST { WORK, ABORT, TIMEOUT, FINISH } sthread;

    SOCKET s;
    SOCKADDR_IN prms;
    int lprms;
    HANDLE hthread;
    HANDLE htimer;
    char msg[50];
    char srvname[15];

    Contact(TE t = EMPTY, const char* namesrv = "")
    {
        memset(&prms, 0, sizeof(SOCKADDR_IN));
        lprms = sizeof(SOCKADDR_IN);
        type = t;
        s = INVALID_SOCKET;
        hthread = NULL;
        htimer = NULL;
        sthread = WORK;
        msg[0] = '\0';
        srvname[0] = '\0';
        if (namesrv)
            strncpy_s(srvname, namesrv, sizeof(srvname) - 1);
        srvname[sizeof(srvname) - 1] = '\0';
    }

    void SetST(ST sth, const char* m = "")
    {
        sthread = sth;
        if (m)
            strncpy_s(msg, m, sizeof(msg) - 1);
        msg[sizeof(msg) - 1] = '\0';
    }
};


DWORD WINAPI EchoServer(LPVOID param)
{
    if (param == nullptr)
        return 1;

    Contact* contact = (Contact*)param;
    SOCKET sClient = contact->s;

    const int bufSize = 512;
    char buffer[bufSize];

    contact->SetST(Contact::WORK, "echo server launched");

    try
    {
        while (true)
        {
            int received = recv(sClient, buffer, bufSize - 1, 0);

            if (received == SOCKET_ERROR)
            {
                int err = WSAGetLastError();
                if (err == WSAEWOULDBLOCK)
                {
                    Sleep(50);
                    continue;
                }
                else
                {
                    cout << "echo server: recv error " << err << endl;
                    break;
                }
            }
            else if (received == 0)
            {
                cout << "Client disconnected" << endl;
                break;
            }

            buffer[received] = '\0';

            string message(buffer);
            if (message == "exit\n" || message == "quit\n" || message.empty())
            {
                cout << "Echo server received exit command" << endl;
                break;
            }

            if (!message.empty() && message.back() == '\n')
                message.pop_back();

            string response = "echo: " + message;

            int sent = send(sClient, response.c_str(), (int)response.length(), 0);
            if (sent == SOCKET_ERROR)
            {
                cout << "echo server: send error: " << WSAGetLastError() << endl;
                break;
            }

            cout << "Echo server processed: '" << message << "' -> sent: '" << response << "'" << endl;
        }
    }
    catch (std::exception& e)
    {
        cout << "echo server exception: " << e.what() << endl;
    }

    contact->SetST(Contact::FINISH, "echo server done");

    delete contact;

    return 0;
}

BEGIN_TABLESERVICE              // таблица 
ENTRYSERVICE("echo", EchoServer)
//ENTRYSERVICE("time", TimeServer),
//ENTRYSERVICE("rand", RandServer)
END_TABLESERVICE;
const int SIZETS = sizeof(__TableService) / sizeof(__TableEntry);

extern "C" __declspec(dllexport) HANDLE SSS(char* id, LPVOID prm) {
    HANDLE rc = NULL;
    int  i = 0;
    while (i < SIZETS && strcmp(TABLESERVICE_ID(i), id) != 0)i++;
    if (i < SIZETS)
        rc = CreateThread(NULL, NULL, TABLESERVICE_FN(i), prm, NULL, NULL);
    return rc;
};
