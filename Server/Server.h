#pragma once

#include <netinet/in.h>
#include <mutex>

class Server
{
private:
    int serverSocket;
    bool isRunning = true;
    sockaddr_in address;
    std::mutex fileMutex;

    void WriteLog(int* clientSocket);

public:
    Server(int port);
    ~Server();
    void Listen();
    void stopServer();
};