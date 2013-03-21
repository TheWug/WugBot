#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "RspPart.h"

#include "IRCBot.h"
#include "UserManager.h"
#include "misc.h"

using namespace std;

void RspPart::PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
}

void RspPart::PostAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
	if (target.at(0) == ':')
		target = target.substr(1);
	cout << speaker.GetNick() << " has left " << target << " (" << message.c_str() << ")" << endl;
	bot.usermgr.UserLeftChannel(target, speaker.GetNick());
	if (speaker.GetNick() == bot.nick)
		bot.OnChannelLeave(target);
}

string RspPart::RespondTo()
{
	return "part";
}
