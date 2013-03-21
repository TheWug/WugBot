#include <string>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <sys/poll.h>
#include <unistd.h>

#include "IRCBot.h"
#include "Timer.h"
#include "errors.h"
#include "Bottime.h"

using namespace std;

IRCBotNetIface::IRCBotNetIface() : timer(*(new Timer()))
{
	maxretries = 10;
	socketstate = -1;
	connectstate = -1;
	for (int i = -1; i < 15; tempbuffer[++i] = 0);
	readpointer = 0;
	readmax = 0;
}

IRCBotNetIface::~IRCBotNetIface()
{
	Disconnect();
}

int IRCBotNetIface::Connect()
{
	socketstate = socket(AF_INET, SOCK_STREAM, 0);

	if (socketstate < 0)
	{
		cout << "Could not create socket.  Terminating..." << endl;
		return M_SOCKET_ERROR;
	}

	int retrys = 0, status = 0;

	while (retrys < maxretries)
	{
		++retrys;
		hostent *hostentry = gethostbyname(address.c_str());
		if (hostentry == 0)
		{
			cout << "could not resolve address.  Retrying in a little while..." << endl;
			errno = 0;
			status = M_COULDNT_FIND_HOST;
			sleep(15);
			continue;
		}

		memset(&addr, 0, sizeof(sockaddr_in));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = ((in_addr *) hostentry->h_addr)->s_addr;

		if( fcntl(socketstate, F_SETFL, fcntl(socketstate, F_GETFL, NULL) | O_NONBLOCK) < 0)
		{
			cout << "Unable to create non-blocking socket." << endl;
			errno = 0;
			status = M_SOCKET_ERROR;
			sleep(15);
			continue;
		}
		connectstate = connect(socketstate, (sockaddr *) &addr, sizeof(addr));
		fcntl(socketstate, F_SETFL, fcntl(socketstate, F_GETFL, NULL) & ~O_NONBLOCK);

		if (connectstate < 0)
		{
			if (errno == EINPROGRESS)
			{
				errno = 0;
				int value = WaitForSocket(socketstate, POLLOUT, 10000);
				if (value == 0)
				{
					cout << "could not resolve address.  Retrying in a little while..." << endl;
					errno = 0;
					status = M_COULD_NOT_CONNECT;
					sleep(15);
					continue;
				}
				if (value < 0)
				{
					cout << "An unknown error occurred. (" << errno << ")" << endl;
					errno = 0;
					status = M_UNKNOWN_ERROR;
					sleep(15);
					continue;
				}
			}
			else
			{
				cout << "An unknown error occurred. (" << errno << ")" << endl;
				errno = 0;
				status = M_UNKNOWN_ERROR;
				sleep(15);
				continue;
			}
		}
		status = M_SUCCESS;
		break;
	}

	if (status == M_SUCCESS)
	{
		cout << "Connected to: " << (unsigned int) addr.sin_addr.s_addr << endl;
		return M_SUCCESS;
	}
	else
	{
		cout << "Error connecting to: " << (unsigned int) addr.sin_addr.s_addr << endl;
		return status;
	}
	return M_UNKNOWN_ERROR;
}

char * IRCBotNetIface::Receive()
{
	int pointer = 0;
	do
	{
		while (readpointer < readmax)
		{
			readbuffer[pointer] = tempbuffer[readpointer];
			bool b = (pointer > 0) && (readbuffer[pointer] == '\n') && (readbuffer[pointer - 1] == '\r');
			++readpointer;
			++pointer;
			if (b)
			{
				readbuffer[pointer] = 0;
				return readbuffer;
			}
		}
		readpointer = 0;
		readmax = 0;
		if (WaitForSocket(socketstate, POLLIN, 180000) != 1)
		{
			if (Send("DOOP\r\n") < 0)
				return NULL;
		}
		int readstate = recv(socketstate, tempbuffer, 16, 0);
		if (readstate == -1) return NULL;
		readmax += readstate;
	} while (readbuffer[pointer - 2] != '\r' || readbuffer[pointer - 1] != '\n');
	return readbuffer;
}

Timer& IRCBotNetIface::GetTimer()
{
	return timer;
}

int IRCBotNetIface::Send(string data)
{
	if (data == "")
		return 0;
	unsigned long long j = Bottime::GetCurrentTimeMillis();
	timer.Sleep();
	if (WaitForSocket(socketstate, POLLOUT, 10000) != 1)
	{
		return -1;
	}
	int i = send(socketstate, data.c_str(), data.length(), 0);
	return i;
}

void IRCBotNetIface::Disconnect()
{
	close(socketstate);
}

bool IRCBotNetIface::BindServer(string newserver, int newport)
{
	address = newserver;
	port = newport;
}

int IRCBotNetIface::WaitForSocket(int socket, int flags, int timeout)
{
	struct timeval time;
	struct pollfd d;
	d.fd = socket;
	d.events = flags;

	return poll(&d, 1, timeout);
}
void IRCBotNetIface::SetMaxRetries(int r)
{
	if (r < 0)
		r = 0;
	maxretries = r;
}
