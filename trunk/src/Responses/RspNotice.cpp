#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "RspNotice.h"

#include "IRCBot.h"

using namespace std;

void RspNotice::PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
	if (ctcpflag)
		cout << setw(32) << left << "+" + speaker.GetNick() + "+" << " " + message << endl;
	else
		cout << setw(32) << left << "+" + speaker.GetNick() + "/" + target + "+" << " " + message << endl;
	return;
}

string RspNotice::RespondTo()
{
	return "notice";
}
