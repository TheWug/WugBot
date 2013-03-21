#ifndef _RSPINVITE_H_
#define _RSPINVITE_H_

#include <string>

#include "rsp.h"
#include "types.h"

using namespace std;

class RspInvite : public rsp
{
	public:
	string RespondTo();
	void PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag);
	void SetInviteMode(invite_mode mode);

	invite_mode responsetype;
};

#endif
