#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    char buffer[1024];
    int bytesReceived;
    
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return 1;
    }
    
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(40000);
    
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = INADDR_ANY;
    clientAddr.sin_port = 0;  
    
    if (bind(clientSocket, (sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    
   
    
    std::string message = "hello from client";
    
    int sendResult = sendto(clientSocket, message.c_str(), message.length(), 0,
                            (sockaddr*)&serverAddr, sizeof(serverAddr));
    
    if (sendResult == SOCKET_ERROR) {
        std::cerr << "sendto failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    
    
    // int timeout = 2000; 
    // setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    
    sockaddr_in fromAddr;
    int fromSize = sizeof(fromAddr);
    
    bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer) - 1, 0,
                             (sockaddr*)&fromAddr, &fromSize);
    
    if (bytesReceived == SOCKET_ERROR) {
        int error = WSAGetLastError();
            std::cerr << "recvfrom failed: " << error << std::endl;
    } else {
        buffer[bytesReceived] = '\0';
        std::cout << buffer << std::endl;
    }
    
    closesocket(clientSocket);
    WSACleanup();
    
    return 0;
}