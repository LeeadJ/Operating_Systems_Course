#include "Server.hpp"
#include "Reactor.hpp"
int main() 
{
    Server server;

    try 
    {
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}