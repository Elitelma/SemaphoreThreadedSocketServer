
#include "include/client.hpp"
#include "include/server.hpp"
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>



void startServer(){

    Server server(8080);
    server.start();
}

void startClient(int choice, const std::string& serverIP, int port){

    Client client(serverIP, port);
    if(client.connectToServer()){
        client.sendChoice(choice);
    }else{
        std::cerr << "Error to connect to server" << std::endl;
    }
}

int main() {

    //starts the thread of server
    std::thread serverThread(startServer);

    //wating a time to ensure that the server is listing
    std::this_thread::sleep_for(std::chrono::seconds(1));

    const std::string serverIP = "127.0.0.1";
    const int port = 8080;
    std::vector<std::thread>clientThreads;

    for(int i = 0; i< 3; ++i){
        clientThreads.push_back(std::thread(startClient, 1, serverIP, port));
    }

    for(int i = 0; i< 4; ++i){
        clientThreads.push_back(std::thread(startClient, 2, serverIP, port));
    }

    for(auto& t : clientThreads){
        t.join();
    }

    serverThread.detach();
    std::cout << "Client-server communication test complete!" << std::endl;
    return 0;
}