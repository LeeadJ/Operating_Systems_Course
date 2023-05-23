#ifndef SERVER_HPP
#define SERVER_HPP

#include "Reactor.hpp"

class Server {
public:
    Server();
    ~Server();
    void start();
    void stop();

private:
    Reactor reactor;
    int serverSocket;
    int server_fd;
};

#endif // SERVER_HPP
