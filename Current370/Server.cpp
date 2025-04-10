#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cstring>
#include <iostream>

#pragma comment(lib, "ws2_32.lib") // Link Winsock library

#define PORT 8080

int main() {
    WSADATA wsaData;

    // Initialize Winsock
    std::cout << "Initializing Winsock..." << std::endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        return 1;
    }

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Create socket
    std::cout << "Creating socket..." << std::endl;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        perror("Socket creation failed");
        WSACleanup();
        return 1;
    }
    std::cout << "Socket created successfully!" << std::endl;

    // Set socket options
    std::cout << "Setting socket options..." << std::endl;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Define address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    std::cout << "Binding socket..." << std::endl;
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    std::cout << "Socket bound successfully!" << std::endl;

    // Listen for connections
    std::cout << "Listening for connections..." << std::endl;
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    std::cout << "Server is listening on port " << PORT << std::endl;

    // Accept a connection
    std::cout << "Waiting for a client to connect..." << std::endl;
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket == INVALID_SOCKET) {
        perror("Accept failed");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    std::cout << "Client connected successfully!" << std::endl;

    // Read data from client
    std::cout << "Waiting for message from client..." << std::endl;
    int valread = recv(new_socket, buffer, 1024, 0);
    if (valread > 0) {
        std::cout << "Message from client: " << buffer << std::endl;
    } else {
        perror("Receive failed");
    }

    // Clean up
    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();
    std::cout << "Server shutdown successfully." << std::endl;

    return 0;
}
