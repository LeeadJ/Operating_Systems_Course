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


/**
 * @brief Creates a reactor and returns a pointer to it.
 *
 * This function creates a reactor object and returns a pointer to it. The returned pointer
 * is of type `void*` and needs to be cast to the appropriate Reactor type before use.
 *
 * @param None.
 * @return A pointer to the created Reactor object.
 */
void* Reactor::createReactor() 
{
    return static_cast<void*>(this);
}


/**
 * @brief Starts the reactor by creating a new thread for the reactor loop.
 *
 * This function starts the reactor by creating a new thread using the pthread_create()
 * function. The new thread will execute the reactorLoop() function. The `reactorThread`
 * member variable will store the thread identifier. It also provides error handling for
 * the pthread_create() function.
 *
 * @param None.
 * @return None.
 */
void Reactor::startReactor() 
{
    int result = pthread_create(&reactorThread, nullptr, &Reactor::reactorLoop, this);
    if(result != 0)
    {
        perror("--ERROR-- pthread_creat failed (in -> startReactor())");
    }
}


/**
 * @brief Stops the reactor and waits for the reactor thread to complete.
 *
 * This function stops the reactor by setting the `shouldStop` flag to true. It then waits for
 * the reactor thread, represented by `reactorThread`, to complete before returning.
 *
 * @param None.
 * @return None.
 */
void Reactor::stopReactor() 
{
    shouldStop = true;
    // wait for the thread to complete before returning
    pthread_join(reactorThread, nullptr);
}


/**
 * @brief Adds a file descriptor and its associated handler to the Reactor.
 *
 * This function adds a file descriptor and its associated handler to the Reactor's eventHandlers
 * map. The file descriptor and handler are stored as a key-value pair in the map.
 *
 * @param fd The file descriptor to be added.
 * @param handler The handler function pointer associated with the file descriptor.
 * @return None.
 * 
 */void Reactor::addFD(int fd, handler_t handler) 
{
    eventHandlers[fd] = std::move(handler);
}


/**
 * @brief Waits for the completion of the reactor thread.
 *
 * This function waits for the completion of the reactorThread, which represents the thread
 * that needs to be joined. It provides error handling for the pthread_join() function.
 *
 * @param None.
 * @return None.
 */
void Reactor::waitFor() const 
{
    //wait for the completion of the reactorThread which represents the thread we want to join.
    int result = pthread_join(reactorThread, nullptr);
    if(result != 0)
    {
        perror("--ERROR-- pthread_join failed (in -> waitFor())");
    }
}

/**
 * @brief Main loop of the reactor.
 *
 * This function continuously monitors the registered file descriptors for readability and
 * invokes the associated event handlers when a file descriptor becomes ready for reading.
 * It also provides error handling for the select() function.
 *
 * @param arg A void pointer to the Reactor object.
 * @return A void pointer.
 */
void* Reactor::reactorLoop(void* arg) 
{
    // cast the void* arg to a pointer of type Reactor*
    Reactor* reactor = static_cast<Reactor*>(arg);
    // creating a map reference to the evanthandlers member variable of the Reactor
    std::map<int, handler_t>& eventHandlers = reactor->eventHandlers;
    int maxFd = -1;

    // loop until shouldStop is true:
    while(!reactor->shouldStop) 
    {
        fd_set readFds; // declare file descriptor set
        FD_ZERO(&readFds); // init the FDs to zero

        // loop over each item in the map"
        for(const auto& pair : eventHandlers) 
        {
            // get the key and value
            int fd = pair.first;
            handler_t handler = pair.second;

            // add the fd to the reading set:
            FD_SET(fd, &readFds);
            if (fd > maxFd) 
            {
                maxFd = fd;
            }
        }

        // loop only if there is at least one fd in the readFds set:
        if (maxFd != -1) {
            // use select to monitor the FDs for readability:
            int ready = select(maxFd + 1, &readFds, nullptr, nullptr, nullptr);
            if (ready == -1)
            {
                perror("--ERROR-- select failed (in -> reactorLoop())");
                break;
            } 
            else if (ready > 0)
            {
                // loop over the event handlers
                for (const auto& pair : eventHandlers) 
                {
                    int fd = pair.first;
                    handler_t handler = pair.second;

                    // if the fd is set in the readFds set, invoke the associated event handler
                    if (FD_ISSET(fd, &readFds)) 
                    {
                        handler();
                    }
                }
            }
        }
    }

    return nullptr;
}
