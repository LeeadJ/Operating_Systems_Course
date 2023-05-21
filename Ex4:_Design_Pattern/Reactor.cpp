#include "Reactor.hpp"

// constructor
Reactor::Reactor() : shouldStop(false), reactorThread(pthread_t()) {}

// destructor
Reactor::~Reactor() 
{
    // loop over each element in the 'eventhandler' map.
    for(const auto& pair : eventHandlers)
    {
        // extract the fd and the handler function
        int fd = pair.first;
        handler_t handler = pair.second;
        // close the corresponding fd file
        close(fd);
    }
}