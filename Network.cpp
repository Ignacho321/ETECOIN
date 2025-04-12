#define _CRT_SECURE_NO_WARNINGS

#include "Network.h"
#include "P2P.h"  // Usa la lógica para procesar bloques
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

void handleClient(SOCKET clientSocket) {
    char buffer[4096];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::string receivedData(buffer);
        std::cout << "📥 Datos recibidos: " << receivedData << std::endl;
        handleIncomingBlock(receivedData);  // función definida en P2P.cpp
    }
    closesocket(clientSocket);
}

void startServer(int port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "❌ Error al iniciar Winsock\n";
        return;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "❌ Error al crear el socket\n";
        return;
    }

    sockaddr_in serverAddr {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "❌ Error al hacer bind\n";
        closesocket(serverSocket);
        return;
    }

    listen(serverSocket, SOMAXCONN);
    std::cout << "🟢 Servidor escuchando en el puerto " << port << "...\n";

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket != INVALID_SOCKET) {
            std::thread(handleClient, clientSocket).detach();
        }
    }

    closesocket(serverSocket);
    WSACleanup();
}

void connectToPeer(const std::string& ip, int port, const std::string& data) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET peerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (peerSocket == INVALID_SOCKET) {
        std::cerr << "❌ Error al crear socket de cliente\n";
        return;
    }

    sockaddr_in peerAddr {};
    peerAddr.sin_family = AF_INET;
    peerAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &peerAddr.sin_addr);

    if (connect(peerSocket, (sockaddr*)&peerAddr, sizeof(peerAddr)) == SOCKET_ERROR) {
        std::cerr << "❌ Error al conectar al nodo " << ip << ":" << port << "\n";
        closesocket(peerSocket);
        return;
    }

    send(peerSocket, data.c_str(), data.size(), 0);
    closesocket(peerSocket);
    WSACleanup();
}
