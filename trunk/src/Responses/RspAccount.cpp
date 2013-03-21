#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "RspAccount.h"

#include "IRCBot.h"
#include "UserManager.h"
#include "misc.h"

using namespace std;

void RspAccount::PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
	if (target == "*")
		cout << speaker.GetNick() << " is no longer logged in." << endl;
	else
		cout << speaker.GetNick() << " is now logged in as " << target << endl;
	bot.usermgr.SetUser(speaker.GetNick(), target);
}

string RspAccount::RespondTo()
{
	return "account";
}
