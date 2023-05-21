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

// Creating the Reactor
void* Reactor::createReactor() 
{
    return static_cast<void*>(this);
}

// Starting the Reactor
void Reactor::startReactor() 
{
    int result = pthread_create(&reactorThread, nullptr, &Reactor::reactorLoop, this);
    if(result != 0)
    {
        perror("--ERROR-- pthread_creat failed (in -> startReactor())");
    }
}

// Stoping the Reactor
void Reactor::stopReactor() {
    shouldStop = true;
    // wait for the thread to complete before returning
    pthread_join(reactorThread, nullptr);
}