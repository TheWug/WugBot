#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/poll.h>
#include <string.h>

#include "ServerPingThread.h"
#include "IRCBotNetIface.h"
#include "IRCBot.h"
#include "misc.h"

ServerPingThread::ServerPingThread(IRCBot& b, Hostname s, string t, string a) : bot(b), speaker(s), target(t), address(a), port(0)
{
	int p = address.rfind(':');
	if (p == string::npos)
		port = 25565;
	else
	{
		istringstream s(address.substr(p + 1));
		s >> port;
		address = address.substr(0, p);
	}
	cout << "Checking: " << address << " port " << port << endl;
}

void ServerPingThread::start()
{
	if (pthread_create(&thread, NULL, (void*(*)(void *)) ServerPingThread::Run, this))
	{
		cout << "An error occurred while branching a thread!" << endl;
		delete this;
		return;
	}
}

void * ServerPingThread::Run(void * param)
{
	((ServerPingThread *) param)->_do();
	delete ((ServerPingThread *) param);
	return NULL;
}

void ServerPingThread::_do()
{
	if (port == 0)
	{
		bot.Say(bot.GetRespondTo(speaker, target), "You must specify a valid port.");
		return;
	}

	int socketstate = socket(AF_INET, SOCK_STREAM, 0);

	if (socketstate < 0)
	{
		bot.Say(bot.GetRespondTo(speaker, target), "Unable to open socket.  This should not happen.");
		return;
	}

	{
		int arg;
		if( (arg = fcntl(socketstate, F_GETFL, NULL)) < 0) 
		{ 
			cout << "Could not set socket to non-blocking mode." << endl; 
			bot.Say(bot.GetRespondTo(speaker, target), "Unable to open socket in non blocking mode.  This should not happen.");
			return; 
		}

		if( fcntl(socketstate, F_SETFL, arg | O_NONBLOCK) < 0)
		{ 
			cout << "Could not set socket to non-blocking mode." << endl; 
			bot.Say(bot.GetRespondTo(speaker, target), "Unable to open socket in non blocking mode.  This should not happen.");
			return; 
		} 
	}
	
	hostent *hostentry = gethostbyname(address.c_str());
	if (hostentry == 0)
	{
		bot.Say(bot.GetRespondTo(speaker, target), "Failed to resolve address: " + address);
		return;
	}
	
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = ((in_addr *) hostentry->h_addr)->s_addr;

	int connectstate = connect(socketstate, (sockaddr *) &addr, sizeof(addr));
	fcntl(socketstate, F_SETFL, fcntl(socketstate, F_GETFL, NULL) & ~O_NONBLOCK);

	if (connectstate < 0)
	{
		if (errno == EINPROGRESS)
		{
			errno = 0;
			int value = IRCBotNetIface::WaitForSocket(socketstate, POLLOUT);
			if (value == 0)
			{
				bot.Say(bot.GetRespondTo(speaker, target), "Unable to connect to address: " + address + ":" + tostr(port) + " (Timed out)");
				close(socketstate);
				return;
			}
			if (value < 0)
			{
				errno = 0;
				bot.Say(bot.GetRespondTo(speaker, target), "Unable to connect to address: " + address + ":" + tostr(port) + " (Error)");
				close(socketstate);
				return;
			}
		}
		else
		{
			bot.Say(bot.GetRespondTo(speaker, target), "Unable to connect to address: " + address + ":" + tostr(port) + " (Connect error)");
			close(socketstate);
			return;
		}
	}

	string serverMOTD, serverPcur, serverPmax;
	{
		send(socketstate, "\xFE", 1, 0);

		int value = IRCBotNetIface::WaitForSocket(socketstate, POLLIN, 1000);

		if (value == 0)
		{
			bot.Say(bot.GetRespondTo(speaker, target), address + ":" + tostr(port) + " does not host a minecraft server.");
			close(socketstate);
			return;
		}
		if (value < 0)
		{
			errno = 0;
			bot.Say(bot.GetRespondTo(speaker, target), "Unable to connect to address: " + address + ":" + tostr(port) + " (Receive error)");
			close(socketstate);
			return;
		}
		
		unsigned char buffer[1024];
		int read = recv(socketstate, buffer, 1024, 0);

		if (read == 0 || buffer[0] != 0xFF)
			bot.Say(bot.GetRespondTo(speaker, target), address + ":" + tostr(port) + " is accepting connections.");
		else
		{
			short s;
			memcpy(&s, buffer + 1, 2);
			s = htons(s);
			unsigned char * c = new unsigned char[s+1];
			char unknown[4] = { '?', '?', '?', '\0' };
			unsigned char * maxplayers = (unsigned char *)unknown;
			unsigned char * currentplayers = (unsigned char *)unknown;
			for (int i = 0; i < s; ++i)
				c[i] = buffer[i * 2 + 4];
			c[s] = 0;
			int i;
			for (i = s; i >= 0; --i)
			{
				if (c[i] == 0xA7)
				{
					maxplayers = c + i + 1;
					c[i] = 0;
					break;
				}
			}
			for (; i >= 0; --i)
			{
				if (c[i] == 0xA7)
				{
					currentplayers = c + i + 1;
					c[i] = 0;
					break;
				}
			}
			serverMOTD = (char *) c;
			serverPcur = (char *) currentplayers;
			serverPmax = (char *) maxplayers;
			delete c;

			bot.Say(bot.GetRespondTo(speaker, target), address + ":" + tostr(port) + " seems to host a minecraft server: " + serverMOTD + " (" + serverPcur + "/" + serverPmax + ")");
		}
	}

	close(socketstate);
}
