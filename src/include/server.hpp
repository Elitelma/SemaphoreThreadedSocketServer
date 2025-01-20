#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>

class Server {
public:
    Server(int port);
    ~Server();
    void start();

private:
    void acceptConnections();
    void* reader(void* param);  
    void* writer(void* param);  
    static void* staticReader(void* param);
    static void* staticWriter(void* param);
    void manageThreads();
    void createWriterThread();
    void createReaderThread();
    void handleChoice(int choice, int socket);
    int receiveChoice(int socket);
    int acceptConnection();

    
    int serverSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
     static constexpr int MAX_THREADS = 50;
    static constexpr int SOCKET_REUSEADDR_ENABLED = 1;
    
    pthread_t readerThreads[50];
    pthread_t writerThreads[50];
    int readercont;
    int writercont;
    sem_t semaphore_a;
    sem_t semaphore_b;

};

#endif // SERVER_HPP
