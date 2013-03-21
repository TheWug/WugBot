#ifndef _COMMANDCALLER_H_
#define _COMMANDCALLER_H_

#include "dcmd.h"

class CommandCaller : public dcmd
{
	public:
	string command, arguments;

	CommandCaller(IRCBot& bot, string command, string arguments);

	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);
};

#endif
