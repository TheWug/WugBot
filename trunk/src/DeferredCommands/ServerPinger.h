#ifndef _SERVERPINGER_H_
#define _SERVERPINGER_H_

#include "dcmd.h"

class ServerPinger : public dcmd
{
	string nickname;
	int port;
	bool didfind;

	public:
	ServerPinger(IRCBot& bot, Hostname speaker, string target, string user);

	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);
};

#endif
