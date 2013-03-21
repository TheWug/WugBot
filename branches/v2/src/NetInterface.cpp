#include "NetInterface.h"

#include <string>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <sys/poll.h>
#include <unistd.h>

#include "BotCore.h"
#include "BotTime.h"
#include "StringUtil.h"

using namespace std;

NetInterface::NetInterface(BotCore& b) : bot(b)
{
	socket_fd = -1;
	for (int i = -1; i < 15; tempbuffer[++i] = 0);
	isconnected = false;
}

NetInterface::~NetInterface()
{
	Disconnect();
}

NetInterface::ErrorState NetInterface::Connect()
{
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_fd < 0)
	{
		bot.BotLog().GetLog(BotLogger::SYS).Put(ERROR, "NetInterface: unable to connect.");
		errno = 0;
		return M_SOCKET_ERROR;
	}

	int flags = fcntl(socket_fd, F_GETFL, NULL);

	if(fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		bot.BotLog().GetLog(BotLogger::SYS).Put(ERROR, "NetInterface: unable to connect.");
		errno = 0;
		Disconnect();
		return M_SOCKET_ERROR;
	}

	int connectstate = connect(socket_fd, (sockaddr *) &addr, sizeof(addr));
	fcntl(socket_fd, F_SETFL, flags);

	if (connectstate < 0)
	{
		if (errno != EINPROGRESS)
		{
			bot.BotLog().GetLog(BotLogger::SYS).Put(ERROR, "NetInterface: unable to connect.");
			errno = 0;
			Disconnect();
			return M_UNKNOWN_ERROR;
		}
	}

	isconnected = true;
	bot.BotLog().GetLog(BotLogger::SYS).Put(INFO, "NetInterface: connection established to " + address + " (" + StringUtil::FromIPAddr(ipaddress) + ")");
	return M_SUCCESS;
}

int NetInterface::Receive(char * buffer, int maxsize) // blocks; use some variation of poll (NetManager::WaitForRead?) to determine if data is available
{
	int readstate = recv(socket_fd, buffer, maxsize, 0);
	if (readstate == -1) Disconnect();
	return readstate;
}

int NetInterface::Send(const char * buffer, int maxsize) // might block? use some variation of poll (NetManager::WaitForWrite?) to check if data can be written
{
	if (buffer == "") return 0;
	int writestate = send(socket_fd, buffer, maxsize, 0);
	if (writestate == -1) Disconnect();
	return writestate;
}

void NetInterface::Disconnect()
{
	isconnected = false;
	close(socket_fd);
}

bool NetInterface::BindServer(string newserver, int newport)
{
	// addr is a member of NetInterface which holds information used by Connect
	memset(&addr, 0, sizeof(sockaddr_in));
	address = newserver;
	port = newport;

	hostent *hostentry = gethostbyname(address.c_str());
	if (hostentry == NULL)
	{
		bot.BotLog().GetLog(BotLogger::SYS).Put(WARNING, "NetInterface: could not find host: gethostbyname failed (" + address + ")");
		ipaddress = 0;
		errno = 0;
		return true; // if error, return true
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = ((in_addr *) hostentry->h_addr)->s_addr;

	ipaddress = htonl(addr.sin_addr.s_addr);

	return false; // if no error, return false
}

bool NetInterface::IsConnected()
{
	return isconnected;
}

bool NetInterface::Populate(pollfd& entry, int events)
{
	entry.fd = socket_fd;
	entry.events = events;
	entry.revents = 0;
}
