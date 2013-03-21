#ifndef _IGNOREACCESSMODIFIER_H_
#define _IGNOREACCESSMODIFIER_H_

#include "dcmd.h"

class IgnoreAccessModifier : public dcmd
{
	public:
	string targetuser;
	string subcommand;
	Hostname userdata;
	bool success;

	IgnoreAccessModifier(IRCBot& bot, Hostname speaker, string target, string user, string cmd);

	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);
};

#endif
