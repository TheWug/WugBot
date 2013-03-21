#ifndef _DEFERREDALLOW_H_
#define _DEFERREDALLOW_H_

#include "dcmd.h"

class DeferredAllow : public dcmd
{
	string calledby;
	string name;
	string level;

	bool founduser;

	public:
	DeferredAllow(Hostname speaker, string target, IRCBot& bot, string args, string c);

	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);
};

#endif
