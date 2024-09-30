#include "Server.h"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <fstream>

using namespace std;

Server::Server(int _port)
{
    address.sin_family = AF_INET;
    address.sin_port = htons(_port);
    address.sin_addr.s_addr = INADDR_ANY;

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        throw runtime_error("Socket was not created");
    if(bind(serverSocket, (sockaddr*)&address, sizeof(address)) == -1)
        throw runtime_error("Bind error");
    if (listen(serverSocket, 10) == -1)
        throw runtime_error("Server cant start listening");
}

Server::~Server()
{
    close(serverSocket);
}

void Server::WriteLog(int* clientSocet)
{
    char buf[64] = {};
    recv(*clientSocet, buf, sizeof(buf), 0);
    close(*clientSocet);
    ofstream logs;
    lock_guard<mutex> lock(fileMutex);
    logs.open("log.txt", ios_base::app);
    if (!logs.is_open())
        cerr << "Error opening file";
    logs << buf << endl;
    logs.close();
    close(*clientSocet);
}

void Server::Listen()
{
    while (isRunning)
    {
        sockaddr_in clientAddress;
        socklen_t socklen = sizeof(clientAddress);
        int* clientSocket = new int;
        *clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &socklen);
        if (*clientSocket == -1)
        {
            cerr << "Client conection error" << endl;
            delete clientSocket;
            continue;
        }

        try
        {
            thread thr(&Server::WriteLog, this, clientSocket);
            thr.detach();
        }
        catch(const exception& e)
        {
            cerr << "Thread creation error" << endl;
            delete clientSocket;
            continue;
        }
    }
}

void Server::stopServer() 
{ 
    isRunning = false;
    shutdown(serverSocket, SHUT_RDWR);
}
