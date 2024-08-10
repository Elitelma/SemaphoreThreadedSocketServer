
# SemaphoreThreadedSocketServer

## Overview

`SemaphoreThreadedSocketServer` is a C/C++ project that demonstrates a multi-threaded server capable of handling multiple clients simultaneously using socket programming. The server employs semaphores for synchronization to manage concurrent access and ensure thread safety.

## Features

- **Multi-threaded Client Handling**: Supports concurrent client connections using threads.
- **Semaphore-based Synchronization**: Utilizes semaphores to coordinate access and prevent race conditions.
- **Socket Programming**: Implements a basic TCP server with socket communication.

## Files

- `src/server.hpp`: Header file defining the server class and its methods.
- `src/server.cpp`: Implementation of the server functionality, including thread management and semaphore synchronization.
- `src/client.cpp`: Basic client implementation for testing server connections.

## Build and Run

### Prerequisites

- C++ Compiler (e.g., g++, clang++)
- pthread library (for threading support)

### Build

To build the server and client, navigate to the project directory and use `make` or a similar build tool. Ensure that the necessary libraries are linked.

```sh
g++ -o server src/server.cpp -lpthread
g++ -o client src/client.cpp -lpthread
