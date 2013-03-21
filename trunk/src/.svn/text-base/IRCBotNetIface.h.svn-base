#ifndef _IRCBOTNETIFACE_H_
#define _IRCBOTNETIFACE_H_

#include <string>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

using namespace std;

class Timer;

class IRCBotNetIface
{
	sockaddr_in addr;

	char readbuffer[2 << 10];
	char tempbuffer[2 << 4 + 1];

	int readpointer;
	int readmax;

	string address;
	string ipaddress;
	int port;

	Timer& timer;

	int socketstate;
	int connectstate;

	pthread_t readthread, writethread;
	int readstatus, writestatus;

	bool running;
	int maxretries;

	public:
	Timer& GetTimer();
	
	IRCBotNetIface();
	~IRCBotNetIface();

	bool BindServer(string server, int port);
	void SetMaxRetries(int r);

	int Connect();
	void Disconnect();

	int Send(string data);
	char * Receive();

	static int WaitForSocket(int socket, int flags, int timeout = 3000);
};

#endif
