# Ex4: Design Pattern
### Table of Content
* [About The Project](#About-The-Project)
* [Files](#Files)
* [Compilation and Execution](#Compilation-and-Execution)
* [Usage](#Usage)
* [Dependencies](#Dependencies)
---
## About The Project
This is an example implementation of the Reactor pattern in C++. The Reactor pattern is a design pattern that handles concurrent and event-driven systems by using an event loop to process events from multiple sources.

## Files

- `Reactor.hpp` and `Reactor.cpp`: These files define the Reactor class, which manages the event loop and handles file descriptors and associated event handlers.

- `Server.hpp` and `Server.cpp`: These files define the Server class, which represents a simple TCP server that uses the Reactor to handle client connections and echo messages back to the clients.

- `main.cpp`: This file contains the main entry point of the program. It creates an instance of the Server class and starts the server using the Reactor.

- `Makefile`: This file contains the compilation instructions for building the code and generating the executable.

## Compilation and Execution

To compile the code, first download or clone this repository. Next navigate to the directory containing the code files and run the command `make`. This will compile the code and generate the executable `react_server`.

To execute the program, run the command `./react_server`. The server will start listening on port 9034 and wait for client connections.

## Usage

Once the server is running, you can use a tool like `telnet` to connect to the server's IP address and port. For example, you can run `telnet localhost 9034` to connect to the server running on the local machine.

After connecting to the server, any message you send will be echoed back by the server. The server will display the received messages along with the client's IP address and port in the terminal where the program is running.

You can stop the server by pressing Ctrl+C in the terminal window or using any other appropriate method for your operating system.

## Dependencies

This code does not have any external dependencies. It uses standard C++ libraries such as `<iostream>`, `<map>`, `<sys/select.h>`, `<vector>`, `<unistd.h>`, `<thread>`, and `<cstdio>`.

## License

This code is provided under the [MIT License](https://opensource.org/licenses/MIT).

Feel free to modify and use the code according to your needs.
