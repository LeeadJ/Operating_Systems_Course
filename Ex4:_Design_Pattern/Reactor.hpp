#ifndef REACTOR_HPP
#define REACTOR_HPP

#include <iostream>
#include <map>
#include <sys/select.h>
#include <vector>
#include <unistd.h>
#include <thread>
#include <cstdio>
#include <functional>



class Reactor 
{
    public:
        typedef std::function<void()> handler_t;

        // Constructor and Destructor
        Reactor();
        ~Reactor();

        //returns a pointer to the reactor struct.
        void* createReactor(); 
        // Member funciton to stop the reactor operation.
        void stopReactor();
        // Member funciton to start the reactor operation.
        void startReactor();
        // Member function to add a file descriptor and its associated event handler to the reactor.
        void addFD(int fileDescriptor, handler_t handler);
        // Member function used to wait for the reactor operation to complete.
        void waitFor() const;

    private:
        std::map<int, handler_t> eventHandlers;
        bool shouldStop;
        pthread_t reactorThread;

        static void* reactorLoop(void* arg);
};

#endif // REACTOR_HPP
