#include <iostream>
#include <stdlib.h>

#include "CmdOp.h"
#include "ModeWatcher.h"
#include "IRCBot.h"
#include "misc.h"

CmdOp::CmdOp()
{
	defaultuserlevel = 4;
	defaultuserflags = "oO";
	acceptsflags = true;
}

vector<string> CmdOp::CommandStrings()
{
	vector<string> types;
	types.push_back("op");
	types.push_back("deop");
	return types;
}

void CmdOp::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	vector<string> arguments = splitcommands(args, " ", false);
	string channel = target;
	string user = speaker.GetNick();
	string mode = "+o";
	if (command == "deop")
	mode = "-o";
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
		bot.Say(respondto, "You must pick a channel to op in.");
	else
	{
		bot.AddDeferredCommand(new ModeWatcher(bot, speaker, target, mode, user, channel));
	}
}

void CmdOp::ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	vector<string> arguments = splitcommands(args, " ", false);
	string channel = target;
	if (arguments.size() > 1)
		channel = arguments[1];
	FlagTestAndGo(bot, command, speaker, target, respondto, args, channel);
}

string CmdOp::HelpMsg(string command)
{
	if (command == "op")
		return "Usage: op <user> [channel] -- gives operator status to the specified user on the specified channel.  If the channel isn't specified, the current one is used.  Will fail if the bot does not have operator privilages.";
	if (command == "deop")
		return "Usage: deop <user> [channel] -- removes operator status from the specified user on the specified channel.  If the channel isn't specified, the current one is used.  Will fail if the bot does not have operator privilages.";
}
