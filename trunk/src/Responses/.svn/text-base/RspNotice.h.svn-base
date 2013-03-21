#ifndef _RSPNOTICE_H_
#define _RSPNOTICE_H_

#include <string>

#include "rsp.h"

using namespace std;

class RspNotice : public rsp
{
	public:
	string RespondTo();
	void PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag);
};

#endif
