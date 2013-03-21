#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "CmdPart.h"
#include "IRCBot.h"
#include "misc.h"

CmdPart::CmdPart()
{
	defaultuserlevel = 3;
	defaultuserflags = "oOj";
	acceptsflags = true;
}

void CmdPart::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream resp(args);
	string channel = target, dummy, reason;
	resp >> dummy;
	if (dummy.length() != 0 && dummy.at(0) == '#')
	{
		channel = dummy;
		reason = getline(resp);
	}
	else
		reason = args;
	channel = tolower(channel);
	if (bot.channellist.find(channel) == bot.channellist.end())
		bot.Say(respondto, "I'm not on that channel.");
	else if (channel.length() != 0 && channel.at(0) == '#')
		bot.Part(channel, reason);
	else
		bot.Say(respondto, "I can't, this is a query.");
	return;
}

void CmdPart::ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream resp(args);
	string channel = target, dummy;
	resp >> dummy;
	if (dummy.length() != 0 && dummy.at(0) == '#')
		channel = dummy;
	FlagTestAndGo(bot, command, speaker, target, respondto, args, channel);
}

vector<string> CmdPart::CommandStrings()
{
	vector<string> types;
	types.push_back("part");
	types.push_back("gtfo");
	types.push_back("leave");
	return types;
}

string CmdPart::HelpMsg(string s)
{
	return "Usage: part <channel> -- Makes the bot leave a channel.";
}
