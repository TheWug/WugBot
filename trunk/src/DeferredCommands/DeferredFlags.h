#ifndef _DEFERREDFLAGS_H_
#define _DEFERREDFLAGS_H_

#include "dcmd.h"

class DeferredFlags : public dcmd
{
	string name;
	string channel;
	string flags;

	public:
	DeferredFlags(Hostname speaker, string target, IRCBot& bot, string c, string n, string f);

	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);
};

#endif
