#include <WinSock2.h>
#include <cstring>
#include <inaddr.h>
#include <iostream>
#include <minwindef.h>
#include <string>

std::string GetErrorMsgText(int code)
{
    std::string msgText;

    switch (code)
    {
    case WSAEINTR:
        msgText = "WSAEINTR. Interrupted function call";
        break;
    case WSAEACCES:
        msgText = "WSAEACCES. Permission denied";
        break;
    case WSAEFAULT:
        msgText = "WSAEFAULT. Bad address";
        break;
    case WSAEINVAL:
        msgText = "WSAEINVAL. Invalid argument";
        break;
    case WSAEMFILE:
        msgText = "WSAEMFILE. Too many open files";
        break;
    case WSAEWOULDBLOCK:
        msgText = "WSAEWOULDBLOCK. Resource temporarily unavailable";
        break;
    case WSAEINPROGRESS:
        msgText = "WSAEINPROGRESS. Operation now in progress";
        break;
    case WSAEALREADY:
        msgText = "WSAEALREADY. Operation already in progress";
        break;
    case WSAENOTSOCK:
        msgText = "WSAENOTSOCK. Socket operation on non-socket";
        break;
    case WSAEDESTADDRREQ:
        msgText = "WSAEDESTADDRREQ. Destination address required";
        break;
    case WSAEMSGSIZE:
        msgText = "WSAEMSGSIZE. Message too long";
        break;
    case WSAEPROTOTYPE:
        msgText = "WSAEPROTOTYPE. Protocol wrong type for socket";
        break;
    case WSAENOPROTOOPT:
        msgText = "WSAENOPROTOOPT. Bad protocol option";
        break;
    case WSAEPROTONOSUPPORT:
        msgText = "WSAEPROTONOSUPPORT. Protocol not supported";
        break;
    case WSAESOCKTNOSUPPORT:
        msgText = "WSAESOCKTNOSUPPORT. Socket type not supported";
        break;
    case WSAEOPNOTSUPP:
        msgText = "WSAEOPNOTSUPP. Operation not supported";
        break;
    case WSAEPFNOSUPPORT:
        msgText = "WSAEPFNOSUPPORT. Protocol family not supported";
        break;
    case WSAEAFNOSUPPORT:
        msgText = "WSAEAFNOSUPPORT. Address family not supported by protocol family";
        break;
    case WSAEADDRINUSE:
        msgText = "WSAEADDRINUSE. Address already in use";
        break;
    case WSAEADDRNOTAVAIL:
        msgText = "WSAEADDRNOTAVAIL. Cannot assign requested address";
        break;
    case WSAENETDOWN:
        msgText = "WSAENETDOWN. Network is down";
        break;
    case WSAENETUNREACH:
        msgText = "WSAENETUNREACH. Network is unreachable";
        break;
    case WSAENETRESET:
        msgText = "WSAENETRESET. Network dropped connection on reset";
        break;
    case WSAECONNABORTED:
        msgText = "WSAECONNABORTED. Software caused connection abort";
        break;
    case WSAECONNRESET:
        msgText = "WSAECONNRESET. Connection reset by peer";
        break;
    case WSAENOBUFS:
        msgText = "WSAENOBUFS. No buffer space available";
        break;
    case WSAEISCONN:
        msgText = "WSAEISCONN. Socket is already connected";
        break;
    case WSAENOTCONN:
        msgText = "WSAENOTCONN. Socket is not connected";
        break;
    case WSAESHUTDOWN:
        msgText = "WSAESHUTDOWN. Cannot send after socket shutdown";
        break;
    case WSAETIMEDOUT:
        msgText = "WSAETIMEDOUT. Connection timed out";
        break;
    case WSAECONNREFUSED:
        msgText = "WSAECONNREFUSED. Connection refused";
        break;
    case WSAEHOSTDOWN:
        msgText = "WSAEHOSTDOWN. Host is down";
        break;
    case WSAEHOSTUNREACH:
        msgText = "WSAEHOSTUNREACH. No route to host";
        break;
    case WSAEPROCLIM:
        msgText = "WSAEPROCLIM. Too many processes";
        break;
    case WSASYSNOTREADY:
        msgText = "WSASYSNOTREADY. Network subsystem is unavailable";
        break;
    case WSAVERNOTSUPPORTED:
        msgText = "WSAVERNOTSUPPORTED. WINSOCK.DLL version out of range";
        break;
    case WSANOTINITIALISED:
        msgText = "WSANOTINITIALISED. Successful WSAStartup not yet performed";
        break;
    case WSAEDISCON:
        msgText = "WSAEDISCON. Graceful shutdown in progress";
        break;
    case WSATYPE_NOT_FOUND:
msgText = "WSATYPE_NOT_FOUND. Class type not found";
        break;
    case WSAHOST_NOT_FOUND:
        msgText = "WSAHOST_NOT_FOUND. Host not found";
        break;
    case WSATRY_AGAIN:
        msgText = "WSATRY_AGAIN. Non-authoritative host not found";
        break;
    case WSANO_RECOVERY:
        msgText = "WSANO_RECOVERY. This is a non-recoverable error";
        break;
    case WSANO_DATA:
        msgText = "WSANO_DATA. Valid name, no data record of requested type";
        break;
    case WSA_INVALID_HANDLE:
        msgText = "WSA_INVALID_HANDLE. Specified event object handle is invalid";
        break;
    case WSA_INVALID_PARAMETER:
        msgText = "WSA_INVALID_PARAMETER. One or more parameters are invalid";
        break;
    case WSA_IO_INCOMPLETE:
        msgText = "WSA_IO_INCOMPLETE. Overlapped I/O event object not in signaled state";
        break;
    case WSA_IO_PENDING:
        msgText = "WSA_IO_PENDING. Overlapped operations will complete later";
        break;
    case WSA_NOT_ENOUGH_MEMORY:
        msgText = "WSA_NOT_ENOUGH_MEMORY. Insufficient memory available";
        break;
    case WSA_OPERATION_ABORTED:
        msgText = "WSA_OPERATION_ABORTED. Overlapped operation aborted";
        break;
    case WSAEINVALIDPROCTABLE:
        msgText = "WSAEINVALIDPROCTABLE. Invalid procedure table from service provider";
        break;
    case WSAEINVALIDPROVIDER:
        msgText = "WSAEINVALIDPROVIDER. Invalid service provider version number";
        break;
    case WSAEPROVIDERFAILEDINIT:
        msgText = "WSAEPROVIDERFAILEDINIT. Unable to initialize a service provider";
        break;
    case WSASYSCALLFAILURE:
        msgText = "WSASYSCALLFAILURE. System call failure";
        break;
    }
    return msgText;
}

std::string SetErrorMsgText(std::string prefix, int code)
{
    return prefix + GetErrorMsgText(code);
}

int main()
{
    SOCKET ss;
    WSAData wsd;

    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsd) != 0) throw SetErrorMsgText("startup: ", WSAGetLastError());

        if ((ss = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) throw SetErrorMsgText("socket: ", WSAGetLastError());
        
        sockaddr_in ssin;
        ssin.sin_family = AF_INET;
        ssin.sin_port = htons(2000);
        ssin.sin_addr.S_un.S_addr = INADDR_ANY;

        printf("\nserver running at port: %d", ssin.sin_port);

        if (bind(ss, (sockaddr*)&ssin, sizeof(ssin)) == SOCKET_ERROR) throw SetErrorMsgText("bind: ", WSAGetLastError());

        char inbuffer[50], outbuffer[50] = "[Server] Hello!";
        int inlength, outlength, msg_count;

        while(true)
        {
            sockaddr_in cs;
            memset(&cs, 0, sizeof(cs));

            int cs_length = sizeof(cs);

            if ((inlength = recvfrom(ss, inbuffer, sizeof(inbuffer) - 1, 0, (sockaddr*)&cs, &cs_length)) == SOCKET_ERROR)
            {
                throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
            }

            printf("\n[Received from client]: %s", inbuffer);

            if ((outlength = sendto(ss, inbuffer, sizeof(inbuffer) - 1, 0, (sockaddr*)&cs, sizeof(ssin))) == SOCKET_ERROR)
            {
                throw SetErrorMsgText("sendto: ", WSAGetLastError());
            }

            if ((inlength = recvfrom(ss, inbuffer, sizeof(inbuffer) - 1, 0, (sockaddr*)&cs, &cs_length)) == SOCKET_ERROR)
            {
                throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
            }
            
            printf("\n[Received from client again]: %s", inbuffer);

            if ((outlength = sendto(ss, inbuffer, sizeof(inbuffer) - 1, 0, (sockaddr*)&cs, sizeof(ssin))) == SOCKET_ERROR)
            {
                throw SetErrorMsgText("sendto: ", WSAGetLastError());
            }            
        }

        if (closesocket(ss) == SOCKET_ERROR) 
        {
            throw SetErrorMsgText("close ss: ", WSAGetLastError());
        }

        if (WSACleanup() == SOCKET_ERROR) 
        {
            throw SetErrorMsgText("cleanup: ", WSAGetLastError());
        }
    }
catch (std::string error_msg)
    {
        std::cout << '\n' + error_msg << std::endl;
    }

    return 10;
}