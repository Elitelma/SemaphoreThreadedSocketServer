#include "include/client.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>

Client::Client(const std::string& ip, int port) : serverIP(ip), port(port) {

    //Initialize client socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0) {
        std::cerr << "Error creating socket. " << std::endl;
        exit(EXIT_FAILURE);
    }

    //set up server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if(inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0){
        std::cerr << "Invalid address/Address not supported." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
}

Client::~Client(){
    close(clientSocket);
}

bool Client::connectToServer(){

    if(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
        std::cerr << "Connection to server failed." << std::endl;
        return false;
    }

    std::cout << "\nConnected to the server at " << serverIP << ":" << port << std::endl;
    return true;
}

void Client::sendChoice(int choice){

    if(send(clientSocket, &choice, sizeof(choice),0) < 0){
        std::cerr << "Error sending data to server. " << std::endl;
    }else {
        std::cout << "\nChoice sent to the server: " << (choice == 1 ? " Read " : " Write ") << std::endl;
    }
}