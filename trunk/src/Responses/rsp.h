#ifndef _RSP_H_
#define _RSP_H_

#include <string>

#include "Hostname.h"

using namespace std;

class IRCBot;

class rsp
{
	public:
	void Register(IRCBot& bot);
	virtual ~rsp() {}

	virtual string RespondTo() = 0;
	virtual void PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag) = 0;
	virtual void PostAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag);
};

#endif
