#include <iostream>
#include <stdlib.h>

#include "CmdOnUser.h"
#include "IRCBot.h"
#include "misc.h"
#include "UserWatcher.h"

CmdOnUser::CmdOnUser(IRCBot& bot)
{
	watcher = new UserWatcher(bot);
	bot.AddDeferredCommand(watcher);
	defaultuserlevel = 0;
}

vector<string> CmdOnUser::CommandStrings()
{
	vector<string> types;
	types.push_back("onuser");
	return types;
}

void CmdOnUser::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	vector<string> arguments = splitcommands(args, " ", false);
	if (arguments.size() == 0)
	{
		bot.Say(respondto, "You have to specify a user.");
		return;
	}
	string user = tolower(arguments[0]);
	string location;
	if (arguments.size() == 1)
		location = "*";
	else
		location = tolower(arguments[1]);
	watcher->Watch(speaker.GetNickL(), user, location);
	ostringstream out;
	out << "I'll tell you if I see " << arguments[0] << (arguments.size() > 1 ? " in " + arguments[1] + "." : ".");
	bot.Say(respondto, out.str());
}

string CmdOnUser::HelpMsg(string command)
{
	return "Usage: onuser <user> <location> -- Instructs the bot to notify you if the provided user becomes active in the provided channel.";
}
