#ifndef _RSPACCOUNT_H_
#define _RSPACCOUNT_H_

#include <string>

#include "rsp.h"

using namespace std;

class RspAccount : public rsp
{
	public:
	string RespondTo();
	void PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag);
};

#endif
