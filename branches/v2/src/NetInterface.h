#ifndef _NetInterface_H_
#define _NetInterface_H_

#include <string>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

using namespace std;

class BotCore;
struct pollfd;

enum ErrorState
{
	M_SUCCESS,
	M_SOCKET_ERROR,
	M_COULDNT_FIND_HOST,
	M_COULD_NOT_CONNECT,
	M_UNKNOWN_ERROR
};

class NetInterface
{
private:
	BotCore& bot;

	sockaddr_in addr;

	string address;
	int ipaddress;
	int port;

	int socket_fd;

	char readbuffer[2 << 10];
	char tempbuffer[2 << 4 + 1];

	bool isconnected;

public:
	NetInterface(BotCore& b);
	~NetInterface();

	bool BindServer(string newserver, int newport);
	void SetMaxRetries(int r);

	ErrorState Connect();
	void Disconnect();

	bool IsConnected();

	int Send(const char * buffer, int maxsize);
	int Receive(char * buffer, int maxsize);

	bool Populate(pollfd& entry, int events);
};

#endif
