#include "Server.hpp"
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>

// constructor:
Server::Server() : reactor(), serverSocket(-1) {}

//destructor:
Server::~Server()
{
    reactor.stopReactor();
    close(serverSocket);
}

// implementation for start():
void Server::start() {
    // Code to start the server using the reactor
    serverSocket = createServerSocket(9034);
    if (serverSocket == -1) {
        std::cerr << "Failed to create server socket." << std::endl;
        return;
    }

    reactor.addFD(serverSocket, [this]() {
        // Handle incoming connection
        int clientSocket = acceptConnection(serverSocket);
        if (clientSocket != -1) {
            // Handle client request
            std::cout << "Received a new client connection." << std::endl;
        }
    });

    reactor.startReactor();
    reactor.waitFor();
}

//implementation for stop():
void Server::stop() {
    // Code to stop the server
    reactor.stopReactor();
    if (serverSocket != -1) {
        close(serverSocket);
        serverSocket = -1;
    }
}

// implementation for createServerSocket():
int Server::createServerSocket(int port) 
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0)
    {
        throw std::runtime_error("--ERROR--Socket opening failed");
    }

    sockaddr_in address{};
    address.sin_family  = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) < 0) 
    {
        throw std::runtime_error("--ERROR--Binding failed");
    }

    if (listen(serverSocket, SOMAXCONN) < 0) 
    {
        throw std::runtime_error("--ERROR--Listening on socket failed");
    }
    return serverSocket; // Placeholder return value
}

// implementation for acceptConnection():
int Server::acceptConnection(int serverSocket) 
{
    struct sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);

    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket < 0) 
    {
        perror("--ERROR--Accept failed");
        reactor.stopReactor();
        return -1;
    }

    char* connectedIP = inet_ntoa(clientAddress.sin_addr);
    int connectedPort = ntohs(clientAddress.sin_port);

    std::cout << "New client connected: IP - " << connectedIP << ", Port - " << connectedPort << std::endl;

    reactor.addFD(clientSocket, [clientSocket, connectedIP, connectedPort]() {
    Server::echo(clientSocket, connectedIP, connectedPort);
    });

    return clientSocket;
}

// implementation for echo():
void Server::echo(int socket, const char* ip, int port) 
{
    // define buffer size
    constexpr int bufferSize = 1024;
    char buffer[bufferSize];

    ssize_t bytesRead = read(socket, buffer, bufferSize - 1);
    if (bytesRead <= 0) {
        std::cout << "Client disconnected: IP - " << ip << ", Port - " << port << std::endl;
        close(socket);
        return;
    }

    buffer[bytesRead] = '\0';
    std::string message(buffer);

    std::cout << "Message from client: IP - " << ip << ", Port - " << port << ": " << message << std::endl;
}