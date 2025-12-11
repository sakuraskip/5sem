#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    char buffer[1024];
    int bytesReceived;
    
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "wsastartup fail: " << WSAGetLastError() << std::endl;
        return 1;
    }
    
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "socket error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(40000);
    
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "bind fail: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    std::cout << "server on port " << 40000 << std::endl;
    
    while (true) {
        bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer) - 1, 0,
                                 (sockaddr*)&clientAddr, &clientAddrSize);
        
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "recvfrom failed: " << WSAGetLastError() << std::endl;
            break;
        }
        
        buffer[bytesReceived] = '\0';
        std::string clientMessage = buffer;
        
        std::cout << "from " << ntohs(clientAddr.sin_port) 
                  << ": " << clientMessage << std::endl;
        
        std::string response = "ECHO: " + clientMessage;
        
        sendto(serverSocket, response.c_str(), response.length(),
         0, (sockaddr*)&clientAddr, clientAddrSize);
    }
    
    closesocket(serverSocket);
    WSACleanup();
    
    std::cout << "server closed" << std::endl;
    return 0;
}