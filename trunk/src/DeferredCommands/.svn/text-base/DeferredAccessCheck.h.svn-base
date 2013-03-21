#ifndef _DEFERREDACCESSCHECK_H_
#define _DEFERREDACCESSCHECK_H_

#include "dcmd.h"

class DeferredAccessCheck : public dcmd
{
	string name;
	string originalcmd;
	string originalmsg;

	bool success;

	public:
	DeferredAccessCheck(IRCBot& bot, string command, Hostname speaker, string arguments);

	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);
};

#endif
