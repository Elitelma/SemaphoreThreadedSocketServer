#ifndef CLIEN_HPP
#define CLIENT_HPP

#include<string>
#include<netinet/in.h>

class Client {

public:
    Client(const std::string& serverIP, int port);
    ~Client();

    bool connectToServer();
    void sendChoice(int choice);

private:
    int clientSocket;
    std::string serverIp;
    int port;
    struct sockadd_in serverAddr;
    
};

#endif