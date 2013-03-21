#ifndef _SERVERPINGTHREAD_H_
#define _SERVERPINGTHREAD_H_

#include <unistd.h>
#include <string>

#include "Hostname.h"

using namespace std;

class IRCBot;

class ServerPingThread
{
	int port;
	string address;
	string target;
	IRCBot& bot;
	Hostname speaker;

	pthread_t thread;

	static void * Run(void * param);
	void _do();

	public:
	ServerPingThread(IRCBot& bot, Hostname speaker, string target, string address);

	void start();
};

#endif
