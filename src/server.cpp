#include "include/server.hpp"
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>


//Initialize server
Server::Server(int port) : readercont(0), writercont(0) {
    
    sem_init(&semaphore_a, 0, 1);
    sem_init(&semaphore_b, 0, 1);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    //Bind the socket to the address an port
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

}

//Destructor to clean up resources
Server::~Server() {

    sem_destroy(&semaphore_a);
    sem_destroy(&semaphore_b);
    close(serverSocket);

}


// Initialize and start acception connections
void Server::start() {

    if(listen(serverSocket, 50) == 0) {
        std::cout << "Listening on port " << ntohs(serverAddr.sin_port) << std::endl;
        acceptConnections();
    } else {
        std::cout << "Error: Unable to listen on port " << ntohs(serverAddr.sin_port) << std::endl;
    }
}

//Function to accept incoming connections and cread threads
void Server::acceptConnections() {
    int newsocket;
    addr_size = sizeof(serverStorage);

    while (true) {
        newsocket = accept(serverSocket, (struct sockaddr*)&serverStorage, &addr_size);
        int choice;
        recv(newsocket, &choice, sizeof(choice), 0);

        if (choice == 1) {
            // Create read thread
            if(pthread_create(&readerThreads[readercont++], NULL, &Server::staticReader, this) != 0) {
                std::cerr << "Failed to create reader thread " << std::endl;
            }

        } else if(choice == 2) {
            if(pthread_create(&writerThreads[writercont++], NULL, &Server::staticWriter, this) != 0) {
                std::cerr << "Failed to create writer thread" << std::endl;
            }
        }

        // Ensure we don't exceed the maximum number of threads
        if(readercont >= 50) {
            for(int i = 0; i < readercont; ++i) {
                pthread_join(readerThreads[i], NULL);
            }
            readercont = 0;
        }

        if(writercont >= 50) {
            for(int i = 0; i < writercont; ++i) {
                pthread_join(writerThreads[i], NULL);
            }
            writercont = 0;
        }
    }
}

//Read thread function
void* Server::reader(void* param){

    sem_wait(&semaphore_a);
    readercont++;
    if(readercont == 1) {
        sem_wait(&semaphore_b);
    }

    sem_post(&semaphore_a);

    std::cout << "Reader is inside, count: " << readercont << std::endl;
    sleep(1);

    sem_wait(&semaphore_a);
    readercont--;
    if(readercont == 0) {
        sem_post(&semaphore_b);
    }
    sem_post(&semaphore_a);

    std::cout << "Reader is leaving, count: " << readercont << std::endl;
    pthread_exit(NULL);

}

// Writer thread function
void* Server::writer(void* param){

    std::cout<< "Writer os trying to enter " << std::endl;
    sem_wait(&semaphore_b);
    std::cout<< "Writer has entered" << std::endl;
    sem_post(&semaphore_b);
    std::cout<< "Writer is leaving " << std::endl;
    pthread_exit(NULL);

}

void* Server::staticReader(void* param) {
    Server* server = static_cast<Server*>(param);
    return server->reader(param);
}


void* Server::staticWriter(void* param) {
    Server* server = static_cast<Server*>(param);
    return server->writer(param);
}