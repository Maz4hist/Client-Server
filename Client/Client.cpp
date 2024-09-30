#include "Client.h"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cstring>
#include <arpa/inet.h>

using namespace std;

Client::Client(const string& _name, int port, int _timeToReconect, string& ip)
{
    strncpy(name, _name.c_str(), sizeof(name));
    timeToReconnect = _timeToReconect;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr);
}

Client::~Client() {}

void Client::Connection()
{
    strncpy(buffer, (*GetDateTime() + " " + name).c_str(), sizeof(buffer) - 1);
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        throw runtime_error("Client's socket was not created");
    if (connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
        cerr << "Connection error" << endl;
    else if (send(clientSocket, buffer, sizeof(buffer), 0) == -1)
        cerr << "Send error" << endl;
    close(clientSocket);
}

void Client::AutoReconnect()
{
    while (isRunning)
    {
        Connection();
        this_thread::sleep_for(chrono::seconds(timeToReconnect));
    }
}

void Client::StopClient() { isRunning = false; }

unique_ptr<string> Client::GetDateTime()
{   auto now = chrono::system_clock::now();
    time_t timeT = chrono::system_clock::to_time_t(now);
    tm timeNow;
    localtime_r(&timeT, &timeNow);
    ostringstream oss;
    oss << '[' << put_time(&timeNow, "%Y-%m-%d %H:%M:%S") << '.' << setw(3) << setfill('0') 
    << (chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000).count() << ']';
    return make_unique<string>(oss.str());
}