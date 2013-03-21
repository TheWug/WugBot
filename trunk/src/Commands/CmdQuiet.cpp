#include <iostream>
#include <stdlib.h>

#include "CmdQuiet.h"
#include "ModeWatcher.h"
#include "IRCBot.h"
#include "misc.h"

CmdQuiet::CmdQuiet()
{
	defaultuserlevel = 4;
	defaultuserflags = "oOcC";
	acceptsflags = true;
}

vector<string> CmdQuiet::CommandStrings()
{
	vector<string> types;
	types.push_back("quiet");
	types.push_back("q");
	types.push_back("dequiet");
	types.push_back("dq");
	return types;
}

void CmdQuiet::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	vector<string> arguments = splitcommands(args, " ", false);
	string channel = target;
	string user;
	string mode = "+q";
	if (command == "dequiet" || command == "dq")
	mode = "-q";
	if (arguments.size() == 0 && target.at(0) != '#')
	{
		bot.Say(respondto, "You must specify a user.");
		return;
	}
	if (arguments.size() > 0)
		user = arguments[0];
	if (arguments.size() > 1)
		channel = arguments[1];
	if (channel.length() == 0 || channel.at(0) != '#')
		bot.Say(respondto, "You must pick a channel to quiet in.");
	else
	{
		bot.AddDeferredCommand(new ModeWatcher(bot, speaker, target, mode, user, channel));
	}
}

void CmdQuiet::ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	vector<string> arguments = splitcommands(args, " ", false);
	string channel = target;
	if (arguments.size() > 1)
		channel = arguments[1];
	FlagTestAndGo(bot, command, speaker, target, respondto, args, channel);
}

string CmdQuiet::HelpMsg(string command)
{
	if (command == "quiet" || command == "q")
		return "Usage: quiet <user> [channel] -- quiets the specified user on the specified channel.  If the channel isn't specified, the current one is used.  Will fail if the bot does not have operator privilages.";
	if (command == "dequiet" || command == "dq")
		return "Usage: dequiet <user> [channel] -- unquiets the specified user on the specified channel.  If the channel isn't specified, the current one is used.  Will fail if the bot does not have operator privilages.";
}
