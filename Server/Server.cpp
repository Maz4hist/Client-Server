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
}

Server::~Server()
{
    close(serverSocket);
}

int Server::StartServer()
{
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        cerr << "Socket was not created" << endl;
        return -1;
    }
    if(bind(serverSocket, (sockaddr*)&address, sizeof(address)) == -1)
    {
        cerr << "Bind error" << endl;
        return -1;
    }
    if (listen(serverSocket, 10) == -1)
    {
        cerr << "Server cant start listening" << endl;
        return -1;
    }
    return 0;
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

void Server::StopServer() 
{ 
    isRunning = false;
    shutdown(serverSocket, SHUT_RDWR);
}
