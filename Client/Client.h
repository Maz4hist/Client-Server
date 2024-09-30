#pragma once
#include <netinet/in.h>
#include <string>
#include <memory>

class Client
{
private:
    char name[38];
    int timeToReconnect;
    int clientSocket;
    bool isRunning = true;
    sockaddr_in serverAddress;
    char buffer[64] = {};

    void Connection();
    std::unique_ptr<std::string> GetDateTime();

public:
    Client(const std::string& name, int port, int timeToReconect, std::string& ip);
    ~Client();
    void AutoReconnect();
    void StopClient();
};