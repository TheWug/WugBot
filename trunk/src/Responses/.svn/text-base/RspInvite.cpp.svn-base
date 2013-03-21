#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "RspInvite.h"

#include "IRCBot.h"
#include "BotPermissions.h"
#include "misc.h"
#include "types.h"

using namespace std;

void RspInvite::PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
	if (message.length() != 0 && message.at(0) == ':')
		message = message.substr(1);
	cout << speaker.GetNick() << " has invited you to " << message << "." << endl;
	if (bot.permission.CanBeIn(message))
	{
		if (responsetype == JOIN)
			bot.Join(message);
		else if (responsetype == NOTICE)
		{
			bot.Join(message);
			bot.Log(speaker.GetNick() + " (" + speaker.GetAccount() + ") invited me to " + message + ".");
		}
		else if (responsetype == REQUEST)
			bot.Log(speaker.GetNick() + " (" + speaker.GetAccount() + ") invited me to " + message + ", clearance required.");
		else
			return;
	}
	else if (bot.permission.CanSendTo(speaker.GetNickL()))
		bot.Say(speaker.GetNick(), "I can't join that channel.");
}

void RspInvite::SetInviteMode(invite_mode mode)
{
	responsetype = mode;
}

string RspInvite::RespondTo()
{
	return "invite";
}
