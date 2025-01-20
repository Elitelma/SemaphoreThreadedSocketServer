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

Server::Server(int port) : readercont(0), writercont(0) {
    sem_init(&semaphore_a, 0, 1); // Semaphore for reader count
    sem_init(&semaphore_b, 0, 1); // Semaphore for writer access

    // Create the socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    // Allow reuse of the address
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &SOCKET_REUSEADDR_ENABLED, sizeof(SOCKET_REUSEADDR_ENABLED)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    // Configure address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // Bind the socket to the address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
}

// Destructor to clean up resources
Server::~Server() {
    sem_destroy(&semaphore_a);
    sem_destroy(&semaphore_b);
    close(serverSocket);
}

// Initialize and start accepting connections
void Server::start() {
    if (listen(serverSocket, 50) == 0) {
        std::cout << "Listening on port " << ntohs(serverAddr.sin_port) << std::endl;
        acceptConnections();
    } else {
        perror("Listen failed");
    }
}

// Method to accept incoming connections and create threads
void Server::acceptConnections() {
    addr_size = sizeof(serverStorage);

    while (true) {
        int newsocket = acceptConnection();
        if (newsocket < 0) {
            continue; 
        }

        int choice = receiveChoice(newsocket);
        if (choice == -1) {
            close(newsocket);
            continue; 
        }

        handleChoice(choice, newsocket);
        close(newsocket);
    }
}

int Server::acceptConnection() {
    int newsocket = accept(serverSocket, (struct sockaddr*)&serverStorage, &addr_size);
    if (newsocket < 0) {
        std::cerr << "Error accepting connection" << std::endl;
    }
    return newsocket;
}

int Server::receiveChoice(int socket) {
    int choice;
    if (recv(socket, &choice, sizeof(choice), 0) <= 0) {
        std::cerr << "Error receiving choice" << std::endl;
        return -1;
    }
    return choice;
}

void Server::handleChoice(int choice, int socket) {
    pthread_t thread;
    if (choice == 1) {
        if (readercont < MAX_THREADS) {
            pthread_create(&thread, NULL, &Server::staticReader, this);
            pthread_detach(thread);
        } else {
            std::cerr << "Maximum reader threads reached" << std::endl;
        }
    } else if (choice == 2) {
        if (writercont < MAX_THREADS) {
            pthread_create(&thread, NULL, &Server::staticWriter, this);
            pthread_detach(thread);
        } else {
            std::cerr << "Maximum writer threads reached" << std::endl;
        }
    } else {
        std::cerr << "Invalid choice received" << std::endl;
    }
}

// Reader thread function
void* Server::reader(void* param) {
    sem_wait(&semaphore_a);
    readercont++;
    if (readercont == 1) {
        sem_wait(&semaphore_b); // First reader locks writer access
    }
    sem_post(&semaphore_a);

    std::cout << "Reader is inside, count: " << readercont << std::endl;
    sleep(1); // Simulate reading

    sem_wait(&semaphore_a);
    readercont--;
    if (readercont == 0) {
        sem_post(&semaphore_b); // Last reader unlocks writer access
    }
    sem_post(&semaphore_a);

    std::cout << "Reader is leaving, count: " << readercont << std::endl;
    pthread_exit(NULL);
}

// Writer thread function
void* Server::writer(void* param) {
    std::cout << "Writer is trying to enter" << std::endl;
    sem_wait(&semaphore_b); // Writers block readers
    std::cout << "Writer has entered" << std::endl;
    sleep(1); // Simulate writing
    std::cout << "Writer is leaving" << std::endl;
    sem_post(&semaphore_b);
    pthread_exit(NULL);
}

// Static method wrappers
void* Server::staticReader(void* param) {
    Server* server = static_cast<Server*>(param);
    return server->reader(nullptr);
}

void* Server::staticWriter(void* param) {
    Server* server = static_cast<Server*>(param);
    return server->writer(nullptr);
}
