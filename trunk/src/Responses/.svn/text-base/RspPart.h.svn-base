#ifndef _RSPPART_H_
#define _RSPPART_H_

#include <string>

#include "rsp.h"

using namespace std;

class RspPart : public rsp
{
	public:
	string RespondTo();
	void PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag);
	void PostAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag);
};

#endif
