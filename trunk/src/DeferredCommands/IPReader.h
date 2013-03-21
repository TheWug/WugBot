#ifndef _IPREADER_H_
#define _IPREADER_H_

#include "dcmd.h"

class IPReader : public dcmd
{
	string target;
	string ip;
	string host;
	string account;
	string request;

	bool didfind;

	public:
	IPReader(string c, Hostname speaker, string target, string args, IRCBot& bot);

	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);
};

#endif
