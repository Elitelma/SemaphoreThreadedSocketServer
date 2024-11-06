#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <netinet/in.h>

class Client {
public:
    Client(const std::string& serverIP, int port);
    ~Client();

    bool connectToServer();
    void sendChoice(int choice);

private:
    int clientSocket;
    std::string serverIP; 
    struct sockaddr_in serverAddr; 
};

#endif // CLIENT_HPP
