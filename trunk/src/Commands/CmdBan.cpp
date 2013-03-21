#include <iostream>
#include <stdlib.h>

#include "CmdBan.h"
#include "ModeWatcher.h"
#include "IRCBot.h"
#include "misc.h"

CmdBan::CmdBan()
{
	defaultuserlevel = 4;
	defaultuserflags = "oOC";
	acceptsflags = true;
}

vector<string> CmdBan::CommandStrings()
{
	vector<string> types;
	types.push_back("ban");
	types.push_back("kickban");
	types.push_back("kb");
	types.push_back("unban");
	return types;
}

void CmdBan::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	vector<string> arguments = splitcommands(args, " ", false);
	string channel = target;
	string user;
	string mode = "+b";
	if (command == "unban")
	mode = "-b"; 
	if (arguments.size() == 0 && target.at(0) != '#')
	{
		bot.Say(respondto, "You must specify a user.");
		return;
	}
	if (arguments.size() > 0)
		user = arguments[0];
	if (arguments.size() > 1 && arguments[1].length() > 0 && arguments[1].at(0) == '#')
		channel = arguments[1];
	if (channel.length() == 0 || channel.at(0) != '#')
		bot.Say(respondto, "You must pick a channel to ban in.");
	else
		bot.AddDeferredCommand(new ModeWatcher(bot, speaker, target, mode, user, channel));
	if (command == "kickban" || command == "kb")
	{
		istringstream ds(args);
		string dummy;
		ds >> dummy;
		bot.commands["k"]->ProcessCommand(bot, "k", speaker, target, respondto, Hostname(user).GetNick() + " " + getline(ds));
	}
}

void CmdBan::ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	vector<string> arguments = splitcommands(args, " ", false);
	string channel = target;
	if (arguments.size() > 1)
		channel = arguments[1];
	FlagTestAndGo(bot, command, speaker, target, respondto, args, channel);
}

string CmdBan::HelpMsg(string command)
{
	if (command == "ban")
		return "Usage: ban <user> [channel] -- bans the specified user on the specified channel.  If the channel isn't specified, the current one is used.  Will fail if the bot does not have operator privilages.";
	if (command == "unban")
		return "Usage: unban <user> [channel] -- unbans the specified user on the specified channel.  If the channel isn't specified, the current one is used.  Will fail if the bot does not have operator privilages.";
	if (command == "kickban" || command == "kb")
		return "Usage: kickban <user> [channel [reason]] -- kickbans the specified user on the specified channel, for the specified (optional) reason.  If the channel isn't specified, the current one is used.  Will fail if the bot does not have operator privilages.";
}
