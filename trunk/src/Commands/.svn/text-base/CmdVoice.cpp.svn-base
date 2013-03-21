#include <iostream>
#include <stdlib.h>

#include "CmdVoice.h"
#include "ModeWatcher.h"
#include "IRCBot.h"
#include "misc.h"

CmdVoice::CmdVoice()
{
	defaultuserlevel = 4;
	defaultuserflags = "oO";
	acceptsflags = true;
}

vector<string> CmdVoice::CommandStrings()
{
	vector<string> types;
	types.push_back("voice");
	types.push_back("v");
	types.push_back("devoice");
	types.push_back("dv");
	return types;
}

void CmdVoice::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	vector<string> arguments = splitcommands(args, " ", false);
	string channel = target;
	string user = speaker.GetNick();
	string mode = "+v";
	if (command == "devoice" || command == "dv")
	mode = "-v";
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
	{
		bot.AddDeferredCommand(new ModeWatcher(bot, speaker, target, mode, user, channel));
	}
}

void CmdVoice::ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	vector<string> arguments = splitcommands(args, " ", false);
	string channel = target;
	if (arguments.size() > 1)
		channel = arguments[1];
	FlagTestAndGo(bot, command, speaker, target, respondto, args, channel);
}

string CmdVoice::HelpMsg(string command)
{
	if (command == "voice" || command == "v")
		return "Usage: voice <user> [channel] -- gives voiced status to the specified user on the specified channel.  If the channel isn't specified, the current one is used.  Will fail if the bot does not have operator privilages.";
	if (command == "devoice" || command == "dv")
		return "Usage: devoice <user> [channel] -- removes voiced status from the specified user on the specified channel.  If the channel isn't specified, the current one is used.  Will fail if the bot does not have operator privilages.";
}
