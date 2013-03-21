#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "RspConnect.h"

#include "IRCBot.h"
#include "BotPermissions.h"
#include "misc.h"

using namespace std;

void RspConnect::PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
	bot.registered = true;
	bot.NSAuth();
	bot.Raw("CAP req extended-join\r\nCAP req account-notify");

	if (target.at(0) == ':')
		target = target.substr(1);
	Hostname me = Hostname(bot.nick);
	me.SetAccount(bot.permission.GetOwner());
	for (vector<string>::iterator s = bot.connectcommands.begin(); s != bot.connectcommands.end(); s++)
	{
		cout << "Connect command: " << *s << endl;
		bot.PerformCommand(me, string(""), string(""), (string) *s);
	}

	set<string> oldchannels = bot.channellist;
	bot.channellist.clear();
	for (set<string>::iterator c = oldchannels.begin(); c != oldchannels.end(); ++c)
		bot.Join(*c);
}

string RspConnect::RespondTo()
{
	return "001";
}
