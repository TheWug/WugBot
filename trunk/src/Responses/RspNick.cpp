#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "RspNick.h"

#include "IRCBot.h"
#include "UserManager.h"
#include "misc.h"

using namespace std;

void RspNick::PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
}

void RspNick::PostAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
	string name = target;
	if (name.length() > 0 && name.at(0) == ':')
		name = name.c_str() + 1;
	cout << speaker.GetNick() << " is now known as " << name << endl;
	bot.usermgr.SetNick(speaker.GetNick(), name);
	if (speaker.GetNick() == bot.nick)
		bot.OnNickChange(name);
}

string RspNick::RespondTo()
{
	return "nick";
}
