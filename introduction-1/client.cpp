#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    cout << "Terhubung ke server.\n";

    char buffer[1024];
    string message;

    while (true) {
        // Kirim pesan ke server
        cout << "Client: ";
        getline(cin, message);
        send(clientSocket, message.c_str(), message.length(), 0);
        if (message == "exit") break;

        // Terima balasan dari server
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) break;
        buffer[bytesReceived] = '\0';
        if (strcmp(buffer, "exit") == 0) break;

        cout << "Server: " << buffer << endl;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
