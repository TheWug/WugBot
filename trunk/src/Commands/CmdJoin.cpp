#include <iostream>
#include <stdlib.h>

#include "CmdJoin.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "misc.h"

CmdJoin::CmdJoin()
{
	defaultuserlevel = 3;
	defaultuserflags = "oOj";
	acceptsflags = true;
}

void CmdJoin::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	args = args.substr(0, args.find(' ', 0));
	if (args.at(0) != '#')
		bot.Say(respondto, "I can't, that's not a channel.");
	else if (!bot.permission.CanBeIn(tolower(args)))
		bot.Say(respondto, "I'm not allowed in that channel.");
	else if (bot.channellist.find(args) != bot.channellist.end())
		bot.Say(respondto, "I'm already there.");
	else
		bot.Join(args);
	return;
}

void CmdJoin::ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	FlagTestAndGo(bot, command, speaker, target, respondto, args, tolower(args.substr(0, args.find(' ', 0))));
}

vector<string> CmdJoin::CommandStrings()
{
	vector<string> types;
	types.push_back("join");
	return types;
}

string CmdJoin::HelpMsg(string s)
{
	return "Usage: join <channel> -- Makes the bot join a channel.";
}
