#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "RspKick.h"

#include "IRCBot.h"
#include "UserManager.h"
#include "misc.h"

using namespace std;

void RspKick::PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
}

void RspKick::PostAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{	istringstream resp(message);
	string kickee;
	resp >> kickee;
	message = getline(resp);
	if (message.length() == 0)
		message = "No reason given";
	if (message.at(0) == ':')
		message = message.substr(1, message.length() - 1);
	cout << speaker.GetNick() << " has kicked " << kickee << " from " << target << " (" << message << ")" << endl;
	bot.usermgr.UserLeftChannel(target, kickee);
	if (kickee == bot.nick)
		bot.OnChannelLeave(target);
}

string RspKick::RespondTo()
{
	return "kick";
}
