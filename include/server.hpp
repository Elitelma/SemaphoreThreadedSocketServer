#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <netinet/in.h>


//Class to handle the server

class Server {

public:
    Server(int port);
    ~Server();

    void start();

private:

    //Socker related members
    int serverSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    // Thread management
    pthread_t readerThreads[100];
    /// @brief 
    pthread_t writerThreads[100];
    int readercont;
    int writercont;

    // Semaphores 
    sem_t semaphore_a, semaphore_b;

    // Reader and writer functions
    void* reader(void* param);
    void* writer(void* param);

    void initializeSocket();
    void acceptConnections();

    // Helper functions for threading
    static void* staticReader(void* param);
    static void* staticWriter(void* param);
};


#endif //SERVER_HPP