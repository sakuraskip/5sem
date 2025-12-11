#pragma warning(disable:4996)
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <iostream>
#include <list>
#include <string>
#include <cstring>
#define MAX_ITERATIONS 10
#pragma comment(lib, "WS2_32.lib")
using namespace std;

string libraryName = "ServiceLibrary";
typedef HANDLE(__cdecl* dll_SSS)(char* id, LPVOID prm);
HMODULE hServiceDll = NULL;
dll_SSS pfnSSS = NULL;
HANDLE g_hAcceptThread = NULL;




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
            strncpy(msg, m, sizeof(msg) - 1);
        msg[sizeof(msg) - 1] = '\0';
    }
};

struct APCParam {
    Contact* contact;
    SYSTEMTIME startTime;
    SYSTEMTIME endTime;
    bool isStart; 
};

enum CommandType { cmd_start, cmd_stop, cmd_get, cmd_unknown, cmd_exit };

CommandType ParseCommand(const char* cmd)
{
    if (strcmp(cmd, "start") == 0) return cmd_start;
    if (strcmp(cmd, "stop") == 0) return cmd_stop;
    if (strcmp(cmd, "exit") == 0) return cmd_exit;
    return cmd_unknown;
}

list<Contact> contacts;
CRITICAL_SECTION scListContact;
SOCKET sS = INVALID_SOCKET;
volatile CommandType g_cmd = cmd_start;
int port = 2000;

bool LoadServiceDll()
{
    hServiceDll = LoadLibraryA("..\\Debug\\ServiceLibrary.dll");
    if (!hServiceDll)
    {
        cerr << "LoadLibrary failed: " << endl;
        return false;
    }

    pfnSSS = (dll_SSS)GetProcAddress(hServiceDll, "SSS");
    if (!pfnSSS)
    {
        cerr << "GetProcAddress failed: " << GetLastError() << endl;
        FreeLibrary(hServiceDll);
        return false;
    }
    return true;
}

void UnloadServiceDll()
{
    if (hServiceDll)
    {
        FreeLibrary(hServiceDll);
        hServiceDll = NULL;
        pfnSSS = NULL;
    }
}
void __stdcall StartServiceAPC(ULONG_PTR dwParam) {
    APCParam* param = (APCParam*)dwParam;
    if (param && param->contact) {
        char timeStr[9];
        sprintf_s(timeStr, "%02d:%02d:%02d",
            param->startTime.wHour, param->startTime.wMinute, param->startTime.wSecond);

        cout << "---------------START service----------------" << endl;
        cout << "server: " << param->contact->srvname << endl;
        cout << "start time: " << timeStr << endl;
        cout << "client: " << inet_ntoa(param->contact->prms.sin_addr)
            << ":" << ntohs(param->contact->prms.sin_port) << endl;
        cout << "--------------------------------------------" << endl;
    }
    delete param;
}

void __stdcall FinishServiceAPC(ULONG_PTR dwParam) {
    APCParam* param = (APCParam*)dwParam;
    if (param && param->contact) {
        char timeStr[9];
        sprintf_s(timeStr, "%02d:%02d:%02d",
            param->endTime.wHour, param->endTime.wMinute, param->endTime.wSecond);

        cout << "---------------finish service----------------" << endl;
        cout << "Server: " << param->contact->srvname << endl;
        cout << "End time: " << timeStr << endl;
        cout << "client: " << inet_ntoa(param->contact->prms.sin_addr)
            << ":" << ntohs(param->contact->prms.sin_port) << endl;
        cout << "Status: " << param->contact->msg << endl;
        cout << "----------------------------------------------" << endl;
        EnterCriticalSection(&scListContact);
        if (param->contact->htimer) {
            CancelWaitableTimer(param->contact->htimer);
            CloseHandle(param->contact->htimer);
            param->contact->htimer = NULL;
        }
        LeaveCriticalSection(&scListContact);
    }
    delete param;
}
void _stdcall TimerAPCProc(
    LPVOID lpArgToCompletionRoutine,
    DWORD dwTimerLowValue,
    DWORD dwTimerHighValue
) {
    Contact* contact = (Contact*)lpArgToCompletionRoutine;
    if (!contact) return;
    cout << "timerapcproc" << endl;
    EnterCriticalSection(&scListContact);

    if (contact->hthread && contact->type == Contact::CONTACT) {
        TerminateThread(contact->hthread, 0);
        CloseHandle(contact->hthread);
        contact->hthread = NULL;

        contact->SetST(Contact::TIMEOUT, "Service timeout (exceeded 1 minute)");

        if (contact->s != INVALID_SOCKET) {
            closesocket(contact->s);
            contact->s = INVALID_SOCKET;
        }

        if (contact->htimer) {
            CancelWaitableTimer(contact->htimer);
            CloseHandle(contact->htimer);
            contact->htimer = NULL;
        }

        contact->type = Contact::EMPTY;

        cout << "TimerAPC: Service timeout, thread terminated" << endl;
    }

    LeaveCriticalSection(&scListContact);
}

DWORD WINAPI EchoServer(LPVOID param);
DWORD WINAPI TimeServer(LPVOID param);
DWORD WINAPI RandServer(LPVOID param);
DWORD WINAPI GarbageCleaner(LPVOID param);




DWORD WINAPI GarbageCleaner(LPVOID param)
{
    CommandType* cmd = (CommandType*)param;

    while (*cmd != cmd_exit)
    {
        EnterCriticalSection(&scListContact);
        
        auto it = contacts.begin();
        while (it != contacts.end())
        {
            if (it->type == Contact::EMPTY)
            {
                if (it->s != INVALID_SOCKET)
                {
                    closesocket(it->s);
                    it->s = INVALID_SOCKET;
                }

                if (it->hthread != NULL)
                {
                    CloseHandle(it->hthread);
                    it->hthread = NULL;
                }

                if (it->htimer != NULL)
                {
                    CloseHandle(it->htimer);
                    it->htimer = NULL;
                }

                it = contacts.erase(it);
                cout << "garbace collector: erased" << endl;
            }
            else
            {
                ++it;
            }
        }

        LeaveCriticalSection(&scListContact);

        Sleep(5000);

    }

    cout << "GarbageCleaner: exiting" << endl;
    return 0;
}
bool AcceptCycle(int iterations)
{
    bool rc = false;
    Contact c(Contact::ACCEPT, "EchoServer");

    while (iterations-- > 0 && !rc)
    {
        c.s = accept(sS, (sockaddr*)&c.prms, &c.lprms);
        if (c.s == INVALID_SOCKET)
        {
            int err = WSAGetLastError();
            if (err == WSAEWOULDBLOCK) //no connections, callin' sleep
            {
                SleepEx(0, TRUE);
            }
            break;
        }
        else
        {
            rc = true; 
            c.type = Contact::ACCEPT;
            c.SetST(Contact::WORK, "Client connected");

            EnterCriticalSection(&scListContact);
            contacts.push_front(c);
            LeaveCriticalSection(&scListContact);

            cout << "New client connected: " << inet_ntoa(c.prms.sin_addr)
                << ":" << ntohs(c.prms.sin_port) << endl;
        }
    }
    return rc;
}
bool SetSocketBlockingMode(SOCKET s, bool blocking)
{
    u_long mode = blocking ? 0 : 1;  //0 - blocking
    int res = ioctlsocket(s, FIONBIO, &mode);
    if (res == SOCKET_ERROR)
    {
        cout << "setsocketMode error: " << WSAGetLastError() << endl;
        return false;
    }
    return true;
}
void CommandsCycle(CommandType& cmd)
{
    int iterations = 0;

    while (cmd != cmd_exit)
    {
        switch (cmd)
        {
        case cmd_start:
            cmd = cmd_get;
            iterations = MAX_ITERATIONS;
            break;

        case cmd_stop:
            cmd = cmd_get;
            iterations = 0;
            break;

        case cmd_get:
            SleepEx(0,TRUE);
            break;

        default:
            SleepEx(0,TRUE);
            break;
        }

        if (iterations > 0)
        {
            if (AcceptCycle(iterations))
            {
                cmd = cmd_get;
                //echo server
            }
        }
        else
        {
            SleepEx(0, TRUE);
        }
    }
}
DWORD WINAPI DispatchServer(LPVOID param)
{
    char buf[256];
    while (g_cmd != cmd_exit)
    {
        EnterCriticalSection(&scListContact);

        auto contact = contacts.begin();
        while (contact != contacts.end())
        {
            if (contact->type == Contact::EMPTY)
            {
                ++contact;
                continue;
            }

            if (contact->type == Contact::ACCEPT)
            {
                SOCKET sc = contact->s;
                char buf[256];
                int received = recv(sc, buf, sizeof(buf) - 1, 0);

                if (received == SOCKET_ERROR)
                {
                    int error = WSAGetLastError();
                    if (error != WSAEWOULDBLOCK)
                    {
                        cout << "DispatchServer: recv error " << error << ", marking for cleanup" << endl;
                        contact->type = Contact::EMPTY; 
                    }
                    ++contact;
                    continue;
                }
                else if (received == 0)
                {
                    cout << "DispatchServer: client disconnected, marking for cleanup" << endl;
                    contact->type = Contact::EMPTY; 
                    ++contact;
                    continue;
                }

                buf[received] = '\0';
                string commandFromClient(buf);
                cout << "Received from client: " << commandFromClient << endl;

                if (commandFromClient.find("echo") != string::npos ||
                    commandFromClient.find("time") != string::npos ||
                    commandFromClient.find("rand") != string::npos)
                {
                    if (!SetSocketBlockingMode(contact->s, true))
                    {
                        cout << "DispatchServer: failed to set blocking mode, marking for cleanup" << endl;
                        contact->type = Contact::EMPTY;
                        ++contact;
                        continue;
                    }
                    SYSTEMTIME startTime;
                    GetLocalTime(&startTime);

                    APCParam* startParam = new APCParam();
                    startParam->contact = &(*contact);
                    startParam->startTime = startTime;
                    startParam->isStart = true;
                    cout << "ale" << endl;
                    if (!QueueUserAPC(StartServiceAPC, g_hAcceptThread, (ULONG_PTR)startParam)) {
                        cout << "QueueUserAPC for start failed: " << GetLastError() << endl;
                        delete startParam;
                    }

                    contact->type = Contact::CONTACT;
                    contact->SetST(Contact::WORK, (commandFromClient + " server requested").c_str());

                    char id[16] = { 0 };
                    strncpy_s(id, commandFromClient.substr(0, commandFromClient.length() - 1).c_str(), sizeof(id) - 1);

                    Contact* pContact = new Contact(*contact);


                    if (contact->hthread != NULL)
                    {
                        CloseHandle(contact->hthread);
                        contact->hthread = NULL;
                    }
                    contact->htimer = CreateWaitableTimer(NULL, FALSE, NULL);
                    if (contact->htimer) {
                        LARGE_INTEGER dueTime;
                        dueTime.QuadPart = -60 * 10000000; 

                        if (!SetWaitableTimer(contact->htimer, &dueTime, 0, TimerAPCProc, (PVOID) & (*contact), FALSE)) {
                            cout << "DispatchServer: SetWaitableTimer failed" << endl;
                            CloseHandle(contact->htimer);
                            contact->htimer = NULL;
                        }
                        else {
                            cout << "DispatchServer: timer set for 60 seconds for service" << endl;
                        }
                    }
                    else {
                        cout << "DispatchServer: CreateWaitableTimer failed" << endl;
                    }
                    HANDLE hThread = pfnSSS(id, (LPVOID)(pContact));
                    if (hThread)
                    {
                        contact->hthread = hThread;
                        cout << "DispatchServer: started " << id << " server thread" << endl;
                    }
                    else
                    {
                        cout << "DispatchServer: DLL thread failed, marking for cleanup" << endl;
                        delete pContact;
                        contact->type = Contact::EMPTY; 
                    }
                }
                else
                {
                    cout << "DispatchServer: unknown command, marking for cleanup" << endl;
                    contact->type = Contact::EMPTY; 
                }
            }
            else if (contact->type == Contact::CONTACT)
            {
                
                if (contact->hthread && WaitForSingleObject(contact->hthread, 0) == WAIT_OBJECT_0)
                {
                    DWORD exitCode;
                    GetExitCodeThread(contact->hthread, &exitCode);
                    SYSTEMTIME endTime;
                    GetLocalTime(&endTime);

                    APCParam* finishParam = new APCParam();
                    finishParam->contact = &(*contact);
                    finishParam->endTime = endTime;
                    finishParam->isStart = false;
                    if (!QueueUserAPC(FinishServiceAPC, g_hAcceptThread, (ULONG_PTR)finishParam)) {
                        cout << "QueueUserAPC for finish failed: " << GetLastError() << endl;
                        delete finishParam;
                    }
                    cout << "DispatchServer: DLL thread finished with code " << exitCode << ", marking for cleanup" << endl;

                    CloseHandle(contact->hthread);
                    contact->hthread = NULL;
                    contact->type = Contact::EMPTY; 
                }
            }

            ++contact;
        }
        LeaveCriticalSection(&scListContact);
        SleepEx(50,TRUE);
    }
    return 0;
}
DWORD WINAPI AcceptServer(LPVOID param)
{
    try
    {
        WSADATA wsadata;
        if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
            throw string("WSAStartup failed");

        sS = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sS == INVALID_SOCKET)
            throw string("socket creation failed");

        sockaddr_in servSettings{};
        servSettings.sin_family = AF_INET;
        servSettings.sin_port = htons(port);
        servSettings.sin_addr.s_addr = INADDR_ANY;

        if (bind(sS, (sockaddr*)&servSettings, sizeof(servSettings)) == SOCKET_ERROR)
            throw string("bind failed");

        if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
            throw string("listen failed");

        u_long nonblk = 1;
        if (ioctlsocket(sS, FIONBIO, &nonblk) == SOCKET_ERROR)
            throw string("ioctlsocket failed");


        CommandType cmd = g_cmd;
        CommandsCycle(cmd);

        closesocket(sS);
        WSACleanup();
    }
    catch (const string& ex)
    {
        cerr << "acceptServer exception: " << ex << endl;
    }
    EnterCriticalSection(&scListContact);
    for (auto& contact : contacts)
    {
        contact.type = Contact::EMPTY;
    }
    LeaveCriticalSection(&scListContact);

    cout << "AcceptServer: exiting" << endl;
    return 0;
}

int main(int argc, char** argv)
{
    if (argc == 3)
    {
        libraryName = argv[2];
        int p = atoi(argv[1]);
        if (p > 0 && p < 65536)
            port = p;
    }
    else if (argc == 2)
    {
        int p = atoi(argv[1]);
        if (p > 0 && p < 65536)
            port = p;
    }

    cout << "port: " << port << endl;
    if (!LoadServiceDll())
    {
        cout << "load service dll fail" << endl;
        return 1;
    }

    InitializeCriticalSection(&scListContact);
    HANDLE threads[10];
    threads[0] = CreateThread(NULL, 0, AcceptServer, (LPVOID)&g_cmd, 0, NULL);
    if (threads[0] == NULL)
    {
        cerr << "create thread failed" << endl;
        DeleteCriticalSection(&scListContact);
        return 1;
    }
    g_hAcceptThread = threads[0];

    threads[1] = CreateThread(NULL, 0, DispatchServer, (LPVOID)&g_cmd, 0, NULL);
    cout << "press any key to start the garbage cleaner " << endl;
    cin.get();
    threads[2] = CreateThread(NULL, 0, GarbageCleaner, (LPVOID)&g_cmd, 0, NULL);
    
    

    WaitForMultipleObjects(3,threads,TRUE, INFINITE);

    CloseHandle(threads[0]);
    CloseHandle(threads[1]);
    CloseHandle(threads[2]);


    DeleteCriticalSection(&scListContact);

    UnloadServiceDll();
    return 0;
}