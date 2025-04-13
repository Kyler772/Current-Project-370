#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cstring>
#include <iostream>

#pragma comment(lib, "ws2_32.lib") // Link Winsock library

#define PORT 8080

int main() {
    bool isEnd = false;
    WSADATA wsaData;

    // Initialize Winsock
    std::cout << "Initializing Winsock..." << std::endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        return 1;
    }

    // Create socket
    std::cout << "Creating socket..." << std::endl;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        perror("Socket creation failed");
        WSACleanup();
        return 1;
    }
    std::cout << "Socket created successfully!" << std::endl;

    // Define server address
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    std::cout << "Resolving server address..." << std::endl;
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // Connect to server
    std::cout << "Connecting to server..." << std::endl;
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    std::cout << "Connected to server successfully!" << std::endl;

    char userMessage[500];
    while (isEnd == false) {
        std::cout << "Enter your message: " << std::endl;
        std::cin.getline(userMessage, 500);

        // Send message to server
        std::cout << "Sending message to server..." << std::endl;
        if (send(sockfd, userMessage, strlen(userMessage), 0) < 0) {
            perror("Send failed");
        } else {
            std::cout << "Message sent successfully!" << std::endl;

            // Receive response from server
            char serverResponse[1024] = {0};
            int valread = recv(sockfd, serverResponse, 1024, 0);
            if (valread > 0) {
                std::cout << "Response from server: " << serverResponse << std::endl;
            } else {
                perror("Receive failed");
            }
        }

        // Check for "exit" command to end communication
        if (strncmp(userMessage, "exit", 4) == 0) {
            isEnd = true;
        }
    }

    // Clean up
    closesocket(sockfd);
    WSACleanup();
    std::cout << "Client shutdown successfully." << std::endl;

    return 0;
}
