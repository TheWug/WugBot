#ifndef _WHOTRACKER_H_
#define _WHOTRACKER_H_

#include "dcmd.h"

class WhoTracker : public dcmd
{
	string channel;
	bool error;

	public:
	WhoTracker(string chan, IRCBot& bot);
	string GetFlags(string flags);

	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);
};

#endif
