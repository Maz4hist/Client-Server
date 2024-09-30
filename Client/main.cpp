#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include "Client.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		cerr << "Usage: ./client <name> <portNumber> <timeToReconnect>" << endl;
		return 1;
	}

	string name = argv[1];
	int port = stoi(argv[2]);
	int timeToReconnect = stoi(argv[3]);

	string input;
	string ip = "0.0.0.0";	// change for run on local network
	Client* c = new Client(name, port, timeToReconnect, ip);
	thread clientThread(&Client::AutoReconnect, c);

	while (true)
	{
		cin >> input;
		if (input == "exit")
		{
			c->StopClient();
			break;
		}
	}

	clientThread.join();
	this_thread::sleep_for(chrono::seconds(1));
	delete c;
	return 0;
}