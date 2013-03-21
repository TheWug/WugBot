#include <iostream>
#include <stdlib.h>

#include "CmdNick.h"
#include "IRCBot.h"

CmdNick::CmdNick()
{
	defaultuserlevel = 3;
}

vector<string> CmdNick::CommandStrings()
{
	vector<string> types;
	types.push_back("nick");
	types.push_back("rename");
	return types;
}

void CmdNick::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	if (args.length() == 0)
		bot.Say(respondto, "I can't be called \"\".");
	else if (args.length() > 24)
		bot.Say(respondto, "I can't, that's too long.");
	else if (args.at(0) >= 48 && args.at(0) <= 57)
		bot.Say(respondto, "Nicknames can't start with numbers.");
	else
		bot.Nick(args);
}

string CmdNick::HelpMsg(string command)
{
	return "Usage: " + command + " <newname> -- Causes the bot to change its nickname to the provided name.";
}
