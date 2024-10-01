#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include "Server.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: ./server <portNumber>" << endl;
        return 1;
    }

    int port = stoi(argv[1]);
    string input;
    cout << "Server is working" << endl;
    Server* s = new Server(port);
    if (s->StartServer() == -1)
    {
        delete s;
        return -1;
    }
    thread serverThread(&Server::Listen, s);
    
    while (true)
    {
        cin >> input;
        if (input == "exit")
        {
            s->StopServer();
            break;
        }
        cout << input << endl;

    }

    serverThread.join();
    this_thread::sleep_for(chrono::seconds(1));
    delete s;
    return 0;
}