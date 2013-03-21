#ifndef _RSPMOTD_H_
#define _RSPMOTD_H_

#include <string>

#include "rsp.h"

using namespace std;

class RspMOTD : public rsp
{
	public:
	string RespondTo();
	void PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag);
};

#endif
