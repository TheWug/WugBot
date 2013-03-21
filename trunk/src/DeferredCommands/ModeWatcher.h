#ifndef _MODEWATCHER_H_
#define _MODEWATCHER_H_

#include "dcmd.h"

class ModeWatcher : public dcmd
{
	public:
	string mode, user, channel;

	ModeWatcher(IRCBot& bot, Hostname speaker, string target, string mode, string user, string channel);
	
	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);
};

#endif
