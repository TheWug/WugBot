#include <string>
#include <iostream>

#include "RspMOTD.h"

class IRCBot;

using namespace std;

void RspMOTD::PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
	cout << "MOTD: " << message.c_str()+1 << endl;
}

string RspMOTD::RespondTo()
{
	return "372";
}
