#include "Server.hpp"
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>


// Placeholder implementation for createServerSocket()
int Server::createServerSocket(int port) {
    int sereverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0)
    {
        throw std::runtime_error("--ERROR--Socket opening failed");
    }

    sockaddr_in address{};
    address.sin_family  = AF_INET;
    address.sin_family = AF_INET;
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