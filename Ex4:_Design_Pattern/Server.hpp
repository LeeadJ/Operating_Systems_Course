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

    //Function declartaions
    int createServerSocket(int port);
    int acceptConnection(int serverSocket);
    static void echo(int socket, const char* ip, int port);
};

#endif // SERVER_HPP
