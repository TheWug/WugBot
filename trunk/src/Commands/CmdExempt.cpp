#include <iostream>
#include <stdlib.h>

#include "CmdExempt.h"
#include "ModeWatcher.h"
#include "IRCBot.h"
#include "misc.h"

CmdExempt::CmdExempt()
{
	defaultuserlevel = 4;
	defaultuserflags = "oOC";
	acceptsflags = true;
}

vector<string> CmdExempt::CommandStrings()
{
	vector<string> types;
	types.push_back("exempt");
	types.push_back("e");
	types.push_back("deexempt");
	types.push_back("de");
	return types;
}

void CmdExempt::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	vector<string> arguments = splitcommands(args, " ", false);
	string channel = target;
	string user;
	string mode = "+e";
	if (command == "deexempt" || command == "de")
	mode = "-e";
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
		bot.Say(respondto, "You must pick a channel to voice in.");
	else
		bot.AddDeferredCommand(new ModeWatcher(bot, speaker, target, mode, user, channel));
}

void CmdExempt::ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	vector<string> arguments = splitcommands(args, " ", false);
	string channel = target;
	if (arguments.size() > 1)
		channel = arguments[1];
	FlagTestAndGo(bot, command, speaker, target, respondto, args, channel);
}

string CmdExempt::HelpMsg(string command)
{
	if (command == "exempt" || command == "e")
		return "Usage: exempt <user> [channel] -- sets ban exemption on the specified user on the specified channel.  If the channel isn't specified, the current one is used.  Will fail if the bot does not have operator privilages.";
	if (command == "deexempt" || command == "de")
		return "Usage: deexempt <user> [channel] -- clears ban exemption on the specified user on the specified channel.  If the channel isn't specified, the current one is used.  Will fail if the bot does not have operator privilages.";
}
